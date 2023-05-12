#include"Map.h"
Map::Map() {
    grounds = nullptr;
    width = 12, height = 20;
    grounds = new Ground * [width];
    for (int i = 0; i < width; i++) {
        grounds[i] = new Ground[height];
    }
}
Map::~Map() {
    if (grounds != nullptr) {
        delete grounds;
    }
    width = 0, height = 0;
    for (auto& saw : saws) {
        delete saw;
    }
    for (auto& spike : spikes) {
        delete spike;
    }
    for (auto box : boxes) {
        delete box;
    }
    key = nullptr;
    
}
void Map::loadMap(std::string path) {
    LevelUp = false;
    backGround = new Texture();
    backGround->loadFromFile("Background.png");
    backGround->setPosition({ 0, 0 });
    backGround->setSize({ 1000, 560 });
    std::ifstream map(path.c_str());
    if (map.fail()) {
        std::cout << "Khong the mo file Map!" << std::endl;
        return;
    }
    std::string base = "Sprite//map//";
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int typeGround = 0;
            map >> typeGround;
            if (typeGround == 0) continue;
            std::string cur = base + std::to_string(typeGround);
            cur += ".png";
            if (typeGround < -9 || typeGround > 18) {
                if (typeGround==20 || typeGround == 21) {
                    Saw* saw = new Saw(typeGround, { (j-1) * 50 ,(i-2) * 50 }, { (j+3) * 50 ,((2+i) * 50) }, { j  * 50,i * 50});
                    saws.push_back(saw);
                }
                if (typeGround == 25) {
                    key = new Key(25, { j * 50,i * 50 });
                }
                if (typeGround == 24 || typeGround == 26) {
                    Box* box = new Box(typeGround, { j * 50,i * 50 });
                    boxes.push_back(box);
                }
                if (typeGround == 22 || typeGround == 23) {
                    Spike* tmp = new Spike(typeGround, { j * 50,i * 50 });
                    spikes.push_back(tmp);
                }
            }
            else
            {
                grounds[i][j].setGround(cur, j * GROUND_WIDTH, i * GROUND_WIDTH, typeGround);
            }
        }
    }
}
void Map::Render() {
    RenderBackground();
    RenderGrounds();
    key->Render();
    RenderSaw();
    SpikeRender();
    RenderBox();
}
void Map::RenderBackground() {
    backGround->render(false);
}
void Map::RenderGrounds() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            grounds[i][j].render();
        }
    }
}
void Map::Ground_Check_Player(Player* player) {
    if (player == nullptr||player->die) {
        return;
    }
    if (key->getColider()->checkCollision(player->collider) != 0) {
        player->LevelTurn = true;
        key->Delete = true;
    }
    for (auto& saw:saws) {
        
        int check = saw->getCollider()->checkCollision(player->collider);
        if (check != 0 && player->Dash == 0) {
            player->die = true;
            //std::cout << "ok";
        }
    }
    for (auto& spike : spikes) {

        int check = spike->getColider()->checkCollision(player->collider);
        if (check != 0 && player->Dash == 0) {
            player->die = true;
            //std::cout << "ok";
        }
    }
    //std::cout << player->rect.x << " " << player->rect.y << std::endl;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (grounds[i][j].getCollider() == nullptr) continue;
            else {
                int Check = grounds[i][j].getCollider()->Colision(player->collider);

                if (Check == collider::top ) {
                    player->velocity.second = 0;
                    player->GroundCheck = true;
                    player->CanJump = 2;
                    player->rect.y = grounds[i][j].rect.y - player->rect.h+4;
                    player->body.rect.y = player->rect.y;
                    /*if (player->GroundCheck) {
                        std::cout << player->getcollider()->down.x<<" "<< player->getcollider()->down.y << std::endl;
                    }*/

                }
                else {
                    player->GroundCheck = false;
                }
                if (Check == collider::down)
                {
                    player->velocity.second = 1;
                    player->GroundCheck = false;
                }
                
            }
        }
    }
    for (auto& box : boxes) {
        if (box->TypeObject == 24 && box->getCollider()->Colision(player->collider) ==top) {
            player->JumpHeight = 121.0f;
            player->velocity.second = 0;
            player->GroundCheck = true;
            player->rect.y = box->rect.y - player->rect.h+10;
            player->body.rect.y = player->rect.y;
            player->CanJump = 2;
            return;
        }
        else
        {
            player->JumpHeight = 81.0f;
        }
        if (box->TypeObject == 26 && box->getCollider()->checkCollision(player->collider) != 0 && player->LevelTurn) {
            LevelUp = true;
        }
        
    }
}
void Map::Wall_check_Player(Player* player) {
    if (player == nullptr) {
        return;
    }
    //std::cout << player->rect.x << " " << player->rect.y << std::endl;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (grounds[i][j].getCollider() == nullptr) continue;
            else {
                player->WallCheck = false;
                int Check = grounds[i][j].getCollider()->checkCollision(player->collider);
                if (Check == 0) continue;
                if (Check == collider::right) {
                    player->rect.x = grounds[i][j].rect.x + player->rect.w;
                    player->body.rect.x = player->rect.x;
                    player->WallCheck = true;
                    return;

                }
                if (Check == collider::left) {
                    player->rect.x = grounds[i][j].rect.x - player->rect.w;
                    player->body.rect.x = player->rect.x;
                    player->WallCheck = true;
                    return;
                }

            }
        }
    }
}

void Map:: Update(const Uint32& deltaTime) {
    UpdateSaw(deltaTime);
    UpdateBox(deltaTime);
}
void Map::RenderSaw() {
    for (auto saw : saws) {
       // std::cout << saw->TileObject <<std:: endl;
        saw->Render();
    }
}
void Map::UpdateSaw(const Uint32& deltaTime) {
 //   std::cout << saws.size() << std::endl;
    for (auto saw : saws) {
        saw->Update(deltaTime);
    }
}
void Map::SpikeRender() {
    for (auto spike : spikes) {
        spike->Render();
    }
}
void Map::RenderBox() {
    for (auto& box : boxes) {
        box->Render();
    }
}
void Map::UpdateBox(const Uint32& deltatime) {
    for (auto& box : boxes) {
        box->Update(deltatime);
    }
}