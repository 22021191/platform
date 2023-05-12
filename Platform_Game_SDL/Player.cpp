#include"Player.h"
Player::Player(int Id,std::pair<int, int> positionStart,std::pair<int,int>size ,const bool& faceRightStart) {
    animation = new Animation("Sprite//Hero//hero"+std::to_string(Id)+".png", {12,7}, 100);
    setSize(size);
    setPosition(positionStart);
    collider = new Collider(*this);
    body.loadFromFile("Sprite//Hero//hero"+std::to_string(Id)+".png");
    body.setSize(size);
    body.setPosition(getPosition());
    face_Right = faceRightStart;
    animation->currentFrame = { 7, 0 };
    
}
Player::~Player() {
    if (collider != nullptr) {
        delete collider;
    }
    for (auto& shadow : shadows) {
        delete shadow;
    }
    if (animation != nullptr) {
        delete animation;
    }
    
}
void Player::PlayerMove() {
    oldPos = this->body.getPosition();
    if (Dash > 0) {
        Dash--;
        CreateShadow(face_Right);
        this->body.rect.x += 20 * (face_Right ? -1 : 1);
    }
    if(Dash<=0) {
        if (IsMove) {
            velocity.first = speed * (face_Right ? -1 : 1);
        }
        else {
            velocity.first = 0;
        }
        if (IsJump && CanJump>0) {
            if (CanJump == 1) {
                JumpHeight = 36;
                CanJump = 0;
            }else if (CanJump == 2) {
                CanJump = 1;
                JumpHeight = 49;
            }
            velocity.second = -sqrt(2.0f * 2.0f * JumpHeight);
            Mix_PlayChannel(-1, jump, 0);
        }
        this->body.rect.x += velocity.first;
        this->rect.x = this->body.rect.x;
        this->body.rect.y += velocity.second;
        this->rect.y = this->body.rect.y;
        if (!GroundCheck && Dash <= 0) {
            velocity.second += 0.5;
            if (WallCheck == true&&velocity.second>=0) {
                velocity.second = 1;
            }
        }
        /*if (WallCheck == true) {
            CanJump = 2;
        }*/
        if (GroundCheck && velocity.second > 0) {
            velocity.second = 0;
            CanJump = 2;
        }
    }
    if (shadows.size() > 0) {
        if (shadows[0]->die) {
            shadows.erase(shadows.begin() + 0);
        }
    }
    UpdateShadow();
    if (!die) {
        UpdateMove(10);
        if (Dash > 0) animation->update(10, { 6, 2 }, face_Right);
    }

}
void Player::Render() {
    if (die == false) {
        for (auto& shadow : shadows) {
            shadow->render(shadow->face_Right);
        }
        body.render(animation->getFlip(), animation->getmBox());
    }
    else {
        animation->update(30, {3,8}, face_Right);
        body.render(animation->getFlip(), animation->getmBox());
    }
}
void Player::CreateShadow(bool faceRight) {
    if (Dash & 1) {
        Shadow* tmp = new Shadow(true, faceRight);
        tmp->setPosition(oldPos);
        shadows.push_back(tmp);
    }
    else {
        Shadow* tmp = new Shadow(false, faceRight);
        tmp->setPosition({ oldPos.first - 10, oldPos.second });
        shadows.push_back(tmp);
    }
}
void Player:: UpdateShadow() {
    for (auto& shadow : shadows) {
            shadow->Update(30);
    }
}
void Player:: UpdateMove(const Uint32& deltaTime) {
    if (!IsMove) {
        if (CanJump==2) {
            animation->update(deltaTime, { 11, 5 }, face_Right);
        }
        else {
            if (velocity.second < 0) {
                if (CanJump == 1) {
                    animation->update(deltaTime, { 1, 6 }, face_Right);
                }if (CanJump == 0) {
                    animation->update(deltaTime, { 6,1 }, face_Right);
                }
            }
            else {
                animation->update(deltaTime, { 1, 4 }, face_Right);
            }
        }
    }
    else {
        if (velocity.first < 0) face_Right = true;
        else face_Right = false;

        if (CanJump==2) {
            animation->update(deltaTime, { 12, 0 }, face_Right);
        }
        else {
            if (velocity.second < 0) {
                if (CanJump == 1) {
                    animation->update(deltaTime, { 6,1 }, face_Right);
                }else if (CanJump == 0) {
                    animation->update(deltaTime, { 1, 6 }, face_Right);
                }
            }
            else {
                if (WallCheck == true) {
                    animation->update(deltaTime, { 5, 2 }, face_Right);
                }
                else {
                    animation->update(deltaTime, { 1, 4 }, face_Right);
                }
            }
        }
    }
}
Collider* Player::getcollider() {
    return collider;
}
