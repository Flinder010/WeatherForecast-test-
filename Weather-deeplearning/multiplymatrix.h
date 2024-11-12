// MultiplyMatrix.h

#ifndef MULTIPLYMATRIX_H
#define MULTIPLYMATRIX_H

#include "matrix.h"

class MultiplyMatrix
{
public:
    MultiplyMatrix( Matrix *a, Matrix *b );

    Matrix *execute();

private:
    Matrix *a;
    Matrix *b;
    Matrix *c;
};

#endif // MULTIPLYMATRIX_H
