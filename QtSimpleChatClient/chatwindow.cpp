#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::ChatWindow),
      m_chatClient(new ChatClient(this)),
      m_chatModel(new QStandardItemModel(this)),
      m_serversWindow(new ServersWindow(this))
{
    ui->setupUi(this);

    m_chatModel->insertColumn(0);
    ui->listMessages->setModel(m_chatModel);

    connect(m_chatClient, &ChatClient::connected, this, &ChatWindow::connectedToServer);
    connect(m_chatClient, &ChatClient::loggedIn, this, &ChatWindow::loggedIn);
    connect(m_chatClient, &ChatClient::loginError, this, &ChatWindow::loginFailed);
    connect(m_chatClient, &ChatClient::messageReceived, this, &ChatWindow::messageReceived);
    connect(m_chatClient, &ChatClient::disconnected, this, &ChatWindow::disconnectedFromServer);
    connect(m_chatClient, &ChatClient::error, this, &ChatWindow::error);
    connect(m_chatClient, &ChatClient::userJoined, this, &ChatWindow::userJoined);
    connect(m_chatClient, &ChatClient::userLeft, this, &ChatWindow::userLeft);

    connect(m_serversWindow, &ServersWindow::connectToServer, this, &ChatWindow::attemptConnectionFromSerWin);

    // connect(ui->actionConnect, &QAction::triggered, this, &ChatWindow::attemptConnection);
    connect(ui->buttonSend, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    connect(ui->editMessage, &QLineEdit::returnPressed, this, &ChatWindow::sendMessage);

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->buttonSend->setEnabled(false);
    ui->editMessage->setEnabled(false);
    ui->listMessages->setEnabled(false);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}



// ---------------------------------------------------------------------


void ChatWindow::attemptConnection()
{
    const QString hostAddress = QInputDialog::getText(
                    this,
                    tr("Choose Server"),
                    tr("Server Address"),
                    QLineEdit::Normal,
                    QStringLiteral("127.0.0.1")
                    );

    if (hostAddress.isEmpty())
        return;

    ui->actionConnect->setEnabled(false);
    ui->actionDisconnect->setEnabled(true);
    m_chatClient->connectToServer(QHostAddress(hostAddress), 1967);
}


void ChatWindow::attemptConnectionFromSerWin()
{
    QString hostAddress = m_serversWindow->currentAddr();
    m_chatClient->connectToServer(QHostAddress(hostAddress), 1967);
}


void ChatWindow::connectedToServer()
{
    const QString newUsername = QInputDialog::getText(this, tr("Choose username"), tr("Username"));

    if (newUsername.isEmpty())
    {
        loginFailed("Username is empty.");
        // return m_chatClient->disconnectFromHost();
    }

    attemptLogin(newUsername);
}

void ChatWindow::attemptLogin(const QString &userName)
{
    if (m_chatClient->login(userName) == 0)
    {
        // ui->buttonConnect->setText("Disconnect");
        // ui->buttonConnect->setEnabled(true);

        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->buttonSend->setEnabled(true);
        ui->editMessage->setEnabled(true);
        ui->listMessages->setEnabled(true);
    }
}

void ChatWindow::loggedIn()
{

    m_lastUserName.clear();
}



void ChatWindow::loginFailed(const QString &reason)
{
    QMessageBox::critical(this, tr("Error"), reason);
    connectedToServer();
}



void ChatWindow::messageReceived(const QString &sender, const QString &text)
{
    int newRow = m_chatModel->rowCount();

    if (m_lastUserName != sender)
    {
        m_lastUserName = sender;
        QFont boldFont;
        boldFont.setBold(true);
        m_chatModel->insertRows(newRow, 2);
        m_chatModel->setData(m_chatModel->index(newRow, 0), sender + QLatin1String(" : "));
        m_chatModel->setData(m_chatModel->index(newRow, 0), int(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextAlignmentRole);
        m_chatModel->setData(m_chatModel->index(newRow, 0), boldFont, Qt::FontRole);
        ++newRow;
    }

    else
    {
        m_chatModel->insertRow(newRow);
    }

    m_chatModel->setData(m_chatModel->index(newRow, 0), text);
    m_chatModel->setData(m_chatModel->index(newRow, 0), int(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextAlignmentRole);
    ui->listMessages->scrollToBottom();
}



void ChatWindow::sendMessage()
{
    if (ui->editMessage->text().isEmpty())
        return;

    QString message = m_chatClient->username() + QLatin1String(" : ") + ui->editMessage->text();
    m_chatClient->sendMessage(message);
    // m_chatClient->sendMessage(ui->editMessage->text());

    const int newRow = m_chatModel->rowCount();
    m_chatModel->insertRow(newRow);
    // m_chatModel->setData(m_chatModel->index(newRow, 0), ui->editMessage->text());
    m_chatModel->setData(m_chatModel->index(newRow, 0), message);
    m_chatModel->setData(m_chatModel->index(newRow, 0), QBrush(Qt::black), Qt::ForegroundRole);
    m_chatModel->setData(m_chatModel->index(newRow, 0), int(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextAlignmentRole);

    ui->editMessage->clear();
    ui->listMessages->scrollToBottom();
    m_lastUserName.clear();
}


void ChatWindow::disconnectedFromServer()
{
    // QMessageBox::warning(this, tr("Disconnected"), tr("The host terminated the connection"));

    ui->buttonSend->setEnabled(false);
    ui->editMessage->setEnabled(false);
    ui->listMessages->setEnabled(false);

    // ui->buttonConnect->setEnabled(true);
    // ui->buttonConnect->setText("Connect");
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);

    m_lastUserName.clear();
}


void ChatWindow::userJoined(const QString &username)
{
    const int newRow = m_chatModel->rowCount();

    m_chatModel->insertRow(newRow);
    m_chatModel->setData(m_chatModel->index(newRow, 0), tr("%1 Joined the chat").arg(username));
    m_chatModel->setData(m_chatModel->index(newRow, 0), Qt::AlignCenter, Qt::TextAlignmentRole);
    m_chatModel->setData(m_chatModel->index(newRow, 0), QBrush(Qt::blue), Qt::ForegroundRole);

    ui->listMessages->scrollToBottom();
    m_lastUserName.clear();
}



void ChatWindow::userLeft(const QString &username)
{
    const int newRow = m_chatModel->rowCount();
    m_chatModel->insertRow(newRow);
    m_chatModel->setData(m_chatModel->index(newRow, 0), tr("%1 left the chat").arg(username));
    m_chatModel->setData(m_chatModel->index(newRow, 0), Qt::AlignCenter, Qt::TextAlignmentRole);
    m_chatModel->setData(m_chatModel->index(newRow, 0), QBrush(Qt::red), Qt::ForegroundRole);
    ui->listMessages->scrollToBottom();
    m_lastUserName.clear();
}



void ChatWindow::error(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
        case QAbstractSocket::ProxyConnectionClosedError:
            return; // handled by disconnectedFromServer
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::critical(this, tr("Error"), tr("The host refused the connection"));
            break;
        case QAbstractSocket::ProxyConnectionRefusedError:
            QMessageBox::critical(this, tr("Error"), tr("The proxy refused the connection"));
            break;
        case QAbstractSocket::ProxyNotFoundError:
            QMessageBox::critical(this, tr("Error"), tr("Could not find the proxy"));
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::critical(this, tr("Error"), tr("Could not find the server"));
            break;
        case QAbstractSocket::SocketAccessError:
            QMessageBox::critical(this, tr("Error"), tr("You don't have permissions to execute this operation"));
            break;
        case QAbstractSocket::SocketResourceError:
            QMessageBox::critical(this, tr("Error"), tr("Too many connections opened"));
            break;
        case QAbstractSocket::SocketTimeoutError:
            QMessageBox::warning(this, tr("Error"), tr("Operation timed out"));
            return;
        case QAbstractSocket::ProxyConnectionTimeoutError:
            QMessageBox::critical(this, tr("Error"), tr("Proxy timed out"));
            break;
        case QAbstractSocket::NetworkError:
            QMessageBox::critical(this, tr("Error"), tr("Unable to reach the network"));
            break;
        case QAbstractSocket::UnknownSocketError:
            QMessageBox::critical(this, tr("Error"), tr("An unknown error occurred"));
            break;
        case QAbstractSocket::UnsupportedSocketOperationError:
            QMessageBox::critical(this, tr("Error"), tr("Operation not supported"));
            break;
        case QAbstractSocket::ProxyAuthenticationRequiredError:
            QMessageBox::critical(this, tr("Error"), tr("Your proxy requires authentication"));
            break;
        case QAbstractSocket::ProxyProtocolError:
            QMessageBox::critical(this, tr("Error"), tr("Proxy communication failed"));
            break;
        case QAbstractSocket::TemporaryError:
        case QAbstractSocket::OperationError:
            QMessageBox::warning(this, tr("Error"), tr("Operation failed, please try again"));
            return;
        default:
            Q_UNREACHABLE();
        }

    // ui->buttonConnect->setEnabled(true);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);

    ui->buttonSend->setEnabled(false);
    ui->editMessage->setEnabled(false);
    ui->listMessages->setEnabled(false);

    m_lastUserName.clear();
}


// ---------------------------------------------------------------------


void ChatWindow::on_actionConnect_triggered()
{
    attemptConnection();
}

void ChatWindow::on_actionDisconnect_triggered()
{
    m_chatClient->disconnectFromHost();
}

void ChatWindow::on_actionQuit_triggered()
{
    if (m_chatClient->isLoggedIn())
    {
        m_chatClient->disconnectFromHost();
    }

    QApplication::quit();
}


// ---------------------------------------------------------------------


void ChatWindow::on_actionServers_Servers_list_triggered()
{
    m_serversWindow->show();
    m_serversWindow->readServersFromSrv();
}
