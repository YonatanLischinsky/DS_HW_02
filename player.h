#ifndef PLAYER_H
#define PLAYER_H

class Player
{
private:
    int id;
    int level;
    int group;
    int score;

public:
    Player(int id, int lvl, int g, int s) : id(id), level(lvl), group(g), score(s)
    { } //C'tor

    Player(Player& p) = default; //Copy c'tor
    ~Player() = default;

    friend class Group;
    friend class Level;
    friend class PlayersManager;
};

#endif