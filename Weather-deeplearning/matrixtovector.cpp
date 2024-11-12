#include "matrixtovector.h"

MatrixtoVector::MatrixtoVector( Matrix *a )
{
    this -> a = a;
}

std::vector< double > MatrixtoVector::execute(){
    std::vector <double> result;

    for( int r = 0; r < a -> getNumRows(); r++ ){
        for( int c = 0; c < a -> getNumCols(); c++){
            result.push_back( a -> getValue( r, c ) );
        }
    }
    return result;
}
