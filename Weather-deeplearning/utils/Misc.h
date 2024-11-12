#ifndef MISC_H
#define MISC_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <assert.h>

using namespace std;

namespace utils
{
  class Misc
  {
  public:
    static vector< vector<double> > fetchData(string path);
  };
}

#endif // MISC_H
