#include "tour_generator.h"
#include "tourcmd.h"
#include "geotools.h"

#include <vector>

using namespace std;

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router)
:geoDB(&geodb), routerPtr(&router)
{
}
string TourGenerator::determineDirection(const GeoPoint& pt1, const GeoPoint& pt2) const {
    double angle = angle_of_line(pt1, pt2);
    if(0 <= angle && angle < 22.5)
        return "east";
    else if(angle < 67.5)
        return "northeast";
    else if(angle < 112.5)
        return "north";
    else if(angle < 157.5)
        return "northwest";
    else if(angle < 202.5)
        return "west";
    else if(angle < 247.5)
        return "southwest";
    else if(angle < 292.5)
        return "south";
    else if(angle < 33.5)
        return "southeast";
    else
        return "east";
}

vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const {
    vector<TourCommand> result;

    string nextPoi;
    string commentary;
    stops.get_poi_data(0, nextPoi, commentary);
    for(int i = 0; i < stops.size(); i++) {
        TourCommand commentCommand;
        commentCommand.init_commentary(nextPoi, commentary);
        result.push_back(commentCommand);
        if(i < stops.size() - 1) {
            GeoPoint start;
            GeoPoint end;
            
            geoDB->get_poi_location(nextPoi, start);
            stops.get_poi_data(i + 1, nextPoi, commentary);
            
            geoDB->get_poi_location(nextPoi, end);
            vector<GeoPoint> route = routerPtr->route(start, end);
            if(route.size() == 0) return result;
            for(int i = 0; i < route.size() - 1; i++) {
                GeoPoint& pt1 = route[i];
                GeoPoint& pt2 = route[i + 1];
                
                TourCommand proceedCommand;
                proceedCommand.init_proceed(determineDirection(pt1, pt2), geoDB->get_street_name(pt1, pt2), distance_earth_miles(pt1, pt2), pt1, pt2);
                result.push_back(proceedCommand);
                
                if(i < route.size() - 2) {
                    GeoPoint& pt3 = route[i + 2];
                    if(geoDB->get_street_name(pt1, pt2) != geoDB->get_street_name(pt2, pt3)) {
                        double angle = angle_of_turn(pt1, pt2, pt3);
                        if(angle >= 1 && angle < 180){
                            TourCommand turnCommand;
                            turnCommand.init_turn("left", geoDB->get_street_name(pt2, pt3));
                            result.push_back(turnCommand);
                        }
                        else if(angle >= 180 && angle <= 359) {
                            TourCommand turnCommand;
                            turnCommand.init_turn("right", geoDB->get_street_name(pt2, pt3));
                            result.push_back(turnCommand);
                        }
                    }
                }
            }
        }
    }

    return result;
}

TourGenerator::~TourGenerator(){
    
}
