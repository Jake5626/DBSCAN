#ifndef AGNES_H
#define AGNES_H

#include <readdata.h>

#include <string>
#include <map>
#include <vector>

using namespace std;
class AGNES
{
public:
    AGNES();

    void setClusterNum(int);

    bool setPointData(string);

    unsigned int getClusterNum();

    map<int,point> getResult();

    void display();

private:
    double calClusterDist(map<int,point>,map<int,point>);

    double calDist(point,point);

    readData *_read;

    map<int,point> _pointData;

    int _clusterNum;

    void cluster(int);
};

#endif // AGNES_H
