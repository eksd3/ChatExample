#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QInputDialog>
#include <QMessageBox>

#include "chatclient.h"
#include "serverswindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
QT_END_NAMESPACE

class ChatWindow : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(ChatWindow)

public:
    ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private:
    Ui::ChatWindow *ui;
    ChatClient * m_chatClient;
    QStandardItemModel * m_chatModel;
    QString m_lastUserName;

    ServersWindow * m_serversWindow;

private slots:
    void attemptConnection();
    void attemptConnectionFromSerWin();
    void connectedToServer();
    void attemptLogin(const QString &userName);
    void loggedIn();
    void loginFailed(const QString &reason);
    void messageReceived(const QString &sender, const QString &text);
    void sendMessage();
    void disconnectedFromServer();
    void userJoined(const QString &username);
    void userLeft(const QString &username);
    void error(QAbstractSocket::SocketError socketError);
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionQuit_triggered();
    void on_actionServers_Servers_list_triggered();
};
#endif // CHATWINDOW_H
