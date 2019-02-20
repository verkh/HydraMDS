#include "LevelView.h"

#include "ObjectsList.h"

#include <QDrag>
#include <QDebug>
#include <QPainter>
#include <QMimeData>
#include <QDragEnterEvent>

LevelView::LevelView(int numOfVetiocalCells, int numOfHorizontalCells, QWidget* parent)
    :QWidget(parent)
    ,objectSize_(30) // FIXME not magic const
    ,height_(numOfVetiocalCells*  objectSize_)
    ,width_(numOfHorizontalCells* objectSize_)
{
    setAcceptDrops(true);

    setMinimumSize({height_, width_});
}

void LevelView::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasFormat(Object::mimeType()))
        event->accept();
    else
        event->ignore();
}

void LevelView::dragLeaveEvent(QDragLeaveEvent* event)
{
    QRect updateRect = highlightedRect_;
    highlightedRect_ = QRect();
    update(updateRect);
    event->accept();
}

void LevelView::dragMoveEvent(QDragMoveEvent* event)
{
    QRect updateRect = highlightedRect_.united(targetPlace(event->pos()));

    if(event->mimeData()->hasFormat(Object::mimeType())
        && findObject(targetPlace(event->pos())) == -1) {

        dragObject_ = Object::from(event->mimeData());

        highlightedRect_ = targetPlace(event->pos());
        event->acceptProposedAction();
    } else {
        highlightedRect_ = QRect();
        event->ignore();
    }

    update(updateRect);
}

void LevelView::dropEvent(QDropEvent* event)
{
    if(event->mimeData()->hasFormat(Object::mimeType())
        && findObject(targetPlace(event->pos())) == -1)
    {
        dragObject_.setRect(targetPlace(event->pos()));
        objects_.push_back(dragObject_);

        highlightedRect_ = QRect();
        update(dragObject_.getRect());

        event->acceptProposedAction();
    }
    else
    {
        highlightedRect_ = QRect();
        event->ignore();
    }
}

int LevelView::findObject(const QRect& rect) const
{
    for(int i = 0, size = objects_.size(); i < size; ++i)
    {
        if(objects_[i].getRect() == rect)
            return i;
    }
    return -1;
}

const QRect LevelView::targetPlace(const QPoint& position) const
{
    const auto x = position.x()/objectSize_;
    const auto y = position.y()/objectSize_;
    return QRect(QPoint(x, y)*objectSize_, QSize(objectSize_, objectSize_));
}

void LevelView::mousePressEvent(QMouseEvent* event)
{
    const QRect square = targetPlace(event->pos());
    const int found = findObject(square);

    if(found == -1)
        return;

    const auto& object = objects_.at(found);

    update(square);

    const auto drag = new QDrag(this);
    drag->setMimeData(object.toMimeData());
    drag->setPixmap(object.getPixmap());

    auto e = drag->exec(Qt::MoveAction);
    if(e != Qt::MoveAction)
    {
        objects_.insert(objects_.begin() + found, object);
        update(targetPlace(event->pos()));
    }
}

void LevelView::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), Qt::white);

    if(highlightedRect_.isValid())
    {
        QColor color(Qt::yellow);
        color.setAlpha(100);
        painter.setBrush(color);
        painter.setPen(Qt::NoPen);
        painter.drawRect(highlightedRect_);
        painter.drawPixmap(highlightedRect_, dragObject_.getPixmap());
    }

    for(const auto& object : objects_)
        painter.drawPixmap(object.getRect(), object.getPixmap());
}
