// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

class Matrix
{
public:
    Matrix( int numRows, int numCols, bool isRandom );

    Matrix *transpose();
    Matrix *copy();

    double getRandomNumber();

    void setValue( int r, int c, double v){ this -> values.at(r).at(c) = v; }

    double getValue( int r, int c ){ return values.at(r).at(c); }

    std::vector< std::vector< double > > getValues() { return this->values; }

    void printToConsole();

    int getNumRows(){ return this -> numRows; }
    int getNumCols(){ return this -> numCols; }

private:
    int numRows;
    int numCols;
    bool isRandom;
    std::mt19937 gen;

    std::vector< std::vector< double > > values;
};

#endif // MATRIX_H
