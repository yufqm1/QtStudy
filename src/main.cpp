
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include "math.h"

#include <QDebug>
#include <QDir>

#include "Test.h"
#include "drawdashboard.h"
#include "chartwin.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Test test;
    //test.show();

    ChartWin cwin;
    cwin.show();

    return a.exec();
}
