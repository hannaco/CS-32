#include "provided.h"
#include <vector>
#include<iterator>
#include<algorithm>

using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    const StreetMap* s;
    double getTravelDistance(GeoCoord depot, vector<DeliveryRequest> requests) const
    {
        PointToPointRouter p(s);
        list<StreetSegment> route;
        double travel;
        double curDist;
        p.generatePointToPointRoute(depot, requests[0].location, route, curDist);
        travel = curDist;
        for (int i = 1; i < requests.size(); i++)
        {
            p.generatePointToPointRoute(depot, requests[i].location, route, curDist);
            travel += curDist;
        }
        p.generatePointToPointRoute(requests[requests.size() - 1].location, depot, route, curDist);
        travel += curDist;
        return travel;
    }
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
    s = sm;
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    oldCrowDistance = getTravelDistance(depot, deliveries);
    double travel = getTravelDistance(depot, deliveries);
    double tempTravel;
    vector<DeliveryRequest> optimal = deliveries;
    for (int i = 0; i < pow(deliveries.size(), 2); i++)
    {
        random_shuffle(deliveries.begin(), deliveries.end());
        tempTravel = getTravelDistance(depot, deliveries);
        if (tempTravel < travel)
        {
            travel = tempTravel;
            optimal = deliveries;
        }
    }
    deliveries = optimal;
    newCrowDistance = getTravelDistance(depot, deliveries);
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
