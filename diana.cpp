#include "diana.h"

#include <readdata.h>

#include <map>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

DIANA::DIANA()
{
    _read = new readData();
}

bool DIANA::setPointData(string filename){
    bool ok = _read->openFile(filename);
    vector<point> data = _read->getStdData();
    for(int i = 0; i < data.size(); i++){
        _pointData.insert(pair<int,point>(i,data.at(i)));
    }
    return ok;
}

void DIANA::setClusterNum(int num){
    _clusterNum = num;
    cluster(_clusterNum);
}

double DIANA::calDistance(point item1, point item2){
    return sqrt(pow(item1.x-item2.x,2)+pow(item1.y-item2.y,2)+pow(item1.z-item2.z,2)+pow(item1.m-item2.m,2));
}

void DIANA::cluster(int num){
    num = _clusterNum;
    vector<map<int,point>> clusterTemp;
    map<int,point> newCluster;
    map<int,point> remainCluster;
    clusterTemp.push_back(_pointData);

    while(clusterTemp.size() < num){
        newCluster.clear();
        double maxDist = 0;
        double dist;
        int maxClusterIndex;
        point remain;
        point out;
        for(int i = 0 ; i< clusterTemp.size() ; i++){
            map<int,point> cluster = clusterTemp.at(i);
            map<int,point>::iterator itI;
            for(itI = cluster.begin(); itI != cluster.end(); itI++){
                map<int,point>::iterator itJ;
                for(itJ = cluster.begin(); itJ != cluster.end() ;itJ++){
                    if(itI->first == itJ->first) continue;
                    dist = calDistance(itI->second,itJ->second);
                    if(dist > maxDist){
                        maxDist = dist;
                        maxClusterIndex = i;
                    }
                }
            }
        }
        double avgDist = 0;
        double avgMaxDist = 0;
        int avgIndex;
        map<int,point>::iterator itmapI = clusterTemp.at(maxClusterIndex).begin();
        for(;itmapI != clusterTemp.at(maxClusterIndex).end(); itmapI++){
            map<int,point>::iterator itmapJ = clusterTemp.at(maxClusterIndex).begin();
            for(;itmapJ != clusterTemp.at(maxClusterIndex).end(); itmapJ++){
                if(itmapI->first == itmapJ->first){ continue;}
                avgDist = avgDist+calDistance(itmapI->second, itmapJ->second);
            }
            avgDist = avgDist/(double)((clusterTemp.at(maxClusterIndex).size())-1);
            if(avgDist > avgMaxDist){
                avgMaxDist = avgDist;
                avgIndex = itmapI->first;
            }
        }
        remainCluster.clear();
        out = _pointData.find(avgIndex)->second;
        cout<<"the most different point(out point):"<<out.index<<":its distance:"<<avgMaxDist<<endl;
//        if(avgMaxDist > 0.7){
//            clusterTemp.at(maxClusterIndex).erase(clusterTemp.at(maxClusterIndex).find(out.index));
//            continue;
//        }
        newCluster.insert(pair<int,point>(out.index,out));
        clusterTemp.at(maxClusterIndex).erase(clusterTemp.at(maxClusterIndex).find(out.index));
        remainCluster = clusterTemp.at(maxClusterIndex);
        map<int,point>::iterator remainIt = remainCluster.begin();
        cout<<"remain size:"<<remainCluster.size()<<endl;
        for(;remainIt != remainCluster.end(); remainIt++){
            double remainDist = 0,outDist = 0;
            map<int,point>::iterator itStay = remainCluster.begin();
            for(; itStay != remainCluster.end(); itStay++){
                if(remainIt->first == itStay->first) continue;
//                cout<<calDistance(remainIt->second,itStay->second)<<endl;
//                if(calDistance(remainIt->second,itStay->second) < remainDist){
//                    remainDist = calDistance(remainIt->second,itStay->second);
//                }
                remainDist += calDistance(remainIt->second,itStay->second);
//                cout<<"remain:"<<remainDist<<endl;
            }
            remainDist /= (remainCluster.size()-1);
            map<int,point>::iterator itOut = newCluster.begin();
            //cout<<itOut->first<<endl;
            for(;itOut!= newCluster.end(); itOut++){
//                cout<<calDistance(remainIt->second,itOut->second)<<endl;
//                if(calDistance(remainIt->second,itOut->second) < outDist){
//                    outDist = calDistance(remainIt->second,itOut->second);
//                }
//                cout<<"out:"<<outDist<<endl;
                outDist+=calDistance(remainIt->second,itOut->second);
            }
            outDist/=newCluster.size();
            cout<<"index:"<<remainIt->first<<" stay distance:"<<remainDist<<" outDistance:"<<outDist<<endl;
            if(remainDist >= outDist){
                newCluster.insert(pair<int,point>(remainIt->first,remainIt->second));
                clusterTemp.at(maxClusterIndex).erase(clusterTemp.at(maxClusterIndex).find(remainIt->first));
            }
            cout<<"remain size:"<<clusterTemp.at(maxClusterIndex).size()<<" out size:"<<newCluster.size()<<endl;
        }
        clusterTemp.push_back(newCluster);
    }


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

}

void DIANA::display(){
    cout<<"result: "<<endl;
    map<int,point>::iterator it;
    for(it = _pointData.begin(); it!=_pointData.end(); it++){
        point item = it->second;
        cout<<"index: "<<item.index<<" label:"<<item.label<<" result:"<<item.targetLabel<<endl;
    }

//    map<int,point>::iterator pt = _pointData.begin();
//    for(;pt != _pointData.end();pt++){
//        cout<<calDistance(_pointData.find(0)->second,pt->second)<<endl;
//    }

}

map<int,point> DIANA::getResult(){
    return _pointData;
}


