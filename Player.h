#ifndef PLAYER_H
#define PLAYER_H

class Player{
// private:
public:
    int color;
    CC_str* name;
    Vector* pos;
    double brutality;
    Player(int color_, CC_str* name_, Vector* pos_);
    CC_str* to_str();
};

CC_str* player_name(Player* player){
    return (player ? player->name : new CC_str("no player"));
}

#include "Player.cpp"
#endif
