#include "ExpandableHashMap.h"
#include"provided.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream> 
#include<sstream>
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(std::string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_street;
};

StreetMapImpl::StreetMapImpl()
{

}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(std::string mapFile)
{
    ifstream inf(mapFile);
    // Test for failure to open
    if (!inf)
    {
        return false;
    }
    string line;
    string street;
    string s1;
    string s2;
    string s3;
    string s4;
    string segName;
    char dummy;
    while (getline(inf, line))
    {
        // To extract the information from the line, we'll
        // create an input stringstream from it, which acts
        // like a source of input for operator>>
        std::istringstream iss(line);
        //if we can extract exactly four strings, each string beginning with a digit, meaning it's two GeoCoords
        if ((iss >> s1 >> s2 >> s3 >> s4) && !(iss >> dummy) && (isdigit(s1[0]) || s1[0] == '-') && (isdigit(s2[0]) || s2[0] == '-'))
        {
            vector<StreetSegment>* theStreet;
            vector<StreetSegment>* theStreetRev;
            GeoCoord start = GeoCoord(s1, s2);
            GeoCoord end = GeoCoord(s3, s4);
            StreetSegment street = StreetSegment(start, end, segName);
            StreetSegment revStreet = StreetSegment(end, start, segName);
            theStreet = m_street.find(start);
            theStreetRev = m_street.find(end);
            if (theStreet != nullptr)               //if the start coord is already in m_street
            {
                theStreet->push_back(street);
            }
            else
            {
                vector<StreetSegment> newStreet;
                newStreet.push_back(street);                   //push the street segment onto vector
                m_street.associate(start, newStreet);          //create a new association in the map
            }
            if (theStreetRev != nullptr)               //if the reversed street
            {
                theStreetRev->push_back(revStreet);
            }
            else
            {
                vector<StreetSegment> newStreetRev;
                newStreetRev.push_back(revStreet);                   //push the street segment onto vector
                m_street.associate(end, newStreetRev);          //create a new association in the map
            }
        }
        else
        {
            if (line.size() > 3)
                segName = line;
        }
    }
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    const vector<StreetSegment>* temp = m_street.find(gc);
    if (temp == nullptr)
        return false;
    else
        segs = *temp;
    return true;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    return m_impl->getSegmentsThatStartWith(gc, segs);
}