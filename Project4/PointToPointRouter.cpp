#include "provided.h"
#include"ExpandableHashMap.h"
#include <list>
#include<math.h>
#include<algorithm>

const double PI = 4 * atan(1);
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* s;
    struct Node
    {
        StreetSegment s;
        GeoCoord search;
        double f;
    };
};



PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    s = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
    const GeoCoord& start,
    const GeoCoord& end,
    list<StreetSegment>& route,
    double& totalDistanceTravelled) const
{
    if (start == end)
    {
        totalDistanceTravelled = 0;
        route.clear();
        return DELIVERY_SUCCESS;
    }
    vector<StreetSegment> open;
    vector<Node> nodes;
    ExpandableHashMap<GeoCoord, Node> m_route(0.5);
    s->getSegmentsThatStartWith(end, open);
    if (open.empty())
        return BAD_COORD;
    open.clear();
    s->getSegmentsThatStartWith(start, open);
    if (open.empty())
        return BAD_COORD;
    open.clear();
    Node begin = Node();
    begin.f = 0;
    begin.search = start;
    nodes.push_back(begin);
    Node current = nodes[0];
    Node prev = Node();
    prev.f = -1;
    while (!nodes.empty())
    {
        current = nodes[0];
        long double small = current.f;
        int index = 0;
        for (int i = 0; i < nodes.size(); i++)
        {
            if (nodes[i].f < small)
            {
                small = nodes[i].f;
                current = nodes[i];
                index = i;
            }
        }
        m_route.associate(current.search, prev);
        nodes.erase(nodes.begin() + index);
        if (current.search == end)
        {
            route.clear();
            totalDistanceTravelled = 0;
            while (current.search != start)
            {
                route.push_front(current.s);
                totalDistanceTravelled += distanceEarthMiles(current.s.start, current.s.end);
                current = *(m_route.find(current.search));
            }
            return DELIVERY_SUCCESS;
        }
        s->getSegmentsThatStartWith(current.search, open);
        for (int i = 0; i < open.size(); i++)
        {
            if (m_route.find(open[i].end) == nullptr)
            {
                Node pos = Node();
                pos.s = open[i];
                pos.search = open[i].end;
                pos.f = distanceEarthMiles(pos.search, current.search) + distanceEarthMiles(pos.search, end);
                nodes.push_back(pos);
            }
        }
        open.clear();
        prev = current;
    }
    return NO_ROUTE;
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
    const GeoCoord& start,
    const GeoCoord& end,
    list<StreetSegment>& route,
    double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
