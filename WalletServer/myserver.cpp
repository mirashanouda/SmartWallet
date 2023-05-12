#include "myserver.h"
#include "ui_myserver.h"

MyServer::MyServer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyServer)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    if(server->listen(QHostAddress::Any, 8080)){
        connect(this, &MyServer::newMessage, this, &MyServer::displayMessage);
        connect(server, &QTcpServer::newConnection, this, &MyServer::newConnection);
    }
    else{
        QMessageBox::critical(this,"Server", QString("Unable to start the server: %1.").arg(server->errorString()));
        exit(EXIT_FAILURE);
    }
}

void MyServer::newConnection(){
    while(server->hasPendingConnections())
        addToSocketSet(server->nextPendingConnection());
}


void MyServer::addToSocketSet(QTcpSocket *socket)
{
    connections.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &MyServer::readFromSocket);
    connect(socket, &QTcpSocket::disconnected, this, &MyServer::discardSocket);
//    displayMessage(QString("INFO :: Client with sockd:%1 has just entered the room").arg(socket->socketDescriptor()));
}


void MyServer::readFromSocket()
{
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray server_msg;

    QDataStream socketStream(socket);
    socketStream.startTransaction();
    socketStream >> server_msg;

    if(!socketStream.commitTransaction())
    {
        QString message = "Waiting for more data to come..";
        emit newMessage(message);
        return;
    }

    QString header = server_msg.mid(0,128);
    QString fileType = header.split(",")[0].split(":")[1];

    server_msg = server_msg.mid(128);

    QString msg = QString::fromStdString(server_msg.toStdString());
    emit newMessage(msg);
}

void MyServer::discardSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = connections.find(socket);
    if (it != connections.end()){
        displayMessage(QString("INFO :: A client has just left the room").arg(socket->socketDescriptor()));
        connections.remove(*it);
    }
    socket->deleteLater();
}

void MyServer::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
        break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, "QTCPServer", "The host was not found. Please check the host name and port settings.");
        break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, "QTCPServer", "The connection was refused by the peer. Make sure QTCPServer is running, and check that the host name and port settings are correct.");
        break;
        default:
            QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
            QMessageBox::information(this, "QTCPServer", QString("The following error occurred: %1.").arg(socket->errorString()));
        break;
    }
}

void MyServer::displayMessage(const QString& str)
{
    //QMessageBox::information(this,"Server",str);
    ui->textBrowser->append(str);
}

MyServer::~MyServer()
{
    foreach (QTcpSocket* socket, connections)
    {
        socket->close();
        socket->deleteLater();
    }

    server->close();
    server->deleteLater();
    delete ui;
}
