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
        QMessageBox::critical(this,"Client", QString("The following error occurred: %1.").arg(socket->errorString()));
        exit(EXIT_FAILURE);
    }
    sendToServer("I" + info);
}


MainApp::~MainApp()
{
    if(socket->isOpen())
        socket->close();
    delete ui;
}

void MainApp::readFromSocket()
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
    QString header = buffer.mid(0,128);
    QString fileType = header.split(",")[0].split(":")[1];
    buffer = buffer.mid(128);

    QString msg = QString("%2").arg(QString::fromStdString(buffer.toStdString()));
    if (msg[0] == 'N'){
        msg.remove(0,1);
        ui->label->setText("Welcome, " + msg + "!");
    }
    else if(msg[0] == 'B'){
        msg.remove(0,1);
        ui->label_2->setText("Account Balance: " + msg);
    }
    else if (msg[0] == 'R'){
        msg.remove(0,1);
        ui->textEdit->setText(msg);
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


void MainApp::on_pushButton_clicked()
{
    sendToServer(QString::fromStdString("B" + to_string(show_hide)));
    show_hide ^= 1;
}

void MainApp::on_push_deposite_clicked()
{
    if(ui->text_deposite->text() != "") {
        bool isNum = false;
        float value = ui->text_deposite->text().toFloat(&isNum);
        if(isNum) sendToServer("T" + ui->text_deposite->text());
        else QMessageBox::critical(this, "Error!", "Please enter a valid number");
        ui->text_deposite->clear();
        ui->textEdit->clear();
    }
    else QMessageBox::critical(this, "Error!", "Please enter the amount of money to deposite");
}

void MainApp::on_push_withdraw_clicked()
{
    if(ui->text_withdraw->text() != "") {
        bool isNum = false;
        float value = ui->text_withdraw->text().toFloat(&isNum);
        if(isNum) sendToServer("T-" + ui->text_withdraw->text());
        else QMessageBox::critical(this, "Error!", "Please enter a valid number");
        ui->text_withdraw->clear();
        ui->textEdit->clear();
    }
    else QMessageBox::critical(this, "Error!", "Please enter the amount of money to withdraw");
}

void MainApp::on_pushButton_cancel_clicked()
{
    sendToServer("X");
    QMessageBox::information(this, "Confirmation", "The last transaction is canceled");
}

void MainApp::on_pushButton_show_trans_clicked()
{
    sendToServer("R");
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

