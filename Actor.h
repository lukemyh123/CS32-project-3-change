#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Goodie;

class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld *this_world);
	virtual void doSomething() = 0;
	bool getStatus() { return object_alive; }
	void setDead() { object_alive = false; }
	void setAlive() { object_alive = true; }
	StudentWorld * getWorld() { return s_world; }
	virtual bool isHuman() { return false; }
	virtual bool isZombie() { return false; }

	virtual bool isGoodie() { return false; }
	virtual bool isBlockActor() = 0;
	virtual bool canBeDamagedByFlame() = 0;
	virtual bool canBeDamagedByVomit() { return false; }
	virtual bool canBlockFlame() { return false; }
	virtual bool isAFlame() { return false; }  //----------------------
	virtual bool person() { return false; }

	virtual void score_doSomething() { return; }
	virtual void chanceToCreateVaccine() { return; }

	bool getInfection_status() { return infection_status; }
	void addInfection() { infection++; }
	void setInfection() { infection_status = true; }
	void cureInfection() { infection_status = false; infection = 0; }
	int getInfectedNumber() { return infection; }

	~Actor();
private:
	bool object_alive = true;
	StudentWorld* s_world;
	int infection;
	bool infection_status;
};

class Agent : public Actor
{
public:
	Agent(int imageID, double startX, double startY, StudentWorld *this_world);
	//virtual void doSomething() = 0;
	virtual bool isBlockActor() { return true; }
	virtual bool canBeDamagedByFlame() { return true; }
	virtual bool person() { return true; }
	virtual void follow_player(double x, double y, double player_x, double player_y, int pixel);
	virtual void runAwayFromZombie(double x, double y, double zombie_x, double zombie_y, int pixel);
	virtual void goLeft(double x, double y, int p);
	virtual void goRight(double x, double y, int p);
	virtual void goUp(double x, double y, int p);
	virtual void goDown(double x, double y, int p);
};

class Human : public Agent
{
public:
	Human(int imageID, double start_x, double start_y, StudentWorld *this_world);
	virtual void doSomethingCom();
	virtual void doSomething() = 0;
	virtual bool canBeDamagedByVomit() { return true; }
	virtual bool isHuman() { return true; }

private:

};


class Penelope : public Human
{
public:
	Penelope(double startX, double startY, StudentWorld *this_world);
	virtual void doSomething();

	void reset_goodies() { vaccine = 0; flamethrower = 0; landmines = 0; }
	void add_vaccine() { vaccine++; }
	void reduce_vaccine() { vaccine--; }
	void add_flamethrower() { flamethrower += 5; }
	void reduce_flamethrower() { flamethrower--; }
	void add_landmines() { landmines += 2; }
	void reduce_landmines() { landmines--; }
	int get_vaccine() { return vaccine; }
	int get_flamethrower() { return flamethrower; }
	int get_landmines() { return landmines; }
private:
	int vaccine = 0;
	int flamethrower = 0;
	int landmines = 0;
};

class Citizen : public Human
{
public:
	Citizen(double startX, double startY, StudentWorld * this_world);
	virtual void doSomething();
	virtual void score_doSomething();
private:
	int tick;
	double player_x = 0;
	double player_y = 0;
	double zombie_x = 0;
	double zombie_y = 0;
	double distanceFromPlayer = 0;
	double distanceFromNearestZombie = 0;
	bool isThreat;
};

class Wall : public Actor
{
public:
	Wall(double startX, double startY, StudentWorld *this_world);
	virtual void doSomething() {}
	virtual bool isBlockActor() { return true; }
	virtual bool canBeDamagedByFlame() { return false; }
	virtual bool canBlockFlame() { return true; }

private:
};

class Exit : public Actor  // 3 for Exit
{
public:
	Exit(double startX, double startY, StudentWorld *this_world);
	virtual void doSomething();
	virtual bool isBlockActor() { return false; }
	virtual bool canBeDamagedByFlame() { return false; }
	virtual bool canBlockFlame() { return true; }

};

class Pit : public Actor
{
public:
	Pit(double startX, double startY, StudentWorld *this_world);
	virtual void doSomething();
	virtual bool isBlockActor() { return false; }
	virtual bool canBeDamagedByFlame() { return false; }
};

class Projectile :public Actor
{
public:
	Projectile(int imageID, double startX, double startY, StudentWorld *this_world);
	virtual bool doSomethingCom();
	virtual void doSomething() = 0;
	virtual bool isBlockActor() { return false; }
	virtual bool canBeDamagedByFlame() { return false; }
	virtual bool isAFlame() = 0;

private:
	int tick = 0;
};
class Flame : public Projectile
{
public:
	Flame(double startX, double startY, StudentWorld *this_world);
	virtual void doSomething();
	virtual bool isBlockActor() { return false; }
	virtual bool canBeDamagedByFlame() { return false; }
	virtual bool isAFlame() { return true; }
};

class Vomit : public Projectile
{
public:
	Vomit(double startX, double startY, StudentWorld *this_world);
	virtual void doSomething();
	virtual bool isBlockActor() { return false; }
	virtual bool canBeDamagedByFlame() { return false; }
	virtual bool isAFlame() { return false; }
};

class Goodie : public Actor
{
public:
	Goodie(int imageID, double startX, double startY, StudentWorld *this_world);
	virtual bool doSomethingCom();
	virtual void pickUp(Penelope* p) = 0;
	virtual bool isBlockActor() { return false; }
	virtual bool isGoodie() { return true; }
	virtual bool canBeDamagedByFlame() { return true; }
};

class Vaccine_goodie : public Goodie
{
public:
	Vaccine_goodie(double startX, double startY, StudentWorld *this_world);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);
private:
};

class Gas_can_goodie : public Goodie
{
public:
	Gas_can_goodie(double startX, double startY, StudentWorld *this_world);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);

private:
};

class Landmine_goodie : public Goodie
{
public:
	Landmine_goodie(double startX, double startY, StudentWorld *this_world);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);
private:
};

class Landmine : public Actor
{
public:
	Landmine(double startX, double startY, StudentWorld *this_world);
	virtual void doSomething();
	virtual bool isBlockActor() { return false; }
	virtual bool canBeDamagedByFlame() { return false; }
	virtual bool isALandmine() { return true; }

private:
	int tick = 30;
};

class Zombie : public Agent
{
public:
	Zombie(double startX, double startY, StudentWorld *this_world);
	virtual void doSomethingCom();
	virtual void doSomethingVomitCom();
	virtual void doSomething() = 0;
	virtual bool isZombie() { return true; }
private:
	int tick;
	int movement_plan;
	int rand_dir;
};


class DumbZombie : public Zombie
{
public:
	DumbZombie(double startX, double startY, StudentWorld *this_world);
	virtual void doSomething();
	virtual void score_doSomething();
	virtual void chanceToCreateVaccine();
};

class SmartZombie : public Zombie
{
public:
	SmartZombie(double startX, double startY, StudentWorld *this_world);
	virtual void doSomething();
	virtual void score_doSomething();
private:
	int tick;
	double cloest_x = 0;
	double cloest_y = 0;
	double distance = 0;
	bool isThreat;
};

#endif // ACTOR_H_