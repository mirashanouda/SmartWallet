#ifndef MYSERVER_H
#define MYSERVER_H

#include <QMainWindow>
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <QSet>
#include <QString>
#include <QMessageBox>



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

private slots:
    void newConnection();
    void addToSocketSet(QTcpSocket* socket);

    void readFromSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);

    void displayMessage(const QString& str);

};
#endif // MYSERVER_H
