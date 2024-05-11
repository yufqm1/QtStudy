#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QColor>

class QTimer;
// 球状水波进度
class WaterProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit WaterProgressBar(QWidget* parent = nullptr);

    double getValue() const;
    int getRadius() const;
    QColor getWaterColor() const;
    QColor getBgColor() const;
    void start();
    void stop();

signals:

public slots:
    void setValue(int value);
    void setRadius(int radius);
    void setWaterColor(QColor color);
    void setBgColor(QColor color);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    void drawWater(QPainter* painter);
    void drawText(QPainter* painter);
    void drawBg(QPainter* painter);

private:
    double value_;
    int radius_;
    int offset_; // 动画偏移量
    QColor waterColor_;
    QColor bgColor_;
    QTimer *timer_;

};

// 条状动态进度
class RotatingProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit RotatingProgressBar(QWidget *parent = nullptr);
    void setProgress(int value);
    int progress() const;
    void setStripeColors(const QColor &primaryColor,const QColor &secondColor);
    void setStripeWidth(int width);
    void setStripeSpacing(int spacing);
protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updatePattern();// 更新滚动模式
private:
    QTimer *timer;
    int m_progress;
    int patternOffset;

    QColor primaryStripeColor; // 主要条纹颜色
    QColor secondaryStripeColor; // 次要条纹颜色
    int stripeWidth; // 条纹宽度
    int stripeSpacing; // 条纹间距
};

#endif // MYWIDGET_H
