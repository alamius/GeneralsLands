#include <iostream>
// #include <cstdlib>
// #include <cstddef>
// #include <stdlib.h>
// #include <math.h>
#include <string>
// #include <cstring>
#include <sstream>
#include <fstream>
#include <ncurses.h>
#include <unistd.h>
#include <limits>
// #include <stdio.h>
// #include <stdlib.h>
// #include <vector>
#include "Vector.h"
#include "sett.h"
#include "utils.h"

#include "Game.h"
#include "Move.h"
#include "CC_str.h"
// #include "Tile.h"
#include "Player.h"
#include "defines.h"
// #include "Game.cpp"
// #include "Tile.cpp"
// #include "Player.cpp"
#include "perlin.cpp"

using namespace std;

void init_print(){
	dbg("init_print: print_mode == "+print_mode, 2);
	if(print_mode == "cout"){
		//depecated: mode "cout"
	}else if(print_mode == "ncurses"){
		initscr();                      /* Start curses mode              */
		// cbreak();                       // dont't wait for [ENTER]
		halfdelay(1);                  // wait for arg/10 seconds for user input before stopping and returning ERR
		noecho();                       // no standard output of user input to the screen
		keypad(stdscr, TRUE);           // enables reading keys like F1, Arrows, ...
		start_color();
		for(int i = 0; i < colors_num; i++){
			// foreground and background colors from sett.h
			init_pair(i+1, fg_colors[i], bg_colors[i]);
		}
		int x, y;
		getmaxyx(stdscr, y, x);
		WINDOWSIZE = new Vector(x, y);
	}
}
void end_print(){
	if(print_mode == "cout"){
		// screen_clear();
		usleep(1000000);
	}else if(print_mode == "ncurses"){
		halfdelay(1000);
		getch();
		endwin();                       /* End curses mode                */
		// cout << endl;
		dbg("program finished.");
	}
}
void test_PerlinNoise2D(){
	// from the file main.cpp or main_perlin.cpp from the perlin noise code by Ian Parberry
	const float x = 7777;
    const float y = 9999;
	float di = 0.2;
	float radius = 5;
    for(float i = 0; i < 6.28; i += di){
    	float pnoise = noise(x+cos(i)*radius, y+sin(i)*radius, 1);
		dbg(_to_str(pnoise));
		print(new CC_str(chr_str().append((int)(pnoise*WINDOWSIZE->x*0.9), '#')), 0, i/di);
    }
	int noise_num = 1000;
	double noise_arr[noise_num*noise_num];
	for(float y = 0; y < noise_num; y++){
		for(float x = 0; x < noise_num; x++){
			double n = noise(x, y, 1);
			// cout << fl(n, 15);
			noise_arr[(int)(x*noise_num+y)] = n;
		}
		// cout << endl;
	}
	//density analysis of the generated random number array -> histogram-like
	draw_density(noise_arr, noise_num, 30, 0.5);
}
void test_CC_str_constructors(){
	int clr[3] = {2, 1, 1};
	CC_str* str[10] = {
		new CC_str(),
		new CC_str("BLA"),
		new CC_str("BLA", 2),
		new CC_str("BLA", "2a-"),
		new CC_str("BLA", "2\0"),
		new CC_str("BLA", clr),
		new CC_str('b', 1),
		new CC_str(basic_string<char>("BLA"), basic_string<int>(clr)),
		new CC_str(),
		new CC_str(),
	};
	str[8] = new CC_str(str[7]->substr(1, 2)),
	str[9] = new CC_str(str[3]);
	system("clear");
	int x = 2;
	int y = 2;
	// for(int i = 0; i < 10; i++){
	// // 	dbg("str["+_to_str(i)+"]->_dbg == "+str[i]->_dbg());
	// 	// print_std(str[i], 2, 2+i);
	// }
	print(str, 10, 2, 2);
}
// void test_CC_str_2(){
// 	static const int arr_len = 80;
// 	CC_str* arr[arr_len];
// 	CC_str* cc_ch;
// 	for(int j = 0; j < 30; j++){
// 		arr[j] = new CC_str();
// 		for(int i = 0; i < arr_len; i++){
// 			cc_ch = new CC_str((char)i+33, i+j-5);
// 			arr[j]->append(cc_ch);
// 			// print(cc_ch, i, j); //printing each character
// 		}
// 		// cout << arr[j]->chrs << endl; // printing each row
// 		// print(arr[j], 2, j+5);
// 	}
// 	print(arr, 30, 2, 5); //printing all at once (also just a loop over the rows)
// }
void test_CC_str_methods(){
	dbg(				"test_CC_str_methods");
	int clr[3] = {2, 1, 1};
	CC_str* C[3] = {
		new CC_str("HIG", basic_string<int>(clr)),
		new CC_str("ABCDEFG", "0123456"),
		new CC_str(),
	};
	C[2] = new CC_str(C[0]);
	dbg_obj_arr(		"test_CC_str_methods: ", "C", C, 3);
	print(C, 3, 22, 2);
	dbg(				"test_CC_str_methods: C[0] equals C[2]: "+_to_str_bool(C[0]->equals(C[2])));
	C[0]->append(C[1]);
	dbg_obj_arr(		"test_CC_str_methods: ", "C", C, 3);
	print(C, 3, 22, 6);
	dbg(				"test_CC_str_methods: C[0] equals C[2]: "+_to_str_bool(C[0]->equals(C[2])));
	CC_str* substr_1 = C[0]->substr(0, 3);
	dbg(				"test_CC_str_methods: C[0][0:3] (C[0]->substr(0, 3)) == "+substr_1->_dbg());
	dbg(				"test_CC_str_methods: C[0][0:3] equals C[2]: "+_to_str_bool(substr_1->equals(C[2])));
	substr_1 = new CC_str("HIJ", "345");
	dbg(				"test_CC_str_methods: additional string 1: "+substr_1->_dbg());
	// dbg_lvl = 5;
	CC_str* copy_C_2 = new CC_str(C[2]);
	C[2]->append("...")->append(substr_1)->append(copy_C_2);
	// C[2]->chrs[8] = 'G';
	dbg_obj_arr(		"test_CC_str_methods: ", "C", C, 3);
	print(C, 3, 22, 10);
	int index_1 = C[2]->find("HIG");
	dbg(				"test_CC_str_methods: find by chars: C[2]->find('HIG') == "+_to_str(index_1)
						+", found substring: "+C[2]->substr(index_1, 3)->_dbg()
	+"");
	int index_2 = C[2]->find(substr_1->clrs);
	dbg(				"test_CC_str_methods: find by ints:  C[2]->find('"+substr_1->int_arr()+"') == "+_to_str(index_2)
						+", found substring: "+C[2]->substr(index_2, substr_1->length())->_dbg()
	+"");
	int index_3 = C[2]->find(substr_1);
	dbg(				"test_CC_str_methods: find by both:  C[2]->find('"+substr_1->_dbg()+    "') == "+_to_str(index_3)
						+", found substring: "+C[2]->substr(index_3, substr_1->length())->_dbg()
	+"");
}
void test_CC_str(){
	test_CC_str_constructors();
	test_CC_str_methods();
	// return;
	Log_add(new CC_str("WAITING: ", 1));
	Log_print(5, min(game_size_y, (int)WINDOWSIZE->y/tile_size_y)*tile_size_y+6);
	get_char();
	clear_empty_or_NULL_element(Log, Log_len, new CC_str("WAITING: ", 1));
	Log_print(5, min(game_size_y, (int)WINDOWSIZE->y/tile_size_y)*tile_size_y+6);
}
void test_time_ms(){
	double time_1 = time_ms();
	Log_add(new CC_str("CURRENT TIME: "+_to_str(time_1)));
	// Log_print(5, 5, false);
	// refresh();
	usleep(1000000);
	// get_char();
	double time_2 = time_ms();
	Log_add(new CC_str("CURRENT TIME: "+_to_str(time_2)));
	Log_add(new CC_str("DIFFERENCE: "+_to_str(time_2-time_1)));
	Log_print(5, 5, false);
	// Log_update_multiple(3);
	refresh();
	// get_char();
}
void test_Log(){
	char inp;
	Log_add(new CC_str("ALWAYS"));
	clear();
	Log_print(5, 5);
	refresh();
	inp = get_char();
	Log_add(new CC_str("THREE"));
	Log_add(new CC_str("LINES"));
	clear();
	Log_print(5, 5);
	refresh();
	inp = get_char();
}
void test_clear_empty_or_NULL_element(){
	dbg("test_clear_empty_or_NULL_element");
	const int arr_len = 6;
	CC_str* arr[arr_len];
	for(int i = 0; i < arr_len; arr[i++] = NULL);
	arr[1] = new CC_str("HI");
	arr[3] = new CC_str("TEST");
	arr[5] = new CC_str("COMPLEX", "111555\0");
	arr[0] = new CC_str();
	arr[2] = new CC_str();
	dbg("test_clear_empty_or_NULL_element: unchanged array: ");
	dbg_obj_arr("test_clear_empty_or_NULL_element: ", "arr", arr, arr_len);
	clear_empty_or_NULL_element(arr, arr_len, (CC_str*)NULL);
	dbg("test_clear_empty_or_NULL_element: cleared of NULL: ");
	dbg_obj_arr("test_clear_empty_or_NULL_element: ", "arr", arr, arr_len);
	clear_empty_or_NULL_element(arr, arr_len, empty_CC_str);
	dbg("test_clear_empty_or_NULL_element: cumulatively cleared of empty_CC_str: ");
	dbg_obj_arr("test_clear_empty_or_NULL_element: ", "arr", arr, arr_len);
	dbg("test_clear_empty_or_NULL_element: done");
}
void test_make_str_ncurses_bug_proof(){
	dbg("test_make_str_ncurses_bug_proof", 0);
	CC_str* str1 = new CC_str("------", "-12-12");
	CC_str* str2 = new CC_str("AAAAAA", "-123-1");
	CC_str* str1_c = make_str_ncurses_bug_proof(new CC_str(str1));
	CC_str* str2_c = make_str_ncurses_bug_proof(new CC_str(str2));
	CC_str* unchanged = new CC_str("UNCHANGED: ", "111111111\0");
	CC_str*   changed = new CC_str("  CHANGED: ", "--1111111\0");
	print((new CC_str(unchanged))->append(str1), 20, 2, false);
	print((new CC_str(  changed))->append(str1_c), 20, 3, false);

	print((new CC_str(unchanged))->append(str2), 20, 5, false);
	print((new CC_str(  changed))->append(str2_c), 20, 6, false);
}
void test_utils(){
	test_make_str_ncurses_bug_proof();
	return;
	test_clear_empty_or_NULL_element();
	test_Log();
	test_Log();
	test_Log();
	test_time_ms();
	test_time_ms();
	test_time_ms();
}

float g_fMu = 1.0f; //some value for PerlinNoise2D

int main(int argc, char *argv[]){
	clear_dbg();
	int start_time = time_ms();
	// int seed = 1;
	int seed = start_time;
	srand(seed);
	//The function PerlinNoise2D(x, y, n) returns values somewhere roughly in [-0.5 .. 0.5]. (not always completely bound), use noise(x, y, n)
	//x, y are the coorinates, n is te the detail of the noise
	initPerlin2D();
	init_print();
	Log_setup();

	// mvprintw(2, 2, "AAAAAA");
	// mvprintw(3, 2, "AAA");
	// // set_color_on(1);
	// mvprintw(3, 5, "AAAA");
	//
	// mvprintw(6, 2, "AAAAAA");
	// mvprintw(7, 2, "AAA");
	// // set_color_on(1);
	// mvprintw(7, 6, "AAAA");
	//
	// mvprintw(10, 2, "AAAAAA");
	// mvprintw(11, 2, "AAA");
	// set_color_on(1);
	// mvprintw(11, 5, "AAAA");
	// refresh();
	// dbg_obj_arr("main: ", "final_screen", final_screen, final_screen_len);
	// print(final_screen, final_screen_len, 2, 2);

	// test_utils();
	// test_CC_str();
	// test_time_ms();
	// test_time_ms();
	// test_time_ms();
	// test_make_str_ncurses_bug_proof();
	//
	// end_print();
	// exit(0);

	Game* game = new Game();
	Player* user = game->players[user_index];

	Log_add((new CC_str())->append("The Game starts. Your Name is ")->append(user->name)->append("."));
	Log_add((new CC_str())->append("Your color is ")->append(new CC_str("this", user->color))->append("."));
	dbg("main: user->color == "+_to_str(user->color), 5);

	// Vectors telling the game what rectangle of tiles to put into the thing for the game-print (printed at the end)
	Vector* from = new Vector(0, 0);
	Vector* to = new Vector(
		min(game_size_x, (int)WINDOWSIZE->x/(tile_size_x-1)),
		min(game_size_y, (int)WINDOWSIZE->y/tile_size_y)
	);
	// the game-thing printed at the end
	int game_print_size = max((to->y-from->y)*tile_size_y+1, 22.0f/* height of two statistics */);
	CC_str* game_print[game_print_size];
	CC_str* statistics[game_print_size];
	for(int y = 0; y < game_print_size; game_print[y++] = new CC_str());
	for(int y = 0; y < game_print_size; statistics[y++] = new CC_str());

	int direction = -1; //0 = WASD[0], 1 = WASD[1], ..., -1 = no movement
	char move_mode = 'm'; //move the cursor around
	/*	'm' - move only cursor
 		'a' - attack/send military
		'i' - inspect: dump information
 		'c' - run a check over the whole board (see Game/check_tiles)
		*/

	CC_str* user_position_info = new CC_str("User position: ", 4);

	double total_time = -1; // ms
	double last_frame_time = time_ms();
	char inp, inp_peek;
	while(last_frame_time - start_time < total_time || total_time <= 0){
		game->_print(game_print, from, to);
		//TODO: printing always looses some characters... tile should have width 12, chrs have 10, clrs have 12. still, colors are not shifting around
		// dbg("main: length of one row of the game_print: "+_to_str(game_print[0]->length()));
		// dbg("main: what it should be: "+_to_str(tile_size_x*(to->x-from->x)+1));
		int statistics_length_1 = game->get_statistics(game_print, NULL, (Tile*)1);
		int statistics_length_2 = game->get_statistics(statistics, (Player*)1, game->get_tile(user->pos));
		for(int i = 0; i < statistics_length_2; game_print[statistics_length_1+i++]->append(statistics[i]));
		print(game_print, game_print_size, 2, 2);
		for(int y = 0; y < game_print_size; game_print[y++] = new CC_str());
		for(int y = 0; y < game_print_size; statistics[y++] = new CC_str());
		Log_print(5, game_print_size+5);

		game->update();
		// game->random_moves(3, time_ms());

		inp = 'e';
		inp = get_char();
		// //clear input stream
		// cin.clear();
		// // while(!cin.fail()){
		// while(inp_peek != '\xff'){//'\xff' == EOF
		// 	inp_peek = get_char();
		// 	dbg("main: get_char() == "+_to_str(inp_peek)+"; cin.peek() == "+_to_str((char)cin.peek()));
		// }
		// dbg("main: cin cleared");

		direction = -1; //std: no move
		if(inp == 'q'){ break; } //kills the loop -> final screen
		if(inp == 'w'){		if(move_mode == 'a'){ move_mode = 'm'; };	direction = 0; 		}else
		if(inp == 'a'){		if(move_mode == 'a'){ move_mode = 'm'; };	direction = 1; 		}else
		if(inp == 's'){		if(move_mode == 'a'){ move_mode = 'm'; };	direction = 2; 		}else
		if(inp == 'd'){		if(move_mode == 'a'){ move_mode = 'm'; };	direction = 3; 		}else
		if(inp == 'W'){		move_mode = 'a'; direction = 0; 	}else
		if(inp == 'A'){		move_mode = 'a'; direction = 1;		}else
		if(inp == 'S'){		move_mode = 'a'; direction = 2;		}else
		if(inp == 'D'){		move_mode = 'a'; direction = 3;  	}else
		if(inp == 'I'){
			move_mode = 'i'; //inspect -> dumps info about tile to debug
		}else if(inp == 'C'){
			move_mode = 'c'; //check -> dumps info about game tiles to debug
		}
		print((new CC_str(user_position_info))->append(new CC_str(user->pos->to_str(), 3)), 1, 1);

		//all the following are only executed if a move of cursor happened
		if(direction != -1){
			Vector* old_pos = new Vector(user->pos);
			user->pos->_add(WASD[direction]);
			if(user->pos->in_ranges(game_size)){
				//attacking from the old tile to the new
				if(move_mode == 'a'){
					bool attack_success = game->get_tile(old_pos)->attack(user, user->pos, 1);
					if(!attack_success){
						user->pos = new Vector(old_pos);
					}
				}
			}else{
				user->pos = new Vector(old_pos);
			}
		}
		//inspecting the current tile
		if(move_mode == 'i'){
			dbg("inspecting:\n"+game->get_tile(user->pos)->to_str()->str(), 2);
		}else if(move_mode == 'c'){
			game->check_tiles(true);
		}

		// sleeping so long that the frame lasts precisely "usleep(?X? - frame_duration)" -> X microseconds (millionths) or more, is needed "max(_, 0)"
		double frame_duration = 0; //(time_ms() - last_frame_time);
		// print(new CC_str("REFRESH", 1), 1, 1);
		// refresh();
		clear();
		// dbg("main: sleeping for 200 000 - ("+_to_str(time_ms())+" - "+_to_str(last_frame_time)+" == "+_to_str(frame_duration)+")µs!");
		usleep(200000 - (unsigned int)min(max(frame_duration, 0.0), 200000.0));
		last_frame_time = time_ms();
	}
	if(time_ms() - start_time > total_time){
		Log_add(new CC_str("TIME RAN OUT.", 3));
	}

	clear();
	print(final_screen, final_screen_len, 10, 4);
	Log_print(5, game_print_size+5);
	move(23, 0);

	end_print();
	return 0;
}
