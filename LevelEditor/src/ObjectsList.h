#ifndef OBJECTSLIST_H
#define OBJECTSLIST_H

#include "Object.h"

#include <QListWidget>

class ObjectsList : public QListWidget
{
    Q_OBJECT

public:
    explicit ObjectsList(QWidget *parent = nullptr);
    ~ObjectsList();

protected:
    void startDrag(Qt::DropActions) override;

    std::map<QListWidgetItem*, Object> objects_;
};

#endif // OBJECTSLIST_H
