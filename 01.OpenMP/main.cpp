#include <stdio.h>
#include "matrix.h"

int main(int argc, char* argv[])
{
    printf("### Example ###\n\n");

    double data1[3][3] = {{1.356, 2.456, 5.45675432},
                     {0.345, 4.3, 5},
                     {9.3545425, 5.43, 6}};

    Matrix matrix1 = Matrix(data1);
    printf("matrix1\n");
    matrix1.print();
    printf("\n");

    double data2[3][3] = {{2.45, 7.93, 3.0},
                          {1, 1, 1},
                          {0, -5, -1.1}};

    printf("matrix2\n");
    Matrix matrix2 = Matrix(data2);
    matrix2.print();
    printf("\n");

    printf("add\n");
    Matrix sum = matrix1.add(matrix2);
    sum.print();
    printf("\n");

    printf("add_parallel\n");
    Matrix sum1 = matrix1.add_parallel(matrix2);
    sum1.print();
    printf("\n");

    printf("multiply\n");
    Matrix mult1 = matrix1.multiply(matrix2);
    mult1.print();
    printf("\n");

    printf("multiply inplace\n");
    Matrix matrix1copy = matrix1.copy();
    Matrix mult2 = matrix1copy.multiply(matrix2, true);
    mult2.print();
    printf("\n");

    printf("multiply_parallel\n");
    Matrix mult3 = matrix1.multiply_parallel(matrix2);
    mult3.print();
    printf("\n");

    printf("trace\n");
    double trace = mult3.trace();
    printf("%f\n", trace);
    printf("\n");

    printf("atrace\n");
    double atrace = mult3.atrace();
    printf("%f\n", atrace);
    printf("\n");

    printf("static trace\n");
    double s_trace = Matrix::trace(Matrix::multiply(matrix1,matrix2));
    printf("%f\n", s_trace);
    printf("\n");

    printf("multiply_scalar\n");
    Matrix matrix3 = matrix1.multiply_scalar(2);
    matrix3.print();
    printf("\n");

    printf("multiply_scalar_parallel\n");
    Matrix matrix3p = matrix1.multiply_scalar_parallel(2);
    matrix3p.print();
    printf("\n");

    printf("equal?\n");
    bool res = matrix3.equal(matrix3p);
    printf("%s\n", res ? "true" : "false");
    printf("\n");

    printf("identity\n");
    Matrix matrix4 = Matrix::create_identity(5);
    matrix4.print();
    printf("\n");

    printf("random (seed = 1)\n");
    Matrix matrix5 = Matrix::create_random(3, 4, 1);
    matrix5.print();
    printf("\n");

    printf("random (seed = 2)\n");
    Matrix matrix6 = Matrix::create_random(3, 4, 2);
    matrix6.print();
    printf("\n");

    printf("random1 \n");
    Matrix matrix7 = Matrix::create_random(3, 4);
    matrix7.print();
    printf("\n");

    printf("random2 \n");
    Matrix matrix8 = Matrix::create_random(3, 4);
    matrix8.print();
    printf("\n");

    printf("power 0 \n");
    Matrix matrix9 = matrix1.power(0);
    matrix9.print();
    printf("\n");

    printf("power 1 \n");
    Matrix matrix10 = matrix1.power(1);
    matrix10.print();
    printf("\n");

    printf("power 3 \n");
    Matrix matrix11 = matrix1.power(3);
    matrix11.print();
    printf("\n");

    printf("power_parallel 3 \n");
    Matrix matrix11p = matrix1.power_parallel(3);
    matrix11p.print();
    printf("\n");
}