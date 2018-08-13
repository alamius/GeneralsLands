#ifndef MOVE_H
#define MOVE_H
class Vector;
#include "Player.h"

class Move {
public:
    Vector* from;
    Vector* to;
    Player* player;
    double exec_time; //when it was set
    double power; //how much of the players military will be moved
    Move(Vector* from_, Vector* to_, Player* player_, double power_, double time_, double wait_);
    CC_str* to_str();
    string  _dbg();
    bool    equals(Move* move){ return false; } // a useless function needed for compatibility with utils/clear_empty_or_NULL_element
};

Move::Move(Vector* from_, Vector* to_, Player* player_, double power_, double time_, double wait_){
    dbg("Move/init", 4);
    from = from_;
    to = to_;
    player = player_;
    power = min(max(power_, 0.0), 1.0);
    exec_time = time_ + wait_;
}
CC_str* Move::to_str(){
    dbg("Move/to_str", 4);
    CC_str* result;
    result->append(
        "Move{from:"
        +from->to_str()
        +", to:"
        +  to->to_str()
        +", player:"
        +player_name(player)
    );
    result->append("}");
    return result;
}
string  Move::_dbg(){
    dbg("Move/to_str", 5);
    string result = "Move{";
    result += "from:"+from->to_str();
    result += ", to:"+  to->to_str();
    result += ", player:"+player_name(player)->chrs;
    result += ", power:"+_to_str(power);
    // result += ", time set for execution:"+_to_str(exec_time);
    // result += ", wait_left:"+_to_str(time_ms()-exec_time);
    result += "}";
    return result;
}
#endif
