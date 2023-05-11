#include "myserver.h"
#include "ui_myserver.h"

MyServer::MyServer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyServer)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    if(server->listen(QHostAddress::Any, 8080)){
        connect(server, SIGNAL(newConnection()), this, SLOT(newConnection())
);
        QMessageBox::information(this, "Server", "Sever connected");
    }
    else {
        QMessageBox::warning(this, "Server", "Sever failed");
    }
}

MyServer::~MyServer()
{
    delete ui;
}

void MyServer::read_data_from_socket()
{
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray server_msg = socket->readAll();
    QString msg = QString::fromStdString(server_msg.toStdString());
    QMessageBox::information(this, "Server", "Message from client" + msg);
}

void MyServer::add_new_connection(QTcpSocket *socket)
{
    connection_list.append(socket);
    connect(socket, SIGNAL(readyRead()), this, SLOT(read_data_from_socket));
}

void MyServer::newConnection(){
    while(server->hasPendingConnections()){
        add_new_connection(server->nextPendingConnection());
    }
}
