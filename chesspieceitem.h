#pragma once

#include <QGraphicsPixmapItem>

class GameWindow;

class ChessPieceItem : public QGraphicsPixmapItem {
public:
    ChessPieceItem(const QPixmap& pixmap, int row, int col);
    ChessPieceItem(const QPixmap& pixmap, int row, int col, GameWindow* parentWindow);
    ChessPieceItem(const QPixmap& pixmap, int row, int col, QString myColor, GameWindow* parentWindow);
    QString pieceType;

    QString pieceClass() const { return pieceType; }
    void setPieceClass(const QString& t) { pieceType = t; }

    int row() const;
    int col() const;
    QString myColor() const;
    void setBoardPos(int newRow, int newCol);
    QString my_Color;
    void setColor(QString newColor);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;


private:
    int m_row;
    int m_col;


    GameWindow* gameWindow;
};
