// Matrix.cpp

#include "matrix.h"
#include <vector>
#include <QDebug>
#include <random>

// constructor
Matrix::Matrix( int numRows, int numCols, bool isRandom ) : gen( std::random_device()() )
{
    this -> numRows = numRows;
    this -> numCols = numCols;

    for( int i = 0; i < numRows; i++ )
    {
        std::vector< double > colValues;

        for( int j = 0; j < numCols; j++ )
        {
            double r = 0.0;
            if( isRandom )
            {
                r = this -> getRandomNumber();
            }
            colValues.push_back( r );
        }

        this -> values.push_back( colValues );
    }
}

// printToConsole
void Matrix::printToConsole()
{
    for( int i = 0; i < numRows; i++ )
    {
        for( int j = 0; j < numCols; j++ )
        {
            qDebug() << this -> values.at(i).at(j) << "  ";
        }
        qDebug() << endl;
    }
}

// transpose
Matrix *Matrix::transpose()
{
    Matrix *m = new Matrix( this -> numCols, this -> numRows, false );
    for( int i = 0; i < numRows; i++ )
    {
        for( int j = 0; j < numCols; j++ )
        {
            m -> setValue( j, i, this -> values.at( i ).at( j ) );
        }
    }
    return m;
}

// RandomNumber
double Matrix::getRandomNumber()
{
    //std::random_device rd;
    //std::mt19937 gen( rd() );
    std::uniform_real_distribution<> dis( -0.001 , 0.001 );

    return dis( gen );
}

// copy
Matrix *Matrix::copy()
{
    Matrix *m = new Matrix( this -> numRows, this -> numCols, false );
    for( int i = 0; i < numRows; i++ )
    {
        for( int j = 0; j < numCols; j++ )
        {
            m -> setValue( i, j, this -> values.at( i ).at( j ) );
        }
    }
    return m;
}
