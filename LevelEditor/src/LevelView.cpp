#include "LevelView.h"

#include "ObjectsList.h"

#include <QDrag>
#include <QDebug>
#include <QPainter>
#include <QMimeData>
#include <QDragEnterEvent>
LevelView::LevelView(int numOfVetiocalCells, int numOfHorizontalCells, QWidget *parent)
    :QWidget(parent)
    ,objectSize_(30) // FIXME not magic const
    ,height_(numOfVetiocalCells * objectSize_)
    ,width_(numOfHorizontalCells* objectSize_)
{
    setAcceptDrops(true);
}

void LevelView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(ObjectsList::mimeType())) //TODO move out of ObjectList
        event->accept();
    else
        event->ignore();
}

void LevelView::dragLeaveEvent(QDragLeaveEvent *event)
{
    QRect updateRect = highlightedRect_;
    highlightedRect_ = QRect();
    update(updateRect);
    event->accept();
}

void LevelView::dragMoveEvent(QDragMoveEvent *event)
{
    QRect updateRect = highlightedRect_.united(targetPlace(event->pos()));

    if (event->mimeData()->hasFormat(ObjectsList::mimeType())
        && findObject(targetPlace(event->pos())) == -1) {

        highlightedRect_ = targetPlace(event->pos());
        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else {
        highlightedRect_ = QRect();
        event->ignore();
    }

    update(updateRect);
}

void LevelView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(ObjectsList::mimeType())
        && findObject(targetPlace(event->pos())) == -1)
    {

        QByteArray pieceData = event->mimeData()->data(ObjectsList::mimeType());
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        Object object;
        object.rect = targetPlace(event->pos());
        dataStream >> object.pixmap >> object.position;

        objects_.push_back(object);

        highlightedRect_ = QRect();
        update(object.rect);

        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
    else
    {
        highlightedRect_ = QRect();
        event->ignore();
    }
}

int LevelView::findObject(const QRect &rect) const
{
    for (int i = 0, size = objects_.size(); i < size; ++i)
    {
        if (objects_[i].rect == rect)
            return i;
    }
    return -1;
}

const QRect LevelView::targetPlace(const QPoint &position) const
{
    return QRect(position/objectSize_*objectSize_, QSize(objectSize_, objectSize_));
}

void LevelView::mousePressEvent(QMouseEvent *event)
{
    QRect square = targetPlace(event->pos());
    const int found = findObject(square);

    if (found == -1)
        return;

    auto object = objects_.at(found);

    update(square);

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << object.pixmap << object.position;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(ObjectsList::mimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos() - square.topLeft());
    drag->setPixmap(object.pixmap);

    if (drag->exec(Qt::MoveAction) != Qt::MoveAction) {
        objects_.insert(objects_.begin() + found, object);
        update(targetPlace(event->pos()));
    }
}

void LevelView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), Qt::white);

    if (highlightedRect_.isValid()) {
        painter.setBrush(QColor("#ffcccc"));
        painter.setPen(Qt::NoPen);
        painter.drawRect(highlightedRect_.adjusted(0, 0, -1, -1));
    }

    for (const auto &object : objects_)
        painter.drawPixmap(object.rect, object.pixmap);
}
