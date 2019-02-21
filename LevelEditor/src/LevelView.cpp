#include "LevelView.h"

#include "ObjectsList.h"

#include <QDrag>
#include <QDebug>
#include <QPainter>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QGraphicsPixmapItem>

#include <cmath>

namespace Constants
{

const int notFound          = -1;
const double noOpacity      = 1.;
const double defaultOpacity = 0.5;

}

LevelView::LevelView(int numOfVetiocalCells, int numOfHorizontalCells, QWidget* parent)
    :QGraphicsView(parent)
    ,objectSize_(30) // FIXME not magic const
    ,height_(numOfVetiocalCells*  objectSize_)
    ,width_(numOfHorizontalCells* objectSize_)
{
    setAcceptDrops(true);
    setScene(new QGraphicsScene(0,0, width_, height_));

    setMaximumSize(sizeHint());
    setGrid();
}

void LevelView::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasFormat(Object::mimeType()))
    {
        dragObject_ = std::make_unique<Object>(event->mimeData());
        if(!highlight_)
        {
            highlight_ = std::make_unique<QGraphicsPixmapItem>(dragObject_->getPixmap());
            highlight_->setOpacity(Constants::defaultOpacity);
            highlight_->setPos(targetPlace(mapToScene(event->pos())));
            addNewObject(highlight_.get());
        }
        event->accept();
    }
    else
        event->ignore();
}

void LevelView::dragLeaveEvent(QDragLeaveEvent* event)
{
    highlight_.reset();
    event->accept();
}

void LevelView::dragMoveEvent(QDragMoveEvent* event)
{
    if(event->mimeData()->hasFormat(Object::mimeType()) && findObject(targetPlace(mapToScene(event->pos()))) == Constants::notFound)
    {
        qDebug() << !highlight_;
        highlight_->setPos(targetPlace(mapToScene(event->pos())));
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

void LevelView::dropEvent(QDropEvent* event)
{
    if(event->mimeData()->hasFormat(Object::mimeType()))
    {
        dragObject_->setPosition(targetPlace(mapToScene(event->pos())));
        //objects_.push_back(dragObject_);

        highlight_->setOpacity(Constants::noOpacity);

        highlight_.release(); // now scene will handle this item

        event->acceptProposedAction();
    }
    else
    {
        highlight_.reset();
        event->ignore();
    }
}

int LevelView::findObject(const QPoint& position) const
{
    for(int i = 0, size = objects_.size(); i < size; ++i)
    {
        if(objects_[i].getPosition() == position)
            return i;
    }
    return Constants::notFound;
}

const QPoint LevelView::targetPlace(const QPoint& position) const
{
    const auto x = position.x()/objectSize_;
    const auto y = position.y()/objectSize_;
    return QPoint(x, y)*objectSize_;
}

void LevelView::mousePressEvent(QMouseEvent* event)
{
    if(scene()->selectedItems().size() > 1) return;

    const auto position = targetPlace(mapToScene(event->pos()));
    const int found = findObject(position);

    if(found == Constants::notFound)
        return;

    const auto item = itemAt(position);

    Q_ASSERT(item);
    if(!item) return;

    const auto& object = objects_.at(found);

    highlight_.reset(item);
    highlight_->setOpacity(Constants::defaultOpacity);

    const auto drag = new QDrag(this);
    drag->setMimeData(object.toMimeData());
    drag->setPixmap(object.getPixmap());

    drag->exec(Qt::MoveAction);
}

void LevelView::addNewObject(const Object &object, const QPoint& position)
{
    auto item = addNewObject(new QGraphicsPixmapItem(object.getPixmap()));
    item->setPos(position);
}

QGraphicsPixmapItem* LevelView::addNewObject(QGraphicsPixmapItem* item)
{
    scene()->addItem(item);
    item->setFlags(QGraphicsPixmapItem::ItemIsSelectable);
    item->show();
    return item;
}

void LevelView::setGrid()
{
    const auto addLine = [this] (const QLineF& line)
    {
        const auto item = scene()->addLine(line, QPen(Qt::blue));
        item->setOpacity(Constants::defaultOpacity);
        item->setFlags(QGraphicsItem::ItemNegativeZStacksBehindParent);
    };

    for (int x = 0; x <= width_; x+=objectSize_)
        addLine({x,0,x,height_});

    for (int y = 0; y <= height_; y+=objectSize_)
        addLine({0,y,width_,y});
}

QGraphicsPixmapItem* LevelView::itemAt(const QPoint& position) const
{
    const auto centerPosition = QRectF(position, QSize(objectSize_, objectSize_)).center();
    const auto item = dynamic_cast<QGraphicsPixmapItem*>(scene()->itemAt(centerPosition, transform()));
    return item;
}

QPoint LevelView::mapToScene(const QPoint& position) const
{
    return QGraphicsView::mapToScene(position).toPoint();
}

