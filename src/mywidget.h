#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QColor>

class QTimer;
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

#endif // MYWIDGET_H
