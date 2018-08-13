#ifndef SETT_H
#define SETT_H
#include <string>
#include <ncurses.h>

using namespace std;

//If you were to change (add to, remove from) the color list, change the colors_len too.
static const int colors_num = 7;//8;
#include "CC_str.h"

static 			int 	dbg_lvl = 0;
static 			bool 	dbg_set_up = false;

static 	const 	int 	players_num_max 	= 	5; //number of players
static 			int 	players_num 		= 	1; //number of players
static 			int 	user_index 			= 	0;
// some have to be constants since they are used as array sizes for class members (see Game.h, Tile.h, CC_str*[].h)
static 	const 	int 	game_size_y 		= 	5; //size of the board that the game holds
static 	const 	int 	game_size_x 		=  12;
//how many rows of strings will be saved in the Tile->_print(result*) procedure
static 	const 	int 	tile_size_y 		= 	4;
static 	const 	int 	tile_size_x 		=  12;
static 			Vector* game_size 			= new Vector(game_size_x, game_size_y);
static 			Vector* tile_size 			= new Vector(tile_size_x, tile_size_y);
		const 	int		max_moves			= 256; // a limit on the number of moves that can be stored at one time
static 			int		players_min_dist 	= 	5;
static 			string 	fill_str 			= " ";
static 			CC_str* frame_chr 			= new CC_str("|");
static 			string 	frame_cross 		= "+";
static 			string 	print_mode 			= "ncurses";
// static 			string 	print_mode 			= "openFrameworks";
//the windowsize can be reset by init_print(), dependig of the print_mode
static 	const 	Vector* WINDOWSIZE 			= new Vector(170, 45); //GNOME TERMINAL: 170x45
static 	const 	double 	e 					= 	2.71828;
static 	const 	int 	Log_len 			=  10;
static 			CC_str* Log[Log_len];
static 			int 	Log_pos 			= 	0; //the place of an empty line in the Log.

static 	const 	int 	wait_for_move_len 	= 	2;
static			char 	wait_for_move_keys[wait_for_move_len] = {
	'm',
	'a',
};
static			double 	wait_for_move[wait_for_move_len] = {
	0.1,
	0.3,
};

static 	const 	int 	player_name_num 	=  20;
static 			CC_str* player_names[player_name_num] = {
	new CC_str("Mathilde"),
	new CC_str("Wieland"),
	new CC_str("Alma"),
	new CC_str("Theo"),
	new CC_str("Karl"),

	new CC_str("Clara"),
	new CC_str("Magdalena"),
	new CC_str("Tobias"),
	new CC_str("Marlen"),
	new CC_str("Andreas"),

	new CC_str("Kurt"),
	new CC_str("Maximilian"),
	new CC_str("Marx"),
	new CC_str("Engels"),
	new CC_str("Lenin"),

	new CC_str("Stalin"),
	new CC_str("Hitler"),
	new CC_str("Mussolini"),
	new CC_str("Franco"),
	new CC_str("Trump"),
};

static	const	int 	fg_colors[colors_num+1] = {
	COLOR_WHITE,
	COLOR_BLACK,
	COLOR_BLACK,
	COLOR_WHITE,
	COLOR_WHITE,
	COLOR_BLACK,
	COLOR_BLACK,
	COLOR_WHITE,
};
static	const	int		bg_colors[colors_num+1] = {
	COLOR_RED,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_BLUE,
	COLOR_MAGENTA,
	COLOR_CYAN,
	COLOR_WHITE,
	COLOR_BLACK,
};
				CC_str* color_names[colors_num+1] = {
	new CC_str("red", 1),
	new CC_str("green", 2),
	new CC_str("yellow", 3),
	new CC_str("blue", 4),
	new CC_str("magenta", 5),
	new CC_str("cyan", 6),
	new CC_str("white", 7),
	new CC_str("black", -1),
};
//index in the color arrays; -1 = default of terminal
static 			int 	current_color 		=  -1;

static 	const 	int 	symbols_num 		= 	9;
//symbols are used, where numbers are converted to smaller symbols of maximum length 2
static			string	symbols[symbols_num] = {
	"-#",
	"-!",
	"--",
	"- ",
	"0 ",
	"+ ",
	"++",
	"+!",
	"+#",
};
//the numbers represented by the symbols (symbols_ranges[0] <= symbols[0] < symbols_ranges[1] <= symbols[1] < ...)
static			double	symbols_ranges[symbols_num+1] = {
	-INFINITY,
	-0.2,
	0.0,
	0.3,
	0.5,
	0.7,
	1.0,
	1.2,
	INFINITY,
};
static 	const 	char 	general_symbol 		= 'G';
static 	const 	char 	    spy_symbol 		= 's';

// #include "Vector.h"
static 			Vector* WASD[4] = {
	new Vector(0, -1),
	new Vector(-1, 0),
	new Vector( 0, 1),
	new Vector( 1, 0),
};

static 	const 	int 	final_screen_len 	=  11;
static 			CC_str* final_screen[final_screen_len] = {
	new CC_str("/--------------------------------\\"), //1
	new CC_str("||                              ||"), //2
	new CC_str("||         W E L C O M E        ||"), //3
	new CC_str("||            to  the           ||"), //4
	new CC_str("||     F I N A L  S C R E E N   ||"), //5
	new CC_str("||              of              ||"), //6
	new CC_str("||        G E N E R A L S       ||"), //7
	new CC_str("||           by Alamius         ||"), //8
	new CC_str("||                              ||"), //9
	new CC_str("||                              ||"), //10
    new CC_str("\\=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-/"), //11
};

static	const	int		similar_characters_len = 10;
static			char	similar_characters[similar_characters_len] = {
	'-', '~',
	'+', '*',
	'.', ',',
	'=', '#',
	' ', '`',
};

#endif

//35 SLOC
