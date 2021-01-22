#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include<cmath>
#include<sstream>
#include<iomanip>

using namespace std;

const double PI = 4 * atan(1);

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_numDirt = max(180 - 20 * getLevel(), 20);
    m_numFood = min(5 * getLevel(), 25);
    m_numPits = getLevel();
    m_numBacteria = 0;
}

bool StudentWorld:: checkOverlap(Actor* cur, int limit)
{
    for (int i = 0; i < limit; i++)
    {
        Actor* test = m_actors[i];
        double distance = cur->getDistance(cur->getX(), cur->getY(), test->getX(), test->getY());
        if (distance >= (2.0 * SPRITE_RADIUS))
            return true;
        else
            return false;
    }
    return true;
}

void StudentWorld::deletePit()
{
    m_numPits--;
}

void StudentWorld::deleteDead()
{
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end())
    {
        if (!((*it)->isAlive()))
        {
            Actor* p = *it;
            if (p->canIncPoints())
                m_numBacteria--;
            delete p;
            it = m_actors.erase(it);
        }
        else
            it++;
    }
}

Socrates* StudentWorld::getSoc()
{
    return m_soc;
}

Actor* StudentWorld::getActor(int index)
{
    return m_actors[index];
}

bool StudentWorld::addFungus()
{
    int chanceFungus = max(510 - getLevel() * 10, 200);
    int generate = randInt(0, chanceFungus - 1);
    if (generate == 0)
        return true;
    return false;
}

bool StudentWorld::addGoodie()
{
    int chanceGoodie = max(510 - getLevel() * 10, 250);
    int generate = randInt(0, chanceGoodie - 1);
    if (generate == 0)
        return true;
    return false;
}

void StudentWorld::addBacteria(Pit* cur, double x, double y)
{
    vector<int> options;
    if (cur->getRSal() != 0)
        options.push_back(0);
    if (cur->getASal() != 0)
        options.push_back(1);
    if (cur->getEColi() != 0)
        options.push_back(2);
    int choose = randInt(0, options.size() - 1);
    int choice = options[choose];
    Bacteria* p;
    switch (choice)
    {
    case 0:
        p = new rSalmon(x, y, this);
        m_actors.push_back(p);
        m_numBacteria++;
        cur->decRSal();
        break;
    case 1:
        p = new aSalmon(x, y, this);
        m_actors.push_back(p);
        m_numBacteria++;
        cur->decASal();
        break;
    case 2:
        p = new eColi(x, y, this);
        m_actors.push_back(p);
        m_numBacteria++;
        cur->decEColi();
        break;
    }
}

void StudentWorld::addBacteria(Actor* cur)
{
    m_actors.push_back(cur);
    m_numBacteria++;
}

void StudentWorld::addFood(double x, double y)
{
    Actor* p = new Food(x, y, this);
    m_actors.push_back(p);
}

void StudentWorld::addSpray(double x, double y, Direction start)
{
    Actor* p = new Spray(x, y, start, this);
    m_actors.push_back(p);
}

void StudentWorld::addFlame(double x, double y, Direction start) 
{
    Actor* p = new Flame(x, y, start, this);
    m_actors.push_back(p);
}

bool StudentWorld::becomeFood()
{
    int chance = randInt(0, 1);
    if (chance == 1)
        return true;
    else
        return false;
}

bool StudentWorld::damage(double x, double y, int amt)
{
    double distance;
    for (int i = 0; i < m_actors.size(); i++)
    {
        distance = m_actors[i]->getDistance(x, y, m_actors[i]->getX(), m_actors[i]->getY());
        if ((m_actors[i]->isDamageable()) && (distance <= SPRITE_WIDTH))
        {
            m_actors[i]->setHP(m_actors[i]->getHP() - amt);
            if (m_actors[i]->canIncPoints())
                increaseScore(100);
            if (m_actors[i]->getHP() <= 0)
            {
                m_actors[i]->setDead();
                m_actors[i]->playSpecificSound(true);
                if (becomeFood() && m_actors[i]->canIncPoints())
                    addFood(m_actors[i]->getX(), m_actors[i]->getY());
            }
            else
                m_actors[i]->playSpecificSound(false);
            return true;
        }
    }
    return false;
}

bool StudentWorld::blocked(double x, double y, double distance)
{
    double howFar;
    for (int i = 0; i < m_actors.size(); i++)
    {
        howFar = m_actors[i]->getDistance(x, y, m_actors[i]->getX(), m_actors[i]->getY());
        if ((m_actors[i]->blocks()) && (howFar <= distance))
        {
            return true;
        }
    }
    return false;
}


bool StudentWorld::eat(double x, double y)
{
    double distance;
    for (int i = 0; i < m_actors.size(); i++)
    {
        distance = m_actors[i]->getDistance(x, y, m_actors[i]->getX(), m_actors[i]->getY());
        if ((m_actors[i]->isEdible()) && (distance <= SPRITE_WIDTH))
        {
            m_actors[i]->setDead();
            return true;
        }
    }
    return false;
}

bool StudentWorld::findFood(Actor* cur, Direction &dir)
{
    double closest = 129;
    Actor* p;
    double distance;
    for (int i = 0; i < m_actors.size(); i++)
    {
        distance = cur->getDistance(cur->getX(), cur->getY(), m_actors[i]->getX(), m_actors[i]->getY());
        if (m_actors[i]->isEdible() && distance <= 128)
        {
            if (distance < closest)
            {
                closest = distance;
                p = m_actors[i];
            }
        }
    }
    if (closest <= 128)
    {
        double xDist = p->getX() - cur->getX();
        double yDist = p->getY() - cur->getY();
        dir = Direction(atan2(yDist, xDist) * (180.0 / PI));
        return true;
    }
    else
        return false;
}

bool StudentWorld::findSocrates(Actor* cur, Direction &dir, int howFar)
{
    double distance = cur->getDistance(m_soc->getX(), m_soc->getY(), cur->getX(), cur->getY());
    if (distance <= howFar)
    {
        double xDist = m_soc->getX() - cur->getX();
        double yDist = m_soc->getY() - cur->getY();
        dir = Direction(atan2(yDist, xDist) * (180.0 / PI));
        return true;
    }
    return false;
}

int StudentWorld::init()
{
    m_soc = new Socrates(this);
    for (int i = 0; i < m_numPits; i++)
    {
        double x = double(rand() % (VIEW_WIDTH + 1));
        double y = double(rand() % (VIEW_HEIGHT + 1));
        Pit* cur = new Pit(x, y, this);
        double distance = cur->getDistance(x, y, VIEW_WIDTH / 2.0, VIEW_HEIGHT / 2.0);
        if ((distance <= 120) && checkOverlap(cur, m_actors.size()))
        {
            m_actors.push_back(cur);
        }
        else
        {
            delete cur;
            i--;
        }
    }
    for (int i = 0; i < m_numFood; i++)
    {
        double x = double(randInt(0, VIEW_WIDTH));
        double y = double(randInt(0, VIEW_HEIGHT));
        Food* cur = new Food(x, y, this);
        double distance = cur->getDistance(x, y, VIEW_WIDTH / 2.0, VIEW_HEIGHT / 2.0);
        if ((distance <= 120) && checkOverlap(cur, m_actors.size()))
        {
            m_actors.push_back(cur);
        }
        else
        {
            delete cur;
            i--;
        }
    }
    for (int i = 0; i < m_numDirt; i++)
    {
        double x = double(rand() % (VIEW_WIDTH + 1));
        double y = double(rand() % (VIEW_HEIGHT + 1));
        Dirt* cur = new Dirt(x, y, this);
        double distance = cur->getDistance(x, y, VIEW_WIDTH / 2.0, VIEW_HEIGHT / 2.0);
        if ((distance <= 120) && checkOverlap(cur, m_numFood + m_numPits))
        {
            m_actors.push_back(cur);
        }
        else
        {
            delete cur;
            i--;
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if (m_soc->isAlive())
        m_soc->doSomething();
    else
        return GWSTATUS_PLAYER_DIED;

    for (int i = 0; i < m_actors.size(); i++)
    {
        m_actors[i]->doSomething();
        if (!(m_soc->isAlive()))
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (m_numPits == 0 && m_numBacteria == 0)
            return GWSTATUS_FINISHED_LEVEL;
    }
    deleteDead();
    if (m_numPits == 0 && m_numBacteria == 0)
        return GWSTATUS_FINISHED_LEVEL;
    if (addFungus())
    {
        const double PI = 4 * atan(1);
        double angle = randInt(0, 359);
        double x = ((VIEW_RADIUS * cos(angle * PI / 180)) + VIEW_RADIUS);
        double y = ((VIEW_RADIUS * sin(angle * PI / 180)) + VIEW_RADIUS);
        Actor* p = new Fungus(x, y, this);
        m_actors.push_back(p);
    }
    if (addGoodie())
    {
        const double PI = 4 * atan(1);
        double angle = randInt(0, 359);
        double x = ((VIEW_RADIUS * cos(angle * PI / 180)) + VIEW_RADIUS);
        double y = ((VIEW_RADIUS * sin(angle * PI / 180)) + VIEW_RADIUS);
        int goodieType = randInt(1, 10);
        if (goodieType == 1)
        {
            Actor* p = new LifeGoodie(x, y, this);
            m_actors.push_back(p);
        }
        else if (goodieType >= 2 && goodieType <= 4)
        {
            Actor* p = new FlameGoodie(x, y, this);
            m_actors.push_back(p);
        }
        else
        {
            Actor* p = new HealthGoodie(x, y, this);
            m_actors.push_back(p);
        }
    }
    ostringstream oss;
    oss << "Score: ";
    int score = getScore();
    if (score < 0)
    {
        oss.fill('0');
        oss << "-" << setw(6) << abs(score);
    }
    else
    {
        oss.fill('0');
        oss << setw(6) << score;
    }
    oss.fill(' ');
    oss << setw(9) << "Level: " << getLevel() << setw(8) << "Lives: " << getLives() << setw(10) << "health: " << getSoc()->getHP() << setw(10) << "Sprays: " << getSoc()->getSpray() << setw(9) << "Flames: " << getSoc()->getFlame();
    string str = oss.str();
    setGameStatText(str);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_soc;
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end())
    {
        Actor* cur = *it;
        delete cur;
        it = m_actors.erase(it);
    }
    m_actors.clear();
    m_numDirt = max(180 - 20 * getLevel(), 20);
    m_numFood = min(5 * getLevel(), 25);
    m_numPits = getLevel();
    m_numBacteria = 0;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}