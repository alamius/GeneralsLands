#ifndef GAME_H
#define GAME_H
#include "sett.h"
// #include "Player.h"
// #include "Tile.h"
class Tile;
class Player;
#include "Move.h"

class Game{
// private:
public:
    Player* players[players_num_max];
    Tile* tiles[game_size_y][game_size_x];
    int moves_index;
    Move* moves[max_moves];
    Game();
    bool    position_conflict(Vector*, int);
    Tile*   get_players_tile(int player_index);
    Tile*   get_tile(Vector*);
    void    update();
    void    _print(CC_str** result, Vector* from, Vector* to); //writes to result -> no output
    void    random_moves(int number, double time);
    bool    check_tiles(bool dbg_);
    void    moves_add(Move* move);
    //filtering the tiles that fit the Player or the Tile or both (if one is t_size 1, it means for him is not filtered, both selects every Tile)
    int     get_statistics(CC_str** result, Player* player, Tile* tile);
    // virtual ~Game ();
};

#include "Game.cpp"

#endif
