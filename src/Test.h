#pragma once
#include "ui_Test.h"
#include <QMainWindow>
#include "drawdashboard.h"
#include <QSlider>
class Test : public QMainWindow {
    Q_OBJECT
    
public:
    Test(QWidget* parent = nullptr);
    ~Test();

private slots:
    void on_pushButton_clicked();

    void on_label_linkActivated(const QString &link);
private:
    QSlider *horizontalSlider;
    DrawDashboard* dashBoard;
private:
    Ui_Test* ui;
};


