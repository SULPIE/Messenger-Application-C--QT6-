//#include <qTcpServer>
#include <iostream>
#include <QCoreApplication>
#include <qtcpserver>
#include "server.h"
#include "server_cfg.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");

    QCoreApplication a(argc, argv);

    ServerCfg cfg;
    Server server;

    return a.exec();
}