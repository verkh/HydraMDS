#include "LevelView.h"

#include "ObjectsList.h"

#include <QDrag>
#include <QDebug>
#include <QPainter>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QGraphicsPixmapItem>

#include <cmath>

LevelView::LevelView(int numOfVetiocalCells, int numOfHorizontalCells, QWidget* parent)
    :QGraphicsView(parent)
    ,objectSize_(30) // FIXME not magic const
    ,height_(numOfVetiocalCells*  objectSize_)
    ,width_(numOfHorizontalCells* objectSize_)
{
    setAcceptDrops(true);
    setScene(new QGraphicsScene(0,0, width_, height_));

    ensureVisible(QRect());
    setMaximumSize({width_, height_});
    setGrid();
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

        addNewObject(dragObject_, highlightedRect_.topLeft());

        highlightedRect_ = QRect();

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
    auto mapedToScepePoint = mapToScene(position);
    const auto x = mapedToScepePoint.x()/objectSize_;
    const auto y = mapedToScepePoint.y()/objectSize_;
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

void LevelView::spaintEvent(QPaintEvent* event)
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

void LevelView::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
        zoomIn(6);
    else
        zoomOut(6);
    event->accept();
}

void LevelView::zoomIn(int level)
{
    setupZoom(lastMatrix + level);
}

void LevelView::zoomOut(int level)
{
    setupZoom(lastMatrix - level);
}

void LevelView::setupZoom(int value)
{
    auto scale = std::pow(2., (value - 250.) / 50.);
    lastMatrix = value;
    QMatrix matrix;
    matrix.scale(scale, scale);

    setMatrix(matrix);
}

int LevelView::getLastMatrix() const
{
    return lastMatrix;
}

void LevelView::addNewObject(const Object &object, const QPoint& position)
{
    auto newItem = new QGraphicsPixmapItem(object.getPixmap());
    scene()->addItem(newItem);
    newItem->setPos(mapToScene(position));
    newItem->show();
}

void LevelView::setGrid()
{
    for (int x = 0; x <= width_; x+=objectSize_)
    {
        QGraphicsLineItem* item = scene()->addLine(x,0,x,height_, QPen(Qt::blue));
        item->setOpacity(0.25);
        item->setFlags(QGraphicsItem::ItemNegativeZStacksBehindParent);
    }

    for (int y = 0; y <= height_; y+=objectSize_)
    {
        QGraphicsLineItem* item =  scene()->addLine(0,y,width_,y, QPen(Qt::blue));
        item->setOpacity(0.25);
        item->setFlags(QGraphicsItem::ItemNegativeZStacksBehindParent);
    }
}

