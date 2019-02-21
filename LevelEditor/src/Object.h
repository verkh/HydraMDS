/* ---------------------------------------------------------------------------
 * Авторское право © ОАО «Концерн «МАНС» 2010-2018. Авторские права защищены.
 * Copyright. JSC «Concern «IANS» 2010-2018. All rights reserved
 * Компания: МАНС
 * Подразделение: СС УВД
  * Автор: Денис Верховский
 * ---------------------------------------------------------------------------
 */
#ifndef OBJECT_H
#define OBJECT_H

#include <QRect>
#include <QPoint>
#include <QPixmap>
#include <QMimeData>

class Object
{
public:
    Object() = default;
    Object(const QPixmap& pixmap, const QRect& rect, const QPoint& position);
    Object(const QMimeData*);

    static QString mimeType();

    QMimeData* toMimeData() const;

    static Object from(const QMimeData* mimeData);

    QPixmap getPixmap() const;
    void setPixmap(const QPixmap& value);

    QRect getRect() const;
    void setRect(const QRect& value);

    QPoint getPosition() const;
    void setPosition(const QPoint& value);

private:
    QPixmap pixmap;
    QRect rect;
    QPoint position;
};

#endif // OBJECT_H
