#pragma once
#include"Texture.h"
#include"BaseFunc.h"
#include"Collider.h"
#include"Animation.h"

struct Shadow : public Texture {
	int timeDie = 100;
	bool die = false;
	Shadow(const bool& input,bool faceRight) {
		loadFromFile("Sprite//Hero//shadow" + std::to_string(input) + ".png");
		setSize({ 50, 50 });
		face_Right = faceRight;

	}
	void Update(const Uint32& deltaTime) {
		timeDie -= (int)deltaTime;
		if (timeDie <= 0)
			die = true;
	}
	void Render() {
		if (die == false) {
			render(face_Right);
		}
	}
	bool face_Right = 0;
};

class Player:public Texture
{
public:
	Player(int Id,std::pair<int, int> positionStart, std::pair<int,int>size,const bool& faceRightStart);
	~Player();
	float speed = 5.0f;
	bool IsMove = false, IsJump = false;
	bool GroundCheck=false,WallCheck=false,face_Right;
	float JumpHeight = 81.0f;
	int CanJump = 2;
	bool die = false,LevelTurn;
	int Dash = 0;
	std::pair<int, float>velocity;
	Collider* collider;
	void PlayerMove();
	void Render();
	Collider* getcollider();
	Texture body;
	void UpdateMove(const Uint32& deltaTime);
private:
	std::vector<Shadow*> shadows;
	void CreateShadow(bool faceRight);
	void UpdateShadow();
	std::pair<int, int> oldPos;
	Animation* animation;

};

