// MultiplyMatrix.cpp

#include "multiplymatrix.h"

#include <QDebug>

MultiplyMatrix::MultiplyMatrix( Matrix *a, Matrix *b )
{
    this -> a = a;
    this -> b = b;

    if( a -> getNumCols() != b -> getNumRows() ){
        qDebug() << "A_rows" << a -> getNumRows() << "!= B_cols: " << b -> getNumCols() << endl;
        assert( false );
    }

    this -> c = new Matrix( a -> getNumRows(), b -> getNumCols(), false );
}

Matrix *MultiplyMatrix::execute(){
    for( int i = 0; i < a -> getNumRows(); i++ ){
        for( int j = 0; j < b -> getNumCols(); j++ ){
            for( int k = 0; k < b -> getNumRows(); k++ ){
                double p = this -> a -> getValue( i,k ) * this -> b -> getValue( k, j );
                double newVal = this -> c -> getValue( i, j ) + p;
                this -> c -> setValue( i, j, newVal );
            }
        }
    }
    return this -> c;
}
