#include "Test.h"
#include <QVBoxLayout>
Test::Test(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_Test)
{
    ui->setupUi(this);

    QVBoxLayout *vlayout = new QVBoxLayout;

    dashBoard = new DrawDashboard(this);


    QRect rect = this->geometry();
    //计算显示原点
    int x = rect.width()/2 - dashBoard->width() / 2;
    int y = rect.height()/10;
    dashBoard->move(x, y);



    horizontalSlider = new QSlider(Qt::Horizontal,this);
    horizontalSlider->setGeometry(0,450,800,30);

    vlayout->addWidget(dashBoard);
    vlayout->addWidget(horizontalSlider);

    connect(horizontalSlider,&QSlider::valueChanged,dashBoard,&DrawDashboard::valueChanged);

}

Test::~Test()
{
    delete ui; 
}
void Test::on_pushButton_clicked()
{

}


void Test::on_label_linkActivated(const QString &link)
{

}

