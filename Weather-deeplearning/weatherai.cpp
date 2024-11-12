#include "weatherai.h"
#include "ui_weatherai.h"
#include "mainwindow.h"
#include "neuron.h"
#include "matrix.h"
#include "neuralnetwork.h"
#include "layer.h"
#include "utils/Math.h"
#include "utils/Misc.h"
#include "json.h"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <ostream>
#include <time.h>

WeatherAI::WeatherAI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WeatherAI)
{
    ui->setupUi(this);
}

WeatherAI::~WeatherAI()
{
    delete ui;
}

void WeatherAI::on_Button1_clicked()
{
    std::ifstream configFile( "D:/C++daima/weather_prs/Weather-deeplearning/config.json" );
    string str( ( std::istreambuf_iterator< char >( configFile ) ),
                  std::istreambuf_iterator< char >() );

    auto config = nlohmann::json::parse( str );

    double learningRate = config[ "learningRate" ];
    double momentum = config[ "momentum" ];
    double bias = config[ "bias" ];
    int epoch = config[ "epoch" ];
    string trainingFile = config[ "trainingData" ];  // mean = 3.48, std_dev = 5.37
    string labelsFile = config[ "labelData" ];    // mean = 1.89, std_dev = 6.11
    string weightsFile = config[ "weightsFile" ];
    vector<int> topology  = config[ "topology" ];

    NeuralNetwork *n = new NeuralNetwork( topology, 2, 3, 1, bias, learningRate, momentum );

    vector< vector< double > > trainingData = utils::Misc::fetchData( trainingFile );
    vector< vector< double > > labelData = utils::Misc::fetchData( labelsFile );

    n -> loadWeights( weightsFile );

    for( int i = 0; i < epoch; i++ )
    {
        for( unsigned int tIndex = 0; tIndex < trainingData.size(); tIndex++ )
        {
            vector< double > input = trainingData.at( tIndex );
            vector< double > target = labelData.at( tIndex );

            n -> train( input, target, bias, learningRate, momentum );

            QString output = "Error at epoch " + QString::number( i + 1 ) + ": " + QString::number( n -> error );
            ui -> textEditC -> append( output );
        }
    }

    n -> saveWeights( weightsFile );
}

void WeatherAI::on_Button1_2_clicked()
{
    vector< double > input;

    QString input1 = ui -> lineEditone -> text();
    double number1 = input1.toDouble();
    input.push_back( number1 );
    //qDebug() << number1 << endl;

    QString input2 = ui -> lineEdittwo -> text();
    double number2 = input2.toDouble();
    input.push_back( number2 );

    QString input3 = ui -> lineEditthree -> text();
    double number3 = input3.toDouble();
    input.push_back( number3 );

    std::ifstream configFile( "D:/C++daima/weather_prs/Weather-deeplearning/config.json" );
    string str( ( std::istreambuf_iterator< char >( configFile ) ),
                  std::istreambuf_iterator< char >() );

    auto config = nlohmann::json::parse( str );

    double learningRate = config[ "learningRate" ];
    double momentum = config[ "momentum" ];
    double bias = config[ "bias" ];
    string trainingFile = config[ "trainingData" ];  // mean = 3.48, std_dev = 5.37
    string labelsFile = config[ "labelData" ];    // mean = 1.89, std_dev = 6.11
    string weightsFile = config[ "weightsFile" ];
    vector<int> topology  = config[ "topology" ];

    NeuralNetwork *n = new NeuralNetwork( topology, 2, 3, 1, bias, learningRate, momentum );

    //n -> loadWeights( weightsFile );
    vector< vector< double > > trainingData = utils::Misc::fetchData( trainingFile );
    vector< vector< double > > labelData = utils::Misc::fetchData( labelsFile );
    double predict;
    for( int i = 0; i < 20; i++ )
    {
        for( unsigned int tIndex = 0; tIndex < trainingData.size(); tIndex++ )
        {
            vector< double > input = trainingData.at( tIndex );
            vector< double > target = labelData.at( tIndex );

            n -> train( input, target, bias, learningRate, momentum );
        }
        if( i == 15 )
        {
            n -> setCurrentInput( input );
            n -> feedForward();
            predict = number3 * n -> printOutput();
        }
    }

    ui -> lineEditfour -> setText( QString::number( predict ) );

}
