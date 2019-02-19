#ifndef LEVELVIEW_H
#define LEVELVIEW_H

#include <QGraphicsView>

class LevelView : public QGraphicsView
{
public:
    LevelView(int height, int width, QWidget* parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    int findObject(const QRect &pieceRect) const;
    const QRect targetPlace(const QPoint &position) const;

    struct Object
    {
        QPixmap pixmap;
        QRect rect;
        QPoint position;
    };

    std::vector<Object> objects_;

    int height_ = 0;
    int width_  = 0;
};

#endif // LEVELVIEW_H
