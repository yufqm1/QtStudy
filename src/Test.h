#pragma once
#include "ui_Test.h"
#include <QMainWindow>
#include "wxmodel.h"
#include "drawdashboard.h"
#include <QSlider>
#include <QVBoxLayout>
class Test : public QMainWindow {
    Q_OBJECT
    
public:
    Test(QWidget* parent = nullptr);
    ~Test();
    void AddDashboard();
    void AddMVDShow();
private slots:
    void on_pushButton_clicked();

    void on_label_linkActivated(const QString &link);
private:
    QSlider *horizontalSlider;
    DrawDashboard* dashBoard;
    WXListView *wxlistView;
private:
    Ui_Test* ui;
    QVBoxLayout *m_vlayout;
};


