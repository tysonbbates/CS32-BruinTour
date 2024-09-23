#ifndef router_h
#define router_h
#include "geopoint.h"
#include "geotools.h"
#include "base_classes.h"
#include <utility>
#include <algorithm>
#include "hashmap.h"

using namespace std;
class Router: public RouterBase
{
public:
    Router(const GeoDatabaseBase& geo_db);
    virtual ~Router();
    virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:
    double heuristic(const GeoPoint& gp, const GeoPoint& dest) const;
    void reconstruct_path(HashMap<GeoPoint>& map, GeoPoint& pt, vector<GeoPoint>& route, const GeoPoint& start) const;
    const GeoDatabaseBase* gdb;
};

#endif

