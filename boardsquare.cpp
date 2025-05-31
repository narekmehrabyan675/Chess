#include "boardsquare.h"
#include "gamewindow.h"
#include <QGraphicsSceneMouseEvent>

BoardSquare::BoardSquare(int row, int col, GameWindow* parentWindow)
    : QGraphicsRectItem(col * 60, (7 - row) * 60, 60, 60), m_row(row), m_col(col), gameWindow(parentWindow)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setZValue(-1);
}

void BoardSquare::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (gameWindow) {
        gameWindow->onSquareClicked(m_row, m_col);
    }
}
