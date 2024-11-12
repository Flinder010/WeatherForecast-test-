// Math.h

#ifndef _MATH_HPP_
#define _MATH_HPP_

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <assert.h>

#include "matrix.h"

using namespace std;

namespace utils
{
    class Math
    {
    public:
        static void multiplyMatrix( Matrix *a, Matrix *b, Matrix *c );
    };
}

#endif
