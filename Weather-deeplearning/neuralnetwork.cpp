#include "neuralnetwork.h"
#include "json.h"

#include <QDebug>
#include <memory>

// setCurrentInput
void NeuralNetwork::setCurrentInput( std::vector<double> input )
{
    this -> input = input;
    for( unsigned int i = 0; i < input.size(); i++ )
    {
        this -> layers.at( 0 ) -> setVal( i, input.at( i ) );
    }
}

// constructor
NeuralNetwork::NeuralNetwork( std::vector< int > topology,
                              double bias,
                              double learningRate,
                              double momentum)
{
    this -> topology = topology;
    this -> topologySize = topology.size();
    this -> learningRate = learningRate;
    this -> momentum = momentum;
    this -> bias = bias;

    for( int i = 0; i < topologySize; i++ )
    {
        if( i > 0 && i < topologySize - 1 )
        {
            this -> layers.push_back( new Layer( topology.at( i ), this -> hiddenActivationType ) );
        }
        else if( i == topologySize - 1 )
        {
            this -> layers.push_back( new Layer( topology.at( i ), this -> outputActivationType ) );
        }
        else
        {
            this -> layers.push_back( new Layer( topology.at( i ) ) );
        }
    }

    for( int i = 0; i < topologySize - 1; i++ )
    {
        Matrix *weightMatrix = new Matrix( topology.at( i ), topology.at( i + 1 ), true );
        this -> weightMatrices.push_back( weightMatrix );
    }

    for( int i = 0; i < topology.at( topologySize - 1 ); i++ )
    {
        errors.push_back( 0.00 );
        derivedErrors.push_back( 0.00 );
    }

    this -> error = 0.00;
}

NeuralNetwork::NeuralNetwork( std::vector< int > topology,
                              int hiddenActivationType,
                              int outputActivationType,
                              int costFunctionType,
                              double bias,
                              double learningRate,
                              double momentum)
{
    this -> topology = topology;
    this -> topologySize = topology.size();
    this -> learningRate = learningRate;
    this -> momentum = momentum;
    this -> bias = bias;
    this -> hiddenActivationType = hiddenActivationType;
    this -> outputActivationType = outputActivationType;
    this -> costFunctionType = costFunctionType;

    for( int i = 0; i < topologySize; i++ )
    {
        if( i > 0 && i <  topologySize - 1 )
        {
            this -> layers.push_back( new Layer( topology.at( i ), this -> hiddenActivationType ) );
        }
        else if( i ==  topologySize - 1  )
        {
            this -> layers.push_back( new Layer( topology.at( i ), this -> outputActivationType ) );
        }
        else
        {
            this -> layers.push_back( new Layer( topology.at( i ) ) );
        }
    }

    for( int i = 0; i < topologySize - 1; i++ )
    {
        Matrix *weightMatrix = new Matrix( topology.at( i ), topology.at( i + 1 ), true );
        this -> weightMatrices.push_back( weightMatrix );
    }

    for( int i = 0; i < topology.at( topologySize - 1 ); i++ )
    {
        errors.push_back( 0.00 );
        derivedErrors.push_back( 0.00 );
    }

    this -> error = 0.00;
}

// setErrors
void NeuralNetwork::setErrors()
{
    switch( costFunctionType )
    {
        case( COST_MSE ): this -> setErrorMSE(); break;
        default: this -> setErrorMSE(); break;
    }
}

// ErrorMSE
void NeuralNetwork::setErrorMSE()
{
    int outputLayerIndex = this -> layers.size() - 1;
    std::vector< Neuron *> outputNeurons = this -> layers.at( outputLayerIndex ) -> getNeurons();

    this -> error = 0.00;
    for( unsigned int i = 0; i < target.size(); i++ )
    {
        double t = target.at( i );
        double y = outputNeurons.at( i ) -> getActivatedVal();

        errors.at( i ) = 0.5 * pow( abs( t - y ), 2 );
        derivedErrors.at( i ) = y - t;

        this -> error += errors.at( i );
    }
}

// feedForward
void NeuralNetwork::feedForward(){
    Matrix *a; // Matrix of Neurons to the left
    Matrix *b; // Matrix of weights to the right of layer
    Matrix *c; // Matrix of Neurons to the next layer

    for( int i = 0; i < ( this -> topologySize - 1 ); i++ ){
        a = this -> getNeuronMatrix( i );
        b = this -> getWeightMatrix( i );
        c = new Matrix( a -> getNumRows(), b -> getNumCols(), false );

        if( i != 0 )
        {
            a = this -> getActivatedNeuronMatrix( i );
        }

        utils::Math::multiplyMatrix( a, b, c );

        for( int c_index = 0; c_index < c -> getNumCols(); c_index++ )
        {
            this -> setNeuronValue( i + 1, c_index, c -> getValue( 0, c_index ) + this -> bias );
        }

        delete  a;
        delete  b;
        delete  c;
    }
}

// backPropagation
void NeuralNetwork::backPropagation()
{
  vector<Matrix *> newWeights;
  Matrix *deltaWeights;
  Matrix *gradients;
  Matrix *derivedValues;
  Matrix *gradientsTransposed;
  Matrix *zActivatedVals;
  Matrix *tempNewWeights;
  Matrix *pGradients;
  Matrix *transposedPWeights;
  Matrix *hiddenDerived;
  Matrix *transposedHidden;

   // 1. output to last hidden layer
  int indexOutputLayer  = this -> topology.size() - 1;

  gradients = new Matrix(
                1,
                this -> topology.at( indexOutputLayer ),
                false
              );

  derivedValues = this -> layers.at( indexOutputLayer ) -> matrixifyDerivedVals();

  for( int i = 0; i < this -> topology.at( indexOutputLayer ); i++ ) {
    double e  = this -> derivedErrors.at( i );
    double y  = derivedValues -> getValue( 0, i );
    double g  = e * y;
    gradients -> setValue( 0, i, g );
  }

  // grants(T) * Z
  gradientsTransposed = gradients -> transpose();
  zActivatedVals      = this -> layers.at( indexOutputLayer - 1 ) -> matrixifyActivatedVals();

  deltaWeights  = new Matrix(
                    gradientsTransposed -> getNumRows(),
                    zActivatedVals -> getNumCols(),
                    false
                  );

  utils::Math::multiplyMatrix( gradientsTransposed, zActivatedVals, deltaWeights );


  // compute for new weights(last hidden -> output)
  tempNewWeights  = new Matrix(
                      this -> topology.at( indexOutputLayer - 1 ),
                      this -> topology.at( indexOutputLayer ),
                      false
                    );

  for( int r = 0; r < this -> topology.at( indexOutputLayer - 1 ); r++ )
  {
    for( int c = 0; c < this -> topology.at( indexOutputLayer ); c++ )
    {
      double originalValue  = this -> weightMatrices.at( indexOutputLayer - 1 ) -> getValue( r, c );
      double deltaValue     = deltaWeights -> getValue( c, r );

      originalValue = this -> momentum * originalValue;
      deltaValue    = this -> learningRate * deltaValue;

      tempNewWeights -> setValue( r, c, ( originalValue - deltaValue ) );
    }
  }

  newWeights.push_back( new Matrix( *tempNewWeights ) );

  // clean-up
  delete gradientsTransposed;
  delete zActivatedVals;
  delete tempNewWeights;
  delete deltaWeights;
  delete derivedValues;

  //  2. last hidden layer to input layeer
  for( int i = ( indexOutputLayer - 1 ); i > 0; i-- )
  {
    pGradients = new Matrix( *gradients );
    delete gradients;

    transposedPWeights = this -> weightMatrices.at( i ) -> transpose();

    gradients   = new Matrix(
                    pGradients -> getNumRows(),
                    transposedPWeights -> getNumCols(),
                    false
                  );

    utils::Math::multiplyMatrix( pGradients, transposedPWeights, gradients );

    hiddenDerived = this -> layers.at( i ) -> matrixifyDerivedVals();

    for( int colCounter = 0; colCounter < hiddenDerived -> getNumCols(); colCounter++ )
    {
      double g = gradients -> getValue( 0, colCounter ) * hiddenDerived -> getValue( 0, colCounter );
      gradients -> setValue( 0, colCounter, g );
    }

    if( i == 1 )
    {
      zActivatedVals = this -> layers.at( 0 ) -> matrixifyVals();
    }
    else
    {
      zActivatedVals = this -> layers.at( i - 1 ) -> matrixifyActivatedVals();
    }

    transposedHidden = zActivatedVals -> transpose();

    deltaWeights = new Matrix(
                       transposedHidden -> getNumRows(),
                       gradients -> getNumCols(),
                       false
                       );

    ::utils::Math::multiplyMatrix( transposedHidden, gradients, deltaWeights );

    // update weights
    tempNewWeights = new Matrix(
                         this -> weightMatrices.at( i - 1 ) -> getNumRows(),
                         this -> weightMatrices.at( i - 1 ) -> getNumCols(),
                         false
                         );

    for( int r = 0; r < tempNewWeights -> getNumRows(); r++ )
    {
      for( int c = 0; c < tempNewWeights -> getNumCols(); c++ )
      {
        double originalValue = this -> weightMatrices.at( i - 1 ) -> getValue( r, c );
        double deltaValue = deltaWeights -> getValue( r, c );

        originalValue = this -> momentum * originalValue;
        deltaValue = this -> learningRate * deltaValue;

        tempNewWeights -> setValue( r, c, ( originalValue - deltaValue ) );
      }
    }

    newWeights.push_back( new Matrix( *tempNewWeights ) );

    delete pGradients;
    delete transposedPWeights;
    delete hiddenDerived;
    delete zActivatedVals;
    delete transposedHidden;
    delete tempNewWeights;
    delete deltaWeights;
  }
  delete gradients;

  for( unsigned int i = 0; i < this->weightMatrices.size(); i++) {
    delete this -> weightMatrices[i];
  }

  this -> weightMatrices.clear();

  reverse( newWeights.begin(), newWeights.end() );

  for( unsigned int i = 0; i < newWeights.size(); i++) {
    this -> weightMatrices.push_back( new Matrix( *newWeights[ i ] ) );
    delete newWeights[i];
  }
}

// print
void NeuralNetwork::printInputToConsole()
{
    for( unsigned int i = 0; i < this -> input.size(); i++ )
    {
        qDebug() << this -> input.at( i ) << "\t";
    }
    qDebug() << endl;
}

void NeuralNetwork::printTargetToConsole()
{
    for( unsigned int i = 0; i < this -> target.size(); i++ )
    {
        qDebug() << this -> target.at( i ) << "\t";
    }
    qDebug() << endl;
}

void NeuralNetwork::printOutputToConsole()
{
    int indexofOutputLayer = this -> layers.size() - 1;
    Matrix *outputValues = this -> layers.at( indexofOutputLayer ) -> matrixifyActivatedVals();
    for( int c = 0; c < outputValues -> getNumCols(); c++ )
    {
        qDebug() << outputValues -> getValue( 0, c ) << "\t";
    }
    qDebug() << endl;
}

double NeuralNetwork::printOutput()
{
    int indexofOutputLayer = this -> layers.size() - 1;
    Matrix *outputValues = this -> layers.at( indexofOutputLayer ) -> matrixifyActivatedVals();
    double Val;
    for( int c = 0; c < outputValues -> getNumCols(); c++ )
    {
        Val = outputValues -> getValue( 0, c );
    }
    return Val;
}

void NeuralNetwork::printHistoricalErrors()
{
    for( unsigned int i = 0; i < this -> historicalErrors.size(); i++ )
    {
        qDebug() << this -> historicalErrors.at( i );
        if( i != this -> historicalErrors.size() - 1 )
        {
            //qDebug() << ",";
        }
    }
    qDebug() << endl;
}

void NeuralNetwork::printToConsole()
{
    for( unsigned int i = 0; i < this -> layers.size(); i++ )
    {
        qDebug() << "LAYER: " << i << endl;
        if( i == 0 )
        {
            Matrix *m = this -> layers.at( i ) -> matrixifyVals();
            m -> printToConsole();
        }
        else
        {
            Matrix *m = this -> layers.at( i ) -> matrixifyActivatedVals();
            m -> printToConsole();
        }
        qDebug() << "=====================" << endl;
        if( i < this -> layers.size() - 1 )
        {
            qDebug() << "Weight Matrix: " << i << endl;
            this -> getWeightMatrix( i ) -> printToConsole();
        }
        qDebug() << "=====================" << endl;
    }
}

// train
void NeuralNetwork::train( std::vector< double > input, std::vector< double > target, double bias, double learningRate, double momentum )
{
    this -> learningRate = learningRate;
    this -> momentum = momentum;
    this -> bias = bias;

    this -> setCurrentInput( input );
    this -> setCurrentTarget( target );

    this -> feedForward();
    this -> setErrors();
    this -> backPropagation();
}

// save Weights
void NeuralNetwork::saveWeights( string file ) {
  nlohmann::json j  = {};

  vector< vector< vector< double > > > weightSet;

  for( unsigned int i = 0; i < this -> weightMatrices.size(); i++ )
  {
    weightSet.push_back( this -> weightMatrices.at( i ) -> getValues() );
  }

  j[ "weights" ] = weightSet;
  j[ "topology" ] = this -> topology;
  j[ "learningRate" ] = this -> learningRate;
  j[ "momentum" ] = this -> momentum;
  j[ "bias" ] = this -> bias;

  std::ofstream o( file );
  o << std::setw( 4 ) << j << endl;
}

// loadWeights
void NeuralNetwork::loadWeights( string file ) {
  std::ifstream i( file );
  nlohmann::json jWeights;
  i >> jWeights;

  vector< vector< vector< double > > > temp = jWeights[ "weights" ];

  for( unsigned int i = 0; i < this -> weightMatrices.size(); i++ )
  {
    for( int r = 0; r < this -> weightMatrices.at( i ) -> getNumRows(); r++ )
    {
      for( int c = 0; c < this -> weightMatrices.at( i ) -> getNumCols(); c++ )
      {
        this -> weightMatrices.at( i ) -> setValue( r, c, temp.at( i ).at( r ).at( c ) );
      }
    }
  }
}
