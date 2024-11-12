// Layer.h

#ifndef LAYER_H
#define LAYER_H

#include <neuron.h>
#include <matrix.h>

#include <iostream>
#include <vector>

class Layer
{
public:
    Layer( int size );
    Layer( int size, int activationType );

    void setVal( int i, double v );

    Matrix *matrixifyVals();
    Matrix *matrixifyActivatedVals();
    Matrix *matrixifyDerivedVals();

    std::vector< double > getActivatedVals();
    std::vector< Neuron *> getNeurons(){ return this -> neurons; }
    void setNeurons( std::vector< Neuron *> neurons ){ this -> neurons = neurons; }

private:
    int size;

    std::vector< Neuron *> neurons;
};

#endif // LAYER_H
