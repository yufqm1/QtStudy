#include "chartwin.h"

#include <QItemSelectionModel>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QStringList>

ChartWin::ChartWin(QWidget *parent)
    : QMainWindow{parent}
{
    setMinimumSize(500,400);
    //setMaximumSize(500,400);

    CreateChartFunc();
    //CreateHistogram();
    //CreatePiechart();
    //CreateHistogrambyQBarSet();
}

void ChartWin::CreateChartFunc()
{
    chartview = new QChartView(this);
    chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle("sin/cos图表测试");
    chartview->setChart(chart);

    setCentralWidget(chartview);// 不设置显示会有问题

    QLineSeries *series1 = new QLineSeries();
    QLineSeries *series2 = new QLineSeries();
    series1->setName("sin曲线");
    series2->setName("cos曲线");
    chart->addSeries(series1);
    chart->addSeries(series2);

    qreal t=0,y1,y2,intv= 0.1;
    int icount = 100;
    for (int i = 0; i < icount; ++i)
    {
        y1=qSin(t);
        series1->append(t,y1);

        y2=qSin(t+20);
        series2->append(t,y2);

        t = t+intv;
    }

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0,10);
    axisX->setTitleText("time(secs)");

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-2,2);
    axisY->setTitleText("values");

    chart->setAxisX(axisX,series1);
    chart->setAxisY(axisY,series1);

    chart->setAxisX(axisX,series2);
    chart->setAxisY(axisY,series2);
}

void ChartWin::CreateHistogram()
{
    createAction();
    createMenu();
    setupModel();
    setupView();
    setWindowTitle(tr("View Example"));
    resize(600,600);
}

void ChartWin::createAction()
{
    openAct = new QAction(tr("打开"),this);
    connect(openAct,SIGNAL(triggered(bool)),this,SLOT(slotOpen()));
}
void ChartWin::createMenu()
{
    fileMenu = new QMenu(tr("文件"),this);
    fileMenu->addAction(openAct);
    menuBar()->addMenu(fileMenu);
}
void ChartWin::setupModel()
{
    model = new QStandardItemModel(4,4,this);
    model->setHeaderData(0,Qt::Horizontal,tr("部门"));
    model->setHeaderData(1,Qt::Horizontal,tr("男"));
    model->setHeaderData(2,Qt::Horizontal,tr("女"));
    model->setHeaderData(3,Qt::Horizontal,tr("退休"));
}
void ChartWin::setupView()
{
    splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);
    histogram=new HistogramView(splitter);
    histogram->setModel(model);
    table = new QTableView;
    table->setModel(model);
    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    table->setSelectionModel(selectionModel);
    histogram->setSelectionModel(selectionModel);
    splitter->addWidget(table);
    splitter->addWidget(histogram);
    setCentralWidget(splitter);
    connect(selectionModel,SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            table,SLOT(selectionChanged(QItemSelection,QItemSelection)));
    connect(selectionModel,SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            histogram,SLOT(selectionChanged(QItemSelection,QItemSelection)));
}
void ChartWin::slotOpen()
{
    QString name = QFileDialog::getOpenFileName(this,"打 开",".","histogram files(*.txt)");
    if(!name.isEmpty())
    {
        openFile(name);
    }
}
//Circular water wave
void ChartWin::CreateHistogrambyQBarSet()
{
    QBarSet *set0 = new QBarSet("Jane");
    QBarSet *set1 = new QBarSet("John");
    QBarSet *set2 = new QBarSet("Axel");
    QBarSet *set3 = new QBarSet("Mary");
    QBarSet *set4 = new QBarSet("Samantha");
    *set0 << 1 << 2 << 3 << 4 << 5 << 6;
    *set1 << 5 << 0 << 0 << 4 << 0 << 7;
    *set2 << 3 << 5 << 8 << 13 << 8 << 5;
    *set3 << 5 << 6 << 7 << 3 << 4 << 5;
    *set4 << 9 << 7 << 5 << 3 << 1 << 2;

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);
    series->append(set3);
    series->append(set4);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple barchart example");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,15);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);// ??
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(this);
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(chartView);
}

void ChartWin::CreatePiechart()
{
    chart = new QChart();
    QPieSeries *series = new QPieSeries();
    series->append("水果:30%",3);
    series->append("零食:20%",2);
    series->append("主食:50%",5);

    series->setLabelsVisible(true);
    series->setUseOpenGL(true);
    series->slices().at(0)->setColor(QColor(13,128,217));   //设置颜色
    series->slices().at(0)->setLabelColor(QColor(13,128,217));
    series->slices().at(1)->setColor(QColor(69,13,217));
    series->slices().at(1)->setLabelColor(QColor(69,13,217));
    series->slices().at(2)->setColor(QColor(13,217,152));
    series->slices().at(2)->setLabelColor(QColor(13,217,152));

    chart->setTheme(QChart::ChartThemeLight);//设置白色主题
    chart->setDropShadowEnabled(true);//背景阴影
    chart->addSeries(series);

    chart->setTitleBrush(QBrush(QColor(0,0,255)));//设置标题Brush
    chart->setTitleFont(QFont("微软雅黑"));//设置标题字体
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle("饼状图");

    //修改说明样式
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);//底部对齐
    chart->legend()->setBackgroundVisible(true);//设置背景是否可视
    chart->legend()->setAutoFillBackground(true);//设置背景自动填充
    chart->legend()->setColor(QColor(222,233,251));//设置颜色
    chart->legend()->setLabelColor(QColor(0,100,255));//设置标签颜色
    chart->legend()->setMaximumHeight(50);

    QChartView* chartview = new QChartView(this);
    chartview->setChart(chart);

    setCentralWidget(chartview);

    chartview->setRenderHint(QPainter::Antialiasing);


}
void ChartWin::openFile(QString path)
{
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString line;
            model->removeRows(0,model->rowCount(QModelIndex()),QModelIndex());
            int row = 0;
            do
            {
                line = stream.readLine();
                if(! line.isEmpty())
                {
                    model->insertRows(row,1,QModelIndex());
                    QStringList pieces = line.split(",");
                    //QStringList pieces = line.split(",",QString::SkipEmptyParts);
                    model->setData(model->index(row,0,QModelIndex()),pieces.value(0));
                    model->setData(model->index(row,1,QModelIndex()),pieces.value(1));
                    model->setData(model->index(row,2,QModelIndex()),pieces.value(2));
                    model->setData(model->index(row,3,QModelIndex()),pieces.value(3));
                    row++;
                }
            }while(! line.isEmpty());
            file.close();
        }
    }
}
