#include "router.h"
#include <queue>

using namespace std;

Router::Router(const GeoDatabaseBase& geo_db)
{
    gdb = &geo_db;
}


// A* finds a path from start to goal.
// h(n) estimates the cost to reach goal from node n.
double Router::heuristic(const GeoPoint &gp, const GeoPoint& dest) const{
    return distance_earth_miles(gp, dest);
}

 void Router::reconstruct_path(HashMap<GeoPoint> &map, GeoPoint& pt, vector<GeoPoint>& route, const GeoPoint& start) const{
     if(map[pt.to_string()].to_string() == start.to_string()){
         return;
     }
     reconstruct_path(map, map[pt.to_string()], route, start);
     route.push_back(pt);
}

vector<GeoPoint> Router::route(const GeoPoint &pt1, const GeoPoint &pt2) const{
    // For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from the start
    // to n currently known.
    HashMap<GeoPoint> cameFrom; //map of geopoint as string to geopoint pointer
    cameFrom[pt1.to_string()] = pt1;

    // For node n, gScore[n] is the cost of the cheapest path from start to n currently known.
    HashMap<pair<double, double>> scores; //must have default of inifinity
    scores[pt1.to_string()].first = 0;

    // For node n, fScore[n] := gScore[n] + h(n). fScore[n] represents our current best guess as to
    // how cheap a path could be from start to finish if it goes through n.
    scores[pt1.to_string()].second = scores[pt1.to_string()].first + heuristic(pt1, pt2);
    auto Compare = [this, &pt1, &pt2, &scores](const GeoPoint& a, const GeoPoint& b){
        if(scores[a.to_string()].second < scores[b.to_string()].second)
            return false;
        return true;
    };
    // priority queue
    priority_queue<GeoPoint, vector<GeoPoint>, function<bool(GeoPoint, GeoPoint)>> openQueue(Compare);
    openQueue.push(pt1);

    while(! openQueue.empty()){


        GeoPoint current = openQueue.top();
        if(current.to_string() == pt2.to_string()){
            vector<GeoPoint> path;
            path.push_back(pt1);
            reconstruct_path(cameFrom, current, path, pt1);
            return path;
        }
        openQueue.pop(); //Remove current from to be examined
        size_t max = gdb->get_connected_points(current).size();
        for(int i = 0; i < max; i++){
            // d(current,neighbor) is the weight of the edge from current to neighbor
            // tentative_gScore is the distance from start to the neighbor through current
            GeoPoint neighbor = gdb->get_connected_points(current)[i];
            double tentative_gScore = scores[current.to_string()].first + distance_earth_miles(current, neighbor);
            bool reached = (nullptr != scores.find(neighbor.to_string()));
            if((!reached) || (reached && (tentative_gScore < scores[neighbor.to_string()].first))){
                // This path to neighbor is better than any previous one. Record it!
                scores[neighbor.to_string()] = pair<double, double> (tentative_gScore,tentative_gScore + heuristic(neighbor, pt2));
                if(cameFrom.find(neighbor.to_string()) == nullptr)
                    openQueue.push(neighbor);
                cameFrom[neighbor.to_string()] = current;
            }
        }
    }

    // Open set is empty but goal was never reached
    return vector<GeoPoint>();
}

Router::~Router(){
    
}
