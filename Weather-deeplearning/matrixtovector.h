#ifndef MATRIXTOVECTOR_H
#define MATRIXTOVECTOR_H

#include "matrix.h"

class MatrixtoVector
{
public:
    MatrixtoVector( Matrix *a );

    std::vector< double > execute();

private:
    Matrix *a;
};

#endif // MATRIXTOVECTOR_H
