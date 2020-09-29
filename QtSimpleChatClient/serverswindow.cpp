#include "serverswindow.h"
#include "ui_serverswindow.h"


#include <QDebug>

ServersWindow::ServersWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServersWindow),
    m_serverlistModel(new QStandardItemModel(this)),
    m_readServers(false),
    m_currentAddr()
{
    ui->setupUi(this);
    ui->listServers->setModel(m_serverlistModel);

    actionsSetEnabled(false);
}

ServersWindow::~ServersWindow()
{
    delete ui;
}


// -------------------------------------------------------------------------------


void ServersWindow::actionsSetEnabled(bool val)
{
    ui->actionRemove->setEnabled(val);
    ui->actionSWConnect->setEnabled(val);
    ui->actionDetails->setEnabled(val);
}


QString ServersWindow::currentAddr() const
{
    return m_currentAddr;
}


void ServersWindow::readServersFromSrv()
{
    if (m_readServers)
    {
        return;
    }

    // QString line = instream.readLine();
    QFile file("servers.srv");

    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        // ERROR MESSAGE
        qDebug() << "Error opening file.";
        return;
    }

    QTextStream instream(&file);
    QString line;
    while (!instream.atEnd())
    {
        line = instream.readLine();

        if (line.isEmpty())
            continue;

        addServerToList(line);
    }

    file.close();
    m_readServers = true;
    return;
}


void ServersWindow::addServerToList(QString &line)
{
    // SERVERNAME|IP
    QStringList qualifiers = line.split(QLatin1Char('|'));

    if (qualifiers.size() != 2)
    {
        // ERROR MESSAGE
        return;
    }

    // qDebug() << qualifiers[0] << qualifiers[1];

    QStandardItem * item = new QStandardItem;
    item->setData(qualifiers[0], 0);
    item->setData(qualifiers[1], 1);

    m_serverlistModel->appendRow(item);
}


void ServersWindow::removeServerFromList(QString& addr)
{
    m_readServers = false;
    QFile file("servers.srv");

    if(!file.open(QIODevice::ReadWrite | QFile::Text))
    {
        // ERROR MESSAGE
        qDebug() << "Error opening file.";
        return;
    }

    QTextStream fstream(&file);
    QString line, newfile;
    while (!fstream.atEnd())
    {
        line = fstream.readLine();
        if (line.isEmpty())
            continue;

        QStringList qualifiers = line.split(QLatin1Char('|'));
        if (qualifiers[1] == addr)
            continue;

        newfile.append(line + '\n');
    }

    file.resize(0);
    fstream << newfile;
    file.close();

    m_serverlistModel->clear();
    readServersFromSrv();
}


// -------------------------------------------------------------------------------


void ServersWindow::on_actionAdd_triggered()
{
    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel("Server"));

    // Add the lineEdits with their respective labels
    QList<QLineEdit *> fields;

    QLineEdit *nameEdit = new QLineEdit(&dialog);
    QString nameLabel = QString("Server name:");
    form.addRow(nameLabel, nameEdit);

    fields << nameEdit;

    QLineEdit *addrEdit = new QLineEdit(&dialog);
    QString addrLabel = QString("Server address:");
    form.addRow(addrLabel, addrEdit);

    fields << addrEdit;

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted)
    {
        // If the user didn't dismiss the dialog, do something with the fields
        QFile file("servers.srv");

        if(!file.open(QIODevice::Append | QFile::Text))
        {
            // ERROR MESSAGE
            qDebug() << "Error opening file.";
            return;
        }

        QString serverStr = fields[0]->text() + QLatin1Char('|') + fields[1]->text();
        QTextStream ofstream(&file);
        ofstream << '\n' << serverStr;
        addServerToList(serverStr);
    }
}

void ServersWindow::on_actionClose_triggered()
{
    hide();
}

void ServersWindow::on_listServers_clicked(const QModelIndex &index)
{
    QString selectedAddr = m_serverlistModel->itemFromIndex(index)->data(1).toString();
    m_currentAddr = selectedAddr;
    actionsSetEnabled(true);
    ui->labelSelectedIP->setText(m_currentAddr);
    // qDebug() << str;
}

void ServersWindow::on_actionSWConnect_triggered()
{
    ui->listServers->selectionModel()->clearSelection();
    actionsSetEnabled(false);
    hide();
    emit connectToServer();
}

void ServersWindow::on_actionRemove_triggered()
{
    removeServerFromList(m_currentAddr);
}
