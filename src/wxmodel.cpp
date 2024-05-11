#include "wxmodel.h"
#include <QScrollBar>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QStandardItem>
/***********************************
 *  Model
 *************************/
WXModel::WXModel(QObject *parent) :
    QAbstractListModel(parent)
{
    for (int i = 0; i < 10; ++i)
    {
        WXData data;
        data.setText(i);
        appendData(data);
    }
}

WXModel::~WXModel()
{

}

QModelIndex WXModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row,column,parent))
    {
        return QModelIndex();
    }

    return createIndex(row,column);
}

QModelIndex WXModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int WXModel::columnCount(const QModelIndex &parent) const
{
//    if(!parent.isValid())
//        return 0;

    return 1;
}

int WXModel::rowCount(const QModelIndex &parent) const
{
//    if(!parent.isValid())
//        return 0;

    return m_datas.size();
}

QVariant WXModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
        return QVariant::fromValue(m_datas.at(index.row()));

    return QVariant();
}

bool WXModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    WXData data = value.value<WXData>();
    m_datas.replace(index.row(),data);
    emit dataChanged(index,index);

    return true;
}

bool WXModel::removeRow(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent,row,row);
    m_datas.removeAt(row);
    endRemoveRows();
    return true;
}

bool WXModel::insertRow(int row, WXData &data)
{
    beginInsertRows(QModelIndex(),row,row);
    m_datas.insert(row,data);
    endInsertRows();
    return true;
}

void WXModel::appendData(WXData data)
{
    m_datas.append(data);
}

void WXModel::setDatas(const QList<WXData> &datas)
{
    m_datas = datas;
}

/***********************************
 *  Delegate
 *************************/

WXDelegate::WXDelegate(QObject *parent)
{
    m_verticalValue = 0;
    m_horizontialValue = 0;
    m_spaceLineColor = QColor(128,128,128);
    m_spaceLineHeight = 1;
}

WXDelegate::~WXDelegate()
{

}

void WXDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.data(Qt::DisplayRole).canConvert<WXData>())
    {
        painter->save();

        QStyleOptionViewItem view_option(option);
        if(view_option.state & QStyle::State_HasFocus)
        {
            view_option.state = view_option.state ^ QStyle::State_HasFocus;
        }
        QStyledItemDelegate::paint(painter,view_option,index);

        WXData data = index.data(Qt::DisplayRole).value<WXData>();
        double width = option.rect.width();
        double height = option.rect.height();

        painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter->translate(data.m_offsetX - m_horizontialValue,height * (index.row() - m_verticalValue));
        QFont font;
        font.setPixelSize(height * 0.6);
        painter->setFont(font);

        // 绘制背景
        painter->setPen(Qt::NoPen);
        painter->setBrush(data.m_bgColor);
        painter->drawRect(QRect(0,0,width + 2 * height,height));
        // 绘制图标
        QRectF iconRect = QRectF(data.m_margin,data.m_margin,height - 2*data.m_margin,height - 2*data.m_margin);
        painter->setPen(QPen(data.m_iconBorderColor,1));
        painter->setBrush(data.m_icon.scaled(iconRect.width(),iconRect.height()));
        painter->drawEllipse(iconRect);
        // 绘制文本
        painter->setPen(data.m_textColor);
        painter->setBrush(Qt::NoBrush);
        QRect textRect = QRect(height,0,width - height,height);
        painter->drawText(textRect,Qt::AlignLeft | Qt::AlignVCenter,data.m_text);
        // 绘制右侧置顶区域
        painter->setPen(Qt::NoPen);
        painter->setBrush(data.m_bgTopRectColor);
        QRect topRect = QRect(width,0,height,height);
        painter->drawRect(topRect);
        // 绘制右侧删除区域
        painter->setBrush(data.m_bgRemoveRectColor);
        QRect removeRect = QRect(width + height,0,height,height);
        painter->drawRect(removeRect);
        // 绘制右侧置顶文本
        font.setPixelSize(height * 0.4);
        painter->setFont(font);
        painter->setPen(data.m_topTextColor);
        painter->drawText(topRect,Qt::AlignCenter,data.m_topText);
        // 绘制右侧删除文本
        painter->setPen(data.m_removeTextColor);
        painter->drawText(removeRect,Qt::AlignCenter,data.m_removeText);
        // 绘制底部分割线
        painter->setPen(QPen(m_spaceLineColor,m_spaceLineHeight));
        painter->setBrush(Qt::NoBrush);
        int y = option.rect.height() - m_spaceLineHeight;
        painter->drawLine(QPoint(0,y),QPoint(option.rect.width(),y));

        painter->restore();
    }
    else
    {
        QStyledItemDelegate::paint(painter,option,index);
        return;
    }
}

bool WXDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        if (m_preIndex.isValid() && m_preIndex != index && m_preIndex.row() < model->rowCount())
        {
            //m_preIndex.row() < model->rowCount()，避免保存之前的索引被删除，而造成索引越界（例如删除最后一个）
            //设置Model
            WXData preData = m_preIndex.data(Qt::DisplayRole).value<WXData>();
            preData.m_offsetX = 0;
            preData.m_canLSlide = true;
            model->setData(m_preIndex, QVariant::fromValue(preData), Qt::DisplayRole);
        }
        m_preIndex = index;
        QMouseEvent *e = dynamic_cast<QMouseEvent *>(event);
        WXData data = index.data(Qt::DisplayRole).value<WXData>();
        data.m_pressed = true;
        data.m_pressPoint = e->pos();
        model->setData(index, QVariant::fromValue(data), Qt::DisplayRole);
        if (!data.m_canLSlide)
        {//若不可以左滑，则证明已滑动到最右端
            int width = option.rect.width();
            int height = option.rect.height();
            QRect topRect = QRect(width - 2 * height - m_horizontialValue, height * (index.row() - m_verticalValue), height, height);
            QRect removeRect = QRect(width - height - m_horizontialValue, height * (index.row() - m_verticalValue), height, height);
            //鼠标点击置顶区域
            if (topRect.contains(data.m_pressPoint))
                emit topItem(index);
            //鼠标点击删除区域
            else if (removeRect.contains(data.m_pressPoint))
                emit removeItem(index);
        }
    }
    else if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *e = dynamic_cast<QMouseEvent *>(event);
        WXData data = index.data(Qt::DisplayRole).value<WXData>();
        if (!data.m_pressed)
        {//防止鼠标未松开的情况，在多个item之间移动
            //设置Model
            WXData preData = m_preIndex.data(Qt::DisplayRole).value<WXData>();
            preData.m_offsetX = 0;
            preData.m_canLSlide = true;
            model->setData(m_preIndex, QVariant::fromValue(preData), Qt::DisplayRole);
        }
        else
        {
            QMouseEvent *e = dynamic_cast<QMouseEvent *>(event);
            QPoint p = e->pos();
            int offsetX = p.x() - data.m_pressPoint.x();
            data.m_offsetX = offsetX;
            data.m_canLSlide ? data.m_offsetX += 0 : data.m_offsetX -= 2 * option.rect.height();
            if (data.m_canLSlide && offsetX < 0 && qAbs(offsetX) < 2 * option.rect.height())
                model->setData(index, QVariant::fromValue(data), Qt::DisplayRole);//可以左滑
            else if (!data.m_canLSlide && offsetX > 0 && qAbs(offsetX) < 2 * option.rect.height())
                model->setData(index, QVariant::fromValue(data), Qt::DisplayRole);//可以右滑
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        WXData data = index.data(Qt::DisplayRole).value<WXData>();
        data.m_pressed = false;
        data.m_pressPoint = QPoint(0, 0);
        //左滑动
        if (data.m_offsetX < 0)
        {
            qAbs(data.m_offsetX) >= option.rect.height() ? data.m_offsetX = -2 * option.rect.height() : data.m_offsetX = 0;
        }
        else if (data.m_offsetX > 0)
        {
            qAbs(data.m_offsetX) >= option.rect.height() ? data.m_offsetX = 0 : data.m_offsetX = -2 * option.rect.height();
        }

        data.m_offsetX == -2 * option.rect.height() ? data.m_canLSlide = false : data.m_canLSlide = true;
        model->setData(index, QVariant::fromValue(data), Qt::DisplayRole);//可以右滑
    }

    return true;
}

QSize WXDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(),40);
}

void WXDelegate::setHorizontalValue(int value)
{
    m_horizontialValue = value;
}

void WXDelegate::setVerticalValue(int value)
{
    m_verticalValue = value;
}

void WXDelegate::setSpaceLineVisible(bool visible)
{
    m_spaceLineVisible = visible;
}

void WXDelegate::setSpaceLineColor(QColor color)
{
    m_spaceLineColor = color;
}

void WXDelegate::setSpaceLineHeight(int height)
{
    m_spaceLineHeight = height;
}

/***********************************
 *  ListView
 *************************/
WXListView::WXListView(QWidget *parent) : QListView(parent)
{
    this->resize(250,1000);

    m_delegate = new WXDelegate();

    m_wxmodel = new WXModel();

    this->setModel(m_wxmodel);
    this->setItemDelegate(m_delegate);

//    QScrollBar *hBar = horizontalScrollBar();
//    connect(hBar,&QScrollBar::valueChanged,this,&WXListView::onHValueChanged);
//    QScrollBar *vBar = verticalScrollBar();
//    connect(vBar,&QScrollBar::valueChanged,this,&WXListView::onVValueChanged);

    connect(m_delegate,SIGNAL(removeItem(const QModelIndex &)),this,SLOT(onRemoveItem(const QModelIndex &)));
    connect(m_delegate,SIGNAL(topItem(const QModelIndex &)),this,SLOT(onTopItem(const QModelIndex &)));

}

void WXListView::setSpaceLineVisible(bool visible)
{
    m_delegate->setSpaceLineVisible(visible);
}

void WXListView::setSpaceLineColor(QColor color)
{
    m_delegate->setSpaceLineColor(color);
}

void WXListView::setSpaceLineHeight(int height)
{
    m_delegate->setSpaceLineHeight(height);
}

void WXListView::setTopItem(const QModelIndex &index)
{
    if (index.model()->rowCount() == 1)
        return;

    WXData data = index.data(Qt::DisplayRole).value<WXData>();
    data.m_offsetX = 0;

    QAbstractItemModel* model = const_cast<QAbstractItemModel*>(index.model());
    WXModel* wxModel = dynamic_cast<WXModel*>(model);
    wxModel->removeRow(index.row());
    wxModel->insertRow(0,data);
}

void WXListView::setRemoveItem(const QModelIndex &index)
{
    QAbstractItemModel *model = const_cast<QAbstractItemModel*>(index.model());
    WXModel *wxModel = dynamic_cast<WXModel *>(model);
    wxModel->removeRow(index.row());
}

void WXListView::onHValueChanged(int value)
{
    m_delegate->setHorizontalValue(value);
}

void WXListView::onVValueChanged(int value)
{
    m_delegate->setVerticalValue(value);
}

void WXListView::onTopItem(const QModelIndex &index)
{
    setTopItem(index);
}

void WXListView::onRemoveItem(const QModelIndex &index)
{
    setRemoveItem(index);
}
