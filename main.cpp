#include "widget.h"
#include <QApplication>

#include <dbscan.h>
#include <agnes.h>
#include <diana.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    DBSCAN *_dbscan = new DBSCAN();
    AGNES *_agnes = new AGNES();
    DIANA *_diana = new DIANA();

//    _dbscan->setPointData("D:\iris-dataset.txt");
//    _dbscan->setConfig(5,0.09);
//    _dbscan->display();

//    _agnes->setPointData("D:\iris-dataset.txt");
//    _agnes->setClusterNum(3);
//    _agnes->display();

    _diana->setPointData("D:\iris-dataset.txt");
    _diana->setClusterNum(3);
    _diana->display();

    return a.exec();
}
