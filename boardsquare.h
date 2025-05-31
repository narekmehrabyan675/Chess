#pragma once
#include <QGraphicsRectItem>

class GameWindow;

class BoardSquare : public QGraphicsRectItem {
public:
    BoardSquare(int row, int col, GameWindow* parentWindow);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    int m_row;
    int m_col;
    GameWindow* gameWindow;
};
