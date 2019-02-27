#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <sstream>   //ostringstream
#include <string>
#include <iomanip>  //setw()
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath) {}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    //advanceToNextLevel();
    go_next_level = false; //reset the player can go to next level by exit
    reset_goodies();
    for (int x = 0; x < SPRITE_WIDTH; x++)
    {
        for (int y = 0; y < SPRITE_HEIGHT; y++)
        {
            string name_actor = check_actorsPos(x, y);
            if (name_actor == "wall")
                m_actors.push_back(new Wall(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
            else if (name_actor == "player")
                m_penelope = new Penelope(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
            else if (name_actor == "exit")
                m_actors.push_back(new Exit(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
            else if (name_actor == "pit")
                m_actors.push_back(new Pit(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
            else if (name_actor == "vaccine_goodie")
                m_actors.push_back(new Vaccine_goodie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
            else if (name_actor == "gas_can_goodie")
                m_actors.push_back(new Gas_can_goodie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
            else if (name_actor == "landmine_goodie")
                m_actors.push_back(new Landmine_goodie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
            else if (name_actor == "dumb_zombie")
                m_actors.push_back(new DumbZombie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
            else if (name_actor == "smart_zombie")
                m_actors.push_back(new SmartZombie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //decLives();
    setGame_info();
    if (m_penelope->getStatus())
        m_penelope->doSomething();
    
    vector<Actor*>::iterator it;  //delete all actors
    for (it = m_actors.begin(); it != m_actors.end();)
    {
        if ((*it)->getStatus() == true)
        {
            (*it)->doSomething();
        }
        
        if ((*it)->getStatus() == false)
        {
            delete *it;
            it = m_actors.erase(it);
            // it = m_actors.begin();
        }
        else
            it++;
        
    }
    
    if (moveToNextLevel())
        return GWSTATUS_FINISHED_LEVEL;
    
    //cout << Player_dead() << endl;
    if (m_penelope->getStatus() == false)
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_penelope;  //delete player
    
    vector<Actor*>::iterator it;  //delete all actors
    for (it = m_actors.begin(); it != m_actors.end();)
    {
        delete *it;
        it = m_actors.erase(it);
    }
}

bool StudentWorld::check_collisionForPlayer(double next_x, double next_y)
{
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->isBlockActor() == true)
        {
            if ((next_x + SPRITE_WIDTH - 1) >= ((*it)->getX())
                && (next_x) <= ((*it)->getX() + SPRITE_WIDTH - 1)
                && (next_y + SPRITE_HEIGHT - 1) >= ((*it)->getY())
                && (next_y) <= ((*it)->getY() + SPRITE_HEIGHT - 1))
                return true;
        }
    }
    return false;
}
bool StudentWorld::check_collision_helper(double x1, double y1, double next_x, double next_y)
{
    //make sure the blocking objects don't block itself;
    if (((next_x + SPRITE_WIDTH - 1) >= x1                     //check any blocking objects in the vector
         && (next_x) <= (x1 + SPRITE_WIDTH - 1)
         && (next_y + SPRITE_HEIGHT - 1) >= y1
         && (next_y) <= (y1 + SPRITE_HEIGHT - 1))
        || ((next_x + SPRITE_WIDTH - 1) >= (m_penelope->getX())           //check the player is blocking or not
            && (next_x) <= (m_penelope->getX() + SPRITE_WIDTH - 1)
            && (next_y + SPRITE_HEIGHT - 1) >= (m_penelope->getY())
            && (next_y) <= (m_penelope->getY() + SPRITE_HEIGHT - 1)))
    {
        return true;
    }
    else
        return false;
}
bool StudentWorld::check_collision(double next_x, double next_y, int dir)
{
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->isBlockActor() == true)  //check whether the actors are bounder boxs collision
        {
            if (dir == 180)  //1 for left
            {
                if (!((*it)->getX() == next_x + 1 && (*it)->getY() == next_y))
                    if (check_collision_helper((*it)->getX(), (*it)->getY(), next_x, next_y))
                        return true;
            }
            else if (dir == 0)  //2 for right
            {
                if (!((*it)->getX() == next_x - 1 && (*it)->getY() == next_y))               //make sure the blocking objects don't block itself;
                    if (check_collision_helper((*it)->getX(), (*it)->getY(), next_x, next_y))
                        return true;
            }
            else if (dir == 90)  //3 for up
            {
                if (!((*it)->getX() == next_x && (*it)->getY() == next_y - 1))               //make sure the blocking objects don't block itself;
                    if (check_collision_helper((*it)->getX(), (*it)->getY(), next_x, next_y))
                        return true;
            }
            else if (dir == 270) //4 for down
            {
                if (!((*it)->getX() == next_x && (*it)->getY() == next_y + 1))               //make sure the blocking objects don't block itself;
                    if (check_collision_helper((*it)->getX(), (*it)->getY(), next_x, next_y))
                        return true;
            }
        }
    }
    return false;
}

bool StudentWorld::check_personInFront(double next_x, double next_y)
{
    double player_x = m_penelope->getX();
    double player_y = m_penelope->getY();
    if (pow(player_x - next_x, 2) + pow(player_y - next_y, 2) <= 100)
        return true;
    
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->canBeDamagedByVomit() == true)  //check whether the actors are bounder boxs collision
        {
            cout << "citizen" << endl;
        }
    }
    
    return false;
}
bool StudentWorld::check_whatCanByDamagedByVomit(double x, double y)
{
    double player_x = m_penelope->getX();
    double player_y = m_penelope->getY();
    
    if (pow(player_x - x, 2) + pow(player_y - y, 2) <= 100)
        return true;
    
    /*vector<Actor*>::iterator it;
     for (it = m_actors.begin(); it != m_actors.end(); it++)
     {
     if ((*it)->canBeDamagedByVomit())
     return true;
     }*/
    return false;
}
bool StudentWorld::block_Flame(double x, double y)
{
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->canBlockFlame())
        {
            if (pow((*it)->getX() - x, 2) + pow((*it)->getY() - y, 2) <= 100)
                return true;
        }
    }
    return false;
}


void StudentWorld::Player_overlapWithExit(double exit_x, double exit_y)    //check if player is overlap with Exit
{
    double player_x = m_penelope->getX();
    double player_y = m_penelope->getY();
    
    if (pow(player_x - exit_x, 2) + pow(player_y - exit_y, 2) <= 100)  //overlap(x1-x2)^2 + (y1-y2)^2 ≤ 10^2
    {
        go_next_level = true;
        return;
    }
}

bool StudentWorld::citizen_overlapWithExit(double exit_x, double exit_y)
{
    vector<Actor*>::iterator it;
    
    return false;
}

void StudentWorld::overlapWithFlame(double flame_x, double flame_y)  //damage by flame
{
    double player_x = m_penelope->getX();   //check if player are overlap with Flame
    double player_y = m_penelope->getY();
    if (pow(player_x - flame_x, 2) + pow(player_y - flame_y, 2) <= 100)
        m_penelope->setDead();
    
    vector<Actor*>::iterator it;
    
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->canBeDamagedByFlame())// && !(*it)->isALandmine())
        {
            if (pow((*it)->getX() - flame_x, 2) + pow((*it)->getY() - flame_y, 2) <= 100)  //overlap(x1-x2)^2 + (y1-y2)^2 ≤ 10^2
                (*it)->setDead();
        }
    }
}

bool StudentWorld::overlapWithVomit(double vomit_x, double vomit_y)
{
    double player_x = m_penelope->getX();   //check if player are overlap with Vomit
    double player_y = m_penelope->getY();
    if (pow(player_x - vomit_x, 2) + pow(player_y - vomit_y, 2) <= 100)
    {
        if (m_penelope->getInfection_status() == false)
            m_penelope->setInfection();   //set infection
        return true;
    }
    
    vector<Actor*>::iterator it;
    
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->canBeDamagedByVomit())
        {
            if (pow((*it)->getX() - vomit_x, 2) + pow((*it)->getY() - vomit_y, 2) <= 100)  //overlap(x1-x2)^2 + (y1-y2)^2 ≤ 10^2
            {
                { cout << "vomitToOther" << endl; }
            }//(*it)->setDead();
        }
    }
    
    return false;
}

bool StudentWorld::Player_overlapWith_Goodies(double goodies_x, double goodies_y)
{
    double player_x = m_penelope->getX();   //check if player are overlap with goodies
    double player_y = m_penelope->getY();
    
    if (pow(player_x - goodies_x, 2) + pow(player_y - goodies_y, 2) <= 100)  //overlap(x1-x2)^2 + (y1-y2)^2 ≤ 10^2
    {
        increaseScore(50);
        return true;
    }
    
    return false;
}

void StudentWorld::overlapWithPit(double pit_x, double pit_y)
{
    double player_x = m_penelope->getX();   //check if player are overlap with pit
    double player_y = m_penelope->getY();
    
    if (pow(player_x - pit_x, 2) + pow(player_y - pit_y, 2) <= 100)  //overlap(x1-x2)^2 + (y1-y2)^2 ≤ 10^2
    {
        m_penelope->setDead();
        return;
    }
    
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->canBeDamagedByFlame())
        {
            if (pow((*it)->getX() - pit_x, 2) + pow((*it)->getY() - pit_y, 2) <= 100)  //overlap(x1-x2)^2 + (y1-y2)^2 ≤ 10^2
                (*it)->setDead();
        }
    }
}

void StudentWorld::fire(double x, double y, int dir)
{
    //cout << ExitBlockFlame << endl;
    if (dir == 1)  //1 for left
    {
        for (int i = 1; i < 4; i++)
        {
            if (block_Flame(x - i * SPRITE_WIDTH, y))
                break;
            m_actors.push_back(new Flame(x - i * SPRITE_WIDTH, y, this));
        }
    }
    else if (dir == 2)  //2 for right
    {
        for (int i = 1; i < 4; i++)
        {
            if (block_Flame(x + i * SPRITE_WIDTH, y))
                break;
            m_actors.push_back(new Flame(x + i * SPRITE_WIDTH, y, this));
        }
    }
    else if (dir == 3) //3 for up
    {
        for (int i = 1; i < 4; i++)
        {
            if (block_Flame(x, y + i * SPRITE_HEIGHT))
                break;
            m_actors.push_back(new Flame(x, y + i * SPRITE_HEIGHT, this));
        }
    }
    else if (dir == 4) //4 for down
    {
        for (int i = 1; i < 4; i++)
        {
            if (block_Flame(x, y - i * SPRITE_HEIGHT))
                break;
            m_actors.push_back(new Flame(x, y - i * SPRITE_HEIGHT, this));
        }
    }
}

void StudentWorld::compute_vomit(double x, double y, int dir)
{
    if (dir == 180)  //180 for left
    {
        if (check_whatCanByDamagedByVomit(x - SPRITE_WIDTH, y))
            m_actors.push_back(new Vomit(x - SPRITE_WIDTH, y, this));
    }
    else if (dir == 0)  //0 for right
    {
        if (check_whatCanByDamagedByVomit(x + SPRITE_WIDTH, y))
            m_actors.push_back(new Vomit(x + SPRITE_WIDTH, y, this));
    }
    else if (dir == 90) //90 for up
    {
        if (check_whatCanByDamagedByVomit(x, y + SPRITE_HEIGHT))
            m_actors.push_back(new Vomit(x, y + SPRITE_HEIGHT, this));
    }
    else if (dir == 270) //270 for down
    {
        if (check_whatCanByDamagedByVomit(x, y - SPRITE_WIDTH))
            m_actors.push_back(new Vomit(x, y - SPRITE_HEIGHT, this));
    }
}

void StudentWorld::searchCloestPeople(double zombie_x, double zombie_y, double& cloest_x, double& cloest_y, double& distance)
{
    double player_x = m_penelope->getX();   //search player location
    double player_y = m_penelope->getY();
    
    distance = pow(player_x - zombie_x, 2) + pow(player_y - zombie_y, 2);
    cloest_x = player_x;
    cloest_y = player_y;
    /*vector<Actor*>::iterator it;
     for (it = m_actors.begin(); it != m_actors.end(); it++)
     {
     if((*it)->canBeDamagedByVomit() && (*it)->canBeDamagedByFlame())
     {
     distance = pow((*it)->getX() - zombie_x, 2) + pow((*it)->getY() - zombie_y, 2);
     }
     }*/
}

void StudentWorld::placeLandmine(double x, double y)
{
    m_actors.push_back(new Landmine(x, y, this));
    reduce_landmines();
}

bool StudentWorld::overlapwithLandmine(double x, double y)
{
    double player_x = m_penelope->getX();   //check if player are overlap with pit
    double player_y = m_penelope->getY();
    if (pow(player_x - x, 2) + pow(player_y - y, 2) <= 100)  //overlap(x1-x2)^2 + (y1-y2)^2 ≤ 10^2
    {
        return true;
    }
    
    vector<Actor*>::iterator it;
    
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->person() || (*it)->isAFlame())
        {
            if (pow((*it)->getX() - x, 2) + pow((*it)->getY() - y, 2) <= 100)  //overlap(x1-x2)^2 + (y1-y2)^2 ≤ 10^2
                return true;
        }
    }
    return false;
}

void StudentWorld::landmineBoom(double landmine_x, double landmine_y)
{
    if (!block_Flame(landmine_x - SPRITE_WIDTH, landmine_y + SPRITE_HEIGHT))
        m_actors.push_back(new Flame(landmine_x - SPRITE_WIDTH, landmine_y + SPRITE_HEIGHT, this)); //northwest
    
    if (!block_Flame(landmine_x + SPRITE_WIDTH, landmine_y))
        m_actors.push_back(new Flame(landmine_x + SPRITE_WIDTH, landmine_y, this));  //east
    
    if (!block_Flame(landmine_x + SPRITE_WIDTH, landmine_y - SPRITE_HEIGHT))
        m_actors.push_back(new Flame(landmine_x + SPRITE_WIDTH, landmine_y - SPRITE_HEIGHT, this));  //southeast
    
    if (!block_Flame(landmine_x, landmine_y - SPRITE_HEIGHT))
        m_actors.push_back(new Flame(landmine_x, landmine_y - SPRITE_HEIGHT, this));  //south
    
    if (!block_Flame(landmine_x - SPRITE_WIDTH, landmine_y - SPRITE_HEIGHT))
        m_actors.push_back(new Flame(landmine_x - SPRITE_WIDTH, landmine_y - SPRITE_HEIGHT, this));  //southwest
    
    if (!block_Flame(landmine_x - SPRITE_WIDTH, landmine_y))
        m_actors.push_back(new Flame(landmine_x - SPRITE_WIDTH, landmine_y, this));  //west
    
    if (!block_Flame(landmine_x, landmine_y + SPRITE_HEIGHT))
        m_actors.push_back(new Flame(landmine_x, landmine_y + SPRITE_HEIGHT, this));  //north
    
    if (!block_Flame(landmine_x + SPRITE_WIDTH, landmine_y + SPRITE_HEIGHT))
        m_actors.push_back(new Flame(landmine_x + SPRITE_WIDTH, landmine_y + SPRITE_HEIGHT, this));  //northeast
    
    //create a pit
    m_actors.push_back(new Pit(landmine_x, landmine_y, this));
}

void StudentWorld::setGame_info()
{
    ostringstream oss;
    oss.setf(ios::fixed);
    
    //Score:    004500        Level:    27        Lives:    3        Vaccines:    2        Flames:    16        Mines:    1        Infected:    0
    oss << "Score: " << setw(2) << getScore() << "  Level: " << setw(2) << getLevel() << "  Lives: " << setw(2) << getLives()
    << "  Vaccines: " << setw(2) << get_vaccine() << "  Flames; " << setw(2) << get_flamethrower()
    << "  Mines: " << setw(2) << get_landmines() << "  Infected: " << setw(2) << m_penelope->getInfectedNumber();
    setGameStatText(oss.str());
}

std::string StudentWorld::check_actorsPos(int x, int y)
{
    Level lev(assetPath());
    ostringstream oss;
    //oss << "level0" << getLevel() << ".txt";
    oss << "level0" << 4 << ".txt";
    string levelFile = oss.str();
    
    Level::LoadResult result = lev.loadLevel(levelFile);
    if (result == Level::load_fail_file_not_found)
    {
        cerr << "Cannot find level01.txt data file" << endl;  // gameover
        exit(1);  //need to change
    }
    else if (result == Level::load_fail_bad_format)
        cerr << "Your level was improperly formatted" << endl;
    else if (result == Level::load_success)
    {
        //cerr << "Successfully loaded level" << endl;
        Level::MazeEntry ge = lev.getContentsOf(x, y); // level_x=5, level_y=10
        switch (ge) // so x=80 and y=160
        {
            case Level::empty:
                return "empty";
                break;
            case Level::smart_zombie:
                return "smart_zombie";
                break;
            case Level::dumb_zombie:
                return "dumb_zombie";
                break;
            case Level::player:
                return "player";
                break;
            case Level::exit:
                return "exit";
                break;
            case Level::wall:
                return "wall";
                break;
            case Level::pit:
                return "pit";
                break;
            case Level::vaccine_goodie:
                return "vaccine_goodie";
                break;
            case Level::gas_can_goodie:
                return "gas_can_goodie";
                break;
            case Level::landmine_goodie:
                return "landmine_goodie";
                break;
            default:
                break;
        }
    }
    
    return "";
}
