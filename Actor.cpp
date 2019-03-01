#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld *this_world)
	:GraphObject(imageID, startX, startY, dir, depth), s_world(this_world)
{
	infection = 0;
	infection_status = false;
}

Actor::~Actor() {}

Agent::Agent(int imageID, double startX, double startY, StudentWorld *this_world)
	:Actor(imageID, startX, startY, right, 0, this_world)
{
	setAlive();
}
void Agent::goLeft(double x, double y, int p)
{
	setDirection(left);
	if (!getWorld()->check_collision(x - p, y, p, left))
	{
		moveTo(x - p, y);
	}
}
void Agent::goRight(double x, double y, int p)
{
	setDirection(right);
	if (!getWorld()->check_collision(x + p, y, p, right))
	{
		moveTo(x + p, y);
	}
}
void Agent::goUp(double x, double y, int p)
{
	setDirection(up);
	if (!getWorld()->check_collision(x, y + p, p, up))
	{
		moveTo(x, y + p);
	}
}
void Agent::goDown(double x, double y, int p)
{
	setDirection(down);
	if (!getWorld()->check_collision(x, y - p, p, down))
	{
		moveTo(x, y - p);
	}
}
void Agent::follow_player(double x, double y, double player_x, double player_y, int pixel)
{
	if (player_x == x)
	{
		if (player_y > y)
		{
			goUp(x, y, pixel);
		}
		else if (player_y < y)
		{
			goDown(x, y, pixel);
		}
	}

	if (player_y == y)
	{
		if (player_x < x)
		{
			goLeft(x, y, pixel);
		}
		else if (player_x > x)
		{
			goRight(x, y, pixel);
		}
	}

	if (player_y > y && player_x < x)  //up left
	{
		int randDir = randInt(1, 2);
		if (randDir == 1)  //horizontal
		{
			goLeft(x, y, pixel);
		}
		else             //vertical
		{
			goUp(x, y, pixel);
		}
	}
	if (player_y > y && player_x > x) // up right
	{
		int randDir = randInt(1, 2);
		if (randDir == 1)  //horizontal
		{
			goRight(x, y, pixel);
		}
		else             //vertical
		{
			goUp(x, y, pixel);
		}
	}
	if (player_y < y && player_x < x) //down left
	{
		int randDir = randInt(1, 2);
		if (randDir == 1)  //horizontal
		{
			goLeft(x, y, pixel);
		}
		else             //vertical
		{
			goDown(x, y, pixel);
		}
	}
	if (player_y < y && player_x > x) // down right
	{
		int randDir = randInt(1, 2);
		if (randDir == 1)  //horizontal
		{
			goRight(x, y, pixel);
		}
		else             //vertical
		{
			goDown(x, y, pixel);
		}
	}
}

void Agent::runAwayFromZombie(double x, double y, double zombie_x, double zombie_y, int pixel)
{
	if (zombie_x == x)
	{
		if (zombie_y > y) //can run away to down or left or right
		{
			if (!getWorld()->check_collision(x, y - pixel, pixel, down))
			{
				setDirection(down);
				moveTo(x, y - pixel);
			}
			if (!getWorld()->check_collision(x - pixel, y, pixel, left))
			{
				setDirection(left);
				moveTo(x - pixel, y);
			}
			if (!getWorld()->check_collision(x + pixel, y, pixel, right))
			{
				setDirection(right);
				moveTo(x + pixel, y);
			}
		}
		else if (zombie_y < y)  //can run up, right, left
		{
			if (!getWorld()->check_collision(x, y + pixel, pixel, up))
			{
				setDirection(up);
				moveTo(x, y + pixel);
			}
			if (!getWorld()->check_collision(x - pixel, y, pixel, left))
			{
				setDirection(left);
				moveTo(x - pixel, y);
			}
			if (!getWorld()->check_collision(x + pixel, y, pixel, right))
			{
				setDirection(right);
				moveTo(x + pixel, y);
			}
		}
	}

	if (zombie_y == y)
	{
		if (zombie_x < x)  //run right, up, down
		{
			if (!getWorld()->check_collision(x + pixel, y, pixel, right))
			{
				setDirection(right);
				moveTo(x + pixel, y);
			}
			if (!getWorld()->check_collision(x, y + pixel, pixel, up))
			{
				setDirection(up);
				moveTo(x, y + pixel);
			}
			if (!getWorld()->check_collision(x, y - pixel, pixel, down))
			{
				setDirection(down);
				moveTo(x, y - pixel);
			}
		}
		else if (zombie_x > x)  //run left
		{
			if (!getWorld()->check_collision(x - pixel, y, pixel, left))
			{
				setDirection(left);
				moveTo(x - pixel, y);
			}
			if (!getWorld()->check_collision(x, y + pixel, pixel, up))
			{
				setDirection(up);
				moveTo(x, y + pixel);
			}
			if (!getWorld()->check_collision(x, y - pixel, pixel, down))
			{
				setDirection(down);
				moveTo(x, y - pixel);
			}
		}
	}

	if (zombie_y > y && zombie_x < x)  //up left
	{
		int randDir = randInt(1, 2);
		if (randDir == 1)  //horizontal
		{
			if (!getWorld()->check_collision(x + pixel, y, pixel, right))
			{
				setDirection(right);
				moveTo(x + pixel, y);
			}
		}
		else             //vertical
		{
			if (!getWorld()->check_collision(x, y - pixel, pixel, down))
			{
				setDirection(down);
				moveTo(x, y - pixel);
			}
		}
	}
	if (zombie_y > y && zombie_x > x) // up right
	{
		int randDir = randInt(1, 2);
		if (randDir == 1)  //horizontal
		{
			if (!getWorld()->check_collision(x - pixel, y, pixel, left))
			{
				setDirection(left);
				moveTo(x - pixel, y);
			}
		}
		else             //vertical
		{
			if (!getWorld()->check_collision(x, y - pixel, pixel, down))
			{
				setDirection(down);
				moveTo(x, y - pixel);
			}
		}
	}
	if (zombie_y < y && zombie_x < x) //down left
	{
		int randDir = randInt(1, 2);
		if (randDir == 1)  //horizontal
		{
			if (!getWorld()->check_collision(x + pixel, y, pixel, right))
			{
				setDirection(right);
				moveTo(x + pixel, y);
			}
		}
		else             //vertical
		{
			if (!getWorld()->check_collision(x, y + pixel, pixel, up))
			{
				setDirection(up);
				moveTo(x, y + pixel);
			}
		}
	}
	if (zombie_y < y && zombie_x > x) // down right
	{
		int randDir = randInt(1, 2);
		if (randDir == 1)  //horizontal
		{
			if (!getWorld()->check_collision(x - pixel, y, pixel, left))
			{
				setDirection(left);
				moveTo(x - pixel, y);
			}
		}
		else             //vertical
		{
			if (!getWorld()->check_collision(x, y + pixel, pixel, up))
			{
				setDirection(up);
				moveTo(x, y + pixel);
			}
		}
	}
}

Human::Human(int imageID, double startX, double startY, StudentWorld *this_world)
	: Agent(imageID, startX, startY, this_world)
{}

void Human::doSomethingCom()
{
	if (getStatus() == false)
		return;

	if (getInfection_status() == true)
		addInfection();

	if (getInfectedNumber() == 500)
	{
		setDead();
		getWorld()->createAZombie(getX(), getY());
	}

}
Penelope::Penelope(double startX, double startY, StudentWorld *this_world)
	: Human(IID_PLAYER, startX, startY, this_world)
{}

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
			if (getDirection() == left && get_flamethrower() != 0)
			{
				getWorld()->fire(getX(), getY(), 1);
				reduce_flamethrower();
			}
			else if (getDirection() == right && get_flamethrower() != 0)
			{
				getWorld()->fire(getX(), getY(), 2);
				reduce_flamethrower();
			}
			else if (getDirection() == up && get_flamethrower() != 0)
			{
				getWorld()->fire(getX(), getY(), 3);
				reduce_flamethrower();
			}
			else if (getDirection() == down && get_flamethrower() != 0)
			{
				getWorld()->fire(getX(), getY(), 4);
				reduce_flamethrower();
			}
			break;
		case KEY_PRESS_TAB:
			if (get_landmines() != 0)
			{
				reduce_landmines();
				getWorld()->placeLandmine(getX(), getY());
			}
			break;
		case KEY_PRESS_ENTER:
			if (get_vaccine() != 0)
			{
				cureInfection();
				reduce_vaccine();
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
	if (getInfectedNumber() == 500)
	{
		getWorld()->playSound(SOUND_ZOMBIE_BORN);
		getWorld()->increaseScore(-1000);
	}

	tick++;

	if (tick % 2 == 0)
		return;

	getWorld()->citizenDistanceToPlayer(getX(), getY(), player_x, player_y, distanceFromPlayer);
	getWorld()->searchNearestZombie(getX(), getY(), zombie_x, zombie_y, distanceFromNearestZombie); //have zombies

	if (zombie_x == 0 && zombie_y == 0)
	{
		if (distanceFromPlayer <= 6400)
			follow_player(getX(), getY(), player_x, player_y, 2);
	}
	else
	{
		if (distanceFromPlayer < distanceFromNearestZombie && distanceFromPlayer <= 6400)
		{
			follow_player(getX(), getY(), player_x, player_y, 2);
		}
		if (distanceFromNearestZombie <= 6400)
		{
			runAwayFromZombie(getX(), getY(), zombie_x, zombie_y, 2);
			return;
		}
	}
}

void Citizen::score_doSomething()
{
	getWorld()->increaseScore(-1000);
}

Wall::Wall(double startX, double startY, StudentWorld *this_world)
	: Actor(IID_WALL, startX, startY, right, 0, this_world) {}

Exit::Exit(double startX, double startY, StudentWorld *this_world)
	: Actor(IID_EXIT, startX, startY, right, 1, this_world)
{}

void Exit::doSomething()
{
	getWorld()->Player_overlapWithExit(getX(), getY());   //If the player is overlap with Exit and no more citizen, goes to next level
	getWorld()->citizen_overlapWithExit(getX(), getY()); //If the citizen are overlap with Eixt, destory them ,and add score
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
		getWorld()->overlapWithVomit(getX(), getY());

}

Goodie::Goodie(int imageID, double startX, double startY, StudentWorld *this_world)
	: Actor(imageID, startX, startY, right, 1, this_world)
{
	setAlive();
}
bool Goodie::doSomethingCom()
{
	if (getStatus() == false)
		return false;

	if (getWorld()->Player_overlapWith_Goodies(getX(), getY()))
	{
		pickUp(getWorld()->getPenelope());
		setDead();
		return true;
	}
	return false;
}

Vaccine_goodie::Vaccine_goodie(double startX, double startY, StudentWorld *this_world)
	: Goodie(IID_VACCINE_GOODIE, startX, startY, this_world) {}
void Vaccine_goodie::doSomething()
{
	if (Goodie::doSomethingCom())
		return;
}

void Vaccine_goodie::pickUp(Penelope *p)
{
	p->add_vaccine();
}

Gas_can_goodie::Gas_can_goodie(double startX, double startY, StudentWorld *this_world)
	: Goodie(IID_GAS_CAN_GOODIE, startX, startY, this_world) {}
void Gas_can_goodie::doSomething()
{
	if (Goodie::doSomethingCom())
		return;
}

void Gas_can_goodie::pickUp(Penelope *p)
{
	p->add_flamethrower();
}

Landmine_goodie::Landmine_goodie(double startX, double startY, StudentWorld *this_world)
	: Goodie(IID_LANDMINE_GOODIE, startX, startY, this_world) {}
void Landmine_goodie::doSomething()
{
	if (Goodie::doSomethingCom())
		return;       
}
void Landmine_goodie::pickUp(Penelope *p)
{
	p->add_landmines();
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

Zombie::Zombie(double startX, double startY, StudentWorld *this_world)
	: Agent(IID_ZOMBIE, startX, startY, this_world)
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
			if (!getWorld()->check_collision(getX(), getY() + 1, 1, up))
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
			if (!getWorld()->check_collision(getX(), getY() - 1, 1, down))
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
			if (!getWorld()->check_collision(getX() - 1, getY(), 1, left))
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
			if (!getWorld()->check_collision(getX() + 1, getY(), 1, right))
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

			int temp = randInt(1, 3);
			if (temp == 1)
			{
				getWorld()->compute_vomit(getX(), getY(), up);
				return;
			}

		}
	}

	if (getDirection() == down)
	{
		if (getWorld()->check_personInFront(getX(), getY() - SPRITE_HEIGHT))
		{
			int temp = randInt(1, 3);
			if (temp == 1)
			{
				getWorld()->compute_vomit(getX(), getY(), down);
				return;
			}
		}

	}

	if (getDirection() == left)
	{
		if (getWorld()->check_personInFront(getX() - SPRITE_WIDTH, getY()))
		{
			int temp = randInt(1, 3);
			if (temp == 1)
			{
				getWorld()->compute_vomit(getX(), getY(), left);
				return;
			}

		}
	}

	if (getDirection() == right)
	{
		if (getWorld()->check_personInFront(getX() + SPRITE_WIDTH, getY()))
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

DumbZombie::DumbZombie(double startX, double startY, StudentWorld *this_world)
	:Zombie(startX, startY, this_world) {}

void DumbZombie::doSomething()
{
	Zombie::doSomethingCom();

}

void DumbZombie::score_doSomething()
{
	getWorld()->increaseScore(1000);
}

void DumbZombie::chanceToCreateVaccine()
{
	getWorld()->createAVaccineGoodie(getX(), getY());
}


SmartZombie::SmartZombie(double startX, double startY, StudentWorld *this_world)
	:Zombie(startX, startY, this_world)
{
	tick = 0;
}

void SmartZombie::doSomething()
{

	getWorld()->searchClosestPeople(getX(), getY(), cloest_x, cloest_y, distance, isThreat);

	if (distance > 6400)
	{
		Zombie::doSomethingCom();
	}
	else if (distance <= 6400)
	{
		tick++;

		if (tick % 2 == 0)
			return;

		follow_player(getX(), getY(), cloest_x, cloest_y, 1);
		Zombie::doSomethingVomitCom();
	}
}

void SmartZombie::score_doSomething()
{
	getWorld()->increaseScore(2000);
}