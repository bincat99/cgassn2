//
//  object.h
//  cgassn1
//
//  Created by Dahun Lee on 2018. 10. 1..
//  Copyright © 2018년 Dahun Lee. All rights reserved.
//

#ifndef object_h
#define object_h

#include "util.h"

enum ObjectType
{
    EMPTY,
    WALL,
    ITEM,
    ENEMY,
    SAFE
};

class GameObject
{

public:
    virtual position getPos () = 0;
    //virtual void move () = 0;
    virtual void display () = 0;
};

#endif /* object_h */
