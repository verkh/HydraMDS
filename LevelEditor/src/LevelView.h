#ifndef LEVELVIEW_H
#define LEVELVIEW_H

#include <QGraphicsView>

class LevelView : public QWidget
{
    Q_OBJECT
public:
    LevelView(int numOfVetiocalCells, int numOfHorizontalCells, QWidget* parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    int findObject(const QRect &rect) const;
    const QRect targetPlace(const QPoint &position) const;

    struct Object
    {
        QPixmap pixmap;
        QRect rect;
        QPoint position;
    };

    std::vector<Object> objects_;

    int objectSize_ = 30;
    int height_ = 0;
    int width_  = 0;

    QRect highlightedRect_;
};

#endif // LEVELVIEW_H
