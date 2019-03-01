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
	Level lev(assetPath());
	ostringstream oss;
	oss << "level0" << getLevel() << ".txt";
	//oss << "level0" << 4 << ".txt"; for testing each level
	string levelFile = oss.str();

	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
	{
		return GWSTATUS_LEVEL_ERROR;
	}
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
	else if (result == Level::load_success)
	{
		//cerr << "Successfully loaded level" << endl;
		go_next_level = false; //reset the player can go to next level by exit

		for (int x = 0; x < SPRITE_WIDTH; x++)
		{
			for (int y = 0; y < SPRITE_HEIGHT; y++)
			{
				Level::MazeEntry ge = lev.getContentsOf(x, y); // level_x=5, level_y=10
				switch (ge) // so x=80 and y=160
				{
				case Level::smart_zombie:
					m_actors.push_back(new SmartZombie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
					break;
				case Level::dumb_zombie:
					m_actors.push_back(new DumbZombie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
					break;
				case Level::player:
					m_penelope = new Penelope(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
					break;
				case Level::exit:
					m_actors.push_back(new Exit(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
					break;
				case Level::wall:
					m_actors.push_back(new Wall(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
					break;
				case Level::pit:
					m_actors.push_back(new Pit(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
					break;
				case Level::vaccine_goodie:
					m_actors.push_back(new Vaccine_goodie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
					break;
				case Level::gas_can_goodie:
					m_actors.push_back(new Gas_can_goodie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
					break;
				case Level::landmine_goodie:
					m_actors.push_back(new Landmine_goodie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
					break;
				case Level::citizen:
					{
						m_actors.push_back(new Citizen(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this));
						num_citizen++; 
					}
					break;
				default:
					break;
				}
			}
		}

	}

	m_penelope->reset_goodies();
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

	list<Actor*>::iterator it;  //delete all actors
	for (it = m_actors.begin(); it != m_actors.end();)
	{
		if ((*it)->getStatus() == true)
		{
			(*it)->doSomething();
		}

		if ((*it)->getStatus() == false)
		{
			if ((*it)->isHuman())
			{
				num_citizen--;
			}
			if ((*it)->isZombie())
			{
				(*it)->chanceToCreateVaccine();  //1/10 chance to create a VaccineGoodie
				playSound(SOUND_ZOMBIE_DIE);
			}
			delete *it;
			it = m_actors.erase(it);
		}
		else
			it++;

	}

	if (moveToNextLevel() && getNumCitizen() == 0)
	{
		playSound(SOUND_LEVEL_FINISHED);
		return GWSTATUS_FINISHED_LEVEL;
	}

	if (m_penelope->getStatus() == false)
	{
		playSound(SOUND_PLAYER_DIE);
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete m_penelope;  //delete player

	list<Actor*>::iterator it;  //delete all actors
	for (it = m_actors.begin(); it != m_actors.end();)
	{
		delete *it;
		it = m_actors.erase(it);
	}
}

bool StudentWorld::check_collisionForPlayer(double next_x, double next_y)
{
	list<Actor*>::iterator it;
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
bool StudentWorld::check_collision_helper(double x1, double y1, double next_x, double next_y, int p)
{
	//make sure the blocking objects don't block itself;
	if (((next_x + SPRITE_WIDTH - p) >= x1                     //check any blocking objects in the vector
		&& (next_x) <= (x1 + SPRITE_WIDTH - p)
		&& (next_y + SPRITE_HEIGHT - p) >= y1
		&& (next_y) <= (y1 + SPRITE_HEIGHT - p))
		|| ((next_x + SPRITE_WIDTH - p) >= (m_penelope->getX())           //check the player is blocking or not
			&& (next_x) <= (m_penelope->getX() + SPRITE_WIDTH - p)
			&& (next_y + SPRITE_HEIGHT - p) >= (m_penelope->getY())
			&& (next_y) <= (m_penelope->getY() + SPRITE_HEIGHT - p)))
	{
		return true;
	}
	else
		return false;
}
bool StudentWorld::check_collision(double next_x, double next_y, int p, int dir)
{
	list<Actor*>::iterator it;
	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isBlockActor() == true)  //check whether the actors are bounder boxs collision
		{
			if (dir == 180)  //1 for left
			{
				if (!((*it)->getX() == next_x + p && (*it)->getY() == next_y))
					if (check_collision_helper((*it)->getX(), (*it)->getY(), next_x, next_y, p))
						return true;
			}
			else if (dir == 0)  //2 for right
			{
				if (!((*it)->getX() == next_x - p && (*it)->getY() == next_y))               //make sure the blocking objects don't block itself;
					if (check_collision_helper((*it)->getX(), (*it)->getY(), next_x, next_y, p))
						return true;
			}
			else if (dir == 90)  //3 for up
			{
				if (!((*it)->getX() == next_x && (*it)->getY() == next_y - p))               //make sure the blocking objects don't block itself;
					if (check_collision_helper((*it)->getX(), (*it)->getY(), next_x, next_y, p))
						return true;
			}
			else if (dir == 270) //4 for down
			{
				if (!((*it)->getX() == next_x && (*it)->getY() == next_y + p))               //make sure the blocking objects don't block itself;
					if (check_collision_helper((*it)->getX(), (*it)->getY(), next_x, next_y, p))
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

	list<Actor*>::iterator it;
	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isHuman())
		{
			if (pow((*it)->getX() - next_x, 2) + pow((*it)->getY() - next_y, 2) <= 100)
				return true;
		}
	}

	return false;
}

bool StudentWorld::block_Flame(double x, double y)
{
	list<Actor*>::iterator it;
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
		if(getNumCitizen() == 0)
			go_next_level = true;
		return;
	}
}

void StudentWorld::citizen_overlapWithExit(double exit_x, double exit_y)
{
	list<Actor*>::iterator it;

	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isHuman())
		{
			if (pow((*it)->getX() - exit_x, 2) + pow((*it)->getY() - exit_y, 2) <= 100)  //overlap(x1-x2)^2 + (y1-y2)^2 ≤ 10^2
			{
				playSound(SOUND_CITIZEN_SAVED);
				(*it)->setDead();
				increaseScore(500);
			}
		}
	}
}

void StudentWorld::overlapWithFlame(double flame_x, double flame_y)  //damage by flame
{
	double player_x = m_penelope->getX();   //check if player are overlap with Flame
	double player_y = m_penelope->getY();
	if (pow(player_x - flame_x, 2) + pow(player_y - flame_y, 2) <= 100)
		m_penelope->setDead();

	list<Actor*>::iterator it;

	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->canBeDamagedByFlame())// && !(*it)->isALandmine())
		{
			if (pow((*it)->getX() - flame_x, 2) + pow((*it)->getY() - flame_y, 2) <= 100)  //overlap(x1-x2)^2 + (y1-y2)^2 ≤ 10^2
			{
				if ((*it)->isHuman())
				{
					playSound(SOUND_CITIZEN_DIE);
					(*it)->score_doSomething();  //-1000
				}
				if ((*it)->isZombie())
					(*it)->score_doSomething();  //-1000 for dumb  -2000 for smart
				(*it)->setDead();
			}
		}
	}
}

void StudentWorld::overlapWithVomit(double vomit_x, double vomit_y)
{
	double player_x = m_penelope->getX();   //check if player are overlap with Vomit
	double player_y = m_penelope->getY();
	if (pow(player_x - vomit_x, 2) + pow(player_y - vomit_y, 2) <= 100)
	{
		if (m_penelope->getInfection_status() == false)
			m_penelope->setInfection();   //set infection for penelope
	}

	list<Actor*>::iterator it;

	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isHuman())
		{
			if (pow((*it)->getX() - vomit_x, 2) + pow((*it)->getY() - vomit_y, 2) <= 100)  //overlap(x1-x2)^2 + (y1-y2)^2 ≤ 10^2
			{
				if ((*it)->getInfection_status() == false)
				{
					playSound(SOUND_CITIZEN_INFECTED);
					(*it)->setInfection();
				}
				//cout << "set infection for citizen;" << endl;   //set infection for citizen;
			}
		}
	}
}

bool StudentWorld::Player_overlapWith_Goodies(double x, double y)
{
	double player_x = m_penelope->getX();   //check if player are overlap with goodies
	double player_y = m_penelope->getY();

	if (pow(player_x - x, 2) + pow(player_y - y, 2) <= 100)  //overlap(x1-x2)^2 + (y1-y2)^2 ≤ 10^2
	{
		playSound(SOUND_GOT_GOODIE);
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

	list<Actor*>::iterator it;
	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->person())
		{
			if (pow((*it)->getX() - pit_x, 2) + pow((*it)->getY() - pit_y, 2) <= 100)  //overlap(x1-x2)^2 + (y1-y2)^2 ≤ 10^2
			{
				if ((*it)->isHuman())
				{
					playSound(SOUND_CITIZEN_DIE);
					(*it)->score_doSomething();  //-1000
				}
				if ((*it)->isZombie())
					(*it)->score_doSomething();  //1000 for dumb  2000 for smart
				(*it)->setDead();
			}
		}
	}
}

void StudentWorld::fire(double x, double y, int dir)
{
	playSound(SOUND_PLAYER_FIRE);
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
		if (check_personInFront(x - SPRITE_WIDTH, y))
		{
			playSound(SOUND_ZOMBIE_VOMIT);
			m_actors.push_back(new Vomit(x - SPRITE_WIDTH, y, this));
		}
	}
	else if (dir == 0)  //0 for right
	{
		if (check_personInFront(x + SPRITE_WIDTH, y))
		{
			playSound(SOUND_ZOMBIE_VOMIT);
			m_actors.push_back(new Vomit(x + SPRITE_WIDTH, y, this));
		}
	}
	else if (dir == 90) //90 for up
	{
		if (check_personInFront(x, y + SPRITE_HEIGHT))
		{
			playSound(SOUND_ZOMBIE_VOMIT);
			m_actors.push_back(new Vomit(x, y + SPRITE_HEIGHT, this));
		}
	}
	else if (dir == 270) //270 for down
	{
		if (check_personInFront(x, y - SPRITE_WIDTH))
		{
			playSound(SOUND_ZOMBIE_VOMIT);
			m_actors.push_back(new Vomit(x, y - SPRITE_HEIGHT, this));
		}
	}
}

void StudentWorld::searchClosestPeople(double x, double y, double& cloest_x, double& cloest_y, double& distance, bool &isThreat)
{
	double player_x = m_penelope->getX();   //search player location
	double player_y = m_penelope->getY();

	double player_distance = pow(player_x - x, 2) + pow(player_y - y, 2);
	double temp_distance = 0;
	double cloest_distance = 0;

	list<Actor*>::iterator it;

	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isHuman())
		{
			cloest_distance = pow((*it)->getX() - x, 2) + pow((*it)->getY() - y, 2);
			break;
		}
	}

	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isHuman())
		{
			temp_distance = pow((*it)->getX() - x, 2) + pow((*it)->getY() - y, 2);
			if (cloest_distance > temp_distance)
			{
				cloest_distance = temp_distance;
				if (cloest_x < 256 && cloest_y < 256 && cloest_x > 0 && cloest_y > 0)
				{
					cloest_x = (*it)->getX();
					cloest_y = (*it)->getY();
				}
			}

			if (player_distance < cloest_distance)
			{
				cloest_x = player_x;
				cloest_y = player_y;
				distance = player_distance;
			}
			else
			{
				distance = cloest_distance;
			}
		}
	}
}

void StudentWorld::searchNearestZombie(double citizen_x, double citizen_y, double& zombie_x, double& zombie_y, double& distance)
{
	zombie_x = 0;
	zombie_y = 0;
	double temp_distance = 0;
	double cloest_distance = 6400;
	list<Actor*>::iterator it;

	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isZombie())
		{
			cloest_distance = pow((*it)->getX() - citizen_y, 2) + pow((*it)->getY() - citizen_y, 2);
			break;
		}
	}

	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isZombie())
		{
			temp_distance = pow((*it)->getX() - citizen_x, 2) + pow((*it)->getY() - citizen_y, 2);
			if (cloest_distance > temp_distance)
			{
				cloest_distance = temp_distance;
				zombie_x = (*it)->getX();
				zombie_y = (*it)->getY();
				distance = cloest_distance;
			}
		}
	}
}

void StudentWorld::citizenDistanceToPlayer(double citizen_x, double citizen_y, double& player_x, double& player_y, double& distance)
{
	player_x = m_penelope->getX();   //search player location
	player_y = m_penelope->getY();

	distance = pow(player_x - citizen_x, 2) + pow(player_y - citizen_y, 2);
}

void StudentWorld::createAZombie(double x, double y)
{
	int temp = randInt(1, 10);
	if (temp <= 3)
		m_actors.push_back(new SmartZombie(x, y, this));
	else
		m_actors.push_back(new DumbZombie(x, y, this));
}

void StudentWorld::createAVaccineGoodie(double x, double y)
{
	int temp = randInt(1, 10);
	if (temp == 1)
		m_actors.push_back(new Vaccine_goodie(x, y, this));
}



void StudentWorld::placeLandmine(double x, double y)
{
	m_actors.push_back(new Landmine(x, y, this));
}

bool StudentWorld::overlapwithLandmine(double x, double y)
{
	double player_x = m_penelope->getX();   //check if player are overlap with pit
	double player_y = m_penelope->getY();
	if (pow(player_x - x, 2) + pow(player_y - y, 2) <= 100)  //overlap(x1-x2)^2 + (y1-y2)^2 ≤ 10^2
	{
		return true;
	}

	list<Actor*>::iterator it;

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
	playSound(SOUND_LANDMINE_EXPLODE);
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

	oss << "Score: " << oss.fill('0') << setw(6) << getScore()
		<< "  Level: " << getLevel() 
		<< "  Lives: " << getLives()
		<< "  Vaccines: " << m_penelope->get_vaccine() 
		<< "  Flames; " << m_penelope->get_flamethrower()
		<< "  Mines: " << m_penelope->get_landmines() 
		<< "  Infected: " << m_penelope->getInfectedNumber();
	setGameStatText(oss.str());
}