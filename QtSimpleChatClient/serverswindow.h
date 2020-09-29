#ifndef SERVERSWINDOW_H
#define SERVERSWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>


namespace Ui {
class ServersWindow;
}

class ServersWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void connectToServer();

public:
    explicit ServersWindow(QWidget *parent = nullptr);
    ~ServersWindow();

    void readServersFromSrv();
    void addServerToList(QString& line);
    void removeServerFromList(QString& addr);
    QString currentAddr() const;

private slots:
    void on_actionAdd_triggered();
    void on_actionClose_triggered();
    void on_listServers_clicked(const QModelIndex &index);
    void on_actionSWConnect_triggered();
    void on_actionRemove_triggered();

private:
    Ui::ServersWindow *ui;
    QStandardItemModel * m_serverlistModel;
    bool m_readServers;
    QString m_currentAddr;
    void actionsSetEnabled(bool val);

};

#endif // SERVERSWINDOW_H
