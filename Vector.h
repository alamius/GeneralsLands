#ifndef VECTOR_H
#define VECTOR_H
#include <string>
#include "/home/alamius/Desktop/PROJECTS/utils.*/utils.cpp/string_utils.h"

using namespace std;

class Vector {
public:
    float x, y;
    bool set; //whether the Vector has been set/initialized -> pointer is not NULL, but Object still not set
    Vector(float, float, bool);
    Vector(float, float);
    Vector();
    Vector(Vector*);
    void _set(float, float, bool);
    float dist(float, float);
    float dist(Vector*);
    float dist();
    Vector* add(float, float);
    Vector* add(Vector*);
    Vector* _add(float, float);
    Vector* _add(Vector*);
    string to_str(string);
    string to_str();
    bool in_ranges(float, float, float, float);
    bool in_ranges(Vector*, Vector*);
    bool in_ranges(float, float);
    bool in_ranges(Vector*);
    bool equals(float, float);
    bool equals(Vector*);
    float abs_sum();
};
static const string std_Vector_format = "(%x, %y)";

#include "Vector.cpp"

#endif
