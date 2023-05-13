#ifndef MYSERVER_H
#define MYSERVER_H

#include <iostream>
#include <QMainWindow>
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <QSet>
#include <QString>
#include <QMessageBox>
#include <QList>
#include <QMap>
#include "usr_record.h"
#include <map>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MyServer; }
QT_END_NAMESPACE

class MyServer : public QMainWindow
{
    Q_OBJECT

public:
    MyServer(QWidget *parent = nullptr);
    ~MyServer();

signals:
    void newMessage(QString);

private:
    Ui::MyServer *ui;
    QTcpServer *server;
    QSet<QTcpSocket*> connections;
    map<QString, UsrRecord> records;

private slots:
    void newConnection();
    void addToSocketSet(QTcpSocket* socket);

    void readFromSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);

    void displayMessage(const QString& str);
    void processIncommingUserData(QString usr_str);

};
#endif // MYSERVER_H
