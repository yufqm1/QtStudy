#include "Test.h"
#include <QVBoxLayout>
#include <QDebug>
Test::Test(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_Test)
{
    ui->setupUi(this);
    m_vlayout = new QVBoxLayout;

    // AddDashboard();
    //AddMVDShow();
    // AddChart();
    AddMyWidget();
}

Test::~Test()
{
    delete ui;
}

void Test::AddDashboard()
{
    dashBoard = new DrawDashboard(this);


    QRect rect = this->geometry();
    //计算显示原点
    int x = rect.width()/2 - dashBoard->width() / 2;
    int y = rect.height()/10;
    dashBoard->move(x, y);



    horizontalSlider = new QSlider(Qt::Horizontal,this);
    horizontalSlider->setGeometry(0,450,800,30);

    m_vlayout->addWidget(dashBoard);
    m_vlayout->addWidget(horizontalSlider);

    connect(horizontalSlider,&QSlider::valueChanged,dashBoard,&DrawDashboard::valueChanged);
}

void Test::AddMVDShow()
{
    wxlistView = new WXListView(this);
    m_vlayout->addWidget(wxlistView);
}

void Test::AddMyWidget()
{
    widget = new WaterProgressBar(this);
    QRect rect = this->geometry();
    //计算显示原点
    int x = rect.width()/2 - widget->width() / 2;
    int y = rect.height()/10;
    widget->move(x, y);


    m_vlayout->addWidget(widget);
}

void Test::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "hhhhhhhh" ;
    if(event->button() == Qt::LeftButton)
    {
        qDebug() << "LeftButton";
    }
}



void Test::on_pushButton_clicked()
{

}


void Test::on_label_linkActivated(const QString &link)
{

}

