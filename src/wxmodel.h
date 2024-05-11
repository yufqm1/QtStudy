#ifndef WXMODEL_H
#define WXMODEL_H

#include <QObject>
#include <QListView>
#include <QAbstractListModel>
#include <QStyledItemDelegate>
#include <QPixmap>
#include <QAbstractItemModel>
struct WXData
{
    QPixmap m_icon;             //图标
    QString m_text;             //文本
    QString m_topText;          //右侧文本1：置顶区域文本
    QString m_removeText;       //右侧文本2：删除区域文本
    int m_margin;               //图标边距

    QColor m_iconBorderColor;   //图标边框颜色
    QColor m_bgColor;           //背景颜色
    QColor m_textColor;         //文本颜色
    QColor m_bgTopRectColor;    //右侧文本1区域背景：置顶区域背景色
    QColor m_bgRemoveRectColor; //右侧文本2区域背景：删除区域背景色
    QColor m_topTextColor;      //右侧文本1区域文本：置顶区域文本颜色
    QColor m_removeTextColor;   //右侧文本2区域文本：删除区域文本颜色

    bool m_pressed;             //鼠标是否按下
    bool m_canLSlide;           //是否可以左滑
    int m_offsetX;              //鼠标移动的距离
    QPoint m_pressPoint;        //鼠标按下的点

    WXData()
    {
        m_icon = QPixmap(":/new/prefix1/res/b2_06.png");             //图标
        m_text = QStringLiteral("山上山，水中水"); //文本
        m_topText = QStringLiteral("置顶");      //右侧文本1：置顶区域文本
        m_removeText = QStringLiteral("删除");   //右侧文本2：删除区域文本
        m_margin = 5;                           //图标边距

        m_iconBorderColor = Qt::gray;           //图标边框颜色
        m_bgColor = Qt::green;                  //背景颜色
        m_textColor = Qt::black;                //文本颜色
        m_bgTopRectColor = Qt::gray;            //置顶区域背景色
        m_bgRemoveRectColor = Qt::red;          //删除区域背景色
        m_topTextColor = Qt::white;             //置顶区域文本颜色
        m_removeTextColor = Qt::white;          //删除区域文本颜色

        m_pressed = false;                      //鼠标是否按下
        m_canLSlide = true;                     //是否可以左滑
        m_offsetX = 0;
        m_pressPoint = QPoint(0, 0);
    }
    void setText(int i)
    {
        m_text = QString("山上山，水中水 %1").arg(QString::number(i));
    }
};

class WXModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WXModel(QObject* parent = 0);
    ~WXModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index,int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex& index,const QVariant &value,int role = Qt::EditRole);
    bool removeRow(int row,const QModelIndex& parent = QModelIndex());
    bool insertRow(int row,WXData &data);
    // 自定义
    void appendData(WXData data);
    void setDatas(const QList<WXData> &datas);
private:
    QList<WXData> m_datas;
};

Q_DECLARE_METATYPE(WXData)

class WXDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    WXDelegate(QObject* parent = 0);
    ~WXDelegate();

    void paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index) const;
    bool editorEvent(QEvent *event,QAbstractItemModel *model,const QStyleOptionViewItem &option,const QModelIndex &index);
    QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const;

public:
    void setHorizontalValue(int value);
    void setVerticalValue(int value);
    void setSpaceLineVisible(bool visible);
    void setSpaceLineColor(QColor color);
    void setSpaceLineHeight(int height);
signals:
    void topItem(const QModelIndex &index);
    void removeItem(const QModelIndex &index);
    void updateItem(const QModelIndex &index);

private:
    int m_horizontialValue;
    int m_verticalValue;
    QModelIndex m_preIndex;
    QColor m_spaceLineColor;
    int m_spaceLineHeight;
    bool m_spaceLineVisible;
};

class WXListView : public QListView
{
    Q_OBJECT
public:
    explicit WXListView(QWidget *parent = nullptr);
    void setSpaceLineVisible(bool visible);
    void setSpaceLineColor(QColor color);
    void setSpaceLineHeight(int height);

    void setTopItem(const QModelIndex &index);
    void setRemoveItem(const QModelIndex &index);

public slots:
    void onHValueChanged(int value);
    void onVValueChanged(int value);
    void onTopItem(const QModelIndex &index);
    void onRemoveItem(const QModelIndex &index);

private:
    WXDelegate *m_delegate;
    WXModel* m_wxmodel;
};

#endif // WXMODEL_H
