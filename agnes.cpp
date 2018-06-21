#include "agnes.h"

#include <readdata.h>

#include <iostream>
#include <map>
#include <cmath>
#include <string>
#include <vector>

using namespace std;
AGNES::AGNES()
{
    _read = new readData();
}

bool AGNES::setPointData(string filename){
    bool ok = _read->openFile(filename);
    vector<point> data = _read->getStdData();
    for(int i = 0; i< data.size(); i++){
        _pointData.insert(pair<int,point>(i,data.at(i)));
    }
    return ok;
}

void AGNES::setClusterNum(int num){
    _clusterNum = num;
    cluster(_clusterNum);
}

double AGNES::calDist(point item1, point item2){
    return sqrt(pow(item1.x-item2.x,2)+pow(item1.y-item2.y,2)+pow(item1.z-item2.z,2)+pow(item1.m-item2.m,2));
}

double AGNES::calClusterDist(map<int,point> data1,map<int,point> data2){
    double minDist = 10;
    double avgDist = 0;
    map<int,point>::iterator it1;
    for(it1 = data1.begin(); it1 != data1.end(); it1++){
        map<int,point>::iterator it2;
        for(it2 = data2.begin(); it2 != data2.end(); it2++){
            double dist = calDist(it1->second,it2->second);
//            if(dist < minDist){
//                minDist = dist;
//            }
            //cout<<"distance"<<dist<<endl;
            avgDist+=dist;
        }
    }

    //return minDist;
    return avgDist/(data1.size()*data2.size());
}

void AGNES::cluster(int clusterNum){
    //clusterNum = _clusterNum;
    vector<map<int,point>> clusterTemp;
    int index1;
    int index2;
    double minDist;
    map<int,point>::iterator it;
    for(it = _pointData.begin(); it != _pointData.end(); it++){
        map<int,point> item;
        item.insert(pair<int,point>(it->first, it->second));
        clusterTemp.push_back(item);
    }
    //cout<<"cluster size:"<<clusterTemp.size()<<" "<<clusterNum<<endl;
    while(clusterTemp.size() > clusterNum){
        //cout<<"remain cluster num:"<<clusterTemp.size()<<endl;
        minDist = 100;
        for(unsigned int i = 0; i< clusterTemp.size(); i++){
            for(unsigned int j = 0; j< clusterTemp.size(); j++){
                double dist = calClusterDist(clusterTemp.at(i),clusterTemp.at(j));
                if(j == i) continue;
                //cout<<"i:"<<i<<" j:"<<j<<" distance:"<<dist<<endl;
                if(dist < minDist){
                    minDist = dist;
                    index1 = i;
                    index2 = j;
                }
            }
        }

        //cout<<"index1:"<<index1<<" index2:"<<index2<<" minist distance:"<<minDist<<endl;
        if(index1 > index2){
            map<int,point> mergeData2 = clusterTemp.at(index1);
            map<int,point>::iterator mapIt = mergeData2.begin();

            for(mapIt; mapIt!= mergeData2.end(); mapIt++){
                if((clusterTemp.at(index2)).find(mapIt->first) == (clusterTemp.at(index2)).end()){
                    (clusterTemp.at(index2)).insert(pair<int,point>(mapIt->first,mapIt->second));
                    //cout<<"add one point"<<endl;
                }
            }
            //cout<<"New cluster size:"<<clusterTemp.at(index1).size()<<endl;
            vector<map<int,point>>::iterator it = clusterTemp.begin();
            clusterTemp.erase(it+index1);
        }
        if(index1 < index2){
            map<int,point> mergeData2 = clusterTemp.at(index2);
            map<int,point>::iterator mapIt = mergeData2.begin();

            for(mapIt; mapIt!= mergeData2.end(); mapIt++){
                if((clusterTemp.at(index1)).find(mapIt->first) == (clusterTemp.at(index1)).end()){
                    (clusterTemp.at(index1)).insert(pair<int,point>(mapIt->first,mapIt->second));
                    //cout<<"add one point"<<endl;
                }
            }
            //cout<<"New cluster size:"<<clusterTemp.at(index1).size()<<endl;
            vector<map<int,point>>::iterator it = clusterTemp.begin();
            clusterTemp.erase(it+index2);
        }

    }
    //cout<<"after size:"<<clusterTemp.size()<<endl;
    map<int,int> result;
    for(int i = 0; i < clusterTemp.size() ; i++){
        map<int,point>::iterator itTag = clusterTemp.at(i).begin();
        result.clear();
        for(itTag; itTag!= clusterTemp.at(i).end(); itTag++){
            if(result.find(itTag->second.label) == result.end()){
                result.insert(pair<int,int>(_pointData.find(itTag->first)->second.label,0));
            }else{
                result.find(itTag->second.label)->second++;
            }
        }
        map<int,int>::iterator it;
        int maxCount = 0;
        int target = 0;
        for(it = result.begin(); it!= result.end(); it++){
            if(maxCount<it->second){
                maxCount = it->second;
                target = it->first;
            }
        }
        for(itTag = clusterTemp.at(i).begin(); itTag!=clusterTemp.at(i).end(); itTag++){
            _pointData.find(itTag->first)->second.targetLabel = target;
        }
        cout<<clusterTemp.at(i).size()<<endl;
    }
    getResult();
}

map<int,point> AGNES::getResult(){
    return _pointData;
}

void AGNES::display(){
    cout<<"result: "<<endl;
    map<int,point>::iterator it;
    for(it = _pointData.begin(); it!=_pointData.end(); it++){
        point item = it->second;
        cout<<"index: "<<item.index<<" label:"<<item.label<<" result:"<<item.targetLabel<<endl;
    }
}

