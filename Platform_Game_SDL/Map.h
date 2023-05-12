#pragma once
#include"BaseFunc.h"
#include"Ground.h"
#include"Player.h"
#include"Collider.h"
#include"Saw.h"
#include"Key.h"
#include"Spike.h"
#include"Box.h"

class Map {
public:
    Map();
    ~Map();
    //render
    void loadMap(std::string path);
    void Render();
    void RenderBackground();
    void RenderGrounds();
    void RenderSaw();
    void SpikeRender();
    void RenderBox();
    //update
    void Update(const Uint32& deltaTime);
    void UpdateSaw(const Uint32& deltaTime);
    void UpdateBox(const Uint32& deltaTime);
    //collider
    void Ground_Check_Player(Player*player);
    void Wall_check_Player(Player* player);
    bool LevelUp = false;
    Key* key;

private:
    std::vector<Box*> boxes;
    std::vector<Saw*> saws;
    std::vector<Spike*> spikes;
    int width, height;
    Ground** grounds;
    Texture* backGround;
};