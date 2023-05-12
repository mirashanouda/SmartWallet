#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QHostAddress>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;

signals:
    void newMessage(QString);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_LogIn_clicked();
    void displayMessage(const QString& str);

    void readFromSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);


};
#endif // MAINWINDOW_H
