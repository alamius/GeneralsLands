#ifndef STRING_UTILS_H
#define STRING_UTILS_H
// #include <iostream>
// #include <stdlib.h>
#include <string>
#include <sstream>
using namespace std;

string slice(string arr, const int from, const int to);
int index(string str, string search);
int strlen(string str, int max_len){
	for(int i = 0; i < max_len; i++){
		if((int)str[i] == 0){
			return i;
		}
	}
	return -1;
}
#define strlen(str) strlen(str, 1 << 20)
char* to_char_pointer(string str, char* result){
	int len = strlen(str);
	for(int i = 0; i < len; i++){
		result[i] = str[i];
	}

	result += '\0';
	return result;
}

template <typename T> string _to_str(T var){
	stringstream ss;
	ss << var;
	return ss.str();
}
string _to_str_round(double var, int digits){
	string str = _to_str(var);
	int length = strlen(str);
	int i = index(str, (string)".");
	int end = min(length, i+digits);
	return slice(str, 0, end);
}
template <typename T> string _to_str_arr(T arr[], int arr_len, string sep){
	stringstream ss;
	for(int i = 0; i < arr_len; i++){
		ss << arr[i];
		if(i < arr_len - 1){
			ss << sep;
		}
	};
	return ss.str();
}
#define _to_str_bool(question) (string)(question ? "True" : "False")
string slice(string arr, const int from, const int to){
	string Slice;
	for(int pos = from; pos < to; pos++){
		Slice += arr[pos];
	}
	return Slice;
}
template <typename T> string repeat(T str, int n){
	string result = "";
	for(int i = 0; i < n; i++){
		result += _to_str(str);
	}
	return result;
}
int index(string str, string search){
	int length = strlen(str);
	int search_len = strlen(search);
	for (int i = 0; i < length; i++){
		string Slice = slice(str, i, i+search_len);
		if(Slice == search){ return i; }
	}
	return -1;
}
string f(int n, int full_length){
	string result;
	string nn = _to_str(n);
	int length = nn.size();
	if(length < full_length){
		result += repeat(" ", full_length - length);
	}
	result += nn;
	return result;
}
template <typename T> string f(T n, int full_length){
	string result;
	string nn = _to_str(n);
	int length = nn.size();
	if(length < full_length){
		result += repeat(" ", full_length - length);
	}
	result += nn;
	return result;
}
template <typename T> string f(T n){    return f(n, 2);  }
template <typename T> string fl(T n, int full_length){
	string result;
	string nn = _to_str(n);
	result += nn;
	int length = nn.size();
	if(length < full_length){
		result += repeat(" ", full_length - length);
	}
	return result;
}
template <typename T> string fl(T n){   return fl(n, 2); }

#endif
