// Layer.cpp

#include "layer.h"

// getActivatedVals
std::vector< double > Layer::getActivatedVals()
{
    std::vector< double > ret;
    for( unsigned int i = 0; i < this -> neurons.size(); i++ )
    {
        double v = this -> neurons.at( i ) -> getActivatedVal();
        ret.push_back( v );
    }
    return ret;
}

// constructor
Layer::Layer( int size )
{
    this -> size = size;
    for( int i = 0; i < size; i++ )
    {
        Neuron *n = new Neuron( 0.00 );
        this -> neurons.push_back( n );
    }
}

Layer::Layer( int size, int activationType)
{
    this -> size = size;
    for( int i = 0; i < size; i++ )
    {
        Neuron *n = new Neuron( 0.00, activationType );
        this -> neurons.push_back( n );
    }
}

// layer_setVal
void Layer::setVal( int i, double v)
{
    this -> neurons.at( i ) -> setVal( v );
}

// matrixifyVals, matrixifyActivatedVals and matrixifyDerivedVals
Matrix *Layer::matrixifyVals()
{
    Matrix *m = new Matrix( 1, this -> neurons.size(), false );
    for( unsigned int i = 0; i < this -> neurons.size(); i++ )
    {
        m -> setValue( 0, i, this -> neurons.at( i ) -> getVal() );
    }
    return m;
}

Matrix *Layer::matrixifyActivatedVals()
{
    Matrix *m = new Matrix( 1, this -> neurons.size(), false );
    for( unsigned int i = 0; i < this -> neurons.size(); i++ )
    {
        m -> setValue( 0, i, this -> neurons.at( i ) -> getActivatedVal() );
    }
    return m;
}

Matrix *Layer::matrixifyDerivedVals()
{
    Matrix *m = new Matrix( 1, this -> neurons.size(), false );
    for( unsigned int i = 0; i < this -> neurons.size(); i++ )
    {
        m -> setValue( 0, i, this -> neurons.at( i ) -> getDerivedVal() );
    }
    return m;
}
