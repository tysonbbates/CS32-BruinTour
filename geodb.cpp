#include "geodb.h"
#include "geotools.h"

GeoDatabase::GeoDatabase(){ //CONSTRUCTOR
    
}

bool GeoDatabase::load(const std::string &map_data_file){
    ifstream input(map_data_file);
    if(!input) return false;
    
    string stName, latlongLine;
    int numPois;
    
    while(getline(input, stName)){
        numPois = 0;
        getline(input, latlongLine);
        istringstream coords(latlongLine);
        string sLat, sLong, eLat, eLong;
        coords >> sLat >> sLong >> eLat >> eLong;
        input >> numPois;
        input.ignore(10000, '\n');

        GeoPoint start = GeoPoint(sLat, sLong);
        GeoPoint end = GeoPoint(eLat, eLong);
        process(stName, start, end);
        if(numPois > 0){ //DEAL WITH POIS
            GeoPoint mid = midpoint(start, end);
            process(stName, start, mid);
            process(stName, mid, end);
            for(int i = 0; i < numPois; i++){
                string poiData, poiName, poiCords, poiLat, poiLong;
                getline(input, poiData);
                size_t barIndex = poiData.find('|');
                poiName = poiData.substr(0, barIndex);
                poiCords = poiData.substr(barIndex+1);
                size_t spaceIndex = poiCords.find(' ');
                poiLat = poiCords.substr(0, spaceIndex);
                poiLong = poiCords.substr(spaceIndex+1);
                GeoPoint poi = GeoPoint(poiLat, poiLong);
                PoiToGeo.insert(poiName, poi);
                process("a path", poi, mid);
            }
        }
    }
    return true;
}

bool GeoDatabase::get_poi_location(const std::string &poi, GeoPoint &point) const{
    const GeoPoint* temp = PoiToGeo.find(poi);
    if(temp == nullptr) return false;
    point = *temp;
    return true;
}

vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint &pt) const{
    return *(GeoToGeos.find(pt.to_string()));
}

string GeoDatabase::get_street_name(const GeoPoint &pt1, const GeoPoint &pt2) const{
    const string* temp = GeosToName.find(pt1.to_string() + " " + pt2.to_string());
    if(temp == nullptr) return "";
    return *temp;
}

void GeoDatabase::process(string street, const GeoPoint& point1, const GeoPoint point2){
    string p1 = point1.to_string();
    string p2 = point2.to_string();
    
    GeosToName.insert(p1 + " " + p2, street);
    GeosToName.insert(p2 + " " + p1, street);
    GeoToGeos[p1].push_back(point2);
    GeoToGeos[p2].push_back(point1);
}

GeoDatabase::~GeoDatabase(){
    
}
