#ifndef DBSCAN_H
#define DBSCAN_H

#include <readdata.h>
#include <vector>
#include <string>
#include <map>


class DBSCAN
{
public:
    DBSCAN();

    map<int,point> getResult();

    map<int,point> getCorePoints();

    void setConfig(int,double);

    bool setPointData(string);

    void display();

private:

    double calDist(point,point);

    void dbscan(double eps,int minPts);

    void setCorePoints();

    void cluster(double);

    int _minPts;

    double _eps;

    readData *_read;

    map<int,point> _corepoints;

    map<int,point> _borderpoints;

    map<int,point> _pointData;

};

#endif // DBSCAN_H
