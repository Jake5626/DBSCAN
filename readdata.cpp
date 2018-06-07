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
}

string readData::getData(){
    return _data;
}

vector<point> readData::getStdData(){
    return _stdData;
}
