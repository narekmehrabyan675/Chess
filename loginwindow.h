#pragma once

#include <QWidget>

class QLineEdit;
class QPushButton;
class QNetworkAccessManager;

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void loginSuccessful(const QString& username);


private slots:
    void tryLogin();
    void playAsGuest();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QNetworkAccessManager *networkManager;
    QPushButton* guestButton;

};
