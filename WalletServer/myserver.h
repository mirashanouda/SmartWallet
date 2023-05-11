#ifndef MYSERVER_H
#define MYSERVER_H

#include <QMainWindow>
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
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

private:
    Ui::MyServer *ui;
    QTcpServer *server;
    QList<QTcpSocket*> connection_list;

private slots:
    void newConnection();
};
#endif // MYSERVER_H
