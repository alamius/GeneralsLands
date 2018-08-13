#ifndef TILE_H
#define TILE_H
class Game;
class Player;
// class CC_str*[];
// #include "perlin.h"

class Tile{
public:
    Vector* pos;
    Player* owner;
    Game* game;
    double  fertility, //in [-1, 2]
            roughness; //in [0, 3]
    double  civilians, //in |R|
            military;  //in |R|
    int     generals,  //in |N|
            spies,     //in |N|
            spies_made,
            townhood;  //in {0, 1} //used as factor->has to be used as (4*townhood + 1)*something
    int     spies_moving[players_num_max]; //in |N|
    bool locked; //whether moves are possible from this tile; true when attacked
    double attack_WASDE[5];
    /* The standard amount of military the Tile sends to it's neighbors, absolute;
     * 0th-3rd listed as WASD implies, 4th is the amount the Tile should keep
     * if the military of the Tile is not big enough,
     *      but the 4th value is met, what is left will be distributed according to the proportions of the demands (0th-3rd)
     *      else, nothing is sent
     * active during Tile->update
     */
    Tile(int, int, Game*);
    Tile(Vector*, Game*);
    void    std_set();
    void    update();
    CC_str*  to_str();
    CC_str**_print(CC_str**); //procedure writes to pointer CC_str* array of length tile_size_y
    bool    attack(Player*, Vector*, double);
    // virtual ~Tile ();
};

#include "Tile.cpp"

#endif
