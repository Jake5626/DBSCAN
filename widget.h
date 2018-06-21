#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <dbscan.h>
#include <agnes.h>
#include <diana.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_openFile_clicked();

    void on_comboBox_algor_currentTextChanged(const QString &arg1);

    void on_lineEdit_num_textChanged(const QString &arg1);

    void on_lineEdit_minPts_textChanged(const QString &arg1);

    void on_lineEdit_eps_textChanged(const QString &arg1);

private:
    Ui::Widget *ui;

    DBSCAN *_dbscan;

    AGNES *_agnes;

    DIANA *_diana;

    double _eps;

    int _minPts;

    int _clusterNum;

signals:
    void textChanged();
};

#endif // WIDGET_H
