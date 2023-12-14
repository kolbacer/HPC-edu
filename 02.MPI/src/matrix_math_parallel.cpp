#include <mpi.h>
#include "matrix.h"

struct RowDistribution {
    // common
    int* sizes;
    int* counts;
    int* displs;
    // per process
    double* procBuf;
    double** procRows;
    size_t pRows;
    size_t pCols;
    size_t startRow;
};

struct Dimensions {
    size_t rows;
    size_t cols;
};

RowDistribution getRowDistribution(size_t rows, size_t cols, int size, int rank, bool getData = true);

// assuming to have matrices only in the process 0
Matrix Matrix::add_parallel(Matrix m, bool inplace) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // share matrix dimensions
    Dimensions dim1 {0, 0};
    if (rank == 0) dim1 = {rows_, cols_};
    MPI_Bcast(&dim1, 2, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    Dimensions dim2 {0, 0};
    if (rank == 0) dim2 = {m.rows_, m.cols_};
    MPI_Bcast(&dim2, 2, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if (dim1.rows != dim2.rows || dim1.cols != dim2.cols)
        throw std::invalid_argument("incompatible matrix dimensions");

    // distribute rows
    RowDistribution rd1 = getRowDistribution(dim1.rows, dim1.cols, size, rank);

    if (MPI_Scatterv((*this)[0], rd1.counts, rd1.displs, MPI_DOUBLE, // send rows
                     rd1.procBuf, rd1.counts[rank], MPI_DOUBLE,      // receive rows
                     0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        throw std::runtime_error("Scatter error");
    }

    RowDistribution rd2 = getRowDistribution(dim2.rows, dim2.cols, size, rank);

    if (MPI_Scatterv(m[0], rd2.counts, rd2.displs, MPI_DOUBLE,  // send rows
                     rd2.procBuf, rd2.counts[rank], MPI_DOUBLE, // receive rows
                     0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        throw std::runtime_error("Scatter error");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // sum
    for (int i = 0; i < rd1.pRows; ++i)
        for (int j = 0; j < rd1.pCols; ++j)
            rd1.procRows[i][j] += rd2.procRows[i][j];

    // gather
    double** data = nullptr;

    if (rank == 0)
        data = getNewData(rows_, cols_);

    if (MPI_Gatherv(rd1.procBuf, rd1.counts[rank], MPI_DOUBLE,                                  // send rows
                    (data != nullptr) ? data[0] : nullptr, rd1.counts, rd1.displs, MPI_DOUBLE,  // receive rows
                    0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        throw std::runtime_error("Gather error");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // cleanup
    delete[] rd1.procBuf;
    delete[] rd2.procBuf;

    if (rank == 0) {
        if (inplace) {
            reassign(data, rows_, cols_);
            return *this;
        } else {
            return Matrix(data, rows_, cols_);
        }
    }

    // if process != 0
    return {};
}

// assuming to have matrices only in the process 0
Matrix Matrix::multiply_parallel(Matrix m, bool inplace) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // share matrix dimensions
    Dimensions dim1 {0, 0};
    if (rank == 0) dim1 = {rows_, cols_};
    MPI_Bcast(&dim1, 2, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    Dimensions dim2 {0, 0};
    if (rank == 0) dim2 = {m.rows_, m.cols_};
    MPI_Bcast(&dim2, 2, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if (dim1.cols != dim2.rows)
        throw std::invalid_argument("incompatible matrix dimensions");

    Dimensions newDim {dim1.rows, dim2.cols};

    // distribute rows
    RowDistribution rd1 = getRowDistribution(dim1.rows, dim1.cols, size, rank);

    if (MPI_Scatterv((*this)[0], rd1.counts, rd1.displs, MPI_DOUBLE, // send rows
                     rd1.procBuf, rd1.counts[rank], MPI_DOUBLE,      // receive rows
                     0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        throw std::runtime_error("Scatter error");
    }

    // broadcast second matrix
    double** m2_data;
    if (rank == 0)
        m2_data = m.data_;
    else
        m2_data = getNewData(dim2.rows, dim2.cols);

    MPI_Bcast(m2_data[0], dim2.rows * dim2.cols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    // new rows buffer
    double** newRows = getNewData(rd1.pRows, newDim.cols);

    // multiply
    for (size_t i = 0; i < rd1.pRows; ++i) {
        for (size_t j = 0; j < newDim.cols; ++j) {
            double x = 0;
            for (size_t k = 0; k < dim1.cols; ++k) {
                x += rd1.procRows[i][k] * m2_data[k][j];
            }
            newRows[i][j] = x;
        }
    }

    // gather
    double** newData = nullptr;

    if (rank == 0)
        newData = getNewData(newDim.rows, newDim.cols);

    RowDistribution newRd = getRowDistribution(newDim.rows, newDim.cols, size, rank, false);

    if (MPI_Gatherv( (newRows != nullptr) ? newRows[0] : nullptr, newRd.counts[rank], MPI_DOUBLE,         // send rows
                     (newData != nullptr) ? newData[0] : nullptr, newRd.counts, newRd.displs, MPI_DOUBLE, // receive rows
                    0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        throw std::runtime_error("Gather error");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // cleanup
    if (rank != 0) delete[] m2_data[0];
    if (newRows != nullptr) delete[] newRows[0];
    delete[] rd1.procBuf;

    if (rank == 0) {
        if (inplace) {
            reassign(newData, newDim.rows, newDim.cols);
            return *this;
        } else {
            return Matrix(newData, newDim.rows, newDim.cols);
        }
    }

    // if process != 0
    return {};
}

// assuming to have matrix only in the process 0
Matrix Matrix::multiply_scalar_parallel(double x, bool inplace) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // share matrix dimensions
    Dimensions dim {0, 0};
    if (rank == 0) dim = {rows_, cols_};
    MPI_Bcast(&dim, 2, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    // share x
    double x_;
    if (rank == 0) x_=x;
    MPI_Bcast(&x_, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    // distribute rows
    RowDistribution rd = getRowDistribution(dim.rows, dim.cols, size, rank);

    if (MPI_Scatterv((*this)[0], rd.counts, rd.displs, MPI_DOUBLE, // send rows
                     rd.procBuf, rd.counts[rank], MPI_DOUBLE,      // receive rows
                     0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        throw std::runtime_error("Scatter error");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // multiply scalar
    for (int i = 0; i < rd.pRows; ++i)
        for (int j = 0; j < rd.pCols; ++j)
            rd.procRows[i][j] *= x_;

    // gather
    double** data = nullptr;

    if (rank == 0)
        data = getNewData(rows_, cols_);

    if (MPI_Gatherv(rd.procBuf, rd.counts[rank], MPI_DOUBLE,                                 // send rows
                    (data != nullptr) ? data[0] : nullptr, rd.counts, rd.displs, MPI_DOUBLE, // receive rows
                    0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        throw std::runtime_error("Gather error");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // cleanup
    delete[] rd.procBuf;

    if (rank == 0) {
        if (inplace) {
            reassign(data, rows_, cols_);
            return *this;
        } else {
            return Matrix(data, rows_, cols_);
        }
    }

    // if process != 0
    return {};
}

// assuming to have matrix only in the process 0
Matrix Matrix::power_parallel(unsigned int k, bool inplace) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // share matrix dimensions
    Dimensions dim {0, 0};
    if (rank == 0) dim = {rows_, cols_};
    MPI_Bcast(&dim, 2, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if (dim.cols != dim.rows)
        throw std::logic_error("matrix is not square");

    size_t n = dim.cols;

    if (k == 0) {
        if (rank == 0) {
            double** data = Matrix::create_identity(n).copyData();
            if (inplace) {
                reassign(data, n, n);
                return *this;
            } else {
                return Matrix(data, n, n);
            }
        } else
            return {};
    }

    if (k == 1) {
        if (rank == 0)
            return inplace ? *this : copy();
        else
            return {};
    }

    // broadcast matrix
    double** m_data;
    if (rank == 0)
        m_data = data_;
    else
        m_data = getNewData(n, n);

    MPI_Bcast(m_data[0], n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    // distribute rows
    RowDistribution rd = getRowDistribution(n, n, size, rank);

    for (size_t i = 0; i < rd.pRows; ++i)
        for (size_t j = 0; j < n; ++j)
            rd.procRows[i][j] = m_data[rd.startRow + i][j];

    // power
    double** res_data = getNewData(rd.pRows, rd.pCols);

    for (int p = 1; (p < k) && (rd.pRows > 0); ++p) {
        for (size_t i = 0; i < rd.pRows; ++i)
            for (size_t j = 0; j < n; ++j) {
                double x = 0;
                for (size_t t = 0; t < n; ++t) {
                    x += rd.procRows[i][t] * m_data[t][j];
                }
                res_data[i][j] = x;
            }
        std::swap(rd.procRows, res_data);
        rd.procBuf = *rd.procRows;
    }

    // gather
    double** newData = nullptr;

    if (rank == 0)
        newData = getNewData(n, n);

    if (MPI_Gatherv( rd.procBuf, rd.counts[rank], MPI_DOUBLE,                                       // send rows
                     (newData != nullptr) ? newData[0] : nullptr, rd.counts, rd.displs, MPI_DOUBLE, // receive rows
                     0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        throw std::runtime_error("Gather error");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // cleanup
    if (rank != 0) delete[] m_data[0];
    if (res_data != nullptr) delete[] res_data[0];
    delete[] rd.procBuf;

    if (rank == 0) {
        if (inplace) {
            reassign(newData, n, n);
            return *this;
        } else {
            return Matrix(newData, n, n);
        }
    }

    // if process != 0
    return {};
}

// static methods

Matrix Matrix::add_parallel(Matrix m1, Matrix m2) {
    return m1.add_parallel(m2);
}

Matrix Matrix::multiply_parallel(Matrix m1, Matrix m2) {
    return m1.multiply_parallel(m2);
}

Matrix Matrix::multiply_scalar_parallel(Matrix m, double x) {
    return m.multiply_scalar_parallel(x);
}

// helper

RowDistribution getRowDistribution(size_t rows, size_t cols, int size, int rank, bool getData) {
    int* sizes = new int[size];
    int* counts = new int[size];
    int* displs = new int[size];
    size_t startRow = 0;

    int quot = (int)rows / size;
    int rem = rows % size;

    for (int i = 0; i < size; ++i) {
        sizes[i] = quot;
        if (i < rem) ++sizes[i];

        if (i < rank) startRow += sizes[i];

        counts[i] = sizes[i] * cols;

        if (i == 0)
            displs[i] = 0;
        else
            displs[i] = displs[i-1] + counts[i - 1];
    }
    double* procBuf;
    double** procRows;

    if (getData) {
        procBuf = new double[counts[rank]];
        procRows = new double*[sizes[rank]];
        for (int i = 0; i < sizes[rank]; ++i)
            procRows[i] = procBuf + i*cols;
    }

    return {sizes, counts, displs, procBuf, procRows, (size_t)sizes[rank], cols, startRow};
}