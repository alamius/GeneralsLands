#include <math.h>
#include <string>

void Vector::_set(float x_, float y_, bool set_){       x = x_; y = y_; set = set_; }
Vector::Vector(float x_, float y_, bool set_){          _set(x_, y_, set_); }
Vector::Vector(float x_, float y_){                     _set(x_, y_, true); }
Vector::Vector(){                                       _set(0, 0, false); }
Vector::Vector(Vector* v){                              _set(v->x, v->y, v->set); }
float Vector::dist(float vx, float vy){                 return sqrt(pow(vx-x, 2)+pow(vy-y, 2)); }
float Vector::dist(Vector* v){                          return dist(v->x, v->y); }
float Vector::dist(){                                   return dist(0, 0); }
Vector* Vector::add(float vx, float vy){                return new Vector(x+vx, y+vy); }
Vector* Vector::add(Vector* v){ if(!set and !v->set){   return new Vector(); } return add(v->x, v->y); }
Vector* Vector::_add(float vx, float vy){               x += vx; y += vy; return this; }
Vector* Vector::_add(Vector* v){ if(!set and !v->set){  return this; } return _add(v->x, v->y); }
string Vector::to_str(string format){
    if(!set){
        return "NULL_VECTOR";
    }
    string result = format;
    int i = 0;
    while(i < result.size()){
        if(result[i]=='%'){
            if(result[i+1]=='x'){          result = slice(result, 0, i)+_to_str(x)+slice(result, i+2, result.size()); }
            else if(result[i+1]=='y'){     result = slice(result, 0, i)+_to_str(y)+slice(result, i+2, result.size()); }
        }
        i++;
    }
    return result;
};
string Vector::to_str(){ return to_str(std_Vector_format); }
bool Vector::in_ranges(float min_x, float min_y, float max_x, float max_y){
    if(!set){ return false; }
    return (min_x <= x && min_y <= y && x < max_x && y < max_y);
}
bool Vector::in_ranges(Vector* min, Vector* max){       return in_ranges(min->x, min->y, max->x, max->y); }
bool Vector::in_ranges(float max_x, float max_y){       return in_ranges(0, 0, max_x, max_y); }
bool Vector::in_ranges(Vector* max){                    return in_ranges(0, 0, max->x, max->y); }
bool Vector::equals(float vx, float vy){ if(!set){return false; } return (vx == x && vy == y); }
bool Vector::equals(Vector* v){ if(!v->set){ return false; } return equals(v->x, v->y); }
float Vector::abs_sum(){        return fabs(x)+fabs(y); }
