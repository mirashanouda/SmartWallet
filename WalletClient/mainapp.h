#ifndef MAINAPP_H
#define MAINAPP_H

#include <QDialog>
#include <QTcpSocket>
#include <QMessageBox>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QHostAddress>
#include <QDebug>
#include <vector>

namespace Ui {
class MainApp;
}

class MainApp : public QDialog
{
    Q_OBJECT

public:
    explicit MainApp(QWidget *parent = nullptr, QString info = "New,User,0,0");
//    MainApp(QString);
    ~MainApp();

private:
    Ui::MainApp *ui;
    QTcpSocket *socket;

signals:
    void newMessage(QString);

private slots:
    void displayMessage(const QString& str);
    void readFromSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);
    void sendToServer(QString info);
};

#endif // MAINAPP_H
