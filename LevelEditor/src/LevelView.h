#ifndef LEVELVIEW_H
#define LEVELVIEW_H

#include "Object.h"

#include <QGraphicsView>

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
    void spaintEvent(QPaintEvent* event);

    void wheelEvent(QWheelEvent *event);

    void zoomIn(int level = 1);
    void zoomOut(int level = 1);
    void setupZoom(int value);

    int getLastMatrix() const;

    void addNewObject(const Object& object);

    void setGrid();

private:
    int findObject(const QRect& rect) const;
    const QRect targetPlace(const QPoint& position) const;

    std::vector<Object> objects_;

    int objectSize_ = 30;
    int height_ = 0;
    int width_  = 0;

    QRect highlightedRect_;
    Object dragObject_;

    int lastMatrix;
};

#endif // LEVELVIEW_H
