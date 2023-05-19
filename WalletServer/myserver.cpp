#include "myserver.h"
#include "ui_myserver.h"
#include "usr_record.h"

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
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MyServer::displayError);
    displayMessage(QString("INFO :: Client with sockd:%1 has just entered the room").arg(socket->socketDescriptor()));
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
    if (msg[0] == 'I') processIncommingUserData(socket, msg.remove(0,1));
    else {
        msg.remove(0,1);
        QString id_str = "";
        QString msg_info = "";
        bool comma_flag = false;
        for(auto c : msg){
            if (c == ',') comma_flag = true;
            else if (!comma_flag) id_str += c;
            else if (comma_flag) msg_info += c;
        }
        if (msg[0] == 'B') sendBalance(id_str, msg_info);
        else if (msg[0] == 'T') updateBalance(id_str, msg.remove(0,1));
        else if (msg[0] == 'R') sendTransactions(id_str);
        else if (msg[0] == 'X') {
            rec->cancel_last_trans();
            sendToClient(socket, QString::fromStdString("B" + rec->get_balance()));
        }
    }
}


void MyServer::processIncommingUserData(QTcpSocket *socket, QString usr_str)
{
    string usr_std_str = usr_str.toStdString();
    QString ID = QString::number(UsrRecord(usr_std_str).get_id());
    usr_sockets[ID].push_back(socket);
    if (records.count(ID)) emit newMessage("User already existing!");
    else {
        emit newMessage("New User Added!");
        records[ID] = new UsrRecord(usr_std_str);
    }
    rec = records[ID];
    QString info = QString::fromStdString("N" + rec->get_fname() + " " + rec->get_lname() + "," + ID.toStdString());
    foreach (QTcpSocket* socket, usr_sockets[ID]) //connections
    {
        sendToClient(socket, info);
    }
}

void MyServer::sendToClient(QTcpSocket* socket, QString data)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QDataStream socketStream(socket);
            QByteArray header;
            header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(data.size()).toUtf8());
            header.resize(128);

            QByteArray byteArray = data.toUtf8();
            byteArray.prepend(header);
            socketStream << byteArray;
        }
        else
            QMessageBox::critical(this,"Server","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"Server","Not connected");
}

void MyServer::sendBalance(QString ID, QString show_hide)
{
    QString bal;
    if(show_hide == "0")
        bal = QString::fromStdString("B" + records[ID]->get_balance());
    else bal = "B******";

    foreach (QTcpSocket* socket, usr_sockets[ID]) //connections
    {
        sendToClient(socket, bal);
    }
}

void MyServer::sendTransactions(QString ID)
{
    stack<float> temp_s = rec->get_transactions();
    int size = temp_s.size();
    string trans_str = "R";
    while (!temp_s.empty()) {
        if (temp_s.top() > 0) trans_str += ("#" + to_string(size) + " Deposite: " + to_string(temp_s.top()) + "\n");
        else trans_str += ("#" + to_string(size) + " Withdraw: " + to_string(-1*temp_s.top()) + "\n");
        temp_s.pop();
        size--;
    }

    foreach (QTcpSocket* socket, usr_sockets[ID]) //connections
    {
        sendToClient(socket, QString::fromStdString(trans_str));
    }
}

void MyServer::updateBalance(QString ID, QString trans)
{
    float bal = stof(trans.toStdString());
    records[ID]->set_balance(bal);
    foreach (QTcpSocket* socket, usr_sockets[ID]) //connections
    {
        sendToClient(socket, QString::fromStdString("B" + records[ID]->get_balance()));
    }
}



void MyServer::displayMessage(const QString& str)
{
    ui->textBrowser->append(str);
}

void MyServer::discardSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = connections.find(socket);
    if (it != connections.end()){
        displayMessage(QString("INFO :: A usr_std_strclient has just left the room").arg(socket->socketDescriptor()));
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
