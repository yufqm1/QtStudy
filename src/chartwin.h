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

public: // 曲线图
    void CreateChartFunc();

public: // 柱状图  自绘
    void CreateHistogram();
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
    QSplitter *splitter;    // 分裂器
    HistogramView *histogram;
public: // 柱状图  QBarSet
    void CreateHistogrambyQBarSet();

public: // 饼图
    void CreatePiechart();

};

#endif // CHARTWIN_H
