#ifndef DRAWDASHBOARD_H
#define DRAWDASHBOARD_H

#include <QObject>
#include <QWidget>

class DrawDashboard : public QWidget
{
    Q_OBJECT
public:
    explicit DrawDashboard(QWidget *parent = nullptr);
    ~DrawDashboard();
    void drawBg(QPainter* painter);
    void drawDial(QPainter* painter);
    void drawScaleNum(QPainter* painter);
    void drawIndicator(QPainter* painter);
    void drawText(QPainter* painter);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    int percent;
signals:

public slots:
    void valueChanged(int value);
};

#endif // DRAWDASHBOARD_H
