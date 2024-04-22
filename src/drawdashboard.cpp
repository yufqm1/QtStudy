#include "drawdashboard.h"

#include <QPainter>

#define M_PI 3.141926536
const int radius = 80;
const int Angle = 50;
DrawDashboard::DrawDashboard(QWidget *parent)
    : QWidget{parent},percent(0)
{
    setMinimumSize(400,400);
    setMaximumSize(400,400);
}

DrawDashboard::~DrawDashboard()
{

}

void DrawDashboard::drawBg(QPainter *painter)
{
    int r = radius;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(172,172,172));
    painter->drawEllipse(-r,-r,r*2,r*2);

    r = radius * 0.9;
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(40,40,40));
    painter->drawEllipse(-r,-r,r*2,r*2);
    painter->restore();
}

void DrawDashboard::drawDial(QPainter *painter)
{
    int r = radius * 0.85;
    double lineWidth = 1;
    painter->save();
    painter->rotate(Angle);
    double rotate = (double)(360 - (Angle*2))/100;

    for (int i = 0;i<=100;i++)
    {
        QColor color = QColor(84,84,84);
        if(i>80)
        {
            color =   QColor(250,0,0);
        }

        if((i%10) == 0)
        {
            painter->setPen(QPen(color,1.3 * lineWidth));
            painter->drawLine(0,r,0,r/1.2);
        }
        else if((i%2)==0){
            painter->setPen(QPen(color,1*lineWidth));
            painter->drawLine(0,r,0,r/1.1);
        }
        painter->rotate(rotate);
    }

    painter->restore();
}

void DrawDashboard::drawScaleNum(QPainter *painter)
{
    painter->save();
    int r = (int)(radius * 0.6);

    painter->setFont(QFont("Arial",10));
    painter->setPen(QPen(QColor(255,255,255)));
    QFontMetricsF fm = QFontMetricsF(painter->font());

    int gap = (360 - Angle * 2) / 10;
    for(int i =0;i<=10;i++)
    {
        int angle = 90 + Angle + gap * i; // 10格子画一个刻度值
        float angleArc = (angle % 360) * 3.14 / 180; // 转换弧度
        int x = (r) * cos(angleArc);
        int y = (r) * sin(angleArc);

        QString value = QString::number(i*10);
        int w = (int)fm.width(value);
        int h = (int)fm.height();
        x = x-w/2;
        y = y+h/4;

        painter->drawText(QPointF(x,y),value);
    }
    painter->restore();
}

void DrawDashboard::drawIndicator(QPainter *painter)
{
    painter->save();
    QPolygon pts;
    int r = radius * 0.6;
    pts.setPoints(3,-2,0,2,0,0,r);
    double degRotate = Angle + (360.0 - Angle*2) / 100 * percent;

    // 画指针
    painter->rotate(degRotate);
    QRadialGradient haloGradient(0,0,60,0,0); // 辐射渐变，内部填充颜色
    haloGradient.setColorAt(0,QColor(100,100,100));
    haloGradient.setColorAt(1,QColor(250,50,50)); // red
    painter->setPen(QColor(250,150,150));// 边框颜色
    painter->setBrush(haloGradient);
    painter->drawConvexPolygon(pts);
    painter->restore();

    // 画中心圆圈
    QRadialGradient radial(0,0,14);// 渐变
    radial.setColorAt(0.0,QColor(100,100,100));
    radial.setColorAt(1.0,QColor(250,50,50));
    painter->setPen(Qt::NoPen); // 填满没有边界
    painter->setBrush(radial);
    painter->drawEllipse(-7,-7,14,14);

    painter->restore();
}

void DrawDashboard::drawText(QPainter *painter)
{
    painter->save();

    painter->setFont(QFont("Arial",10));
    painter->setPen(QPen(QColor(255,255,255)));
    QFontMetricsF fm = QFontMetricsF(painter->font());
    QString speed = QString::number(percent) + " km/h";
    int w = (int)fm.width(speed);
    int h = (int)fm.height();
    painter->drawText(QPointF(-w/2,(int)(0.5*radius)),speed);
    painter->restore();

}

void DrawDashboard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    int width = this->width();
    int height = this->height();

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width / 2,height / 2);
    int side = qMin(width,height);
    painter.scale(side / 200.0,side / 200.0);

    drawBg(&painter);
    drawDial(&painter);
    drawScaleNum(&painter);
    drawIndicator(&painter);
    drawText(&painter);
}


void DrawDashboard::valueChanged(int value)
{
    percent = value;
    update();
}





















