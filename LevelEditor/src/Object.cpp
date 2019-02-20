#include "Object.h"
#include "Settings.h"

Object::Object(const QPixmap& pixmap, const QRect& rect, const QPoint& position)
    :pixmap(pixmap)
    ,rect(rect)
    ,position(position)
{

}

QString Object::mimeType()
{
    return "tile.object";
}

Object Object::from(const QMimeData* mimeData)
{
    Object result;

    QByteArray data = mimeData->data(mimeType());

    QDataStream dataStream(&data, QIODevice::ReadOnly);
    dataStream >> result.pixmap >> result.position;

    return result;
}

QMimeData* Object::toMimeData() const
{
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    QPixmap scaledPixmap = pixmap.scaled(Settings::instance().pixmapSize,
                                         Settings::instance().pixmapSize);

    dataStream << scaledPixmap << position;

    QMimeData* mimeData = new QMimeData;
    mimeData->setData(mimeType(), itemData);

    return mimeData;
}

QPixmap Object::getPixmap() const
{
    return pixmap;
}

void Object::setPixmap(const QPixmap& value)
{
    pixmap = value;
}

QRect Object::getRect() const
{
    return rect;
}

void Object::setRect(const QRect& value)
{
    rect = value;
}

QPoint Object::getPosition() const
{
    return position;
}

void Object::setPosition(const QPoint& value)
{
    position = value;
}
