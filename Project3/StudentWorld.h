#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include<vector>
#include "GameWorld.h"
#include "Actor.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    //checks to see if actors overlap with each other when initializing
    bool checkOverlap(Actor* cur, int limit);
    //returns a pointer to socrates
    Socrates* getSoc();
    //returns a pointer to a specific actor
    Actor* getActor(int index);
    //add a fungus to the world
    bool addFungus();
    //add a goodie to the world
    bool addGoodie();
    //add a bacteria from the pit
    void addBacteria(Pit* cur, double x, double y);
    //add a bacteria if one has eaten enough food to multiply
    void addBacteria(Actor* cur);
    //add a spray
    void addSpray(double x, double y, Direction start);
    //add flames
    void addFlame(double x, double y, Direction start);
    //the bacteria becomes a food object
    void addFood(double x, double y);
    //do damage to actors it encounters if it can be damaged by an attack
    bool damage(double x, double y, int amt);
    //returns true of the object's movement is blocked by dirt
    bool blocked(double x, double y, double distance);
    //returns true if the bacteria should become a food object
    bool becomeFood();
    //if bacteria overlaps with a food object, set the food to dead
    bool eat(double x, double y);
    //returns true if there is a food object within 128 pixels
    bool findFood(Actor* cur, Direction &dir);
    //delete the pit once there is no bacteria left in it 
    void deletePit();
    //delete dead actors
    void deleteDead();
    //if socrates is within the range of howFar, change dir to be the direction towards socrates
    bool findSocrates(Actor* cur, Direction &dir, int howFar);
    virtual int init();
    virtual int move();
    virtual void cleanUp();


private:
    Socrates* m_soc;
    int m_numDirt;
    int m_numFood;
    int m_numPits;
    int m_numBacteria;
    std::vector<Actor*> m_actors;
};

#endif // STUDENTWORLD_H_
