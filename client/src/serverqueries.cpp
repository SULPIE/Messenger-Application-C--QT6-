#include "serverqueries.h"
#include <QTime>

Server::Server()
{
	/*socket->connectToHost(QHostAddress("127.0.0.1"), 2004);
	if (socket->waitForConnected(1000))
		spdlog::error("Connected!");*/

	mMutex = new QMutex;

	QThread* thread = new QThread{};
	thread->start();
	window = new MainWindow;
	window->moveToThread(thread);
	connect(window, &MainWindow::messageSended, this, &Server::SendToServer);
	connect(this, &Server::messageReceived, window, &MainWindow::ShowMessage);
	connect(this, &Server::groupReceived, window, &MainWindow::ShowGroup);
	connect(window, &MainWindow::groupSelected, this, &Server::OnGroupSelected);
	connect(this, &Server::messagesReceived, window, &MainWindow::onReceiveMessages);
}

void Server::serverSettingsInit()
{
	socket = new QTcpSocket(this);
	connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
	connect(socket, &QTcpSocket::disconnected, this, &Server::disconnectedFromHost);
}

void Server::SendToServer(QString sendername, int groupid, const QString &message)
{
	QString flag("FLAG_MESSAGE");
	try
	{
		Data.clear();
		QDataStream out(&Data, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_6_2);
		qint16 qgroupid = groupid;
		out << flag << sendername << qgroupid << message;
		socket->write(Data);
	}
	catch (const std::exception& ex)
	{
		spdlog::error(ex.what());
	}
}

template<typename ...T>
void Server::CallRemoteFunctionIntoClient(std::string functionName, T ...args)
{
	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_6_2);

	nlohmann::json function;
	QString jsonString;
	int argCounter = 1;
	QString flag("FLAG_REMOTE_CALL_FUNCTION");

	function["0"] = functionName;

	([&]
		{
			function[std::to_string(argCounter)] = args;
			argCounter++;
		}(), ...);

	jsonString = QString::fromStdString(std::string(function.dump()));
	out << flag;
	out << QString("");
	out << quint16(0);
	out << jsonString;

	socket->write(Data);
}

void Server::disconnectedFromHost() 
{
	if (socket->state() == QTcpSocket::UnconnectedState)
	{
		spdlog::info("Disconnected");
		socket = nullptr;
		ConnectToHost();
	}
}

void Server::ConnectToHost()
{
	serverSettingsInit();
	socket->connectToHost(QHostAddress("127.0.0.1"), 2004);
	if (!socket->waitForConnected(5000)) {
		spdlog::info("Connection failed retrying...");
		if (socket->state() == QTcpSocket::UnconnectedState)
		{
			ConnectToHost();
		}
	}
	else
	{
		spdlog::info("Succesfully connected");
	}
}

void Server::slotReadyRead()
{
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_6_2);

	if (in.status() != QDataStream::Ok) {
		spdlog::error("some error during reading data from client");
		return;
	}

	in.startTransaction();

	QString nextFortune;
	QString flag;
	QString username;
	quint16 groupid;
	in >> flag >> username >> groupid >> nextFortune;

	if (!in.commitTransaction())
		return;

	nlohmann::json jsonstring;
	std::vector<std::string> args;
	std::string funcname;

	if (flag == "FLAG_REMOTE_CALL_FUNCTION")
	{
		jsonstring = nlohmann::json::parse(nextFortune.toStdString());

		for (int i = 1; i < jsonstring.size(); i++) {
			args.push_back(jsonstring[std::to_string(i)]);
		}

		funcname = jsonstring["0"];
		call(funcname, args);

		//std::cout << flag.toStdString() << nextFortune.toStdString() << std::endl;
	}
	else if (flag == "FLAG_MESSAGE")
	{
		std::cout << groupid << nextFortune.toStdString() << std::endl;
		emit messageReceived(username, groupid, nextFortune);
	}
	else if (flag == "FLAG_RECEIVE_DIALOG_TYPE")
	{
		jsonstring = nlohmann::json::parse(nextFortune.toStdString());
		std::cout << jsonstring << std::endl;

		for (auto& el : jsonstring.items())
		{
			emit groupReceived(std::stoi(el.key()), el.value());
			std::cout << el.key() << el.value() << std::endl;
		}
	}
}

void Server::call(std::string functionName, std::vector<std::string> args)
{
	try {
		functions[functionName](this, args);
	}
	catch (const std::exception& e) {
		spdlog::error(e.what());
	}
}

//==========================Slots to call from other QObjects===================

void Server::callAuth(std::string login, std::string password)
{
	CallRemoteFunctionIntoClient("onUserAuth", login, password);
}

void Server::OnGroupSelected(int groupid)
{
	CallRemoteFunctionIntoClient("onGroupSelect", std::to_string(groupid));
}

//==========================functions to call from the server===================

void Server::test(Server* instance, std::vector<std::string> args)
{
	for (auto start = args.begin(); start != args.end(); start++) {
		spdlog::info(*start);
	}

	spdlog::info("you have entered to test function");
}

void Server::onAuthSuccess(Server* instance, std::vector<std::string> args)
{
	window->show();
	spdlog::info("Auth succesed");
	emit instance->authSuccessed();
	CallRemoteFunctionIntoClient("queryToInitGroupData");
}

void Server::onMessagesInfoReceived(Server* instance, std::vector<std::string> args)
{
	nlohmann::json messages = nlohmann::json::parse(args[0]);

	for (int i = 0; i < messages.size(); i++) {
		emit instance->messagesReceived(messages[std::to_string(i)]["sender"], messages[std::to_string(i)]["message"]);
	}
}
