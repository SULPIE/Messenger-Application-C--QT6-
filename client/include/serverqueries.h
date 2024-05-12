#ifndef SERRVERQUERIES_H
#define SERRVERQUERIES_H
#include <QTcpSocket>
#include <spdlog/spdlog.h>
#include <iostream>
#include <qtimer.h>
#include <QThread>
#include <QMutex>
#include <nlohmann/json.hpp>

#include "mainwindow.h"

class Server : public QTcpSocket
{
    Q_OBJECT

public:
    Server();
    static inline QTcpSocket* socket = nullptr;
    static inline QByteArray Data;
    void ConnectToHost();

public slots:
    void SendToServer(QString sendername, int groupid,const QString& message);
    void disconnectedFromHost();
    void call(std::string functionName, std::vector<std::string> args);

    void callAuth(std::string login, std::string password);
    void OnGroupSelected(int groupid);

private slots:
    void slotReadyRead();
private:
    static inline MainWindow* window;

    QMutex* mMutex;
    void serverSettingsInit();
    std::string userLogin;

    template <typename... T> void static CallRemoteFunctionIntoClient(std::string functionName, T ...args);

    static void test(Server* instance, std::vector<std::string> args);
    static void onAuthSuccess(Server* instance, std::vector<std::string> args);
    static void onMessagesInfoReceived(Server* instance, std::vector<std::string> args);
    std::map<std::string, std::function<void(Server*, std::vector<std::string>)>> functions = {
        {"test", test},
        {"onAuthSuccess", onAuthSuccess},
        {"onMessagesInfoReceived", onMessagesInfoReceived}
    };

signals:
    void messageReceived(QString username, quint16 groupid, const QString str);
    void authSuccessed();
    void groupReceived(int groupid, std::string groupname);
    void messagesReceived(std::string sender, std::string message);
};

#endif 