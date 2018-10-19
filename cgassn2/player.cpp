
#include "player.h"
#include "util.h"
#include "BmpLoader.h"
#include <stdio.h>
#include <time.h>

#include <list>
Player::Player(float x_, float y_, enum Direction dir_, float w_, float h_, float speed_)
{
    
    pos.x = x_;
    pos.y = y_;
    dir = dir_;
    w = w_;
    h = h_;
#ifdef __APPLE__
    speed = speed_ * 10;
    bangDelay = 100000;
    itemDelay = 100000;
#else
    speed = speed_ * 10;
    bangDelay = 200;
    itemDelay = 1000;
#endif
    
    lastItemUse = 0;
    speedDefault = speed;
    status = ALIVE;
    weapon = NULL;
    
    lastbang = 0;
    stimpackDuration = 0;
    accelDuration = 0;
    
    sprite = 0;
    
    
    bl[LEFT * 3 + 0] = new BmpLoader ("left0.bmp");
    bl[LEFT * 3 + 1] = new BmpLoader ("left1.bmp");
    bl[LEFT * 3 + 2] = new BmpLoader ("left2.bmp");
    
    bl[UP * 3 + 0] = new BmpLoader ("up0.bmp");
    bl[UP * 3 + 1] = new BmpLoader ("up1.bmp");
    bl[UP * 3 + 2] = new BmpLoader ("up2.bmp");
    
    bl[RIGHT * 3 + 0] = new BmpLoader ("right0.bmp");
    bl[RIGHT * 3 + 1] = new BmpLoader ("right1.bmp");
    bl[RIGHT * 3 + 2] = new BmpLoader ("right2.bmp");
    
    bl[DOWN * 3 + 0] = new BmpLoader ("down0.bmp");
    bl[DOWN * 3 + 1] = new BmpLoader ("down1.bmp");
    bl[DOWN * 3 + 2] = new BmpLoader ("down2.bmp");
}

void Player::display(void)
{
    for (std::list<Weapon*>::iterator it = listWeapon.begin(); it != listWeapon.end(); it++)
    {
        glColor3f(1.0, .0, .0);
        (*it)->display();
        
    }
    
    if (status == ALIVE)
    {
        sprite = (sprite + 1) % 3;
        glEnable(GL_TEXTURE_2D);
        LoadTexture(dir * 3 + sprite);
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_QUADS);
        
        glTexCoord2f(0.0, 0.0); // Need to check
        glVertex2f(pos.x, pos.y);
        glTexCoord2f(0.0, 1.0);
        glVertex2f(pos.x, pos.y + h);
        
        glTexCoord2f(1.0, 1.0);
        glVertex2f(pos.x + w, pos.y + h);
        
        
        glTexCoord2f(1.0, 0.0);
        glVertex2f(pos.x + w, pos.y);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        
        
        
        /* item box */
        float itemX, itemY;
        itemX = pos.x + 250.0;
        itemY = pos.y - 400.0;

        
        std::list<Item*>::iterator it = listItem.begin();
        int idx = 0;
        while (it != listItem.end ())
        {
            (*it)->displayBox(itemX,itemY, idx);
            it++;
            idx++;
        }
        
        for (; idx < 6; idx++)
        {
            float ItemX = itemX + (idx % 3) * w;
            float ItemY = itemY + (idx / 3) * h;
            glColor3f ((float)0xc0/0xff, 1.0, (float)0xee/0xff);
            glBegin(GL_POLYGON);
            glVertex2f(ItemX, ItemY);
            glVertex2f(ItemX, ItemY + h);
            glVertex2f(ItemX + w, ItemY + h);
            glVertex2f(ItemX + w, ItemY);
            glEnd();
        }
        
    }
    
    
}

void Player::move(void)
{
    int i = 0;
    
    
    for (i = 0; i < KEYBOARD_BUFFER_SIZE; i++)
    {
        if (keyboardBuffer[i])
        {
            if (i == 'a')
            {
                if (!isWall[0])
                {
                    pos.x -= speed;
                    dir = LEFT;
                }
            }
            
            if (i == 's')
            {
                if (!isWall[3])
                {
                    pos.y -= speed;
                    dir = DOWN;
                }
            }
            
            
            if (i == 'd')
            {
                if (!isWall[2])
                {
                    pos.x += speed;
                    dir = RIGHT;
                }
            }
            
            
            if (i == 'w')
            {
                if (!isWall[1])
                {
                    pos.y += speed;
                    dir = UP;
                }
            }
            
            if (i == 'k')
            {
                
                if (clock() - lastbang > bangDelay)
                {
                    
                    bang();
                }
                
            }
            
            if (i == 'j')
            {
                if (clock() - lastItemUse > itemDelay)
                {
                    
                    useItem();
                }
            }
            
            
        }
        
        if (specialKeyBuffer[i])
        {
            if (i == GLUT_KEY_LEFT)
            {
                if (!isWall[0]) {
                    dir = LEFT;
                    pos.x -= speed;
                }
                
            }
            
            if (i == GLUT_KEY_UP)
            {
                if (!isWall[1]) {
                    pos.y += speed;
                    dir = UP;
                }
            }
            
            if (i == GLUT_KEY_RIGHT)
            {
                if (!isWall[2]) {
                    pos.x += speed;
                    dir = RIGHT;
                }
            }
            
            if (i == GLUT_KEY_DOWN)
            {
                if (!isWall[3]) {
                    pos.y -= speed;
                    dir = DOWN;
                }
            }
        }
    }
    
    
    std::list<Weapon*>::iterator it = listWeapon.begin();
    
    while (it != listWeapon.end ())
    {
        
        if ((*it)->getStatus() == ALIVE)
            (*it)->move();
        it++;
        
    }
    
}



void Player::bang(void)
{
    if (dir == LEFT || dir == RIGHT)
    {
        listWeapon.push_back(new Weapon(pos.x, pos.y + GLOBAL_GRID_LENGTH / 4, dir, w, h / 2, speed * 2, speed * 150));
    }
    
    else
    {
        listWeapon.push_back(new Weapon(pos.x + GLOBAL_GRID_LENGTH / 4, pos.y, dir, w / 2, h, speed * 2, speed * 150));
    }
    
    lastbang = clock ();
}



position Player::getPos ()
{
    return pos;
}
void Player::killed(void)
{
    status = KILLED;
}

void Player::checkWall(bool isWall_[4])
{
    isWall[LEFT] = isWall_[0];
    isWall[UP] = isWall_[1];
    isWall[RIGHT] = isWall_[2];
    isWall[DOWN] = isWall_[3];
}


void Player::cleanWall ()
{
    isWall[LEFT] = false;
    isWall[UP] = false;
    isWall[RIGHT] = false;
    isWall[DOWN] = false;
}

void
Player::addItem(Item* item_)
{
    listItem.push_back(item_);
}

void
Player::checkItemDuration ()
{
    if (stimpackDuration != 0)
    {
        if (clock() > stimpackDuration)
        {
            stimpackDuration = 0;
#ifdef __APPLE__
            bangDelay = 2000;
#else
            bangDelay = 200;
#endif
        }
    }
    
    if (accelDuration != 0)
    {
        if (clock() > accelDuration)
        {
            accelDuration = 0;
            speed = speedDefault;
        }
    }
}

bool
Player::useItem(void)
{
    if (listItem.empty() == false)
    {
        lastItemUse = clock ();
        // Do something with Item.
        
        switch ((listItem.front())->getType()) {
            case ONE:
                if (stimpackDuration != 0)
                    return false;
#ifdef __APPLE__
                bangDelay = 10000;
                stimpackDuration = 1000000 + clock();
#else
                bangDelay = 100;
                stimpackDuration = 1000 + clock();
#endif
                break;
                
            case TWO:
                if (accelDuration != 0)
                    return false;
                speed = speed * 3;
#ifdef __APPLE__
                accelDuration = 1000000 + clock();
#else
                accelDuration = 1000 + clock();
#endif
                break;
        }
        
        listItem.pop_front();
        return true;
    }
    else {
        return false;
    }
}
unsigned long
Player::getItemNum ()
{
    return listItem.size();
}

std::list<Weapon*>
Player::getWeaponList (void)
{
    return listWeapon;
}

void
Player::checkWeapon ()
{
    std::list<Weapon*>::iterator it = listWeapon.begin();
    
    while (it != listWeapon.end ())
    {
        
        if ((*it)->getStatus() == KILLED)
        {
            Weapon * tmp = NULL;
            tmp = *it;
            it = listWeapon.erase(it);
            
            delete tmp;
        }
        else it++;
    }
}


void
Player::LoadTexture(unsigned int idx)
{
    BmpLoader * tmp = bl[idx];
    
    glDeleteTextures(1, &textureID);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, tmp->iWidth, tmp->iHeight, GL_RGB, GL_UNSIGNED_BYTE, tmp->textureData);
}

enum Status
Player::getStatus ()
{
    return status;
}

Player::~Player()
{
    for (int i = 0; i < 12; i++)
        delete bl[i];
}
