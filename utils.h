#ifndef UTILS_H
#define UTILS_H
// #include <typeinfo>
// #include <iostream>
// #include <stdlib.h>
#include <string>
#include <sstream>
#include <ctime>
#include <errno.h>
#include "perlin.h"
#include "CC_str.h"

using namespace std;

static string dbg_line_break = "\n";
void clear_dbg(){
	ofstream dbg_out("dbg.output", ios::trunc);
	if(dbg_out.is_open()){
		dbg_out.close();
	}
	dbg_set_up = true;
}
void dbg(string str, int dbg_){  if(dbg_ <= dbg_lvl){  dbg(str);  }  }
void dbg(string str){
	if(!dbg_set_up){
		return;
	}
	ofstream dbg_out("dbg.output", ios::app);
	if(dbg_out.is_open()){
		dbg_out << dbg_line_break << str;
		dbg_out.close();
	}
}
template <typename T> void dbg_obj_arr(string origin, string arr_name, T* arr, int arr_len){
	for(int i = 0; i < arr_len; i++){
		dbg(
			origin+arr_name+"["+_to_str(i)+"] == "+(
				arr[i] == NULL
				? "NULL"
				: _to_str(arr[i])+": "+arr[i]->_dbg()
			)+""
		);
	}
}
void set_color_on(int color, int intensity);
void print(CC_str* arr[], int arr_len, int x, int y);
void print(CC_str* str, int x, int y, bool make_str_ncurses_bug_proof_ = true);
template <typename T> int index(T target, T* arr, int arr_len);

/*
	//from: https://www.linuxquestions.org/questions/programming-9/ncurses-c-how-to-clear-stdin-buffer-4175447149/
	flushes STDIN in a defined fashion rather than 'fflush(stdin)' which is undefined in the C standard
	N.B. only use after testing for excess chars, e.g
	if (count >= maxChars) {
	flushSTDIN();
	}
	otherwise if the buffer is empty it will wait for input.
*/
void flushSTDIN(){
	char c;
	do{
		errno = 0;
		c = getchar();
		if(errno){
			fprintf(stderr, "Error flushing standard input buffer: %i\n", errno);
		}
	}while((c != '\n') && (!feof(stdin)));

	return;
}

char get_similar_character(char c){
	dbg("get_similar_character("+_to_str(c)+")", 5);
	int i = index(c, similar_characters, similar_characters_len);
	if(i == -1){
		return '#';
	}
	if(i % 2 == 0){
		return similar_characters[i+1];
	}else{
		return similar_characters[i-1];
	}
}
int count_same_chars_following(CC_str* target, CC_str* str, int pos = 0){
	dbg("count_same_chars_following", 5);
	int result = 0;
	for(int i = pos; i < str->length(); i++){
		bool the_same = (
			true
			? str->substr(i, target->length())->chrs == target->chrs
			: str->substr(i, target->length())->equals(target)
		);
		if(the_same){
			result++;
		}else{
			break;
		}
	}
	return result;
}
//tries to remove all repetitions of more than 6 (if not minimal: more than 2) characters with those suggested by similar_characters to avoid a bug within ncurses
CC_str* make_str_ncurses_bug_proof(CC_str* str, bool minimal = false){
	dbg("make_str_ncurses_bug_proof", 5);
	char c;
	for(int i = 0; i < str->length(); i++){
		c = str->chrs[i];
		int same_chars = count_same_chars_following(new CC_str(c), str, i);
		bool needs_change = ( //not taking care of color or other attr yet. TODO
			minimal && same_chars > 6
			|| (!minimal) && same_chars > 2
		);
		if(needs_change){
			int j = i;
			for(; j < i + same_chars; j += 2){
				char sim_chr = get_similar_character(str->chrs[j]);
				str->chrs[j] = sim_chr;
			}
			i = j;
		}
	}
	return str;
}

// checks an object of a class with a ->equals(same class) member function against a target of the same class or against a NULL object pointer
template <typename T> bool is_target(T check, T target){
	dbg("utils/is_target: checking: "+_to_str(check)+" against "+_to_str(target), 5);
	bool result = false;
	result = result || target == check;
	if(target != (T)NULL){
		result = result || target->equals(check);
	}
	dbg("utils/is_target: result == "+_to_str_bool(result), 5);
	return result;
}
// replaces all elements of the array that fit target see utils/is_target for replacement
template <typename T> void clear_empty_or_NULL_element(T* arr, int arr_len, T target, T replacement = NULL){
	dbg("utils/clear_empty_or_NULL_element: target == "+((long int)target != (long int)empty_CC_str ? _to_str(target) : "empty_CC_str"), 5);
	int counter = 0;
	int shift[arr_len]; //keeping how much to shift each element back after the first loop
	for(int i = 0; i < arr_len; shift[i++] = 0);
	for(int i = 0; i < arr_len; i++){
		shift[i] = counter;
		if(is_target<T>(arr[i], target)){
			dbg("utils/clear_empty_or_NULL_element: element arr["+_to_str(i)+"] is a target: "+_to_str(arr[i]), 5);
			counter++;
		}
	}
	dbg("utils/clear_empty_or_NULL_element: generated shift table: \t"+_to_str_arr(shift, arr_len, "\t"), 5);
	for(int i = 0; i < arr_len; i++){
		if(shift[i] != 0){
			if(!is_target<T>(arr[i], target)){
				arr[i-shift[i]] = arr[i];
				arr[i] = replacement;
			}
		}
	}
}
// maps a value from one interval to another; code from p5.js library
template <typename T> T map(T value, T from_min, T from_max, T to_min, T to_max){
	return ((value-from_min)/(from_max-from_min))*(to_max-to_min)+to_min;
}

void Log_dbg(string information = ""){
	dbg("Log_dbg: "+information);
	//indentation for readability
	string pre_dbg_line_break = dbg_line_break;
	dbg_line_break += "\t";
	int empty_lines = 0;
	int current_empty_lines = 0;
	for(int i = 0; i < Log_len; i++){
		if(!(Log[i]->equals(empty_CC_str))){
			if(current_empty_lines > 0){
				dbg("Log_dbg: LOG["+_to_str(i-current_empty_lines)+":"+_to_str(i)+"].dbg() are "+_to_str(current_empty_lines)+" 'empty_CC_str's");
				current_empty_lines = 0;
			}
			dbg("Log_dbg: LOG["+_to_str(i)+"] == "+Log[i]->_dbg());
		}else{
			empty_lines++;
			current_empty_lines++;
		}
	}
	if(current_empty_lines > 0){
		dbg("Log_dbg: LOG["+_to_str(Log_len-current_empty_lines)+":end] are "+_to_str(current_empty_lines)+" 'empty_CC_str's");
	}
	//reset indentation
	dbg_line_break = pre_dbg_line_break;
	dbg("Log_dbg: empty_lines == "+_to_str(empty_lines));
}
void Log_add(CC_str* str){ Log[Log_pos++] = str; }
void Log_update_multiple(int n){
	//optimizing instead of this loop seems possible, but not urgent
	for(int j = 0; j < n; j++){
		for(int i = 1; i < Log_len; i++){
			Log[i-1] = Log[i];
		}
		Log[Log_len-1] = new CC_str();
		Log_pos--;
	}
	clear_empty_or_NULL_element(Log, Log_len, empty_CC_str, empty_CC_str);
	// Log_dbg("after update");
}
#define Log_update() Log_update_multiple(1);
void Log_setup(){
	dbg("utils/Log_setup", 5);
	//clearing the Log CC_str* array
	for(int i = 0; i < Log_len; Log[i++] = new CC_str());
}
void Log_print(int x, int y, bool update_ = true){
	print(new CC_str("LOG:", 1), x, y);
	//fixing string length to width; is not useful, always clear before calling Log_print
	// for(int i = 0; i < Log_len; i++){
	// 	if(Log[i]->length() < width){
	// 		Log[i]->append(width - Log[i]->length(), " ");
	// 	}else if(Log[i]->length() > width){
	// 		Log[i] = Log[i]->substr(0, width);
	// 	}
	// }
	print(Log, Log_len, x+1, y+1);
	if(update_){
		Log_update();
	}
}

void set_cursor_pos(int x, int y){ // only in std-output mode (like print_mode == "cout")
	cout << "\e["+_to_str(y+2)+";"+_to_str(x+1)+"H";
}
void print(CC_str* str, int x, int y, bool make_str_ncurses_bug_proof_){
	dbg("utils/print", 5);
	int length = str->chrs.length();
	if(length == 0){
		return;
	}
	if(make_str_ncurses_bug_proof_){
		// this function is returning the string, but it is passed as a pointer and no copying takes place
		make_str_ncurses_bug_proof(str);
	}
	int color = str->clrs[0];
	int last_index = 0;
	for(int i = 0; i <= length; i++){
		if(i == length || str->clrs[i] != color){
			// dbg("utils/print: color == "+_to_str(color));
			// dbg("running!");
			set_color_on(color);
			char out[i-last_index+2];
			CC_str* substring;
			if(i == length){
				substring = str->substr(last_index, length-last_index);
				out[length-last_index] = '\0';
			}else{
				substring = str->substr(last_index, i-last_index+1);
			}
			to_char_pointer(substring->str(), out);
			out[i-last_index+1] = '\0';
			mvprintw(y, x+last_index, out);
			color = str->clrs[i];
			last_index = i;
		}
	}
}
// void print_std(CC_str* str, const int x, const int y){
// 	dbg("utils/print: ", 5);
// 	int length = str->chrs.length();
// 	if(length == 0){
// 		return;
// 	}
// 	int color = str->clrs[0];
// 	int last_index = 0;
// 	char stopper;
// 	for(int i = 0; i <= length; i++){
// 		if(i == length || str->clrs[i] != color){
// 			char out[i-last_index+2];
// 			CC_str* substring;
// 			if(i == length){
// 				substring = str->substr(last_index, i-last_index+1);
// 			}else{
// 				substring = str->substr(last_index, i-last_index+1);
// 			}
// 			set_cursor_pos(x+last_index, y);
// 			cout << substring->str();
// 			set_cursor_pos(x+10, y);
// 			cout << str->int_arr();
// 			set_cursor_pos(x+last_index+20, y);
// 			cout << substring->int_arr();
// 			set_cursor_pos(x+last_index+30, y);
// 			for(int j = last_index; j < i; j++){
// 				cout << (str->clrs[j] == -1 ? 0 : str->clrs[j]);
// 			}
// 			set_cursor_pos(x+last_index+40, y);
// 			for(int j = 0; j < i-last_index; j++){
// 				cout << (substring->clrs[j] == -1 ? 0 : substring->clrs[j]);
// 			}
// 			color = str->clrs[i];
// 			last_index = i;
// 			// cin >> stopper;
// 		}
// 	}
// }

void print(CC_str* arr[], int arr_len, int x, int y){
	for(int i = 0; i < arr_len; i++){
		print(arr[i], x, y+i);
	}
}
char get_char(){
	char inp = '\0';
	if(print_mode == "openFrameworks"){
		cin >> inp;
	}else if(print_mode == "ncurses"){
		inp = getch();
	}
	return inp;
}

double noisy(double x, double y, double z){
	return fmod(
		(
			sin(x-40) //[-1, 1]
			+sin(y) //[-1, 1]+[-1, 1] = [-2, 2]
			+sin(z+40) //[-1, 1]*3 = [-3, 3]
		)/3, //-[3, 3]/3 = [-1, 1]
		2.0 //[-1, 1]%2/2 = [0, 2]/2 = [0, 1]
	)/2;
}
float noise(float x, float y, int n){
	return (PerlinNoise2D(x, y, n) + 1)*0.5;
}
void draw_density(double* arr, int len_of_arr, double resolution, double stretch){
	double min = INFINITY;
	double max = -INFINITY;
	for(int i = 0; i < len_of_arr; i++){
		if(arr[i] < min){	min = arr[i];	}
		if(arr[i] > max){	max = arr[i];	}
	}
	double step = (max - min + 1)/resolution;
	for(double x = min; x < max; x += step){
		int counted = 0;
		for(int i = 0; i < len_of_arr; i++){
			if(x <= arr[i] && arr[i] < x+step){
				counted++;
			}
		}
		cout << f(x) << ".." << fl(x+step) << ":\t" << repeat("#", int(stretch*counted)) << endl;
	}
}
CC_str* match_number_symbol(double num){
	for(int i = 0; i < symbols_num; i++){
		if(num < symbols_ranges[i+1]){
			return new CC_str(symbols[i]);
		}
	}
}
void set_color_on(int color_index){
	if(print_mode == "ncurses"){
		if(color_index <= 0){
			attroff(COLOR_PAIR(current_color));
			current_color = -1;
		}else{
			attron(COLOR_PAIR(color_index%colors_num));
			current_color = color_index%colors_num;
		}
	}else if(print_mode == "openFrameworks"){}
}

template <typename T> int index(T target, T* arr, int arr_len){
	for (int i = 0; i < arr_len; i++){
		if(arr[i] == target){
			return i;
		}
	}
	return -1;
}

string fit_to_symbol(int num, char symb, int digits){
	//The function takes in a number that should be mapped to be in [0, 1] and returns the corresponding code.
	//for the codes, see sett.h, symbols and symbols_ranges
	//they are meant as an abbreviation that can be used where there is no space or need for precise numbers or where these numbers would'nt mean that much.
	dbg("utils/fit_to_symbol ("+_to_str(num)+", "+symb+")", 5);
	string result;
	if(num > 0){
        result += symb;
		if(num > 5){
			result += symbols[7];
		}else if(num > 1){
			result += symbols[2+num];
        }else{
            result.append(digits - 2 + 1, fill_str[0]);
        }
    }else{
        result.append(digits - 1 + 1, fill_str[0]);
    }
	return result;
}
string fit_to_symbol(int num, char symb){ return fit_to_symbol(num, symb, 3); }

double time_ms(){
	return (double)clock()/CLOCKS_PER_SEC*1000000;
}

#endif

//110 SLOC
