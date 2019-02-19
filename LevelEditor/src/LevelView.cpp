#include "LevelView.h"

#include "ObjectsList.h"

#include <QDrag>
#include <QMimeData>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
LevelView::LevelView(int height, int width, QWidget *parent)
    :QGraphicsView(parent)
    ,height_(height)
    ,width_(width)
{

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
        event->setDropAction(Qt::MoveAction);
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
        Object piece;
        piece.rect = targetPlace(event->pos());
        dataStream >> piece.pixmap >> piece.position;

        objects_.push_back(piece);

        highlightedRect_ = QRect();
        update(piece.rect);

        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        highlightedRect_ = QRect();
        event->ignore();
    }
}

int LevelView::findObject(const QRect &pieceRect) const
{
    for (int i = 0, size = objects_.size(); i < size; ++i)
    {
        if (objects_[i].rect == pieceRect)
            return i;
    }
    return -1;
}

const QRect LevelView::targetPlace(const QPoint &position) const
{
    return QRect(position / width_ * width_, QSize(15, 15));
}
