#include "mainapp.h"
#include "ui_mainapp.h"

MainApp::MainApp(QWidget *parent, QString info) :
    QDialog(parent),
    ui(new Ui::MainApp)
{
    ui->setupUi(this);
    socket = new QTcpSocket();
    connect(this, &MainApp::newMessage, this, &MainApp::displayMessage);
    connect(socket, &QTcpSocket::readyRead, this, &MainApp::readFromSocket);
    connect(socket, &QTcpSocket::disconnected, this, &MainApp::discardSocket);
    socket->connectToHost(QHostAddress::LocalHost,8080);

    if(!socket->waitForConnected())
    {
        QMessageBox::critical(this,"QTCPClient", QString("The following error occurred: %1.").arg(socket->errorString()));
        exit(EXIT_FAILURE);
    }
    sendToServer(info);
}


MainApp::~MainApp()
{
    if(socket->isOpen())
        socket->close();
    delete ui;
}

void MainApp::readFromSocket()
{
//    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray buffer;
    QDataStream socketStream(socket);
    socketStream.startTransaction();
    socketStream >> buffer;

    if(!socketStream.commitTransaction())
    {
        QString message = QString("Waiting for more data to come..");
        emit newMessage(message);
        return;
    }
    QString header = buffer.mid(0,128);
    QString fileType = header.split(",")[0].split(":")[1];
    buffer = buffer.mid(128);

    QString message = QString("%2").arg(QString::fromStdString(buffer.toStdString()));
    emit newMessage(message);
}

void MainApp::discardSocket()
{
    socket->deleteLater();
    socket=nullptr;
    QMessageBox::critical(this,"Client","Disconnected!");
}


void MainApp::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
        break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, "Client", "The host was not found. Please check the host name and port settings.");
        break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, "Client", "Make sure QTCPServer is running, and check that the host name and port settings are correct.");
        break;
        default:
            QMessageBox::information(this, "Client", QString("The following error occurred: %1.").arg(socket->errorString()));
        break;
    }
}

void MainApp::sendToServer(QString info)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QDataStream socketStream(socket);
            QByteArray header;
            header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(info.size()).toUtf8());
            header.resize(128);
            QByteArray byteArray = info.toUtf8();
            byteArray.prepend(header);
            socketStream << byteArray;
        }
        else
            QMessageBox::critical(this,"Client","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"Client","Not connected");
}

void MainApp::displayMessage(const QString& str)
{
    QMessageBox::information(this, "Client", str);
}

