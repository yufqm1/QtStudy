#include "mywidget.h"
#include <QTimer>
#include <QPainter>
#include <QPainterPath>
#include <QtCore/qmath.h>
WaterProgressBar::WaterProgressBar(QWidget *parent) : QWidget(parent),
    value_(0),
    radius_(60),
    offset_(50),
    waterColor_(0,240,0),
    bgColor_(0,0,0)
{
    resize(600,600);
    timer_ = new QTimer(this);
    timer_->setInterval(300);
    offset_ = 50;
    connect(timer_,&QTimer::timeout,[=](){
        offset_ += 10;
        if(offset_ == 100)
        {
            offset_ = 50;
        }

        if(value_ < 100){
            value_ += 0.1;
        }

        update(); // 触发重绘
    });

    start();
}

double WaterProgressBar::getValue() const
{
    return value_;
}

int WaterProgressBar::getRadius() const
{
    return radius_;
}

QColor WaterProgressBar::getWaterColor() const
{
    return waterColor_;
}

QColor WaterProgressBar::getBgColor() const
{
    return bgColor_;
}

void WaterProgressBar::start()
{
    timer_->start();
}

void WaterProgressBar::stop()
{
    timer_->stop();
}

void WaterProgressBar::setValue(int value)
{
    value_ = value;
    repaint();
}

void WaterProgressBar::setRadius(int radius)
{
    radius_ = radius;
    repaint();
}

void WaterProgressBar::setWaterColor(QColor color)
{
    waterColor_ = color;
    repaint();
}

void WaterProgressBar::setBgColor(QColor color)
{
    bgColor_ = color;
    repaint();
}

void WaterProgressBar::paintEvent(QPaintEvent *event)
{
    int width = this->width();
    int height= this->height();
    int side = qMin(width,height);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2,height /2);
    painter.scale(side / 200.0,side / 200.0);

    drawBg(&painter);
    drawWater(&painter);
    drawText(&painter);

    QWidget::paintEvent(event);
}

void WaterProgressBar::drawWater(QPainter *painter)
{
    double sinA = 2.5; // 波幅
    double sinAlph = 0.1; // 波长
    QRect rect(-radius_,-radius_,radius_*2,radius_*2);
    int startX = -radius_;
    int startY = -radius_;
    int endX = radius_;
    int endY = radius_;

    float height = radius_ - ((value_*radius_)/50);// 根据进度调整水位

    QPainterPath waterPath1,waterPath2;
    waterPath1.moveTo(startX,endY);
    waterPath2.moveTo(startX,endY);

    for(int i = startX;i<=endX;i++){
        float y1, y2;
        y1 = sinA * qSin(sinAlph * i + offset_) + height; // y = A * sin(ωx + φ) + k
        y2 = sinA * qSin(sinAlph * i + 60 + offset_) + height;

        if (value_ == 0){
            y1 = y2 = endY;
        }
        if (value_ == 100){
            y1 = y2 = -endY;
        }
        waterPath1.lineTo(i, y1);
        waterPath2.lineTo(i, y2);
    }

    waterPath1.lineTo(endX, endY);
    waterPath2.lineTo(endX, endY);

    QPainterPath bigPath;
    bigPath.addEllipse(rect);
    waterPath1 = bigPath.intersected(waterPath1);
    waterPath2 = bigPath.intersected(waterPath2);

    painter->save();
    painter->setPen(Qt::NoPen);

    waterColor_.setAlpha(180);
    painter->setBrush(waterColor_);
    painter->drawPath(waterPath2);

    waterColor_.setAlpha(100);
    painter->setBrush(waterColor_);
    painter->drawPath(waterPath1);

    painter->restore();

}

void WaterProgressBar::drawText(QPainter *painter)
{
    painter->save();
    QString text = QString("%1%").arg(value_);

    QFont font = painter->font();
    font.setPixelSize(30);
    painter->setFont(font);

    QPen pen = painter->pen();
    pen.setColor(Qt::white);
    pen.setWidth(4);
    painter->setPen(pen);

    QRect rect(-radius_,-radius_,radius_*2,radius_*2);
    painter->drawText(rect,Qt::AlignCenter,text);
    painter->restore();
}

void WaterProgressBar::drawBg(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(bgColor_));
    QRect rect(-radius_,-radius_,radius_*2,radius_*2);
    painter->drawEllipse(rect);
    painter->restore();
}
