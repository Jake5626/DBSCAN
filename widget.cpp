#include "widget.h"
#include "ui_widget.h"

#include "readdata.h"
#include <dbscan.h>

#include <string>

#include <QFileDialog>
#include <QMessageBox>

using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->textEdit_display->setFontWeight(40);
    ui->textEdit_eps->setFontWeight(40);
    ui->textEdit_minPts->setFontWeight(40);

    ui->textEdit_eps->setAlignment(Qt::AlignVCenter);
    ui->textEdit_minPts->setAlignment(Qt::AlignVCenter);

    _dbscan = new DBSCAN();
    QObject::connect(this,SIGNAL(textChanged()),this,SLOT(showResult()));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_textEdit_eps_textChanged()
{
    _eps = ui->textEdit_eps->toPlainText().toDouble();
    //emit textChanged();
}

void Widget::on_textEdit_minPts_textChanged()
{
    bool ok;
    _minPts = ui->textEdit_minPts->toPlainText().toInt(&ok);
    //emit textChanged();
}

void Widget::showResult(){
    _dbscan->setConfig(_minPts,_eps);
    _dbscan->display();
}

void Widget::on_pushButton_clicked()
{
    QString display;
    _dbscan->setConfig(_minPts,_eps);
    map<int,point> result = _dbscan->getResult();
    map<int,point>::iterator it = result.begin();
    int wrongNum = 0;
    for(;it!=result.end();it++){
        point item = it->second;
        if(item.targetLabel != item.label){
            wrongNum++;
        }
        display.append("index: "+QString::number(item.index)+
                       " label:"+QString::number(item.label)+
                       " result:"+QString::number(item.targetLabel)+
                        " type:"+QString::fromStdString(item.type)+"\r\n");
    }
    ui->textEdit_display->setText(display);
    ui->label_wrongNum->setText("Wrong number:"+QString::number(wrongNum));
}


void Widget::on_pushButton_openFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open File"),"/Package",tr("Files(*.txt)"));
    string file = fileName.toStdString();
    if(!_dbscan->setPointData(file)){
        int ret = QMessageBox::warning(this,tr("Error"),tr("File path error,please choose file again."),QMessageBox::Cancel);
    }
    ui->label_fileName->setText("File Name:"+fileName);

}
