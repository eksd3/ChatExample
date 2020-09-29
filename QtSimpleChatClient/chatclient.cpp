#include "chatclient.h"

ChatClient::ChatClient(QObject * parent)
    : QObject(parent),
      m_clientSocket(new QTcpSocket(this)),
      m_loggedIn(false)
{
    connect(m_clientSocket, &QTcpSocket::connected, this, &ChatClient::connected);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &ChatClient::disconnected);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
    connect(m_clientSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &ChatClient::error);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &ChatClient::setLoggedInToFalse);
}


int ChatClient::login(const QString &userName)
{
    if (m_clientSocket->state() == QAbstractSocket::ConnectedState)
    {
        QDataStream clientStream(m_clientSocket);
        clientStream.setVersion(QDataStream::Qt_5_7);

        QJsonObject message;
        message["type"] = QStringLiteral("login");
        message["username"] = userName;
        m_username = userName;
        m_loggedIn = true;

        clientStream << QJsonDocument(message).toJson();

        return 0;
    }

    return 1;
}


void ChatClient::sendMessage(const QString &text)
{
    QDataStream clientStream(m_clientSocket);
    clientStream.setVersion(QDataStream::Qt_5_7);

    QJsonObject message;
    message["type"] = QStringLiteral("message");
    message["text"] = text;

    clientStream << QJsonDocument(message).toJson(QJsonDocument::Compact);
}



void ChatClient::disconnectFromHost()
{
    m_loggedIn = false;
    m_clientSocket->disconnectFromHost();
}


void ChatClient::connectToServer(const QHostAddress &address, qint16 port)
{
    if (isLoggedIn())
    {
        disconnectFromHost();
    }
    m_clientSocket->connectToHost(address, port);
}


void ChatClient::jsonRecieved(const QJsonObject &docObj)
{
    const QJsonValue typeVal = docObj.value((QLatin1String("type")));

    if (typeVal.isNull() || !typeVal.isString())
        return;

    if (typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) == 0)
    {
        if (m_loggedIn)
            return;

        const QJsonValue resultVal = docObj.value(QLatin1String("success"));
        if (resultVal.isNull() || !resultVal.isBool())
            return;
        const bool loginSuccess = resultVal.toBool();
        if (loginSuccess)
        {
            emit loggedIn();
            return;
        }

        const QJsonValue textVal = docObj.value(QLatin1String("text"));
        const QJsonValue senderVal = docObj.value(QLatin1String("sender"));
        if (textVal.isNull() || !textVal.isString())
            return;

        emit messageReceived(senderVal.toString(), textVal.toString());
    }

    else if (typeVal.toString().compare(QLatin1String("newuser"), Qt::CaseInsensitive) == 0)
    {
        const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
        if (usernameVal.isNull() || !usernameVal.isString())
            return;

        emit userJoined(usernameVal.toString());
    }

    else if (typeVal.toString().compare(QLatin1String("userdisconnected"), Qt::CaseInsensitive) == 0)
    {
        const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
        if (usernameVal.isNull() || !usernameVal.isString())
            return;

        emit userLeft(usernameVal.toString());
    }
}



void ChatClient::onReadyRead()
{
    QByteArray jsonData;
    QDataStream socketStream(m_clientSocket);
    socketStream.setVersion(QDataStream::Qt_5_7);

    while (1)
    {
        socketStream.startTransaction();
        socketStream >> jsonData;

        if (socketStream.commitTransaction())
        {
            QJsonParseError parseError;
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if (parseError.error == QJsonParseError::NoError)
            {
                if (jsonDoc.isObject())
                    jsonRecieved(jsonDoc.object());
            }
        }

        else
            break;
    }
}
