//
//  enemy.h
//  cgassn1
//
//  Created by Dahun Lee on 2018. 10. 1..
//  Copyright © 2018년 Dahun Lee. All rights reserved.
//

#ifndef enemy_h
#define enemy_h
#include "object.h"
#include "util.h"
#include "BmpLoader.h"


class Enemy : virtual public GameObject
{
    float x;
    float y;
    position pos;
	enum Direction dir;
    unsigned int moveCount;
	float w, h;
	float speed;
	enum Status status;

	bool isWall[4] = {false, false, false, false,};

	int sprite = 0;

	unsigned int textureID;

	BmpLoader * bl[16];
    

public:
    Enemy (float, float, enum Direction, float, float, float);
    void display ();
    position getPos ();
	void move(float x, float y);
    void cleanWall ();
    
    enum Status getStatus ();

	void checkWall(bool isWall_[4]);
	void killed();
	void LoadTexture(unsigned int);
	~Enemy();
};

#endif /* enemy_h */
