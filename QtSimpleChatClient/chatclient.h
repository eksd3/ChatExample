#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QMainWindow>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>


class ChatClient : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ChatClient)

public:
    explicit ChatClient(QObject * parent = nullptr);
    QString username() const { return m_username; };
    bool isLoggedIn() const { return m_loggedIn; };
    void setLoggedIn(bool log) { m_loggedIn = log; };

public slots:
    void connectToServer(const QHostAddress &address, qint16 port);
    int login(const QString &userName);
    void sendMessage(const QString &text);
    void disconnectFromHost();

private slots:
    void onReadyRead();

signals:
    void connected();
    void loggedIn();
    void loginError(const QString &reason);
    void disconnected();
    void messageReceived(const QString &sender, const QString &text);
    void error(QAbstractSocket::SocketError socketError);
    void userJoined(const QString &username);
    void userLeft(const QString &username);

private:
    QString m_username;
    QTcpSocket * m_clientSocket;
    bool m_loggedIn;
    void jsonRecieved(const QJsonObject &doc);
    void setLoggedInToFalse() { m_loggedIn = false; };
};


#endif // CHATCLIENT_H
