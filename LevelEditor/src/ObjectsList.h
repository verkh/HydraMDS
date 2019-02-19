/* ---------------------------------------------------------------------------
 * Авторское право © ОАО «Концерн «МАНС» 2010-2018. Авторские права защищены.
 * Copyright. JSC «Concern «IANS» 2010-2018. All rights reserved
 * Компания: МАНС
 * Подразделение: СС УВД
  * Автор: Денис Верховский
 * ---------------------------------------------------------------------------
 */
#ifndef OBJECTSLIST_H
#define OBJECTSLIST_H

#include <QListWidget>

class ObjectsList : public QListWidget
{
    Q_OBJECT

public:
    explicit ObjectsList(QWidget *parent = nullptr);
    ~ObjectsList();

    static QString mimeType() { return "level.object"; }

protected:
    void startDrag(Qt::DropActions) override;

};

#endif // OBJECTSLIST_H
