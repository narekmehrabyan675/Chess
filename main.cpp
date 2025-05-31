#include <QApplication>
#include "loginwindow.h"
#include "gamewindow.h"
#include "modeselectionwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    LoginWindow loginWindow;
    loginWindow.show();


    QObject::connect(&loginWindow, &LoginWindow::loginSuccessful, &loginWindow, [&](QString username) {
        loginWindow.close();

        if (username == "guest") {
            GameWindow* botGame = new GameWindow(username, true);
            botGame->show();
            return;
        }

        ModeSelectionWindow* modeWindow = new ModeSelectionWindow(username);
        modeWindow->show();

        QObject::connect(modeWindow, &ModeSelectionWindow::startOnlineGame, modeWindow, [=](const QString& user) {
            modeWindow->close();
            GameWindow* onlineGame = new GameWindow(user, false);
            onlineGame->show();
        });

        QObject::connect(modeWindow, &ModeSelectionWindow::startGameWithBot, modeWindow, [=](const QString& user) {
            modeWindow->close();
            GameWindow* botGame = new GameWindow(user, true);
            botGame->show();
        });
    });



    return app.exec();
}
