# Задание H

Написать программу вычисления матричного выражения:

$A = Tr(BC^3)E+Tr'(B)I$,

где $B,C$ - квадратные плотные матрицы, элементы которых имеют тип **double**, $I$ - единичная матрица, $Tr(M)$ - след матрицы $M$, $Tr'(M)$ - "антислед" матрицы $M$ (сумма элементов антидиагонали). Распараллелить эту программу с помощью OpenMP (parallel, problem). Исследовать зависимость масштабируемости параллельной версии программы от её вычислительной трудоёмкости (размера матриц). Проверить корректность параллельной версии.

Проверка закона Амдала. Построить зависимость *ускорение:число потоков* для заданного примера.