#include "ObjectsList.h"

#include <QDrag>
#include <QMimeData>
ObjectsList::ObjectsList(QWidget *parent)
    :QListWidget(parent)
{
    //TMP
    auto item = new QListWidgetItem(QIcon::fromTheme("edit-undo"), "icon");
    this->addItem(item);

    setDragEnabled(true);
    setDefaultDropAction(Qt::CopyAction);
}

ObjectsList::~ObjectsList()
{

}

void ObjectsList::startDrag(Qt::DropActions)
{
    QListWidgetItem *item = currentItem();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    QPixmap pixmap = item->icon().pixmap(30,30);

    dataStream << pixmap << item->text();

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(mimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    drag->setPixmap(pixmap);

    drag->exec(Qt::CopyAction);
}
