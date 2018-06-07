#include "dbscan.h"
#include <readdata.h>

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>

#include <QDebug>


using namespace std;

DBSCAN::DBSCAN()
{
    _read = new readData();
}

bool DBSCAN::setPointData(string filename){
    bool ok = _read->openFile(filename);
    _pointData.clear();
    vector<point> data = _read->getStdData();
    for(uint i = 0; i< data.size(); i++){
        _pointData.insert(pair<int,point>(i,data.at(i)));
    }
    if(ok){
        return true;
    }else return false;
}

void DBSCAN::setConfig(int minPts,double eps){
    _minPts = minPts;
    _eps = eps;
    setCorePoints();
}

void DBSCAN::setCorePoints(){
    _corepoints.clear();
    _borderpoints.clear();
    for(uint i = 0; i < _pointData.size(); i++){
        point item1 = _pointData.at(i);
        int count = 0;
        for(uint j = 0;j <_pointData.size(); j++){
            point item2 = _pointData.at(j);
            double dis = calDist(item1,item2);
            if((dis != 0)&&(dis <= _eps)){
                count++;
            }
        }
        //cout<<count<<endl;
        if(count >= _minPts){
            if(_corepoints.find(item1.index) == _corepoints.end()){
                _corepoints.insert(pair<int,point>(item1.index,item1));
            }
            _pointData.at(i).type = "core";
        }else{
            if(_borderpoints.find(item1.index) == _borderpoints.end()){
                _borderpoints.insert(pair<int,point>(item1.index,item1));
            }
            _pointData.at(i).type = "border";
        }
    }

    cout<<"core points num:"<<_corepoints.size()<<" border points num:"<<_borderpoints.size()<<endl;
    cluster(_eps);

}

void DBSCAN::cluster(double eps){
    int curLabel = 0;
    map<int,point> cluster;
    cluster.empty();
    while(!_corepoints.empty()){
        int clusterNum = 0;
        cout<<"cluster"<<curLabel<<endl;
        cluster.clear();
        if(cluster.empty()){
            point core1 = _corepoints.begin()->second;
            cluster.insert(pair<int,point>(clusterNum,core1));
            clusterNum++;
            if(_corepoints.find(_corepoints.begin()->first) != _corepoints.end()){
            _corepoints.erase(_corepoints.find(_corepoints.begin()->first));}
        }
        map<int,point>::iterator itCluster;
        for(itCluster = cluster.begin(); itCluster!= cluster.end(); itCluster++){
            point item1 = itCluster->second;
            //cout<<"cluster:"<<item1.index<<endl;
            map<int,point>::iterator itCore;
            //cout<<"cluster length:"<<cluster.size()<<endl;
            for(itCore = _corepoints.begin(); itCore!= _corepoints.end(); itCore++){
                point item2 = itCore->second;
                double dist = calDist(item1,item2);
                if((dist <= eps) && (dist!=0)){
                    //cout<<"item1:"<<item1.index<<" item2:"<<item2.index<<" distance:"<<dist<<endl;
                    //if((cluster.find(item2.index) == cluster.end())){
                        cluster.insert(pair<int,point>(clusterNum,item2));
                        cout<<"insert:"<<item2.index<<endl;
                        clusterNum++;
                    //}
                    if(_corepoints.find(item2.index) != _corepoints.end()){
                        _corepoints.erase(_corepoints.find(item2.index));
                    }
                }
                //else cout<<"cancel distance:"<<"item1:"<<item1.index<<" item2:"<<item2.index<<" distance:"<<dist<<endl;
            }
        }
        cout<<"cluster"<<curLabel<<":"<<cluster.size()<<endl;
        map<int,point>::iterator it;
        for(it = cluster.begin(); it!= cluster.end();it++){
            ((_pointData.find((it->second).index))->second).targetLabel = curLabel;
            //cout<<it->second.index<<endl;
        }
        curLabel++;
    }


    map<int,point>::iterator it;
    for(it = _borderpoints.begin();it!=_borderpoints.end();it++){
        point border = it->second;
        map<int,point>::iterator coreIt;
        double minDist = 10;
        point closetCpt;
        for(coreIt = _pointData.begin(); coreIt!= _pointData.end(); coreIt++){
            point core = coreIt->second;
            if(core.type == "border"){
                continue;
            }
            double dist = calDist(border,core);
            if((dist <= eps) && (dist!= 0)){
                _pointData.at(border.index).targetLabel = core.targetLabel;
            }else if((minDist > dist) && dist!= 0){
                minDist = dist;
                closetCpt = core;
            }
            cout<<"border points:"<<border.index<<" core points:"<<core.index<<" distance:"<<dist<<endl;
        }
        cout<<"core points:"<<closetCpt.index<<" border points:"<<border.index<<endl;
        if(minDist<1){
            _pointData.at(border.index).targetLabel = closetCpt.targetLabel;
        }else{
            _pointData.at(border.index).targetLabel = -1;
        }
    }
}

map<int,point> DBSCAN::getCorePoints(){
    return _corepoints;
}

double DBSCAN::calDist(point item1,point item2){
    return sqrt(pow(item1.x-item2.x,2)+pow(item1.y - item2.y,2)+pow(item1.z - item2.z,2)+pow(item1.m-item2.m,2));
}

void DBSCAN::display(){
    cout<<"result: "<<endl;
    map<int,point>::iterator it;
    for(it = _pointData.begin(); it!=_pointData.end(); it++){
        point item = it->second;
        cout<<"index: "<<item.index<<" label:"<<item.label<<" result:"<<item.targetLabel<<
           " type:"<<item.type<<endl;
    }
}

map<int,point> DBSCAN::getResult(){
    return _pointData;
}
