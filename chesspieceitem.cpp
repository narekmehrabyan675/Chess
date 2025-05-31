#include "chesspieceitem.h"
#include "gamewindow.h"
#include <QGraphicsSceneMouseEvent>

ChessPieceItem::ChessPieceItem(const QPixmap& pixmap, int row, int col, GameWindow* parentWindow)
    : QGraphicsPixmapItem(pixmap), m_row(row), m_col(col), gameWindow(parentWindow)
{
    setBoardPos(row, col);
    setScale(0.46);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    my_Color = "white";
}
//Real constructor
ChessPieceItem::ChessPieceItem(const QPixmap& pixmap, int row, int col, QString myColor ,GameWindow* parentWindow)
    : QGraphicsPixmapItem(pixmap), m_row(row), m_col(col), my_Color(myColor), gameWindow(parentWindow)
{
    setBoardPos(row, col);
    setScale(0.46);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setColor(myColor);
}

void ChessPieceItem::setColor(QString newColor){
    my_Color = newColor;
}

void ChessPieceItem::setBoardPos(int newRow, int newCol) {
    m_row = newRow;
    m_col = newCol;

    int cellSize = 60;
    int yRow = 7 - newRow;
    setPos(newCol * cellSize, yRow * cellSize);
}


void ChessPieceItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (gameWindow) {
        gameWindow->onPieceClicked(this);
    }
}

int ChessPieceItem::col() const {
    return m_col;
}
int ChessPieceItem::row() const {
    return m_row;
}
QString ChessPieceItem::myColor() const {
    return my_Color;
}

