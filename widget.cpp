 /**
  * @brief a little program for cluster
  * @param widget
  * @author Jake Zheng
  * @date 2018-6-7
  */
//-----------------------------------------

#include "widget.h"
#include "ui_widget.h"

#include "readdata.h"
#include <dbscan.h>
#include <agnes.h>
#include <diana.h>

#include <string>
#include <iostream>

#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidget>

using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->textEdit_display->setFontWeight(40);

    ui->lineEdit_eps->setAlignment(Qt::AlignCenter);
    ui->lineEdit_minPts->setAlignment(Qt::AlignCenter);
    ui->lineEdit_num->setAlignment(Qt::AlignCenter);

    ui->comboBox_algor->setCurrentIndex(1);
    ui->lineEdit_eps->setEnabled(true);
    ui->lineEdit_minPts->setEnabled(true);
    ui->lineEdit_num->setEnabled(false);

    QStringList header;
    header<<"index"<<"label"<<"result"<<"type";
    //ui->tableWidget_display->setHorizontalHeaderLabels(header);

    _agnes = new AGNES();
    _dbscan = new DBSCAN();
    _diana = new DIANA();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QString display;
    QString algorithm = ui->comboBox_algor->currentText();
    map<int,point> result;
    result.clear();
    cout<<"result:"<<result.size()<<endl;
    if(algorithm == "DBSCAN"){
        _dbscan->setConfig(_minPts,_eps);
        result = _dbscan->getResult();
    }
    if(algorithm == "AGNES"){
        _agnes->setClusterNum(_clusterNum);
        result = _agnes->getResult();
    }
    if(algorithm == "DIANA"){
        _diana->setClusterNum(_clusterNum);
        result = _diana->getResult();
    }
    map<int,point>::iterator it = result.begin();
    int wrongNum = 0;
    for(;it!=result.end();it++){
        point item = it->second;
        if(item.targetLabel != item.label){
            wrongNum++;
        }
        if(ui->comboBox_algor->currentText() == "DBSCAN"){
            display.append("index: "+QString::number(item.index)+
                           " label:"+QString::number(item.label)+
                           " result:"+QString::number(item.targetLabel)+
                            " type:"+QString::fromStdString(item.type)+"\r\n");
        }
        if(ui->comboBox_algor->currentText() == "AGNES"){
            display.append("index: "+QString::number(item.index)+
                           " label:"+QString::number(item.label)+
                           " result:"+QString::number(item.targetLabel)+"\r\n");
        }
        if(ui->comboBox_algor->currentText() == "DIANA"){
            display.append("index: "+QString::number(item.index)+
                           " label:"+QString::number(item.label)+
                           " result:"+QString::number(item.targetLabel)+"\r\n");
        }
    }
    ui->textEdit_display->setText(display);
    ui->label_wrongNum->setText("Wrong number:"+QString::number(wrongNum));
}


void Widget::on_pushButton_openFile_clicked()
{
    ui->label_fileName->setText("File Name:");
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open File"),"dataset",tr("Files(*.txt)"));
    string file = fileName.toStdString();
    if(!_agnes->setPointData(file)||!_dbscan->setPointData(file)||!_diana->setPointData(file)){
        QMessageBox::warning(this,tr("Error"),tr("File path error,please choose file again."),QMessageBox::Cancel);
    }
    ui->label_fileName->setText("File Name:"+fileName);

}

void Widget::on_comboBox_algor_currentTextChanged(const QString &arg1)
{
    if(arg1 == "AGNES"){
        ui->lineEdit_eps->setEnabled(false);
        ui->lineEdit_minPts->setEnabled(false);
        ui->lineEdit_num->setEnabled(true);
    }
    if(arg1 == "DBSCAN"){
        ui->lineEdit_eps->setEnabled(true);
        ui->lineEdit_minPts->setEnabled(true);
        ui->lineEdit_num->setEnabled(false);
    }
    if(arg1 == "DIANA"){
        ui->lineEdit_eps->setEnabled(false);
        ui->lineEdit_minPts->setEnabled(false);
        ui->lineEdit_num->setEnabled(true);
    }
}



void Widget::on_lineEdit_num_textChanged(const QString &arg1)
{
    bool ok;
    _clusterNum = arg1.toInt(&ok);
}

void Widget::on_lineEdit_minPts_textChanged(const QString &arg1)
{
    bool ok;
    _minPts = arg1.toInt(&ok);
}

void Widget::on_lineEdit_eps_textChanged(const QString &arg1)
{
    _eps = arg1.toDouble();
}
