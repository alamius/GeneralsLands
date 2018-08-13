#ifndef PLAYER_CPP
#define PLAYER_CPP

Player::Player(int color_, CC_str* name_, Vector* pos_){
    dbg("Player/init", 4);
    color = color_;
    name = name_;
    pos = new Vector(pos_);
    brutality = 1;
}
CC_str* Player::to_str(){
    dbg("Player/to_str", 4);
    CC_str* result;
    result->append("Player{color: ");
    result->append(color_names[color]);
    result->append("; name: \"");
    result->append(name);
    result->append(
        "\"; pos: "+pos->to_str()
        +"; }"
    );
    return result;
}

#endif
