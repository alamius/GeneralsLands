#ifndef GAME_CPP
#define GAME_CPP
#include "sett.h"
#include "utils.h"
#include "Player.h"
#include "Tile.h"

Game::Game(){
    dbg("Game/init", 4);
    for(int y = 0; y < game_size_y; y++){
    for(int x = 0; x < game_size_x; x++){
            tiles[y][x] = new Tile(x, y, this);
    }
    }
    for(int i = 0; i < players_num; i++){
        Vector* pos;
        int try_count = 0;
        do{
            pos = new Vector(
                int(random()%game_size_x),
                int(random()%game_size_y)
            );
            // dbg("Game/init:position "+pos->to_str()+" chosen!", 2);
            try_count++;
            if(try_count > 1000){
                try_count = 0;
                players_min_dist--;
                if(players_min_dist == 0){
                    dbg("Game/init: The "+_to_str(players_num)+" Players seem not to fit on the "+_to_str(game_size_y)+"x"+_to_str(game_size_x)+" board! Only "+_to_str(i)+" Players could be fit.", 4);
                    if(i == 0){
                        Log_add(new CC_str("No players could be fit on the board, aborting!"));
                        usleep(1000000);
                        exit(0);
                    }
                    players_num = i+1;
                    break;
                }
            }
        }while(position_conflict(pos, i));
        players[i] = new Player(
            (i%(colors_num-1))+1,
            player_names[i%player_name_num],
            new Vector(pos)
        );
        Tile* tile = get_players_tile(i);
        tile->owner = players[i];
        tile->generals = 1;
        dbg("Game/init: Tile "+pos->to_str()+" got player "+_to_str(i)+" as owner.", 4);
    }
    for(int i = 0; i < max_moves; moves[i++] = (Move*)NULL);
    moves_index = 0;
}
bool Game::position_conflict(Vector* pos, int i){ //chechs whether there are any players near pos in players before index i
    for(int j = 0; j < i; j++){
        Player* player = players[j];
        //Technically, it isn't clear what the Player->pos holds, but I define it the cursor, which is (at this point) the pos of its only tile
        if(player->pos->dist(pos) < players_min_dist){
            dbg(
                "Game/position_conflict: Tile at "+pos->to_str()
                +" conflicted with the set Tile of player "+_to_str(player)
                +", '"+player->name->chrs
                +"', "+player->pos->to_str()
            +".", 5);
            return true;
        }
    }
    return false;
}
Tile* Game::get_tile(Vector* at){
    dbg("Game/get_tile: "+at->to_str(), 4);
    Tile* result = NULL;
    if(!at->in_ranges(game_size)){
        dbg("Game/get_tile: The searched tile position "+at->to_str()+" is outside the rectangle from (0, 0) to "+game_size->to_str()+". That could lead to a Segmentation Fault!", 5);
    }else{
        result = tiles[(int)at->y][(int)at->x];
    }
    dbg("Game/get_tile: result == "+_to_str(result), 5);
    return result;
}
Tile* Game::get_players_tile(int player_index){
    dbg("Game/get_players_tile: "+players[player_index]->name->chrs, 4);
    if(player_index > players_num){
        dbg("Game/get_players_tile: The player_index "+_to_str(player_index)+" is bigger than the number of players "+_to_str(players_num)+". That could lead to a Segmentation Fault!", 4);
    }
    Vector* pos = players[player_index]->pos;
    return tiles[(int)pos->y][(int)pos->x];
}
void Game::update(){
    dbg("Game/update", 4);
    for(int y = 0; y < game_size_y; y++){
    for(int x = 0; x < game_size_x; x++){
            tiles[y][x]->update();
    }
    }
    dbg("Game/update:update_moves", 5);
    for(int i = 0; i < max_moves; i++){
        Move* m = moves[i];
        if(m != NULL){
            dbg("Game/update:update_moves: m == "+_to_str(m), 5);
            dbg(
                m->_dbg()
                +", index: "+_to_str(i)
                // +", execution of move: "+_to_str_bool(m != NULL && time_ms() > m->exec_time)
            +"", 5);
        }
        if(m != NULL){// && time_ms() > m->exec_time){
            get_tile(m->from)->attack(m->player, m->to, m->power);
        }
    }
}
void Game::_print(CC_str* result[], Vector* from, Vector* to){
    dbg("Game/_print", 4);
    int from_x = (int)from->x; //colmn
    int from_y = (int)from->y; //row
    int to_x = (int)to->x; //colmn
    int to_y = (int)to->y; //row

    // if from_x not in [0, game_size_x) or from_y not in [0, game_size_y)
    if(!from->in_ranges(game_size)){
        dbg(
            "Game/_print: The beginning of the rectangle from == "+from->to_str()
            +" which is not in range (0, 0) to "+game_size->to_str()+"", 2
        );
        if(from_x < 0){               from_x = 0;             }
        if(from_y < 0){               from_y = 0;             }
        if(from_x >= game_size->x){   from_x = game_size->x-1;}
        if(from_y >= game_size->y){   from_y = game_size->y-1;}
    // if to_x not in [from_x, game_size_x) or to_y not in [from_y, game_size_y)
    }else if(!to->add(-1, -1)->in_ranges(from, game_size)){
        Log_add(new CC_str(
            "Game/_print: The end of the rectangle to == "+to->to_str()
            +" which is not in range from "+from->to_str()
            +" to "+game_size->to_str()
        ));
        if(to_x < from_x){           to_x = from_x+1;   }
        if(to_y < from_y){           to_y = from_y+1;   }
        if(to_x >= game_size->x){    to_x = game_size->x;}
        if(to_y >= game_size->y){    to_y = game_size->y;}
    }

    int begin_print_y = 0;
    //build the frame
    if(frame_chr->chrs != ""){
        //left frame where tile->_print does not create one
        for(int y = 0; y < (to_y - from_y)*tile_size_y+1; y++){
            result[y] = new CC_str((y % tile_size_y) ? frame_chr : new CC_str("+"));
        }
        //top frame where tile->_prnt does not create one
        for(int xx = 0; xx < to_x - from_x; xx++){
            result[0]->append(tile_size_x-3, "-");
            result[0]->append("+");
        }
        begin_print_y = 1;
    }
    CC_str* printed[tile_size_y];
    for(int y = from_y; y < to_y; y++){
        for(int x = from_x; x < to_x; x++){
            for(int i = 0; i < tile_size_y; printed[i++] = new CC_str());
            tiles[y][x]->_print(printed);
            for(int i = 0; i < tile_size_y; i++){
                int index = (y-from_y)*tile_size_y+i+begin_print_y;
                result[index]->append(printed[i]);
            }
        }
    }
}
void Game::random_moves(int number, double time){
    dbg("Game/random_moves", 2);
    double prob = (double)number / (game_size_y*game_size_x);
    Tile* tile;
    Tile* neighbor_tile;
    Vector* pos;
    Vector* neighbor_pos;
    for(int y = 0; y < game_size_y; y++){
    for(int x = 0; x < game_size_x; x++){
        int r = random();
        pos = new Vector(x, y);
        tile = get_tile(pos);
        neighbor_pos = pos->add(WASD[r%4]);
        neighbor_tile = get_tile(neighbor_pos);
        if(r%1000 < prob*1000 && neighbor_tile != NULL && tile->military > neighbor_tile->military){
            dbg(
                "Game/random_moves: Tile "+_to_str(tile)+", "+pos->to_str()
                +" wants to attack Tile "+_to_str(neighbor_tile)+", "+neighbor_pos->to_str()
            +". ", 2);
            double wait = wait_for_move[index('a', wait_for_move_keys, wait_for_move_len)];
            // dbg(
            //     "Game/random_moves: New Move( from: "+pos->to_str()
            //     +", to: "+neighbor_pos->to_str()
            //     +", player: "+_to_str(neighbor_tile->owner)
            //     +", power: "+_to_str(0.5)
            //     +", time: "+_to_str(time)
            //     +", wait: "+_to_str(wait)
            // +")");
            //TODO: test_time!
            moves_add(new Move(
                pos,
                neighbor_pos,
                neighbor_tile->owner,
                0.5,
                time,
                wait
            ));
        }
    }
    }
}
bool Game::check_tiles(bool dbg_){
    dbg("Game/check_tiles", 4);
    bool result = true;
	for(int y = 0; y < game_size_y; y++){
	for(int x = 0; x < game_size_x; x++){
        if(dbg_){ dbg("Game/check_tiles"+_to_str(tiles[y][x]->owner)); }
		result == result && tiles[y][x];
	}
    if(dbg_){ dbg(""); }
	}
    return result;
}
int  Game::get_statistics(CC_str** result, Player* player, Tile* tile){
    dbg("Game/get_statistics", 4);
    bool test_player = (player != (Player*)1);
    bool test_tile   = (tile   != (Tile*  )1);
    int tiles_owned = 0;
    Tile* sum = new Tile(new Vector(0, 0), this);
    sum->std_set();
    sum->fertility = 0;
    sum->roughness = 0;
    sum->civilians = 0;
    for(int y = 0; y < game_size_y; y++){
	for(int x = 0; x < game_size_x; x++){
        Tile* T = tiles[y][x];
        if(
            (test_player && T->owner == player || !test_player) &&
            (test_tile   && T        == tile   || !test_tile  )
        ){
            tiles_owned++;
            sum->fertility += T->fertility;
            sum->roughness += T->roughness;
            sum->civilians += T->civilians;
            sum->military  += T->military;
            sum->townhood += (T->townhood-1)/4; //{1, 5} -> {0, 1}
            sum->generals += T->generals;
            sum->spies += T->spies;
        }
	}
    }
    int i = 0;
    result[i++]->append(" +-")->append(27, "-");
    if(test_player && player){
        result[i++]->append(" | ")->append(player->name)->append("'s Statistics");
    }else if(test_tile && tile){
        result[i++]->append(" | Tile "+tile->pos->to_str()+"'s Statistics");
    }else if(tile && player){
        result[i++]->append(" | The Game's Statistics");
    }else if(!(tile && player)){
        result[i++]->append(" |  Unreigned Statistics");
    }
    result[i++]->append(" | owned Tiles:   "+_to_str(tiles_owned));
    result[i++]->append(" | Generals:      "+_to_str(sum->generals));
    result[i++]->append(" | Towns:         "+_to_str(sum->townhood));
    result[i++]->append(" | Civilians:     "+_to_str(sum->civilians));
    result[i++]->append(" | Military:      "+_to_str(sum->military));
    result[i++]->append(" | Spies:         "+_to_str(sum->spies));
    result[i++]->append(" | ø Fertility:   "+_to_str(sum->fertility/tiles_owned));
    result[i++]->append(" | ø Roughness:   "+_to_str(sum->roughness/tiles_owned));
    return i;
}
void Game::moves_add(Move* move){
    dbg("Game/moves_add", 4);
    if(moves_index < max_moves){
        moves[moves_index++] = move;
    }else{
        clear_empty_or_NULL_element(moves, max_moves, (Move*)NULL);
        moves_index = index((Move*)NULL, moves, max_moves);
        if(moves_index != -1){
            moves[moves_index++] = move;
        }else{
            string Error = "MovesError: The Moves Array is too small to handle all the moves, change the settings at max_moves!";
            dbg(Error);
            Log_add(new CC_str(Error, 1));
        }
    }
}

#endif
