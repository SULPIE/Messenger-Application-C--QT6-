#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QMainWindow>
#include "spdlog/spdlog.h"
#include <QtcpSocket>
#include "serverqueries.h"

namespace Ui {
    class AuthWindow;
}
class AuthWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AuthWindow(Server* server, QWidget *parent = nullptr);
    ~AuthWindow();

private:
    Ui::AuthWindow *ui;

public slots:
    void on_authButton_clicked();
    void closeAuthWindow();

signals:
    void authCalled(std::string login, std::string password);
};

#endif // AUTHWINDOW_H
