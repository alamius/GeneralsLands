#ifndef CC_H
#define CC_H

typedef basic_string<char> chr_str;
typedef basic_string<int > clr_str;

void set_color_on(int color_index, int intensity);
void set_color_on(int color_index);
void dbg(string);
void dbg(string, int);

clr_str empty_clr_str = clr_str();

int confine_color(int color){
	if(color <= 0){
		return -1;
	}else if(color >= colors_num){
		return color % colors_num;
	}else{
		return color;
	}
}

class CC_str {
	//designed to hold two "basic_string"s (like vectors, distantly related to arrays) of characters and colors to represent colored strings
public:
	chr_str chrs;
	clr_str clrs;
	CC_str(string, int*);
	CC_str(string, char*); //encoding the colors (2nd arg) as '\x01' or '1' for clr = 1
	CC_str(string, int);
	CC_str(char, int);
	CC_str(char);
	CC_str(string);
	CC_str(chr_str, clr_str);
	CC_str();
	CC_str(CC_str*);
	template <typename T>CC_str(T);
	string  _dbg();
	string 	str();
	string  int_arr();
	CC_str* append(string);
	CC_str* append(CC_str*);
	template <typename T>CC_str* append(int, T);
	int 	length();
	CC_str* substr(int where, int length);
	bool 	equals(CC_str* cc_str);
	int		find(chr_str, int);
	int		find(clr_str, int);
	int		find(CC_str*, int);
	// CC_str*	replace(int, CC_str*); //not fully tested!
};
CC_str::CC_str(char chr_, int clr_){
	dbg("CC_str(char == '"+_to_str(chr_)+"', int == "+_to_str(clr_)+")", 5);
	chrs.append(1, chr_);
	clrs.append(1, confine_color(clr_));
}
CC_str::CC_str(char chr_){
	dbg("CC_str(char == '"+_to_str(chr_)+"')", 5);
	chrs.append(1, chr_);
	clrs.append(1, -1);
}
CC_str::CC_str(string chrs_, int* clrs_){
	dbg("CC_str(string, int*)", 5);
	int len = chrs_.length();
	for(int i = 0; i < chrs_.length(); i++){
		chrs.append(1, chrs_[i]);
		int clr = confine_color(clrs_[i]);
		clrs.append(1, clr);
	}
}
CC_str::CC_str(string chrs_, char* clrs_){
	// encoding for this constructor's 2nd argument
	// colors [0, 1, 5, 22, 12] either as "0\x01\x05\x16\x0c" or as "015lb"
	// don't use '\x00' === '\0' === (char)0 === NULL! to encode color -1/default, use '0', normal colors start at 1
	// '\0' is the end of string
	// the colors are (in the moment of notation) limited to [1..7] (0 = terminal default)
	dbg("CC_str(string == \""+chrs_.substr(0, 30)+"\", char* == \""+((string)clrs_).substr(0, 30)+"\")", 5);
	int length = chrs_.length();
	chrs.append(chrs_);
	int zero = (int)('0');
	int a = (int)('a');
	int A = (int)('A');
	int i = 0;
	while(i < length){
		int clr = (int)clrs_[i];
		if(clr == 0){ //'\x00'
			break;
		}else if(clrs_[i] == '-'){ //'-'
			clr = -1;
		}else if(zero <= clr && clr <= (int)('9')){ //'0-9'
			clr = clr - zero;
		}else if(a <= clr && clr <= (int)('z')){ //'a-z'
			clr = clr - a + 10;
		}else if(A <= clr && clr <= (int)('Z')){ //'A-Z'
			clr = clr - A + 36;
		}else{
			clr = -1;
		}
		clr = confine_color(clr);
		clrs.append(&clr);
		i++;
	}
	clrs.append(length-i, -1);
}
CC_str::CC_str(string chrs_, int clr_){
	dbg("CC_str(string == \""+chrs_.substr(0, 30)+"\", int == "+_to_str(clr_)+")", 5);
	chrs.append(chrs_);
	clrs.append(chrs_.length(), confine_color(clr_));
}
CC_str::CC_str(string chrs_){
	dbg("CC_str(string == \""+chrs_.substr(0, 30)+"\")", 5);
	chrs.append(chrs_);
	clrs.append(chrs_.length(), -1);
}
CC_str::CC_str(chr_str chrs_, clr_str clrs_){
	dbg("CC_str(chr_str == \""+chrs_.substr(0, 20)+"\", clr_str == {"+_to_str_arr(clrs_.c_str(), clrs_.length(), ", ")+"})", 5);
	chrs.append(chrs_);
	for(int i = 0; i < chrs_.length(); i++){
		int clr = confine_color(clrs_[i]);
		clrs.append(1, clr);
	}
}
CC_str::CC_str(){
	dbg("CC_str()", 5);
	chrs[0] = '\0';
	clrs[0] = 0;
}
CC_str::CC_str(CC_str* cc_str){
	dbg("CC_str(CC_str* == "+cc_str->_dbg().substr(0, 60)+")", 5);
	chrs = chr_str(cc_str->chrs);
	clrs = clr_str(cc_str->clrs);
}
template <typename T>CC_str::CC_str(T arg){
	dbg("CC_str(anything =str=> \""+_to_str(arg).substr(0, 30)+"\")", 5);
	string chrs_ = _to_str(arg);
	chrs.append(chrs_);
	clrs.append(chrs_.length(), -1);
}
string  CC_str::_dbg(){
	dbg("CC_str/dbg", 6);
	string result = "CC_str{";
	result += "chars: '"+str()+"'; ";
	result += "colors: '"+int_arr()+"'; ";
	result += "}";
	return result;
}
string  CC_str::str(){
	dbg("CC_str/str", 6);
	return (string)chrs;
}
string  CC_str::int_arr(){
	dbg("CC_str/int_arr", 6);
	string result = "";
	int zero = (int)'0';
	int a = (int)'a';
	int A = (int)'A';
	for(int i = 0; i < clrs.length(); i++){
		if(clrs[i] == -1){
			result.append("-");
		}else if(0 <= clrs[i] && clrs[i] < 10){
			result.append(1, (char)(zero+clrs[i]));
		}else if(10 <= clrs[i] && clrs[i] < 36){
			result.append(1, (char)(a+clrs[i]-10));
		}else if(36 <= clrs[i] && clrs[i] < 62){
			result.append(1, (char)(A+clrs[i]-36));
		}else{
			result.append("?");
		}
	}
	return result;
}
CC_str* CC_str::append(string str){
	dbg("CC_str/append(str == \'"+str+"\')", 5);
	chrs.append(str);
	clrs.append(str.length(), -1);
	return this;
}
CC_str* CC_str::append(CC_str* cc_str){
	dbg("CC_str/append(CC_str == "+cc_str->_dbg()+")", 5);
	for(int i = 0; i < cc_str->length(); i++){
		chrs.append(1, cc_str->chrs[i]);
		clrs.append(1, cc_str->clrs[i]);
	}
	return this;
}
template <typename T>CC_str* CC_str::append(int count, T element){
	dbg("CC_str/append(int, T)", 5);
	for(int i = 0; i < count; i++){
		append(element);
	}
}
int  	CC_str::length(){
	return chrs.length();
}
CC_str* CC_str::substr(int where, int len){
	// dbg("CC_str/substr", 5);
	if(where >= length()){
		where = length();
	}
	if(len == -1){
		len = length() - where;
	}
	dbg("CC_str/substr(where == "+_to_str(where)+", len == "+_to_str(len)+")", 5);
	return new CC_str(
		chrs.substr(where, len),
		clrs.substr(where, len)
	);
}
bool	CC_str::equals(CC_str* cc_str){
	return (cc_str == (CC_str*)NULL) ? false : (cc_str->chrs == chrs && cc_str->clrs == clrs);
}
int		CC_str::find(chr_str search, int pos = 0){
	return chrs.find(search, pos);
}
int		CC_str::find(clr_str search, int pos = 0){
	return clrs.find(search, pos);
}
int		CC_str::find(CC_str* search, int pos = 0){
	int len = search->length();
	int i = pos;
	while(i < length()-len){
		i = find(search->chrs, i+1);
		if(find(search->chrs, i) == i){
			return i;
		}
	}
	return -1;
}
// CC_str*	CC_str::replace(int index, CC_str* str){
// 	dbg("CC_str/replace(index == "+_to_str(index)+", str == "+str->_dbg().substr(0, 60)+")");
// 	// chrs.replace(index, str->chrs);
// 	// clrs.replace(index, str->clrs);
// 	CC_str* self = new CC_str(this);
// 	CC_str* substring = substr(0, index);
// 	chrs = substring->chrs;
// 	clrs = substring->clrs;
// 	append(str);
// 	substring = substr(index+str->length(), -1);
// 	append(substring);
// 	return this;
// }

CC_str* operator+ (string str, CC_str* cc_str){
	clr_str result_clrs = clr_str();
	result_clrs.append(str.length(), -1);
	result_clrs.append(cc_str->clrs);
	return new CC_str(
		str.append(cc_str->chrs),
		result_clrs
	);
}

CC_str* empty_CC_str = new CC_str(); //use just for comparison and for easier debugging. Don't use when initilizing an array!

#endif
