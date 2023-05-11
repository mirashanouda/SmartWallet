#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    socket = new QTcpSocket();
    socket->connectToHost(QHostAddress::LocalHost, 8080);
    socket->open(QIODevice::ReadWrite);
    if (socket->isOpen()){
        QMessageBox::information(this, "Client", "Connected to a server");
    }
    else {
        QMessageBox::warning(this, "Client", "Failed to connect");
    }
}


void MainWindow::on_LogIn_clicked()
{
    QString input = ui->lineEdit->text();
    if(socket){
        if (socket->isOpen()){
            socket->write(input.toStdString().c_str());
        }
        else {
            QMessageBox::warning(this, "Client", "Failed to send data");
        }
    }
    QMessageBox::information(this, "Title", input);
}
