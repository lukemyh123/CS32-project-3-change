#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld *this_world)
:GraphObject(imageID, startX, startY, dir, depth), s_world(this_world)
{}

Actor::~Actor() {}

Agent::Agent(int imageID, double startX, double startY, StudentWorld *this_world)
:Actor(imageID, startX,startY, right, 0, this_world)
{
    setAlive();
}

Human::Human(int imageID, double startX, double startY, StudentWorld *this_world)
: Agent(imageID, startX,startY, this_world)
{
    infection = 0;
    infection_status = false;
}

void Human::doSomethingCom()
{
    if (getStatus() == false)
        return;
    
    if (getInfection_status() == true)
        infection++;
    else
        infection = 0;
    
    if (infection == 500)
        setDead();
    
}
Penelope::Penelope(double startX, double startY, StudentWorld *this_world)
: Human(IID_PLAYER, startX, startY, this_world){}

void Penelope::doSomething()
{
    int ch;
    Human::doSomethingCom();
    
    if (getWorld()->getKey(ch))
    {
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                if (getDirection() != left)
                {
                    setDirection(left);
                    if (!getWorld()->check_collisionForPlayer(getX() - 4, getY()))
                        moveTo(getX() - 4, getY());
                }
                else if (!getWorld()->check_collisionForPlayer(getX() - 4, getY())) //check if the next left has blocking
                    moveTo(getX() - 4, getY());
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() != right)
                {
                    setDirection(right);
                    if (!getWorld()->check_collisionForPlayer(getX() + 4, getY()))
                        moveTo(getX() + 4, getY());
                }
                else if (!getWorld()->check_collisionForPlayer(getX() + 4, getY())) //check if the next right has blocking
                    moveTo(getX() + 4, getY());
                break;
            case KEY_PRESS_UP:
                if (getDirection() != up)
                {
                    setDirection(up);
                    if (!getWorld()->check_collisionForPlayer(getX(), getY() + 4))
                        moveTo(getX(), getY() + 4);
                }
                else if (!getWorld()->check_collisionForPlayer(getX(), getY() + 4))  //check if the next up has blocking
                    moveTo(getX(), getY() + 4);
                break;
            case KEY_PRESS_DOWN:
                if (getDirection() != down)
                {
                    setDirection(down);
                    if (!getWorld()->check_collisionForPlayer(getX(), getY() - 4))
                        moveTo(getX(), getY() - 4);
                }
                else if (!getWorld()->check_collisionForPlayer(getX(), getY() - 4))  //check if the down has blocking
                    moveTo(getX(), getY() - 4);
                break;
            case KEY_PRESS_SPACE:
                if (getDirection() == left && getWorld()->get_flamethrower() != 0)
                {
                    getWorld()->fire(getX(), getY(), 1);
                    getWorld()->reduce_flamethrower();
                    //std::cout <<" Fire " << std::endl;  //produce fire;
                }
                else if (getDirection() == right && getWorld()->get_flamethrower() != 0)
                {
                    getWorld()->fire(getX(), getY(), 2);
                    getWorld()->reduce_flamethrower();
                    //std::cout <<" Fire " << std::endl;  //produce fire;
                }
                else if (getDirection() == up && getWorld()->get_flamethrower() != 0)
                {
                    getWorld()->fire(getX(), getY(), 3);
                    getWorld()->reduce_flamethrower();
                    //std::cout <<" Fire " << std::endl;  //produce fire;
                }
                else if (getDirection() == down && getWorld()->get_flamethrower() != 0)
                {
                    getWorld()->fire(getX(), getY(), 4);
                    getWorld()->reduce_flamethrower();
                    //std::cout <<" Fire " << std::endl;  //produce fire;
                }
                break;
            case KEY_PRESS_TAB:
                if (getWorld()->get_landmines() != 0)
                    getWorld()->placeLandmine(getX(), getY());
                break;
            case KEY_PRESS_ENTER:
                if (getWorld()->get_vaccine() != 0)
                {
                    cureInfection();
                    getWorld()->reduce_vaccine();
                }
                break;
        }
    }
}

Citizen::Citizen(double startX, double startY, StudentWorld * this_world)
:Human(IID_CITIZEN, startX, startY, this_world)
{
    tick = 0;
}

void Citizen::doSomething()
{
    Human::doSomethingCom();
    
    tick++;
    
    if (tick % 2 == 0)
        return;
    
    
}

Wall::Wall(double startX, double startY, StudentWorld *this_world)
: Actor(IID_WALL, startX, startY, right, 0, this_world) {}

Exit::Exit(double startX, double startY, StudentWorld *this_world)
: Actor(IID_EXIT, startX, startY, right, 1, this_world)
{}

void Exit::doSomething()
{
    getWorld()->Player_overlapWithExit(getX(), getY());   //If the player is overlap with Exit and no more citizen, goes to next level
    //getWorld()->check_FlameoverlapWithExit(getX(), getY()); //flame overlap with Exit, block flame
    //std::cout << "exit blocks flame" << std::endl;
    
    if (getWorld()->citizen_overlapWithExit(getX(), getY()) == true)  //If the citizen are overlap with Eixt, destory them ,and add score
        std::cout << "Delete citizen" << std::endl;
    
}

Pit::Pit(double startX, double startY, StudentWorld *this_world)
: Actor(IID_PIT, startX, startY, right, 0, this_world) {}

void Pit::doSomething()
{
    getWorld()->overlapWithPit(getX(), getY());      //if any actors are overlap with Pit, decLives();
}

Projectile::Projectile(int imageID, double startX, double startY, StudentWorld *this_world)
:Actor(imageID, startX, startY, right, 0, this_world) {
    setAlive();
}
bool Projectile::doSomethingCom()
{
    if (getStatus() == false)
        return false;
    
    tick++;
    if (tick == 2)
        setDead();
    
    return true;
}

Flame::Flame(double startX, double startY, StudentWorld *this_world)
: Projectile(IID_FLAME, startX, startY, this_world) {}
void Flame::doSomething()
{
    if (Projectile::doSomethingCom())
        getWorld()->overlapWithFlame(getX(), getY());
}

Vomit::Vomit(double startX, double startY, StudentWorld *this_world)
: Projectile(IID_VOMIT, startX, startY, this_world) {}
void Vomit::doSomething()
{
    if (Projectile::doSomethingCom())
        bool temp = getWorld()->overlapWithVomit(getX(), getY());
            
}

Googie::Googie(int imageID, double startX, double startY, StudentWorld *this_world)
: Actor(imageID, startX, startY, right, 1, this_world)
{
    setAlive();
}
bool Googie::doSomethingCom()
{
    if (getStatus() == false)
        return false;
    
    if (getWorld()->Player_overlapWith_Goodies(getX(), getY()))
    {
        setDead();
        return true;
    }
    return false;
}

Vaccine_goodie::Vaccine_goodie(double startX, double startY, StudentWorld *this_world)
: Googie(IID_VACCINE_GOODIE, startX, startY, this_world) {}
void Vaccine_goodie::doSomething()
{
    if (Googie::doSomethingCom())
        getWorld()->add_vaccine();
}

Gas_can_goodie::Gas_can_goodie(double startX, double startY, StudentWorld *this_world)
: Googie(IID_GAS_CAN_GOODIE, startX, startY, this_world) {}
void Gas_can_goodie::doSomething()
{
    if (Googie::doSomethingCom())
        getWorld()->add_flamethrower();
}

Landmine_goodie::Landmine_goodie(double startX, double startY, StudentWorld *this_world)
: Googie(IID_LANDMINE_GOODIE, startX, startY, this_world) {}
void Landmine_goodie::doSomething()
{
    if (Googie::doSomethingCom())
        getWorld()->add_landmines();
}

Landmine::Landmine(double startX, double startY, StudentWorld *this_world)
: Actor(IID_LANDMINE, startX, startY, right, 1, this_world)
{
    setAlive();
}
void Landmine::doSomething()
{
    if (getStatus() == false)
        return;
    
    tick--;
    if (tick <= 0)
    {  //landmine becomes active, and check if any person is overlap with landmine
        if (getWorld()->overlapwithLandmine(getX(), getY()))
        {
            getWorld()->landmineBoom(getX(), getY());
            setDead();
        }
    }
    //return;
}

Zombie::Zombie(int imageID, double startX, double startY, StudentWorld *this_world)
: Actor(imageID, startX, startY, right, 0, this_world)
{
    setAlive();
    movement_plan = 0;
    tick = 0;
}

void Zombie::doSomethingCom()
{
    if (getStatus() == false)
        return;
    
    tick++;
    
    if (tick % 2 == 0)
        return;
    
    if (movement_plan == 0)
    {
        movement_plan = randInt(3, 10);
        rand_dir = randInt(1, 4);  //random direction
    }
    
    if (tick % 2 != 0)
    {
        if (rand_dir == 1)
        {
            setDirection(up);
            if (!getWorld()->check_collision(getX(), getY() + 1, up))
            {
                moveTo(getX(), getY() + 1);
                movement_plan--;
            }
            else
                movement_plan = 0;
        }
        else if (rand_dir == 2)
        {
            setDirection(down);
            if (!getWorld()->check_collision(getX(), getY() - 1, down))
            {
                moveTo(getX(), getY() - 1);
                movement_plan--;
            }
            else
                movement_plan = 0;
        }
        else if (rand_dir == 3)
        {
            setDirection(left);
            if (!getWorld()->check_collision(getX() - 1, getY(), left))
            {
                moveTo(getX() - 1, getY());
                movement_plan--;
            }
            else
                movement_plan = 0;
        }
        else if (rand_dir == 4)
        {
            setDirection(right);
            if (!getWorld()->check_collision(getX() + 1, getY(), right))
            {
                moveTo(getX() + 1, getY());
                movement_plan--;
            }
            else
                movement_plan = 0;
        }
        
        Zombie::doSomethingVomitCom();
    }
}

void Zombie::doSomethingVomitCom()
{
    
    if (getDirection() == up)
    {
        if (getWorld()->check_personInFront(getX(), getY() + SPRITE_HEIGHT))
        {
            getWorld()->compute_vomit(getX(), getY(), up);
            if (getWorld()->overlapWithVomit(getX(), getY() + SPRITE_HEIGHT))
            {
                int temp = randInt(1, 3);
                if (temp == 1)
                {
                    getWorld()->compute_vomit(getX(), getY(), up);
                    return;
                }
            }
        }
    }
    
    if (getDirection() == down)
    {
        if (getWorld()->check_personInFront(getX(), getY() - SPRITE_HEIGHT))
        {
            getWorld()->compute_vomit(getX(), getY(), down);
            if (getWorld()->overlapWithVomit(getX(), getY() - SPRITE_HEIGHT))
            {
                int temp = randInt(1, 3);
                if (temp == 1)
                {
                    getWorld()->compute_vomit(getX(), getY(), down);
                    return;
                }
            }
        }
        
    }
    
    if (getDirection() == left)
    {
        if (getWorld()->check_personInFront(getX() - SPRITE_WIDTH, getY()))
        {
            getWorld()->compute_vomit(getX(), getY(), left);
            if (getWorld()->overlapWithVomit(getX() - SPRITE_WIDTH, getY()))
            {
                int temp = randInt(1, 3);
                if (temp == 1)
                {
                    getWorld()->compute_vomit(getX(), getY(), left);
                    return;
                }
            }
        }
    }
    
    if (getDirection() == right)
    {
        if (getWorld()->check_personInFront(getX() + SPRITE_WIDTH, getY()))
        {
            getWorld()->compute_vomit(getX(), getY(), right);
            if (getWorld()->overlapWithVomit(getX() + SPRITE_WIDTH, getY()))
            {
                int temp = randInt(1, 3);
                if (temp == 1)
                {
                    getWorld()->compute_vomit(getX(), getY(), right);
                    return;
                }
            }
        }
    }
}
void Zombie::goUp()
{
    setDirection(up);
    if (!getWorld()->check_collision(getX(), getY() + 1, up))
    {
        moveTo(getX(), getY() + 1);
    }
}

void Zombie::goDown()
{
    setDirection(down);
    if (!getWorld()->check_collision(getX(), getY() - 1, down))
    {
        moveTo(getX(), getY() - 1);
    }
}

void Zombie::goLeft()
{
    setDirection(left);
    if (!getWorld()->check_collision(getX() - 1, getY(), left))
    {
        moveTo(getX() - 1, getY());
    }
}

void Zombie::goRight()
{
    setDirection(right);
    if (!getWorld()->check_collision(getX() + 1, getY(), right))
    {
        moveTo(getX() + 1, getY());
    }
}

DumbZombie::DumbZombie(double startX, double startY, StudentWorld *this_world)
:Zombie(IID_ZOMBIE, startX, startY, this_world) {}

void DumbZombie::doSomething()
{
    Zombie::doSomethingCom();
}

SmartZombie::SmartZombie(double startX, double startY, StudentWorld *this_world)
:Zombie(IID_ZOMBIE, startX, startY, this_world)
{
    tick = 0;
}

void SmartZombie::doSomething()
{
    getWorld()->searchCloestPeople(getX(), getY(), cloest_x, cloest_y, distance, isThreat);
    if (distance > 6400)
    {
        Zombie::doSomethingCom();
    }
    else if (distance <= 6400)
    {
        tick++;
        
        if (tick % 2 == 0)
            return;
        
        // if (tick % 2 != 0)
        //{
        if (cloest_x == getX())
        {
            if (cloest_y > getY())
            {
                Zombie::goUp();
            }
            else if (cloest_y < getY())
            {
                Zombie::goDown();
            }
        }
        
        if (cloest_y == getY())
        {
            if (cloest_x < getY())
            {
                Zombie::goLeft();
            }
            else if (cloest_x > getY())
            {
                Zombie::goRight();
            }
        }
        
        if (cloest_y > getY() && cloest_x < getX())  //up left
        {
            int randDir = randInt(1, 2);
            if (randDir == 1)  //horizontal
            {
                Zombie::goLeft();
            }
            else             //vertical
            {
                Zombie::goUp();
            }
        }
        if (cloest_y > getY() && cloest_x > getX()) // up right
        {
            int randDir = randInt(1, 2);
            if (randDir == 1)  //horizontal
            {
                Zombie::goRight();
            }
            else             //vertical
            {
                Zombie::goUp();
            }
        }
        if (cloest_y < getY() && cloest_x < getX()) //down left
        {
            int randDir = randInt(1, 2);
            if (randDir == 1)  //horizontal
            {
                Zombie::goLeft();
            }
            else             //vertical
            {
                Zombie::goDown();
            }
        }
        if (cloest_y < getY() && cloest_x > getX()) // down right
        {
            int randDir = randInt(1, 2);
            if (randDir == 1)  //horizontal
            {
                Zombie::goRight();
            }
            else             //vertical
            {
                Zombie::goDown();
            }
        }
        Zombie::doSomethingVomitCom();
    }
    //std::cout << distance << std::endl;
}


