#ifndef LEVELVIEW_H
#define LEVELVIEW_H

#include "Object.h"

#include <memory>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

class QWheelEvent;
class QKeyEvent;
class LevelView : public QGraphicsView
{
    Q_OBJECT
public:
    LevelView(int numOfVetiocalCells, int numOfHorizontalCells, QWidget* parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

    void addNewObject(const Object& object, const QPoint &position);
    QGraphicsPixmapItem* addNewObject(QGraphicsPixmapItem* item);

    void setGrid();

    QGraphicsPixmapItem* itemAt(const QPoint& position) const;

    QPoint mapToScene(const QPoint& position) const;
private:
    int findObject(const QPoint& position) const;
    const QPoint targetPlace(const QPoint& position) const;

    std::vector<Object> objects_;

    int objectSize_ = 30;
    int height_ = 0;
    int width_  = 0;

    std::unique_ptr<QGraphicsPixmapItem> highlight_;
    std::unique_ptr<Object> dragObject_;
};

#endif // LEVELVIEW_H
