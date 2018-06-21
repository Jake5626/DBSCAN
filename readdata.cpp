/**
  * @brief use cfile to read data file
  * @param readData.cpp
  * @author Jake Zheng
  * @date 2018-6
  */
//-----------------------------------------

#include "readdata.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>

#include <QDebug>

using namespace std;

readData::readData()
{

}

bool readData::openFile(string fileName){
    bool fileOpen;
    file.open(fileName);
    ostringstream filedata;
    _data.clear();
    if(!file){
        cout<<"open file error"<<endl;
        fileOpen = false;
    }
    else{
        fileOpen = true;
        filedata<<file.rdbuf();
        setData(filedata.str());
        toStdStrData();
    }

    file.close();
    return fileOpen;
}

void readData::setData(string data){
    _data.append(data);
}

void readData::toStdStrData(){
    int index;
    _stdStringData.clear();
    if(!_data.empty()){
        string::iterator i;
        for(i = _data.begin();i!=_data.end();i++){
            if((*i)<58 && (*i)>=48){
                index = _data.find(*i);
                break;
            }
        }
        _data = _data.substr(index);
    }
    uint backIndex;
    while(_data.find("\n") != string::npos){
        backIndex = _data.find("\n");
        _stdStringData.push_back(_data.substr(0, backIndex));
        _data = _data.substr(backIndex+1);
    }
    //cout<<"remain data:"<<_data;
    _stdStringData.push_back(_data);

    setStdData();
}

void readData::setStdData(){
    for(uint i = 0; i<_stdStringData.size(); i++){
        string str = _stdStringData.at(i);
        point item;
        item.index = i;
        item.x = stod(str.substr(0,3));
        item.y = stod(str.substr(4,3));
        item.z = stod(str.substr(8,3));
        item.m = stod(str.substr(12,3));
        if(str.find("Iris-setosa") != string::npos){
            item.label = 0;
        }
        if(str.find("Iris-versicolor") != string::npos){
            item.label = 1;
        }
        if(str.find("Iris-virginica") != string::npos){
            item.label = 2;
        }
        _stdData.push_back(item);
    }
    double maxX = 0,maxY = 0,maxZ = 0,maxM = 0;
    for(int i = 0; i < _stdData.size(); i++){
        if(_stdData.at(i).x > maxX){
            maxX = _stdData.at(i).x;
        }
        if(_stdData.at(i).y > maxY){
            maxY = _stdData.at(i).y;
        }
        if(_stdData.at(i).z > maxZ){
            maxZ = _stdData.at(i).z;
        }
        if(_stdData.at(i).m > maxM){
            maxM = _stdData.at(i).m;
        }
    }

    for(int i = 0; i < _stdData.size(); i++){
        _stdData.at(i).x = _stdData.at(i).x/maxX;
        _stdData.at(i).y = _stdData.at(i).y/maxY;
        _stdData.at(i).z = _stdData.at(i).z/maxZ;
        _stdData.at(i).m = _stdData.at(i).m/maxM;
    }
}

string readData::getData(){
    return _data;
}

vector<point> readData::getStdData(){
    return _stdData;
}
