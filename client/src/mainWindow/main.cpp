#include <QApplication>
#include "authwindow.h"
#include <qTcpServer>
#include <iostream>
#include <qTcpSocket>
#include <QThread>
#include <qcoreapplication.h>
#include "serverqueries.h"
#include <QSharedMemory>

int main(int argc, char *argv[])
{

	QApplication app(argc, argv);
	QSharedMemory global("HghasgsdahSHhdhs32423");
	if (!global.create(1))
		return 1;

	setlocale(LC_ALL, "Russian");

	Server* server = new Server;
	QThread* thread = new QThread{};
	thread->start();
	QObject::connect(thread, &QThread::started, [&]() {server->ConnectToHost();});
	server->moveToThread(thread);

	AuthWindow* authWindow = new AuthWindow(server);
	authWindow->show();

	return app.exec();
}

