#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <dbscan.h>

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


    void on_textEdit_eps_textChanged();

    void on_textEdit_minPts_textChanged();

    void showResult();

    void on_pushButton_clicked();



    void on_pushButton_openFile_clicked();

private:
    Ui::Widget *ui;

    DBSCAN *_dbscan;

    double _eps;

    int _minPts;

signals:
    void textChanged();
};

#endif // WIDGET_H
