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

using namespace std;

namespace Ui {
class MainApp;
}

class MainApp : public QDialog
{
    Q_OBJECT

public:
    explicit MainApp(QWidget *parent = nullptr, QString info = "New,User,0,0");
    ~MainApp();

private:
    Ui::MainApp *ui;
    QTcpSocket *socket;
    int show_hide = 0;

signals:
    void newMessage(QString);

private slots:
    void displayMessage(const QString& str);
    void readFromSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);
    void sendToServer(QString info);
    void on_pushButton_clicked();
    void on_push_deposite_clicked();
    void on_push_withdraw_clicked();
};

#endif // MAINAPP_H
