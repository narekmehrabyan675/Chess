#include "loginwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSettings>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Login");

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Username");

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", this);
    guestButton = new QPushButton("Guest Mode (Play with Bot)", this);


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Login to your account:", this));
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);
    layout->addWidget(guestButton);


    setLayout(layout);

    networkManager = new QNetworkAccessManager(this);

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::tryLogin);
    connect(guestButton, &QPushButton::clicked, this, &LoginWindow::playAsGuest);

}

void LoginWindow::playAsGuest() {
    QString guestName = "guest";

    emit loginSuccessful(guestName);
}


void LoginWindow::tryLogin() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Enter both username and password.");
        return;
    }

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    QNetworkRequest request(QUrl("http://localhost:8080/api/auth/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply, username]() {
        reply->deleteLater();

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QString token = doc.object().value("token").toString();

            if (!token.isEmpty()) {
                QSettings settings(QString("./%1_settings.ini").arg(username), QSettings::IniFormat);
                settings.setValue("auth_token", token);
                settings.setValue("username", username);
                settings.sync();

                emit loginSuccessful(username);
            } else {
                QMessageBox::warning(this, "Login Failed", "No token in response.");
            }
        } else {
            QMessageBox::critical(this, "Network Error", reply->errorString());
        }
    });
}
