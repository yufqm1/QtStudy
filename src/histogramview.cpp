#include "histogramview.h"
#include <QPainter>
HistogramView::HistogramView(QWidget *parent) : QAbstractItemView(parent)
{
}
//paintEvent()函数具体完成柱状统计图的绘制工作
void HistogramView::paintEvent(QPaintEvent *)
{
    QPainter painter(viewport());
    painter.setPen(Qt::black);
    int x0=40;
    int y0=250;
    /*完成了x、y坐标轴的绘制，并标注坐标轴的变量*/
    //y坐标轴
    painter.drawLine(x0,y0,40,30);
    painter.drawLine(38,32,40,30);
    painter.drawLine(40,30,42,32);
    painter.drawText(20,30,tr("人数"));
    //x坐标轴
    painter.drawLine(x0,y0,540,250);
    painter.drawLine(538,248,540,250);
    painter.drawLine(540,250,538,252);
    painter.drawText(545,250,tr("部门"));
    /*完成x轴坐标值的绘制*/
    int posD=x0+20;
    int row;
    for(row=0;row<model()->rowCount(rootIndex());row++)
    {
        QModelIndex index=model()->index(row,0,rootIndex());
        QString dep=model()->data(index).toString();
        painter.drawText(posD,y0+20,dep);
        posD+=50;
    }
    /*完成了表格第一列数据的柱状统计图的绘制*/
    //男
    int posM=x0+20;
    MRegionList.clear();
    for(row=0;row<model()->rowCount(rootIndex());row++)
    {
        QModelIndex index=model()->index(row,1,rootIndex());
        int male=model()->data(index).toDouble();
        int width=10;
        if(selections->isSelected(index)) //(b)
            painter.setBrush(QBrush(Qt::blue,Qt::Dense3Pattern));
        else
            painter.setBrush(Qt::blue);
        painter.drawRect(QRect(posM,y0-male*10,width,male*10));
        QRegion regionM(posM,y0-male*10,width,male*10);
        MRegionList.insert(row,regionM);
        posM+=50;
    }
    /*完成了表格第一列数据的柱状统计图的绘制*/
    //女
    int posF=x0+30;
    FRegionList.clear();
    for(row=0;row < model()->rowCount(rootIndex());row++)
    {
        QModelIndex index=model()->index(row,2,rootIndex());
        int female=model()->data(index).toDouble();
        int width=10;
        if(selections->isSelected(index))
            painter.setBrush(QBrush(Qt::red,Qt::Dense3Pattern));
        else
            painter.setBrush(Qt::red);
        painter.drawRect(QRect(posF,y0-female*10,width,female*10));
        QRegion regionF(posF,y0-female*10,width,female*10);
        FRegionList.insert(row,regionF);
        posF+=50;
    }
    /*完成了表格第3列数据的柱状统计图的绘制*/
    //退休
    int posS=x0+40;
    SRegionList.clear();
    for(row=0;row<model()->rowCount(rootIndex());row++)
    {
        QModelIndex index=model()->index(row,3,rootIndex());
        int retire=model()->data(index).toDouble();
        int width=10;
        if(selections->isSelected(index))
            painter.setBrush(QBrush(Qt::green,Qt::Dense3Pattern));
        else
            painter.setBrush(Qt::green);
        painter.drawRect(QRect(posS,y0-retire*10,width,retire*10));
        QRegion regionS(posS,y0-retire*10,width,retire*10);
        SRegionList.insert(row,regionS);
        posS+=50;
    }
}
void HistogramView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    QAbstractItemView::dataChanged(topLeft,bottomRight);
    viewport()->update();
}
void HistogramView::setSelectionModel(QItemSelectionModel *selectionModel)
{
    selections=selectionModel;
}
void HistogramView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    viewport()->update();
}
void HistogramView::mousePressEvent(QMouseEvent *event)
{
    QAbstractItemView::mousePressEvent(event);
    setSelection(QRect(event->pos().x(),event->pos().y(),1,1),QItemSelectionModel::SelectCurrent);
}
void HistogramView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags)
{
    int rows=model()->rowCount(rootIndex());
    int columns=model()->columnCount(rootIndex());
    QModelIndex selectedIndex;
    for(int row=0;row<rows;++row)
    {
        for(int column=1;column<columns;++column)
        {
            QModelIndex index=model()->index(row,column,rootIndex());
            QRegion region=itemRegion(index);
            if(!region.intersected(rect).isEmpty())
                selectedIndex=index;
        }
    }
    if(selectedIndex.isValid())
        selections->select(selectedIndex,flags);
    else
    {
        QModelIndex noIndex;
        selections->select(noIndex,flags);
    }
}
QModelIndex HistogramView::indexAt(const QPoint &point) const
{
    QPoint newPoint(point.x(),point.y());
    QRegion region;
    //男 列
    foreach(region,MRegionList)
    {
        if(region.contains(newPoint))
        {
            int row=MRegionList.indexOf(region);
            QModelIndex index=model()->index(row,1,rootIndex());
            return index;
        }
    }
    //女 列
    foreach(region,FRegionList)
    {
        if(region.contains(newPoint))
        {
            int row=FRegionList.indexOf(region);
            QModelIndex index=model()->index(row,2,rootIndex());
            return index;
        }
    }
    //合计 列
    foreach (region, SRegionList)
    {
        if(region.contains(newPoint))
        {
            int row=FRegionList.indexOf(region);
            QModelIndex index=model()->index(row,3,rootIndex());
            return index;
        }
    }
    return QModelIndex();
}
QRect HistogramView::visualRect(const QModelIndex &index) const
{
    return QRect();
}
void HistogramView::scrollTo(const QModelIndex &index, ScrollHint hint)
{
}
QModelIndex HistogramView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    return QModelIndex();
}
int HistogramView::horizontalOffset() const
{
    return 0;
}
int HistogramView::verticalOffset() const
{
    return 0;
}
bool HistogramView::isIndexHidden(const QModelIndex &index) const
{
    return true;
}
QRegion HistogramView::visualRegionForSelection(const QItemSelection &selection) const
{
    return QRegion();
}
QRegion HistogramView::itemRegion(QModelIndex index)
{
    QRegion region;
    if(index.column()==1)
        region=MRegionList[index.row()];
    if(index.column()==2)
        region=FRegionList[index.row()];
    if(index.column()==3)
        region=SRegionList[index.row()];
    return region;
}
