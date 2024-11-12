// NeuralNetwork.h

#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#define COST_MSE 1

#include "matrix.h"
#include "layer.h"
#include "utils/Math.h"
#include "json.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <time.h>

class NeuralNetwork
{
public:

    NeuralNetwork(
            std::vector< int > topology,
            double bias = 1.0,
            double learningRate = 0.05,
            double momentum = 1.0
            );

    NeuralNetwork(
            std::vector< int > topology,
            int hiddenActivationType,
            int outputActivationType,
            int costFunctionType,
            double bias = 1.0,
            double learningRate = 0.05,
            double momentum = 1.0
            );

    void train( std::vector< double > input, std::vector< double > target, double bias, double learningRate, double momentum );

    void setCurrentInput( std::vector< double > input );
    void setCurrentTarget( std::vector< double > target ){ this -> target = target; }

    void printToConsole();
    void printInputToConsole();
    void printOutputToConsole();
    void printTargetToConsole();
    void printHistoricalErrors();

    double printOutput();

    void feedForward();
    void backPropagation();
    void setErrors();

    void saveWeights( string File );
    void loadWeights( string File );

    int topologySize;
    int hiddenActivationType = RELU;
    int outputActivationType = SIGM;
    int costFunctionType = COST_MSE;

    std::vector< int > topology;
    std::vector< Layer *> layers;
    std::vector< Matrix *> weightMatrices;
    std::vector< Matrix *> gradientMatrices;

    std::vector< double > input;
    std::vector< double > target;
    std::vector< double > errors;
    std::vector< double > derivedErrors;

    double error = 0.00;
    double bias = 1.00;
    double momentum;
    double learningRate;

    Matrix *getNeuronMatrix( int index ){ return this -> layers.at( index ) -> matrixifyVals(); }
    Matrix *getActivatedNeuronMatrix( int index ){ return this -> layers.at( index ) -> matrixifyActivatedVals(); }
    Matrix *getDerivedNeuronMatrix( int index ){ return this -> layers.at( index ) -> matrixifyDerivedVals(); }
    Matrix *getWeightMatrix( int index ){ return new Matrix( *this -> weightMatrices.at( index ) ); }

    void setNeuronValue( int indexLayer, int indexNeuron, double val ){ this -> layers.at( indexLayer ) -> setVal( indexNeuron, val); }
    double getTotalError(){ return this -> error; }

    std::vector< double > getErrors(){ return this -> errors; }
    std::vector< double > getActivateVals( int index ){ return this -> layers.at( index ) -> getActivatedVals(); }

private:
    void setErrorMSE();
    std::vector< double > historicalErrors;
};

#endif // NEURALNETWORK_H
