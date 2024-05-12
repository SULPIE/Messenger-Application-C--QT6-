#include "authwindow.h"
#include "ui_authwindow.h"

AuthWindow::AuthWindow(Server* server, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AuthWindow)
{
    connect(this, &AuthWindow::authCalled, server, &Server::callAuth);
    connect(server, &Server::authSuccessed, this, &AuthWindow::closeAuthWindow);
    ui->setupUi(this);
}

AuthWindow::~AuthWindow()
{
    delete ui;
}

void AuthWindow::closeAuthWindow()
{
    this->close();
}

void AuthWindow::on_authButton_clicked()
{
    std::string login = ui->loginEdit->text().toStdString();
    std::string password = ui->passEdit->text().toStdString();

    emit authCalled(login, password);
}