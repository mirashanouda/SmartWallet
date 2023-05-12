#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket();
    connect(this, &MainWindow::newMessage, this, &MainWindow::displayMessage);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readFromSocket);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::discardSocket);
    socket->connectToHost(QHostAddress::LocalHost,8080);

    if(!socket->waitForConnected())
//        ui->statusBar->showMessage("Connected to Server");
    /*else*/{
        QMessageBox::critical(this,"QTCPClient", QString("The following error occurred: %1.").arg(socket->errorString()));
        exit(EXIT_FAILURE);
    }
}

MainWindow::~MainWindow()
{
    if(socket->isOpen())
        socket->close();
    delete ui;
}

void MainWindow::readFromSocket()
{
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
    QString message = QString("%2").arg(QString::fromStdString(buffer.toStdString()));
    emit newMessage(message);
}

void MainWindow::discardSocket()
{
    socket->deleteLater();
    socket=nullptr;
    QMessageBox::critical(this,"Client","Disconnected!");
}


void MainWindow::displayError(QAbstractSocket::SocketError socketError)
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

void MainWindow::displayMessage(const QString& str)
{
    QMessageBox::information(this, "Client", str);
}

void MainWindow::on_pushButton_clicked()
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QString fname = ui->fn_lineEdit->text();
            QString lname = ui->ln_lineEdit->text();
            QString phone = ui->phone_lineEdit->text();
            QString id = ui->id_lineEdit->text();

            QString all_info = fname + "," + lname + "," + phone + "," + id;
            QDataStream socketStream(socket);
            QByteArray header;
            header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(all_info.size()).toUtf8());
            header.resize(128);
            QByteArray byteArray = all_info.toUtf8();
            byteArray.prepend(header);
            socketStream << byteArray;

            ui->fn_lineEdit->clear();
            ui->ln_lineEdit->clear();
            ui->phone_lineEdit->clear();
            ui->id_lineEdit->clear();
        }
        else
            QMessageBox::critical(this,"Client","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"Client","Not connected");
}
