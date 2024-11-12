// Neuron.h

#ifndef NEURON_H
#define NEURON_H

#define TANH 1
#define RELU 2
#define SIGM 3

#include <iostream>
#include <math.h>

class Neuron
{
public:
    Neuron( double val );
    Neuron( double val , int activationType );

    void setVal( double val );

    // Fast Sigmoid Function
    // f(x) = x / ( 1 + |x| )
    void activate();

    // Derivative for fast sigmoid function
    // f'(x) = f(x) * ( 1 - f(x) )
    void derive();

    // Getter
    double getVal(){ return this -> val; }
    double getActivatedVal(){ return this -> activatedVal; }
    double getDerivedVal(){ return this -> derivedVal; }

private:
    // 1.5
    double val;

    // 0-1
    double activatedVal;

    double derivedVal;

    int activationType = 3;

};
#endif // NEURON_H
