#include "Actor.h"
#include "StudentWorld.h"
#include <math.h>
#include<cmath>
using namespace std;

const double PI = 4 * atan(1);

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////
//IMPLEMENTATION OF BASE CLASS FOR ALL ACTORS//
///////////////////////////////////////////////

Actor::Actor(int ID, double x, double y, Direction start, int dep, bool damage, bool block, int hp, bool edible, bool incPoints, StudentWorld* sw) : GraphObject(ID, x, y, start, dep)
{
	m_alive = true;
	m_world = sw;
	m_damage = damage;
	m_block = block;
	m_edible = edible;
	m_hp = hp;
	m_incPoints = incPoints;
}

double Actor::getDistance(double x1, double y1, double x2, double y2)
{
	double xDist = abs(x2 - x1);
	double yDist = abs(y2 - y1);
	double dist = (pow(xDist, 2) + pow(yDist, 2));
	dist = sqrt(dist);
	return dist;
}

bool Actor::isAlive()
{
	return m_alive;
}

void Actor::setDead()
{
	m_alive = false;
}

StudentWorld* Actor::getWorld()
{
	return m_world;
}

bool Actor::isDamageable()
{
	return m_damage;
}

bool Actor::blocks()
{
	return m_block;
}

bool Actor::isEdible()
{
	return m_edible;
}

int Actor::getHP()
{
	return m_hp;
}

void Actor::setHP(int amt)
{
	m_hp = amt;
}

bool Actor::canIncPoints()
{
	return m_incPoints;
}

void Actor::playSpecificSound(bool isDead)
{
	return;
}

/////////////////////////////////
//SOCRATES CLASS IMPLEMENTATION//
/////////////////////////////////

Socrates::Socrates(StudentWorld* sw) : Actor(IID_PLAYER, 0, VIEW_HEIGHT / 2, 0, 0, false, false, 100, false, false, sw)
{
	spray = 20;
	flame = 5;
}

double Socrates::calculate(Direction d, char axis)
{
	const double PI = 4 * atan(1);
	switch (axis)
	{
	case 'x':
		return ((VIEW_WIDTH / 2 * cos((d + 180.0) * PI / 180)) + VIEW_WIDTH / 2);
	case 'y':
		return ((VIEW_HEIGHT / 2 * sin((d + 180.0) * PI / 180)) + VIEW_HEIGHT / 2);
	}
	std::cout << "Not a valid axis." << std::endl;
	return -1;
}


int Socrates::getFlame()
{
	return flame;
}

void Socrates::setFlame(int amt)
{
	flame = amt;
}

int Socrates::getSpray()
{
	return spray;
}

void Socrates::doSomething()
{
	int ch;
	double x;
	double y;
	int dir = getDirection();
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_SPACE:
			if (spray > 0)
			{
				getPositionInThisDirection(dir, SPRITE_WIDTH, x, y);
				getWorld()->addSpray(x, y, dir);
				spray--;
				getWorld()->playSound(SOUND_PLAYER_SPRAY);
			}
			break;
		case KEY_PRESS_ENTER:
			if (flame > 0)
			{
				for (int i = 0; i < 16; i++)
				{
					getPositionInThisDirection(dir, SPRITE_WIDTH, x, y);
					getWorld()->addFlame(x, y, dir);
					dir += 22;
				}
				flame--;
				getWorld()->playSound(SOUND_PLAYER_FIRE);
			}
			break;
		case KEY_PRESS_LEFT:
		case 'a':
			moveTo(calculate(getDirection() + 5, 'x'), calculate(getDirection() + 5, 'y'));
			setDirection(getDirection() + 5);
			break;
		case KEY_PRESS_RIGHT:
		case 'd':
			moveTo(calculate(getDirection() - 5, 'x'), calculate(getDirection() - 5, 'y'));
			setDirection(getDirection() - 5);
			break;
		}
	}
	else
	{
		if (spray < 20)
			spray++;
	}
}

///////////////////////////////
//STILLS CLASS IMPLEMENTATION//
///////////////////////////////

Stills::Stills(int ID, double x, double y, Direction start, bool canBlock, bool canBeDamage, bool edible, StudentWorld* sw) : Actor(ID, x, y, start, 1, canBeDamage, canBlock, 1, edible, false, sw)
{}

void Stills::doSomething()
{
	return;
}

/////////////////////////////
//DIRT CLASS IMPLEMENTATION//
/////////////////////////////

Dirt::Dirt(double x, double y, StudentWorld* sw) : Stills(IID_DIRT, x, y, 0, true, true, false, sw)
{}

/////////////////////////////
//FOOD CLASS IMPLEMENTATION//
/////////////////////////////

Food::Food(double x, double y, StudentWorld* sw) : Stills(IID_FOOD, x, y, 90, false, false, true, sw)
{}

///////////////////////////////
//ATTACK CLASS IMPLEMENTATION//
///////////////////////////////

Attack::Attack(int ID, double x, double y, int travel, Direction start, int attackAmount, StudentWorld* sw) : Actor(ID, x, y, start, 1, false, false, 1, false, false, sw)
{
	maxTravel = travel;
	amountDamage = attackAmount;
}

int Attack::getAmountDamage()
{
	return amountDamage;
}

int Attack::getMaxTravel()
{
	return maxTravel;
}

void Attack::setMaxTravel(int amt)
{
	maxTravel = amt;
}

void Attack::doAttack()
{
	if (!isAlive())
		return;
	else if (getWorld()->damage(getX(), getY(), getAmountDamage()))
	{
		setDead();
		return;
	}
	else
	{
		moveAngle(getDirection(), SPRITE_WIDTH);
		setMaxTravel(getMaxTravel() - SPRITE_WIDTH);
		if (getMaxTravel() == 0)
		{
			setDead();
			return;
		}
	}
}

//////////////////////////////
//FLAME CLASS IMPLEMENTATION//
//////////////////////////////

Flame::Flame(double x, double y, Direction start, StudentWorld* sw) : Attack(IID_FLAME, x, y, 32, start, 5, sw)
{}

void Flame::doSomething()
{
	doAttack();
}

//////////////////////////////
//SPRAY CLASS IMPLEMENTATION//
//////////////////////////////

Spray::Spray(double x, double y, Direction start, StudentWorld* sw) : Attack(IID_SPRAY, x, y, 112, start, 2, sw)
{}
void Spray::doSomething()
{
	doAttack();
}

///////////////////////////////
//GOODIE CLASS IMPLEMENTATION//
///////////////////////////////

Goodie::Goodie(int ID, int life, double x, double y, StudentWorld* sw) : Actor(ID, x, y, 0, 1, true, false, 1, false, false, sw)
{
	lifeSpan = life;
}

void Goodie::setLifeSpan(int amt)
{
	lifeSpan = amt;
}

int Goodie::getLifeSpan()
{
	return lifeSpan;
}

void Goodie::commonGoodie(int ptInc)
{
	setLifeSpan(getLifeSpan() - 1);
	if (!isAlive())
		return;
	double distance = getDistance(getX(), getY(), getWorld()->getSoc()->getX(), getWorld()->getSoc()->getY());
	if (distance <= SPRITE_WIDTH)
	{
		getWorld()->increaseScore(ptInc);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		goodieBonus();
		return;
	}
	if (lifeSpan <= 0)
		setDead();
}

//////////////////////////////////////
//HEALTH GOODIE CLASS IMPLEMENTATION//
//////////////////////////////////////

HealthGoodie::HealthGoodie(double x, double y, StudentWorld* sw) : Goodie(IID_RESTORE_HEALTH_GOODIE, max(rand() % (300 - 10 * sw->getLevel()), 50), x, y, sw)
{}

void HealthGoodie::goodieBonus()
{
	getWorld()->getSoc()->setHP(100);
}

void HealthGoodie::doSomething()
{
	commonGoodie(250);
}

/////////////////////////////////////
//FLAME GOODIE CLASS IMPLEMENTATION//
/////////////////////////////////////

FlameGoodie::FlameGoodie(double x, double y, StudentWorld* sw) : Goodie(IID_FLAME_THROWER_GOODIE, max(rand() % (300 - 10 * sw->getLevel()), 50), x, y, sw)
{}

void FlameGoodie::goodieBonus()
{
		getWorld()->getSoc()->setFlame(getWorld()->getSoc()->getFlame() + 5);
}

void FlameGoodie::doSomething()
{
	commonGoodie(300);
}

/////////////////////////////////////
//LIFE GOODIE CLASS IMPLEMENTATION///
/////////////////////////////////////

LifeGoodie::LifeGoodie(double x, double y, StudentWorld* sw) : Goodie(IID_EXTRA_LIFE_GOODIE, max(rand() % (300 - 10 * sw->getLevel()), 50), x, y, sw)
{}

void LifeGoodie::goodieBonus()
{
		getWorld()->incLives();
}

void LifeGoodie::doSomething()
{
	commonGoodie(500);
}

////////////////////////////////
//FUNGUS CLASS IMPLEMENTATION///
////////////////////////////////

Fungus::Fungus(double x, double y, StudentWorld* sw) :Actor(IID_FUNGUS, x, y, 0, 1, true, false, 1, false, false, sw)
{
	lifeSpan = max(randInt(0, 300 - 10 * sw->getLevel() - 1), 50);
}

void Fungus::doSomething()
{
	lifeSpan--;
	if (!isAlive())
		return;
	double distance = getDistance(getX(), getY(), getWorld()->getSoc()->getX(), getWorld()->getSoc()->getY());
	if (distance <= SPRITE_WIDTH)
	{
		getWorld()->increaseScore(-50);
		setDead();
		getWorld()->getSoc()->setHP(getWorld()->getSoc()->getHP() - 20);
		if (getWorld()->getSoc()->getHP() > 0)
			getWorld()->playSound(SOUND_PLAYER_HURT);
		else
		{
			getWorld()->getSoc()->setDead();
			getWorld()->playSound(SOUND_PLAYER_DIE);
		}
		return;
	}
	if (lifeSpan <= 0)
		setDead();
}

/////////////////////////////
//PIT CLASS IMPLEMENTATION///
/////////////////////////////

Pit::Pit(double x, double y, StudentWorld* sw) : Actor(IID_PIT, x, y, 0, 1, false, false, 1, false, false, sw)
{
	rSal = 5;
	aSal = 3;
	eColi = 2;
}


int Pit::getRSal()
{
	return rSal;
}

int Pit::getASal()
{
	return aSal;
}

int Pit::getEColi()
{
	return eColi;
}

void Pit::decRSal()
{
	rSal--;
}

void Pit::decASal()
{
	aSal--;
}

void Pit::decEColi()
{
	eColi--;
}

void Pit::doSomething()
{
	if (rSal == 0 && aSal == 0 && eColi == 0)
	{
		setDead();
		getWorld()->deletePit();
	}
	int prob = randInt(1, 50);
	if (prob == 1)
	{
		getWorld()->addBacteria(this, getX(), getY());
	}
}

/////////////////////////////////
//BACTERIA CLASS IMPLEMENTATION//
/////////////////////////////////

Bacteria::Bacteria(int ID, double x, double y, int hp, StudentWorld* sw) : Actor(ID, x, y, 90, 0, true, false, hp, false, true, sw)
{}

void Bacteria::setMPD(int amt)
{
	movePlanDist = amt;
}

int Bacteria::getMPD()
{
	return movePlanDist;
}

int Bacteria::getEaten()
{
	return eaten;
}

void Bacteria::setEaten(int amt)
{
	eaten = amt;
}

void Bacteria::attackSocrates(int damageAmt)
{
	getWorld()->getSoc()->setHP(getWorld()->getSoc()->getHP() - damageAmt);
	if (getWorld()->getSoc()->getHP() > 0)
		getWorld()->playSound(SOUND_PLAYER_HURT);
	else
	{
		getWorld()->getSoc()->setDead();
		getWorld()->playSound(SOUND_PLAYER_DIE);
	}
}

double Bacteria::multiplyX()
{
	double newX = getX();
	if (getX() < VIEW_WIDTH / 2)
		newX += (SPRITE_WIDTH / 2);
	else if (getX() > VIEW_WIDTH / 2)
		newX -= (SPRITE_WIDTH / 2);
	return newX;
}

double Bacteria::multiplyY()
{
	double newY = getY();
	if (getY() < VIEW_HEIGHT / 2)
		newY += (SPRITE_WIDTH / 2);
	else if (getY() > VIEW_HEIGHT / 2)
		newY -= (SPRITE_WIDTH / 2);
	return newY;
}

void Bacteria::salmonMove(Direction dir)
{
	setMPD(getMPD() - 1);
	double newX;
	double newY;
	getPositionInThisDirection(dir, 3, newX, newY);
	if (getWorld()->blocked(newX, newY, SPRITE_WIDTH / 2.0) || getDistance(newX, newY, VIEW_WIDTH / 2, VIEW_HEIGHT / 2) >= VIEW_RADIUS)
	{
		setDirection(randInt(0, 359));
		setMPD(10);
	}
	else
		moveTo(newX, newY);
}

void Bacteria::playSpecificSound(bool isDead)
{
	if(isDead)
		getWorld()->playSound(SOUND_SALMONELLA_DIE);
	else
		getWorld()->playSound(SOUND_SALMONELLA_HURT);
}

void Bacteria::eatFood(Bacteria* cur)
{
	Direction food;
	bool foundFood = cur->getWorld()->findFood(cur, food);
	if (foundFood)
	{
		cur->salmonMove(food);
		if (cur->getWorld()->eat(cur->getX(), cur->getY()))
			cur->setEaten(cur->getEaten() + 1);
		return;
	}
	else
	{
		cur->setDirection(randInt(0, 359));
		cur->setMPD(10);
		return;
	}
}

///////////////////////////////////////////
//REGULAR SALMONELLA CLASS IMPLEMENTATION//
///////////////////////////////////////////

rSalmon::rSalmon(double x, double y, StudentWorld* sw) : Bacteria(IID_SALMONELLA, x, y, 4, sw)
{}

void rSalmon::doSomething()
{
	bool skipToFive = false;
	if (!isAlive())
	{
		return;
	}
	double distance = getDistance(getX(), getY(), getWorld()->getSoc()->getX(), getWorld()->getSoc()->getY());
	if (distance <= SPRITE_WIDTH)
	{
		attackSocrates(1);
		skipToFive = true;
	}

	if (getEaten() == 3 && !skipToFive)
	{
		double x = multiplyX();
		double y = multiplyY();
		rSalmon* p = new rSalmon(x, y, getWorld());
		getWorld()->addBacteria(p);
		setEaten(0);
		skipToFive = true;
	}

	if (getWorld()->eat(getX(), getY()) && !skipToFive)
		setEaten(getEaten() + 1);

	if (getMPD() > 0)
	{
		salmonMove(getDirection());
		return;
	}
	else
	{
		eatFood(this);
	}
	return;
}

//////////////////////////////////////////////
//AGGRESSIVE SALMONELLA CLASS IMPLEMENTATION//
//////////////////////////////////////////////

aSalmon::aSalmon(double x, double y, StudentWorld* sw) : Bacteria(IID_SALMONELLA, x, y, 10, sw)
{}

void aSalmon::doSomething()
{
	bool skipToSix = false;
	bool endAtSix = false;
	if (!isAlive())
	{
		return;
	}

	Direction dir;
	bool found = getWorld()->findSocrates(this, dir, 72);
	if (found)
	{
		endAtSix = true;
		double newX;
		double newY;
		getPositionInThisDirection(dir, 3, newX, newY);
		if (!(getWorld()->blocked(newX, newY, SPRITE_WIDTH / 2)) && getDistance(newX, newY, VIEW_WIDTH / 2, VIEW_HEIGHT / 2) < VIEW_RADIUS)
			moveTo(newX, newY);
		endAtSix = true;
	}
	double socDist = getDistance(getX(), getY(), getWorld()->getSoc()->getX(), getWorld()->getSoc()->getY());
	if (socDist <= SPRITE_WIDTH)
	{
		attackSocrates(2);
		skipToSix = true;
		if (endAtSix)
			return;
	}

	if (getEaten() == 3 && !skipToSix)
	{
		skipToSix = true;
		setEaten(0);
		double x = multiplyX();
		double y = multiplyY();
		aSalmon* p = new aSalmon(x, y, getWorld());
		getWorld()->addBacteria(p);
		if (endAtSix)
			return;
	}

	if (getWorld()->eat(getX(), getY()) && !skipToSix)
	{
		setEaten(getEaten() + 1);
	}

	if (endAtSix)
		return;
	else
	{
		if (getMPD() > 0)
		{
			salmonMove(getDirection());
			return;
		}
		else
		{
			eatFood(this);
		}
	}
	return;
}

///////////////////////////////
//E.COLI CLASS IMPLEMENTATION//
///////////////////////////////

eColi::eColi(double x, double y, StudentWorld* sw) : Bacteria(IID_ECOLI, x, y, 5, sw)
{}

void eColi::playSpecificSound(bool isDead)
{
	if (isDead)
		getWorld()->playSound(SOUND_ECOLI_DIE);
	else
		getWorld()->playSound(SOUND_ECOLI_HURT);
}

void eColi::doSomething()
{
	bool skipToFive = false;
	if (!isAlive())
	{
		return;
	}

	double socDist = getDistance(getX(), getY(), getWorld()->getSoc()->getX(), getWorld()->getSoc()->getY());

	if (socDist <= SPRITE_WIDTH)
	{
		skipToFive = true;
		attackSocrates(4);
	}

	if (getEaten() == 3 && !skipToFive)
	{
		skipToFive = true;
		setEaten(0);
		double x = multiplyX();
		double y = multiplyY();
		eColi* p = new eColi(x, y, getWorld());
		getWorld()->addBacteria(p);
	}

	if (getWorld()->eat(getX(), getY()) && !skipToFive)
		setEaten(getEaten() + 1);

	Direction dir;
	bool found = getWorld()->findSocrates(this, dir, 256);
	if (found)
	{
		for (int i = 0; i < 10; i++)
		{
			double newX;
			double newY;
			getPositionInThisDirection(dir, 2, newX, newY);
			if (getWorld()->blocked(newX, newY, SPRITE_RADIUS) || getDistance(newX, newY, VIEW_WIDTH / 2, VIEW_HEIGHT / 2) >= VIEW_RADIUS)
				dir += 10;
			else
			{
				moveTo(newX, newY);
				return;
			}
		}
	}
}