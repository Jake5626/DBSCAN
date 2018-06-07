#ifndef READDATA_H
#define READDATA_H

#include <string>
#include <fstream>
#include <vector>

using namespace std;

struct point{
    int index;
    double x;
    double y;
    double z;
    double m;
    int label;
    int targetLabel;
    string type;
};

class readData
{

public:
    readData();

    string getData();

    vector<point> getStdData();

    bool openFile(string);

private:
    ifstream file;

    string _data;

    void setData(string);

    void setStdData();

    void toStdStrData();

    vector<string> _stdStringData;

    vector<point> _stdData;
};

#endif // READDATA_H
