#include "provided.h"
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* s;
    string getDirection(double dir) const
    {
        if (dir >= 0 && dir < 22.5)
            return "east";
        else if (dir >= 22.5 && dir < 67.5)
            return "northeast";
        else if (dir >= 67.5 && dir < 112.5)
            return "north";
        else if (dir >= 112.5 && dir < 157.5)
            return "northwest";
        else if (dir >= 157.6 && dir < 202.5)
            return "west";
        else if (dir >= 202.5 && dir < 247.5)
            return "southwest";
        else if (dir >= 247.5 && dir < 292.5)
            return "south";
        else if (dir >= 292.5 && dir < 337.5)
            return "southeast";
        else
            return "east";
    }
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
    s = sm;
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
double oldCrow, newCrow;
    DeliveryResult result;
    vector<DeliveryRequest> opt = deliveries;
    DeliveryOptimizer optimizer(s);
    optimizer.optimizeDeliveryOrder(depot, opt, oldCrow, newCrow);
    PointToPointRouter route(s);
    totalDistanceTravelled = 0;
    GeoCoord begin = depot;
    double curDist = 0;
    list <StreetSegment> path;
    StreetSegment prev;

    for (int i = 0; i < opt.size(); i++)
    {
        result = route.generatePointToPointRoute(begin, opt[i].location, path, curDist);
        totalDistanceTravelled += curDist;
        if (result != DELIVERY_SUCCESS)
            return result;
        auto itr = path.begin();
        prev = *itr;
        double direction = angleOfLine(*itr);
        double turnAngle;
        double distance = distanceEarthMiles(itr->start, itr->end);
        DeliveryCommand cmd = DeliveryCommand();
        cmd.initAsProceedCommand(getDirection(direction), itr->name, distance);
        while (itr != path.end())
        {
            direction = angleOfLine(*itr);
            distance = distanceEarthMiles(itr->start, itr->end);
            if (prev.name == itr->name)
            {
                cmd.increaseDistance(distance);
            }
            else
            {
                commands.push_back(cmd);
                turnAngle = angleBetween2Lines(prev, *itr);
                if (turnAngle >= 1 && turnAngle < 180)
                {
                    cmd.initAsTurnCommand("left", itr->name);
                    commands.push_back(cmd);
                }
                else if (turnAngle >= 180 && turnAngle <= 359)
                {
                    cmd.initAsTurnCommand("right", itr->name);
                    commands.push_back(cmd);
                }
                cmd.initAsProceedCommand(getDirection(direction), itr->name, distance);
            }
            prev = *itr;
            itr++;
        }
        commands.push_back(cmd);
        cmd.initAsDeliverCommand(opt[i].item);
        commands.push_back(cmd);
        begin = opt[i].location;
    }

    list <StreetSegment> back;
    result = route.generatePointToPointRoute(opt[opt.size() - 1].location, depot, back, curDist);
    totalDistanceTravelled += curDist;
    if (result != DELIVERY_SUCCESS)
        return result;
    auto it = back.begin();
    prev = *it;
    double direction = angleOfLine(*it);
    double turnAngle;
    double distance = distanceEarthMiles(it->start, it->end);
    DeliveryCommand cmd = DeliveryCommand();
    cmd.initAsProceedCommand(getDirection(direction), it->name, distance);
    if (result != DELIVERY_SUCCESS)
        return result;
    while (it != back.end())
    {
        direction = angleOfLine(*it);
        distance = distanceEarthMiles(it->start, it->end);
        if (prev.name == it->name)
        {
            cmd.increaseDistance(distance);
        }
        else
        {
            commands.push_back(cmd);
            turnAngle = angleBetween2Lines(prev, *it);
            if (turnAngle >= 1 && turnAngle < 180)
            {
                cmd.initAsTurnCommand("left", it->name);
                commands.push_back(cmd);
            }
            else if (turnAngle >= 180 && turnAngle <= 359)
            {
                cmd.initAsTurnCommand("right", it->name);
                commands.push_back(cmd);
            }
            cmd.initAsProceedCommand(getDirection(direction), it->name, distance);
        }
        prev = *it;
        it++;
    }
    commands.push_back(cmd);
    return DELIVERY_SUCCESS;
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
