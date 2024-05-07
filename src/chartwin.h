#ifndef CHARTWIN_H
#define CHARTWIN_H

#include <QMainWindow>
#include <QObject>
#include <QtCharts>

#include <QStandardItemModel>
#include <QTableView>
#include <QMenuBar>
#include <QAction>
#include <QSplitter>
#include "histogramview.h"

QT_CHARTS_USE_NAMESPACE

class ChartWin : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChartWin(QWidget *parent = nullptr);

signals:
private:
    QChartView *chartview;
    QChart *chart;
    void CreateChartFunc();
    void CreateHistogram();

public: // 柱状图
    void createAction();
    void createMenu();
    void setupModel();
    void setupView();
    void openFile(QString);
    public slots:
    void slotOpen();
    private:
    QMenu *fileMenu;
    QAction *openAct;
    QStandardItemModel *model;
    QTableView *table;
    QSplitter *splitter;
    HistogramView *histogram;


};

#endif // CHARTWIN_H
