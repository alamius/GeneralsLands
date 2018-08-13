#ifndef TILE_CPP
#define TILE_CPP
class Game;
class Player;
#include "perlin.cpp"
#include "sett.h"
#include "utils.h"

Tile::Tile(int x, int y, Game* game_){   pos = new Vector(x, y); std_set(); game = game_;  }
Tile::Tile(Vector* pos_, Game* game_){   pos = new Vector(pos_); std_set(); game = game_;  }
void    Tile::std_set(){
    dbg("Tile"+pos->to_str()+"/std_set", 4);
    int x = pos->x;
    int y = pos->y;
    owner = NULL;
    fertility = noise(x+1000, y+1000, 1)*3;
    roughness = noise(x+2000, y+2000, 1)*3 - 1;
    civilians = 1;
    military = 0;
    generals = 0;
    spies = 0;
    spies_made = 0;
    townhood = 0;
    for(int i = 0; i < players_num; i++){
        spies_moving[i] = 0;
    }
    locked = false;
    for(int i = 0; i < 4; i++){
        attack_WASDE[i] = 0;
    }
    attack_WASDE[4] = 1;
}
CC_str* Tile::to_str(){
    dbg("Tile"+pos->to_str()+"/to_str", 4);
    dbg("Tile"+pos->to_str()+"/to_str: owner: "+_to_str(owner), 4);
    CC_str* result = new CC_str();
    result->append("Tile{");
    if(owner){
        result->append("owned: \"");
        result->append(owner->name);
        result->append("\"; ");
    }
    result->append("pos: "+pos->to_str()+"; ");
    result->append("fert: "+_to_str_round(fertility, 5)+"; ");
    result->append("rough: "+_to_str_round(roughness, 5)+"; ");
    result->append("town: "+_to_str_round(townhood, 5)+"; ");
    result->append("civs: "+_to_str_round(civilians, 5)+"; ");
    result->append("military: "+_to_str_round(military, 5)+"; ");
    result->append("generals: "+_to_str(generals)+"; ");
    result->append("spies: "+_to_str(spies)+"; ");
    result->append("}");
    return result;
}
CC_str**Tile::_print(CC_str* result[tile_size_y]){
    dbg("Tile"+pos->to_str()+"/_print (CC_str*[])", 4);
    int color = owner ? owner->color : -1;
    bool active = game->players[user_index]->pos->equals(pos);
    string upper_frame = active ? "=" : "-";
    if(tile_size_y < 3){
        Log_add(new CC_str("There are not enough rows to print on in Tile/print. tile_size_y is "+_to_str(tile_size_y)+" and must be at least 3!"));
    }
    result[0]->append(
        fl("F:"+fl(match_number_symbol(fertility)->chrs, 2), (tile_size_x-1)/2)
        +f("R:"+fl(match_number_symbol(roughness)->chrs, 2), (tile_size_x-1)/2-1)
    );
    result[1]->append(
        "C:"+fl((int)civilians, (tile_size_x-7))+fit_to_symbol(generals/5+0.5, general_symbol, 3)
    );
    result[2]->append(
        "M:"+fl(_to_str_round(military, 2),  (tile_size_x-7))+fit_to_symbol(spies/5+0.5, spy_symbol, 3)
    );
    if(tile_size_y >= 4){
        result[tile_size_y-1]->append(tile_size_x-3, upper_frame);
        result[tile_size_y-1]->append(frame_cross);
    }
    for(int y = 0; y < tile_size_y; y++){
        CC_str* line = new CC_str();
        line->append(result[y]->chrs+frame_chr);
        if(y == tile_size_y - 1){
            line = (new CC_str(upper_frame))->append(line);
        }else{
            line = (new CC_str(" "))->append(line);
        }
        // dbg("Tile"+pos->to_str()+"/_print (CC_str*[]): append "+_to_str(max(tile_size_x-(int)(line->chrs.length()), 0))+"times ' '.");
        result[y]->chrs.append(max(tile_size_x-(int)(line->chrs.length()), 0), ' ');
        result[y]->clrs.clear();
        result[y]->clrs.append(tile_size_x-1, color);
        result[y]->clrs.append(1, -1); //keeping a trench of non-colored characters
    }
    // dbg_obj_arr("Tile"+pos->to_str()+"/_print (CC_str*[])", "result", result, tile_size_y);
    return result;
}
void    Tile::update(){
    dbg("Tile"+pos->to_str()+"/update:", 4);
    double c_max = pow(e, 2*fertility-roughness)*(4*townhood + 1);
    double c_max_total = pow(e, 2/5-0/10); //maximal fertility - minimal roughness
        // 1/2**0 == 1.0
        // 1/2**1 == 0.5
        // 1/2**2 == 0.25
        // 1/2**3 == 0.125
        // 1/2**4 == 0.0625
        // 1/2**5 == 0.03125
        // 1/2**6 == 0.015625
        // 1/2**7 == 0.0078125
        // 1/2**8 == 0.00390625
        // 1/2**9 == 0.001953125
        // 1/2**10 == 0.0009765625
        // 1/2**11 == 0.00048828125
        // 1/2**12 == 0.000244140625
        // 1/2**13 == 0.0001220703125

    civilians += (c_max - civilians)*pow(2, -4);
    military += (civilians*0.2 - military)*pow(2, -4);

    //standard attacks/sending military: see Tile.h
    //WASD stands for the directions, thus, the sum over it is the military requested by all four surrounding Tiles
    //E stands for the tile itself and it's minimum savings of military which the WASD won't take away
    double summed_WASD = 0;
    for(int i = 0; i < 4; i++){
        summed_WASD += attack_WASDE[i];
    }
    if(summed_WASD != 0){
        double summed = summed_WASD + attack_WASDE[4];
        //enough military to send it out
        if(military >= summed){
            for(int i = 0; i < 4; i++){
                attack(owner, pos->add(WASD[i]), attack_WASDE[i]/military);
            }
        // military is not enough for the requests in attack_WASDE[0:3], if attack_WASDE[4] in military is on Tile, the rest will be sent out
        }else if(military < attack_WASDE[4]){
            double summed_send = military - attack_WASDE[4];
            double send;
            for(int i = 0; i < 4; i++){
                send = (attack_WASDE[i]/summed_WASD) * summed_send;
                attack(owner, pos->add(WASD[i]), send/military);
            }
        }//else: none is sent
    }
}
bool    Tile::attack(Player* player, Vector* to, double power){ //power in [0, 1] -> 0 = no power, 1 = all goes
    int dbg_ = 3;
    if(player == 0 && owner == 0){
        dbg_ = 5;
    }
    dbg(
        "Tile"+pos->to_str()
        +"/attack: "+to->to_str(),
        dbg_
    );
    bool result = false; //whether the tile has been conquered at the end
    dbg(
        "Tile"+pos->to_str()
        +"/attack: locked == "+(locked ? "True" : "False")
        +"; military == "+_to_str(military)
        +"; from == "+pos->to_str()
        +"; to == "+to->to_str()
        +"; owner == "+_to_str(owner)
        +"; player == "+_to_str(player)
        +"; destination == "+to->to_str()
        +"; game_size == "+game_size->to_str()
        +"; destination in range of game_size == "+(to->in_ranges(game_size) ? "True" : "False")
    , dbg_);
    if(
        locked
        || !to->in_ranges(game_size)
        || owner != player
    ){
        dbg(
            "Tile"+pos->to_str()
            +"/attack: fail: owner of tile "+pos->to_str()
            +"; owner == "+_to_str(owner)
            +", '"+(owner  ? owner->name->chrs  : "no player")+"'"
            +"; player == "+_to_str(player)
            +", '"+(player ? player->name->chrs : "no player")+"'"
            +"; locked == "+(locked ? "True" : "False")
        , 2);
        return false;
    }
    power = min(power, 1.0);
    power = max(power, 0.0);
    Tile* at = game->tiles[(int)to->y][(int)to->x];
    //different players or no players attacking means fighting of the military, same player means just moving it
    if(owner != at->owner || owner == NULL || at->owner == NULL){
        dbg(
            "Tile"+pos->to_str()+
            "/attack: Before tile attacked:   "+_to_str(at->to_str())
        , dbg_);
        double m1 = military*power;
        double m2 = at->military;
        double c1 = civilians;
        double c2 = at->civilians;
        double r = at->roughness;
        double t = at->townhood;
        double b = player ? player->brutality : 1;
        military -= m1;
        //      dmg = 10/(5*1.2+20*0.1)
        // double damage = m1/(m2*1.2+c2*0.1);
        // double dmg_mil = pow(damage+1, 2)-1;
        // double dmg_civ = pow(damage+1, 0.5);
        // dbg(
        //     "Tile"+pos->to_str()+"/attack: dmg: "+_to_str(damage)
        //     +", mil_dmg: "+_to_str(dmg_mil)
        //     +", civ_dmg: "+_to_str(dmg_civ)
        // +"", 2);
        // at->military *= dmg_mil;
        // at->civilians*= dmg_civ;
        at->military -= m1 * 0.9;
        at->civilians -= m1 * pow(0.9, b);
        if(at->military < 0){
            at->owner = owner;
            at->military *= -1;
            result = true;
        }
        military = max(military, 0.0);
        civilians = max(civilians, 0.0);
        at->military = max(at->military, 0.0);
        at->civilians = max(at->civilians, 0.0);
        dbg(
            "Tile"+pos->to_str()+
            "/attack: After tile attacked:   "+at->to_str()->chrs+
        "", dbg_);
        dbg("Tile"+pos->to_str()+"/attack: Tile "+at->to_str()->str()+"was successfully attacked!");
    }else{
        at->military += military * power * 0.99;
        military -= military * power * 0.99;
        result = true;
        dbg("Tile"+pos->to_str()+"/attack: military power was successfully transerred to Tile "+at->to_str()->str()+"!");
    }
    return result;
}

#endif
