#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

////////////////////////////////////////////
//DECLARATION OF BASE CLASS FOR ALL ACTORS//
////////////////////////////////////////////

class Actor : public GraphObject
{
public:
	Actor(int ID, double x, double y, Direction start, int dep, bool damage, bool block, int hp, bool edible, bool incPoints, StudentWorld* sw);
	//gets distance between two points
	double getDistance(double x1, double y1, double x2, double y2);
	//returns whether or not the actor is alive
	bool isAlive();
	virtual void doSomething() = 0;
	//returns a pointer to the studentworld
	StudentWorld* getWorld();
	//sets object to dead
	void setDead();
	//returns whether or not the object can be damaged by flame or spray
	bool isDamageable();
	//returns whether or not you should increase points when the object is damaged
	bool canIncPoints();
	//returns whether or not the actor is edible
	bool isEdible();
	//returns whether or not the object blocks movement
	bool blocks();
	//gets the hit points of the actor
	int getHP();
	//sets the actors hp
	void setHP(int amt);
	//plays a specific sound when hit depending on if the actor is dead or not
	virtual void playSpecificSound(bool isDead);
private:
	bool m_alive;
	StudentWorld* m_world;
	bool m_damage;
	bool m_block;
	bool m_edible;
	int m_hp;
	bool m_incPoints;
};

//////////////////////////////
//SOCRATES CLASS DECLARATION//
//////////////////////////////

class Socrates : public Actor
{
public:
	Socrates(StudentWorld* sw);
	virtual void doSomething();
	//returns how many flames socrates has
	int getFlame();
	//set the number of flames socrates has
	void setFlame(int amt);
	//gets how many sprays socrates has
	int getSpray();
	//a helper function for moving socrates
	double calculate(Direction d, char axis);
private:
	int spray;
	int flame;
};

////////////////////////////
//STILLS CLASS DECLARATION//
////////////////////////////

class Stills : public Actor
{
public:
	Stills(int ID, double x, double y, Direction start, bool canBlock, bool canBeDamage, bool edible, StudentWorld* sw);
	virtual void doSomething();
};


//////////////////////////
//DIRT CLASS DECLARATION//
//////////////////////////

class Dirt : public Stills
{
public:
	Dirt(double x, double y, StudentWorld* sw);
};

//////////////////////////
//FOOD CLASS DECLARATION//
//////////////////////////

class Food : public Stills
{
public:
	Food(double x, double y, StudentWorld* sw);
};


///////////////////////////
//ATTACK CLASS DECLARATION//
///////////////////////////
class Attack : public Actor
{
public:
	Attack(int ID, double x, double y, int travel, Direction start, int attackAmount, StudentWorld* sw);
	virtual void doSomething() = 0;
	//tries to attack actors it overlaps with
	void doAttack();
	//gets its max travel distance
	int getMaxTravel();
	//gets how much damage it can do
	int getAmountDamage();
	//sets its max travel amount
	void setMaxTravel(int amt);
private:
	int maxTravel;
	int amountDamage;
};

///////////////////////////
//FLAME CLASS DECLARATION//
///////////////////////////

class Flame : public Attack
{
public:
	Flame(double x, double y, Direction start, StudentWorld* sw);
	void doSomething();
};

///////////////////////////
//SPRAY CLASS DECLARATION//
///////////////////////////

class Spray : public Attack
{
public:
	Spray(double x, double y, Direction start, StudentWorld* sw);
	void doSomething();
};

////////////////////////////
//GOODIE CLASS DECLARATION//
////////////////////////////

class Goodie : public Actor
{
public:
	Goodie(int ID, int life, double x, double y, StudentWorld* sw);
	//sets life span of the goodie
	void setLifeSpan(int amt);
	//returns the goodie's life span
	int getLifeSpan();
	virtual void doSomething() = 0;
	//common actions between the goodies
	void commonGoodie(int ptInc);
	//how the goodie affects socrates
	virtual void goodieBonus() = 0;
private:
	int lifeSpan;
};

///////////////////////////////////
//HEALTH GOODIE CLASS DECLARATION//
///////////////////////////////////

class HealthGoodie : public Goodie
{
public:
	HealthGoodie(double x, double y, StudentWorld* sw);
	void doSomething();
	virtual void goodieBonus();
};

//////////////////////////////////
//FLAME GOODIE CLASS DECLARATION//
//////////////////////////////////

class FlameGoodie : public Goodie
{
public:
	FlameGoodie(double x, double y, StudentWorld* sw);
	void doSomething();
	virtual void goodieBonus();
};

/////////////////////////////////
//LIFE GOODIE CLASS DECLARATION//
/////////////////////////////////

class LifeGoodie : public Goodie
{
public:
	LifeGoodie(double x, double y, StudentWorld* sw);
	void doSomething();
	virtual void goodieBonus();
};

////////////////////////////
//FUNGUS CLASS DECLARATION//
////////////////////////////

class Fungus :public Actor
{
public:
	Fungus(double x, double y, StudentWorld* sw);
	void doSomething();
private:
	int lifeSpan;
	bool damage = true;
	bool block = false;
};

/////////////////////////
//PIT CLASS DECLARATION//
/////////////////////////

class Pit : public Actor
{
public:
	Pit(double x, double y, StudentWorld* sw);
	//gets regular salmonella count
	int getRSal();
	//gets aggressive salmonella count
	int getASal();
	//gets e coli count
	int getEColi();
	//decreases regular salmonella count
	void decRSal();
	//decreases aggressive salmonella count
	void decASal();
	//decreases e coli count
	void decEColi();
	void doSomething();
private:
	int rSal;
	int aSal;
	int eColi;
	bool damage = false;
	bool block = false;
};

//////////////////////////////
//BACTERIA CLASS DECLARATION//
//////////////////////////////

class Bacteria :public Actor
{
public:
	Bacteria(int ID, double x, double y, int hp, StudentWorld* sw);
	virtual void doSomething() = 0;
	//sets move plan distance
	void setMPD(int amt);
	//returns move plan distance
	int getMPD();
	//gets the number of food the bacteria has eaten
	int getEaten();
	//sets the number of food the bacteria has eaten
	void setEaten(int amt);
	//do damage to socrates
	void attackSocrates(int damageAmt);
	//calculate a new x coordinate
	double multiplyX();
	//calculate a new y coordinate
	double multiplyY();
	//move the salmonella in that direction if it is not blocked by dirt or outside the radius
	void salmonMove(Direction dir);
	//find the closest food and try to move towards it
	void eatFood(Bacteria* cur);
	virtual void playSpecificSound(bool isDead);
private:
	int movePlanDist = 0;
	int eaten = 0;
};
////////////////////////////////////////
//REGULAR SALMONELLA CLASS DECLARATION//
////////////////////////////////////////

class rSalmon : public Bacteria
{
public:
	rSalmon(double x, double y, StudentWorld* sw);
	void doSomething();
};

///////////////////////////////////////////
//AGGRESSIVE SALMONELLA CLASS DECLARATION//
///////////////////////////////////////////

class aSalmon : public Bacteria
{
public:
	aSalmon(double x, double y, StudentWorld* sw);
	void doSomething();
};

////////////////////////////
//E.COLI CLASS DECLARATION//
////////////////////////////

class eColi : public Bacteria
{
public:
	eColi(double x, double y, StudentWorld* sw);
	void doSomething();
	//overrides the base playSpecficSound function to play its own sound
	virtual void playSpecificSound(bool isDead);
};
#endif // ACTOR_H_