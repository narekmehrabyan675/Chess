#ifndef MODESELECTIONWINDOW_H
#define MODESELECTIONWINDOW_H

#include <QWidget>
#include <QPushButton>

class ModeSelectionWindow : public QWidget {
    Q_OBJECT
public:
    explicit ModeSelectionWindow(const QString& username, QWidget *parent = nullptr);

signals:
    void startGameWithBot(const QString& username);
    void startOnlineGame(const QString& username);

private:
    QString currentUsername;
};

#endif // MODESELECTIONWINDOW_H
