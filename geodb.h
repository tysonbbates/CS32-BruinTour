#ifndef geodb_hpp
#define geodb_hpp

#include <string>
#include "base_classes.h"
#include "hashmap.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class GeoDatabase: public GeoDatabaseBase
{
public:
GeoDatabase();
virtual ~GeoDatabase();
virtual bool load(const std::string& map_data_file);
virtual bool get_poi_location(const std::string& poi,GeoPoint& point) const;
virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt)
const;
virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:
    HashMap<GeoPoint> PoiToGeo;
    HashMap<vector<GeoPoint>> GeoToGeos;
    HashMap<string> GeosToName;
    void process(string street, const GeoPoint& point1, const GeoPoint point2);
};

#endif
