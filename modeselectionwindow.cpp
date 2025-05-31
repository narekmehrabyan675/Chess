#include "modeselectionwindow.h"
#include <QVBoxLayout>
#include <QLabel>

ModeSelectionWindow::ModeSelectionWindow(const QString& username, QWidget *parent)
    : QWidget(parent), currentUsername(username)
{
    setWindowTitle("Choose Game Mode");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Choose game mode:"));

    QPushButton *botButton = new QPushButton("Play with Bot", this);
    QPushButton *onlineButton = new QPushButton("Play Online", this);

    connect(botButton, &QPushButton::clicked, this, [=]() {
        emit startGameWithBot(currentUsername);
    });

    connect(onlineButton, &QPushButton::clicked, this, [=]() {
        emit startOnlineGame(currentUsername);
    });

    layout->addWidget(botButton);
    layout->addWidget(onlineButton);
}
