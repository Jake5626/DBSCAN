#ifndef DIANA_H
#define DIANA_H

#include <readdata.h>

#include <map>

using namespace std;
class DIANA
{
public:
    DIANA();

    void setClusterNum(int num);

    bool setPointData(string);

    map<int,point> getResult();

    void display();

private:
    map<int,point> _pointData;

    void cluster(int);

    double calDistance(point,point);

    readData *_read;

    int _clusterNum;


};

#endif // DIANA_H
