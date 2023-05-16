#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
//            QString fname = ui->fn_lineEdit->text();
//            QString lname = ui->ln_lineEdit->text();
//            QString phone = ui->phone_lineEdit->text();
//            QString id = ui->id_lineEdit->text();

    QString fname = "Mira";
    QString lname = "Medhat";
    QString phone = "123456";
    QString id = "789000";

    QString all_info = fname + "," + lname + "," + phone + "," + id;

    ui->fn_lineEdit->clear();
    ui->ln_lineEdit->clear();
    ui->phone_lineEdit->clear();
    ui->id_lineEdit->clear();

    hide();
    main_app = new MainApp(this, all_info);
    main_app->show();
}
