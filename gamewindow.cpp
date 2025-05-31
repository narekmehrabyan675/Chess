#include "gamewindow.h"
#include "boardsquare.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QDebug>
#include <QCoreApplication>
#include <qmessagebox.h>

GameWindow::GameWindow(const QString& username, bool withBot, QWidget *parent)
    : QWidget(parent), currentUsername(username), withBotMode(withBot){
    setWindowTitle("Chess Game");

    QVBoxLayout *layout = new QVBoxLayout(this);
    statusLabel = new QLabel("Connecting to game server...", this);

    boardView = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    boardView->setScene(scene);
    boardView->setFixedSize(500, 500);

    layout->addWidget(statusLabel);
    layout->addWidget(boardView);



    setupBoard();
    setupPieces();
    if (withBotMode) {
        setupBot();
    } else {
        connectToServer();
    }


     myTurn = true;
}

GameWindow::~GameWindow() {
}

#include "boardsquare.h"

void GameWindow::setupBoard() {
    int cellSize = 60;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QColor color = (row + col) % 2 == 0 ? QColor("#F0D9B5") : QColor("#B58863");

            // 👇 создаём клетку, которая знает свою позицию и реагирует на клик
            BoardSquare* square = new BoardSquare(row, col, this);
            square->setBrush(color);
            square->setPen(QPen(Qt::black));
            scene->addItem(square);
        }
    }
}


void GameWindow::setupPieces() {
    //QString basePath = "C:/Projects/Qt0/pieces/";
    QString basePath = QCoreApplication::applicationDirPath() + "/pieces/";



    QPixmap w_pawn(basePath + "white-pawn.png");
    QPixmap w_rook(basePath + "white-rook.png");
    QPixmap w_knight(basePath + "white-knight.png");
    QPixmap w_bishop(basePath + "white-bishop.png");
    QPixmap w_queen(basePath + "white-queen.png");
    QPixmap w_king(basePath + "white-king.png");

    QPixmap b_pawn(basePath + "black-pawn.png");
    QPixmap b_rook(basePath + "black-rook.png");
    QPixmap b_knight(basePath + "black-knight.png");
    QPixmap b_bishop(basePath + "black-bishop.png");
    QPixmap b_queen(basePath + "black-queen.png");
    QPixmap b_king(basePath + "black-king.png");

    // Pawns
    for (int col = 0; col < 8; ++col) {
        ChessPieceItem* wp = new ChessPieceItem(w_pawn, 6, col , "white", this);
        wp->setPieceClass("pawn");
        scene->addItem(wp);
        pieceMap[6][col] = wp;

        ChessPieceItem* bp = new ChessPieceItem(b_pawn, 1, col, "black" , this);
        bp->setPieceClass("pawn");
        scene->addItem(bp);
        pieceMap[1][col] = bp;
    }

    // White
    scene->addItem(pieceMap[7][0] = new ChessPieceItem(w_rook, 7, 0,"white", this));
    pieceMap[7][0]->setPieceClass("rook");
    scene->addItem(pieceMap[7][1] = new ChessPieceItem(w_knight, 7, 1,"white", this));
    pieceMap[7][1]->setPieceClass("knight");
    scene->addItem(pieceMap[7][2] = new ChessPieceItem(w_bishop, 7, 2,"white", this));
    pieceMap[7][2]->setPieceClass("bishop");
    scene->addItem(pieceMap[7][3] = new ChessPieceItem(w_queen, 7, 3,"white", this));
    pieceMap[7][3]->setPieceClass("queen");
    scene->addItem(pieceMap[7][4] = new ChessPieceItem(w_king, 7, 4,"white", this));
    pieceMap[7][4]->setPieceClass("king");
    scene->addItem(pieceMap[7][5] = new ChessPieceItem(w_bishop, 7, 5,"white", this));
    pieceMap[7][5]->setPieceClass("bishop");
    scene->addItem(pieceMap[7][6] = new ChessPieceItem(w_knight, 7, 6,"white", this));
    pieceMap[7][6]->setPieceClass("knight");
    scene->addItem(pieceMap[7][7] = new ChessPieceItem(w_rook, 7, 7,"white", this));
    pieceMap[7][7]->setPieceClass("rook");

    // Black
    scene->addItem(pieceMap[0][0] = new ChessPieceItem(b_rook, 0, 0,"black" , this));
    pieceMap[0][0]->setPieceClass("rook");
    scene->addItem(pieceMap[0][1] = new ChessPieceItem(b_knight, 0, 1,"black" , this));
    pieceMap[0][1]->setPieceClass("knight");
    scene->addItem(pieceMap[0][2] = new ChessPieceItem(b_bishop, 0, 2,"black" , this));
    pieceMap[0][2]->setPieceClass("bishop");
    scene->addItem(pieceMap[0][3] = new ChessPieceItem(b_queen, 0, 3,"black" , this));
    pieceMap[0][3]->setPieceClass("queen");
    scene->addItem(pieceMap[0][4] = new ChessPieceItem(b_king, 0, 4,"black" , this));
    pieceMap[0][4]->setPieceClass("king");
    scene->addItem(pieceMap[0][5] = new ChessPieceItem(b_bishop, 0, 5,"black" , this));
    pieceMap[0][5]->setPieceClass("bishop");
    scene->addItem(pieceMap[0][6] = new ChessPieceItem(b_knight, 0, 6,"black" , this));
    pieceMap[0][6]->setPieceClass("knight");
    scene->addItem(pieceMap[0][7] = new ChessPieceItem(b_rook, 0, 7,"black" , this));
    pieceMap[0][7]->setPieceClass("rook");
}




void GameWindow::connectToServer() {
    socket = new QWebSocket();
    connect(socket, &QWebSocket::connected, this, &GameWindow::onConnected);
    connect(socket, &QWebSocket::textMessageReceived, this, &GameWindow::onMessageReceived);

    QUrl url("ws://localhost:8080/ws/game");
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + getToken().toUtf8());
    socket->open(request);
}

QString GameWindow::getToken() {
    QSettings settings(QString("./%1_settings.ini").arg(currentUsername), QSettings::IniFormat);
    return settings.value("auth_token").toString();
}

QString GameWindow::getUsername() {
    return currentUsername;

}

void GameWindow::onConnected() {
    statusLabel->setText("Connected! Waiting for opponent...");

    QJsonObject joinJson;
    joinJson["type"] = "join";
    joinJson["room"] = withBotMode ? QString("bot_%1").arg(getUsername()) : "room1";
    joinJson["username"] = getUsername();

    socket->sendTextMessage(QJsonDocument(joinJson).toJson());
    qDebug() << " WebSocket connected!";
    qDebug() << " Username sent:" << getUsername();
    connect(socket, &QWebSocket::textMessageReceived, this, &GameWindow::onMessageReceived);
    qDebug() << " Connected textMessageReceived signal";
}






void GameWindow::onPieceClicked(ChessPieceItem* piece) {
    qDebug() << myColor;
    if ((myColor == "white" && piece->my_Color == "black") ||
        (myColor == "black" && piece->my_Color == "white")) {
        statusLabel->setText("You cannot move the opponent's pieces!");
        return;  // Do not allow interaction with the opponent's pieces
    }

    if (!selectedPiece) {
        selectedPiece = piece;
        piece->setOpacity(0.6);
        return;
    }

    if (piece == selectedPiece) {
        selectedPiece->setOpacity(1.0);
        selectedPiece = nullptr;
        return;
    }

    // If dont your turn can not play
    if (!myTurn) {
        statusLabel->setText("Not your turn!");
        selectedPiece->setOpacity(1.0);
        selectedPiece = nullptr;
        return;
    }

    // Collect to UCI
    int fromRow = selectedPiece->row();
    int fromCol = selectedPiece->col();
    int toRow   = piece->row();
    int toCol   = piece->col();
    QString from = QString("%1%2").arg(QChar('a' + fromCol)).arg(8 - fromRow);
    QString to   = QString("%1%2").arg(QChar('a' + toCol)).arg(8 - toRow);
    QString move = from + to;
    if (selectedPiece->pieceClass() == "pawn" && (toRow==0 || toRow==7))
        move += 'q';

    selectedPiece->setOpacity(1.0);
    selectedPiece = nullptr;

    if (withBotMode) {
        if (isValidOfflineMove(move)) {
            movePieceOnBoard(move);
            myTurn = false;
            sendMoveToBot(move);   // your Python bot driver
        } else {
            statusLabel->setText("Invalid move!");
            //myTurn = true;
        }
        return;
    }
     else {  // For online mode
        // Send move to server for validation
        QJsonObject msg;
        msg["type"] = "move";
        msg["move"] = move;
        socket->sendTextMessage(QJsonDocument(msg).toJson());
        myTurn = true;
        statusLabel->setText("Waiting for opponent...");
    }
}

void GameWindow::onSquareClicked(int row, int col) {
    if (!myTurn) {
        statusLabel->setText("Not your turn!");
        return;
    }

    if (!selectedPiece) return;

    int fromRow = selectedPiece->row();
    int fromCol = selectedPiece->col();

    QString from = QString("%1%2").arg(QChar('a' + fromCol)).arg(8 - fromRow);
    QString to   = QString("%1%2").arg(QChar('a' + col)).arg(8 - row);
    QString move = from + to;

    // Handle pawn promotion
    if (selectedPiece->pieceClass() == "pawn" && (row == 0 || row == 7)) {
        move += "q";
    }

    selectedPiece->setOpacity(1.0);
    selectedPiece = nullptr;

    if (withBotMode) {
        if (isValidOfflineMove(move)) {
            movePieceOnBoard(move);
            sendMoveToBot(move);   // your Python bot driver
        } else {
            statusLabel->setText("Invalid move!");
            //myTurn = true;
        }
        return;
    } else {  // Online mode
        // Send the move to the server for online play
        QJsonObject moveJson;
        moveJson["type"] = "move";
        moveJson["move"] = move;
        socket->sendTextMessage(QJsonDocument(moveJson).toJson());

        myTurn = true;
        statusLabel->setText("Waiting for opponent...");
    }
}


bool GameWindow::isValidOfflineMove(const QString& move) {
    if (!validatorProcess || validatorProcess->state() != QProcess::Running) {
        qWarning() << "[Validator] Process is not running!";
        return false;
    }

    // Send the move to the Python script for validation
    validatorProcess->write(move.toUtf8() + "\n");

    if (!validatorProcess->waitForBytesWritten(100)) {
        qWarning() << "[Validator] Failed to write move to process!";
        return false;
    }

    if (!validatorProcess->waitForReadyRead(500)) {
        qWarning() << "[Validator] No response from Python validator!";
        return false;
    }

    const QString resp = QString::fromUtf8(validatorProcess->readAllStandardOutput()).trimmed();
    qDebug() << "[Validator] Response from Python bot:" << resp;

    if (resp == "LOSS\r\nINVALID") {
        QMessageBox::critical(this, "Game Over", "LOSS");
        myTurn = false;
        statusLabel->setText("Game Over. You lost.");
        boardView->setEnabled(false);
        return false;
    }

    return resp == "OK";
}




void GameWindow::movePieceOnBoard(const QString& move) {
    if (move.length() < 4) return;

    int fromCol = move[0].toLatin1() - 'a';
    int fromRow = 8 - QString(move[1]).toInt();
    int toCol   = move[2].toLatin1() - 'a';
    int toRow   = 8 - QString(move[3]).toInt();

    ChessPieceItem* piece = pieceMap[fromRow][fromCol];
    if (!piece) {
        qDebug() << "No piece found at source:" << fromRow << fromCol;
        return;
    }

    // Eating figure
    if (pieceMap[toRow][toCol]) {
        scene->removeItem(pieceMap[toRow][toCol]);
        delete pieceMap[toRow][toCol];
    }

    pieceMap[fromRow][fromCol] = nullptr;
    piece->setBoardPos(toRow, toCol);
    pieceMap[toRow][toCol] = piece;

    // Pawn to Queen
    if (piece->pieceClass() == "pawn" && (toRow == 0 || toRow == 7) &&
        move.length() == 5 && move[4] == 'q') {
        QString basePath = "C:/Projects/Qt0/pieces/";
        QPixmap newQueen = (toRow == 0)
                               ? QPixmap(basePath + "black-queen.png")
                               : QPixmap(basePath + "white-queen.png");
        piece->setPixmap(newQueen);
        piece->setPieceClass("queen");
    }

    qDebug() << "Move applied:" << move;
}

void GameWindow::onMessageReceived(const QString &message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject obj = doc.object();
    QString type = obj["type"].toString();

    if (type == "start") {
        QString white = obj["white"].toString();
        QString black = obj["black"].toString();
        QString currentUser = getUsername();

        myColor = (currentUser == white) ? "white" : "black";
        myTurn = (myColor == "white");

        if (myTurn) {
            statusLabel->setText("Game started: " + white + " vs " + black + "\nYour move!");
        } else {
            statusLabel->setText("Game started: " + white + " vs " + black + "\nWaiting for opponent...");
        }
    }
    else if (type == "opponent_move") {
        QString move = obj["move"].toString();
        movePieceOnBoard(move);

        myTurn = true;
        statusLabel->setText("Your move!");
    }
    else if (type == "error") {
        QString errorMessage = obj["message"].toString();
        statusLabel->setText("Error: " + errorMessage);
        myTurn = true;
    }
}


void GameWindow::applyMoveOnBoard(ChessPieceItem* piece, int row, int col) {
    if (pieceMap[row][col]) {
        scene->removeItem(pieceMap[row][col]);
        delete pieceMap[row][col];
    }

    pieceMap[piece->row()][piece->col()] = nullptr;
    piece->setBoardPos(row, col);
    pieceMap[row][col] = piece;

    if (piece->pieceClass() == "pawn" && (row == 0 || row == 7)) {
        QString basePath = "C:/Projects/Qt0/pieces/";
        QPixmap newQueen = (row == 0)
                               ? QPixmap(basePath + "white-queen.png")
                               : QPixmap(basePath + "black-queen.png");
        piece->setPixmap(newQueen);
        piece->setPieceClass("queen");
    }
}

void GameWindow::setupBot() {
    QString scriptPath = QCoreApplication::applicationDirPath() + "/chess_bot.py";
    pythonProcess = new QProcess(this);
//    pythonProcess->start("python", QStringList{"C:/Projects/Qt0/chess_bot.py"});
    pythonProcess->start("python", QStringList() << scriptPath);

    if (!pythonProcess->waitForStarted(2000)) {
        qWarning() << "[BotSetup] Failed to start the Python bot";
        return;
    }
    qDebug() << "[BotSetup] Python bot started successfully.";

    // The bot game starts here, no need for Stockfish setup in C++

    QString scriptPath1 = QCoreApplication::applicationDirPath() + "/chess_validator.py";
    validatorProcess = new QProcess(this);
    //validatorProcess->start("python", {"C:/Projects/Qt0/chess_validator.py"});
    validatorProcess->start("python", QStringList() << scriptPath1);

    if (!validatorProcess->waitForStarted(2000)) {
        qWarning() << "[Validator] Failed to start chess_validator.py";
    }


    statusLabel->setText("Game vs Bot started! Your move.");
    myTurn = true;
}


void GameWindow::sendMoveToBot(const QString& move) {
    // Send move to Python bot (this assumes your Python script is listening for moves)
    if (pythonProcess) {
        pythonProcess->write(move.toUtf8() + '\n');
        pythonProcess->waitForBytesWritten(100);

        if (!pythonProcess->waitForReadyRead(5000)) {
            qWarning() << "[BotTurn] No response from Python bot!";
            return;
        }

        QString botMove = QString::fromUtf8(pythonProcess->readAllStandardOutput()).trimmed();
        const QString botMove1 = botMove;

        qDebug() << "[BotTurn] Bot move:" << botMove1;
        isValidOfflineMove(botMove1);
        if (!QRegularExpression("^[a-h][1-8][a-h][1-8]q?$").match(botMove).hasMatch()) {
            qWarning() << "[BotTurn] Bot move is not valid UCI format:" << botMove;
            return;
        }

        if (!botMove.isEmpty()) {
            movePieceOnBoard(botMove);
        } else {
            qWarning() << "[BotTurn] Bot failed to generate a move!";
        }
        statusLabel->setText("Your move!");
        myTurn = true;
    }
}
