/*#ifndef tour_generator_h
#define tour_generator_h

#endif
*/


#ifndef TOUR_GENERATOR_H
#define TOUR_GENERATOR_H

#include "base_classes.h"
#include "router.h"
#include "stops.h"
#include "tourcmd.h"
#include <vector>

using namespace std;

class TourGenerator: public TourGeneratorBase
{
public:
    TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
    virtual ~TourGenerator();
    virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
private:
    const RouterBase* routerPtr;
    const GeoDatabaseBase* geoDB;
    string determineDirection(const GeoPoint& pt1, const GeoPoint& pt2) const;
};
#endif
