#include "ObjectsList.h"

#include "Object.h"

#include <QDrag>
#include <QMimeData>
ObjectsList::ObjectsList(QWidget *parent)
    :QListWidget(parent)
{

    objects_ = //TMP
    {
        { new QListWidgetItem(QIcon("images/player.png"), "Player"), Object(QPixmap("images/player.png"), QRect(), QPoint()) },
        { new QListWidgetItem(QIcon("images/ground.png"), "Ground"), Object(QPixmap("images/ground.png"), QRect(), QPoint()) },
        { new QListWidgetItem(QIcon("images/ladder.png"), "Ladder"), Object(QPixmap("images/ladder.png"), QRect(), QPoint()) },
    };

    for(const auto& pair : objects_)
        addItem(pair.first);

    setDragEnabled(true);
}

ObjectsList::~ObjectsList()
{

}

void ObjectsList::startDrag(Qt::DropActions)
{
    QListWidgetItem *item = currentItem();

    auto object = objects_.at(item);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(object.toMimeData());
    drag->setPixmap(object.getPixmap());

    drag->exec(Qt::CopyAction);
}
