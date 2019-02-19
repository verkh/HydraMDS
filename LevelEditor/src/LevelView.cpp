#include "LevelView.h"

LevelView::LevelView(int height, int width, QWidget *parent)
    :QGraphicsView(parent)
    ,height_(height)
    ,width_(width)
{

}

void LevelView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(PiecesList::puzzleMimeType()))
        event->accept();
    else
        event->ignore();
}

void LevelView::dragLeaveEvent(QDragLeaveEvent *event)
{
    QRect updateRect = highlightedRect;
    highlightedRect = QRect();
    update(updateRect);
    event->accept();
}

void LevelView::dragMoveEvent(QDragMoveEvent *event)
{
    QRect updateRect = highlightedRect.united(targetSquare(event->pos()));

    if (event->mimeData()->hasFormat(PiecesList::puzzleMimeType())
        && findPiece(targetSquare(event->pos())) == -1) {

        highlightedRect = targetSquare(event->pos());
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        highlightedRect = QRect();
        event->ignore();
    }

    update(updateRect);
}

void LevelView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(Level::puzzleMimeType())
        && findPiece(targetSquare(event->pos())) == -1)
    {

        QByteArray pieceData = event->mimeData()->data(PiecesList::puzzleMimeType());
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        Piece piece;
        piece.rect = targetSquare(event->pos());
        dataStream >> piece.pixmap >> piece.location;

        pieces.append(piece);

        highlightedRect = QRect();
        update(piece.rect);

        event->setDropAction(Qt::MoveAction);
        event->accept();

        if (piece.location == piece.rect.topLeft() / pieceSize())
        {
            inPlace++;
        }
    }
    else
    {
        highlightedRect = QRect();
        event->ignore();
    }
}

int LevelView::findPiece(const QRect &pieceRect) const
{
    for (int i = 0, size = pieces.size(); i < size; ++i)
    {
        if (pieces.at(i).rect == pieceRect)
            return i;
    }
    return -1;
}

QRect LevelView::targetPlace(const QPoint &position) const
{
    return QRect(position / pieceSize() * pieceSize(), QSize(pieceSize(), pieceSize()));
}

int LevelView::pieceSize() const
{
    return 15;
}

int LevelView::imageSize() const
{
    return 15*30;
}
