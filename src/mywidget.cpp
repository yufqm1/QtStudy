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
//********************RotatingProgressBar***************************************
RotatingProgressBar::RotatingProgressBar(QWidget *parent) : QWidget(parent),
    m_progress(0),
    patternOffset(0),
    primaryStripeColor(Qt::darkGreen),
    secondaryStripeColor(Qt::green),
    stripeWidth(10),
    stripeSpacing(3)
{

    resize(300,40);

    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&RotatingProgressBar::updatePattern);
    timer->start(50);
}

void RotatingProgressBar::setProgress(int value)
{
    m_progress=value;
}

int RotatingProgressBar::progress() const
{
    return m_progress;
}

void RotatingProgressBar::setStripeColors(const QColor &primaryColor, const QColor &secondColor)
{
    primaryStripeColor = primaryColor;
    secondaryStripeColor = secondColor;
}

void RotatingProgressBar::setStripeWidth(int width)
{
    stripeWidth = width;
}

void RotatingProgressBar::setStripeSpacing(int spacing)
{
    stripeSpacing = spacing;
}

void RotatingProgressBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //// 设置this的背景
//    painter.setPen(Qt::NoPen);
//    painter.setBrush(Qt::black);
//    painter.drawRect(rect());

    const qreal radius = height() / 2.0;
    QRectF fullRect = rect();
    QRectF progressRect(0,0,static_cast<int>(width() * (m_progress / 100.0)),height());

    // 绘制带圆角的进度条背景
    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(fullRect,radius,radius);
    painter.fillPath(backgroundPath,Qt::gray);

    // 设置剪裁区，确保在圆角内不绘制条纹
    QPainterPath progressPath;
    progressPath.addRoundedRect(progressRect,radius,radius);
    painter.setClipPath(progressPath);

    // 计算条纹起始位置，确保在圆角之后开始
    qreal stripeStartX = radius + patternOffset;

    for(int x = stripeStartX - width(); x < width(); x+= stripeWidth + stripeSpacing)
    {
        QLinearGradient gradient(QPointF(x, 0), QPointF(x + height(), height()));
        gradient.setColorAt(0, primaryStripeColor);
        gradient.setColorAt(1, secondaryStripeColor);

        QPainterPath path;
        path.moveTo(QPointF(x, 0));
        path.lineTo(QPointF(x + height(), height()));
        path.lineTo(QPointF(x + height() + stripeWidth, height()));
        path.lineTo(QPointF(x + stripeWidth, 0));
        path.closeSubpath();

        painter.fillPath(path, gradient);
    }

    painter.save();
    painter.setFont(QFont("Arial",10));
    painter.setPen(QPen(QColor(255,0,0)));
    QString rate = QString("%1%").arg(m_progress);
    painter.drawText(QPointF(rect().width()/2,rect().height()/2),rate);
    painter.restore();

}

void RotatingProgressBar::updatePattern()
{
    patternOffset += 2;
    if(patternOffset >= stripeWidth + stripeSpacing)
    {
        patternOffset = 0;
    }

    if(m_progress < 100)
    {
        m_progress += 1;
    }else
    {
        m_progress = 0;
    }

    update();
}








