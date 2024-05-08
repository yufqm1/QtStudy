#ifndef HISTOGRAMVIEW_H
#define HISTOGRAMVIEW_H
#include <QAbstractItemView>
#include <QItemSelectionModel>
#include <QRegion>
#include <QMouseEvent>

class HistogramView : public QAbstractItemView
{
    Q_OBJECT
public:
    HistogramView(QWidget *parent=0);
    QRect visualRect(const QModelIndex &index) const;
    void scrollTo(const QModelIndex &index,ScrollHint hint=EnsureVisible);
    QModelIndex indexAt(const QPoint &point) const;
    void setSelectionModel(QItemSelectionModel *selectionModel);
    QRegion itemRegion(QModelIndex index);

    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent *event);

protected slots:
    void selectionChanged(const QItemSelection &selected,const QItemSelection &deselected);
    void dataChanged(const QModelIndex &topLeft,const QModelIndex &bottomRight);
protected:
    //虚函数声明
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
    int horizontalOffset() const;
    int verticalOffset() const;
    bool isIndexHidden(const QModelIndex &index) const;
    void setSelection(const QRect &rect,QItemSelectionModel::SelectionFlags flags); //(f)
    QRegion visualRegionForSelection(const QItemSelection &selection) const;
private:
    QItemSelectionModel *selections;
    QList<QRegion> MRegionList;
    QList<QRegion> FRegionList;
    QList<QRegion> SRegionList;

};

// 参考： https://zhuanlan.zhihu.com/p/636586914


#endif // HISTOGRAMVIEW_H
