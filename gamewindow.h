#pragma once
#include "chesspieceitem.h"
#include <QWidget>
#include <QWebSocket>
#include <QProcess>
#include <QTextStream>

class QLabel;
class QGraphicsView;
class QGraphicsScene;

class GameWindow : public QWidget {
    Q_OBJECT

public:
    bool myTurn;
    QString myColor;
    QString currentTurn;
    bool isValidlocalMovetesting(ChessPieceItem* piece, int targetRow, int targetCol);
    //void playBotTurn(ChessPieceItem* piece, int targetRow, int targetCol);
    explicit GameWindow(const QString& username, bool withBot, QWidget *parent = nullptr);
    void onPieceClicked(ChessPieceItem* piece);
    void onSquareClicked(int row, int col);
    void applyMoveOnBoard(ChessPieceItem* piece, int row, int col);
    bool withBotMode;

    ~GameWindow();

    void updateMoveHistory(const QString &move);
private slots:
    void onConnected();
    void onMessageReceived(const QString &message);

private:
    ChessPieceItem* pieceMap[8][8] = {{nullptr}};
    void connectToServer();
    void setupBoard();
    void setupPieces();
    QString getToken();
    QString getUsername();
    void movePieceOnBoard(const QString& move);
    void syncBoardWithFen(const QString& fen);

    // Bot mode helpers
    void setupLocalBot();
    void sendMoveToValidator(const QString& move);
    bool isValidLocalMove(const QString& move);
    void updateFenWithMove(const QString& move);
    QString getLocalBotMove();
    void playBotTurn(const QString& playerMove);
    bool isValidOfflineMove(const QString& uciMove);
    QVector<QString> moveHistory;
    QProcess* validatorProcess;



    QString currentFen = "startpos";

    QString currentUsername;
    QWebSocket *socket = nullptr;
    QGraphicsView *boardView;
    QGraphicsScene *scene;
    QLabel *statusLabel;

    QProcess *pythonProcess = nullptr;
    QTextStream *pythonWriter = nullptr;
    QTextStream *pythonReader = nullptr;

    QProcess *stockfishProcess = nullptr;
    QTextStream *stockfishWriter = nullptr;
    QTextStream *stockfishReader = nullptr;

    ChessPieceItem* selectedPiece = nullptr;

    void setupBot();
    void sendMoveToBot(const QString &move);
};
