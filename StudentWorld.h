#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Penelope;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	bool moveToNextLevel() { return go_next_level; }

	bool check_collisionForPlayer(double next_x, double next_y);
	bool check_collision(double next_x, double next_y, int p, int dir);
	bool check_collision_helper(double x1, double y1, double next_x, double next_y, int p);
	bool check_personInFront(double next_x, double next_y);

	bool block_Flame(double x, double y);
	void Player_overlapWithExit(double exit_x, double exit_y);
	void citizen_overlapWithExit(double exit_x, double exit_y);
	bool Player_overlapWith_Goodies(double x, double y);

	void overlapWithPit(double pit_x, double pit_y);
	void overlapWithFlame(double flame_x, double flame_y);
	void overlapWithVomit(double vomit_x, double vomit_y);

	void fire(double x, double y, int dir);
	void compute_vomit(double x, double y, int dir);

	void searchClosestPeople(double x, double y, double& cloest_x, double& cloest_y, double& distance, bool &isThreat);
	void citizenDistanceToPlayer(double citizen_x, double citizen_y, double& player_x, double& player_y, double& distance);

	void searchNearestZombie(double citizen_x, double citizen_y, double& zombie_x, double& zombie_y, double& distance);

	void createAZombie(double x, double y);
	void createAVaccineGoodie(double x, double y);
	void placeLandmine(double x, double y);
	void landmineBoom(double landmine_x, double landmine_y);
	bool overlapwithLandmine(double x, double y);

	void setGame_info();
	Penelope* getPenelope() { return m_penelope; }

	int getNumCitizen() { return num_citizen; }

private:
	std::list<Actor*> m_actors;
	Penelope* m_penelope;
	GameWorld* gameWorld;
	bool go_next_level = false;
	int num_citizen = 0;
};

#endif // STUDENTWORLD_H_