#include "server.h"
#include <iostream>

Server::Server()
{
	if (this->listen(QHostAddress::Any, 2004))
	{
		spdlog::info("Server succesfully started");
	}
	else
	{
		spdlog::error(this->errorString().toStdString());
        return;
	}

	/*pqxx::result response = DatabaseQuery::ExecuteWithResult("SELECT * FROM test");

	for (size_t i = 0; i < response.size(); i++)
	{
		spdlog::info("{0} count is {1}", pqxx::to_string(response[i][1]), pqxx::to_string(response[i][0]));
	}*/
}

void Server::incomingConnection(qintptr socketDescriptor)
{
	socket = new QTcpSocket;
	socket->setSocketDescriptor(socketDescriptor);

	connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
	connect(socket, &QTcpSocket::disconnected, this, &Server::disconnectedFromHost);
	//connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

	Sockets.push_back(socket);

	spdlog::info("Client has been connected: {}", socketDescriptor);
}

void Server::disconnectedFromHost()
{
	socket = (QTcpSocket*)sender();
	Sockets.erase(std::remove(Sockets.begin(), Sockets.end(), socket));
	spdlog::info("{} has disconnected", users[socket]->getUsername());
	delete(users[socket]);
	users.erase(socket);
	socket = nullptr;
}

void Server::slotReadyRead()
{
	socket = (QTcpSocket*)sender();
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_6_2);

	if (in.status() != QDataStream::Ok) {
		spdlog::error("some error during reading data from client");
		return;
	}

	in.startTransaction();

	QString nextFortune;
	QString flag;
	QString nameofSender;
	qint16 groupid;
	in >> flag >> nameofSender >> groupid >> nextFortune;

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
		call(socket, funcname, args);
	}
	else if (flag == "FLAG_MESSAGE")
	{
		SendMessageToClient(nameofSender, groupid, nextFortune.toStdString());
		DatabaseQuery::Execute(std::format("INSERT INTO messages(message, groupid, receiver) VALUES ('{0}', '{1}', '{2}')", nextFortune.toStdString(), groupid, nameofSender.toStdString()));
	}
}

template<typename ...T>
void Server::CallRemoteFunctionIntoClient(QTcpSocket* socket, std::string functionName, T ...args)
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


void Server::SendDialogInfoToClient(QTcpSocket* clientSocket)
{
	nlohmann::json groupsdata;
	QString jsonString;

	pqxx::result responsegroupid = DatabaseQuery::ExecuteWithResult(std::format("SELECT * FROM group_members WHERE userid={0}", users[clientSocket]->getUserid()));
	std::cout << users[clientSocket]->getUserid() << std::endl;

	for (size_t i = 0; i < responsegroupid.size(); i++)
	{
		pqxx::result responsegroupname = DatabaseQuery::ExecuteWithResult(std::format("SELECT * FROM groups WHERE id={0}", pqxx::to_string(responsegroupid[i][1])));
		groupsdata[pqxx::to_string(responsegroupid[i][1])] = pqxx::to_string(responsegroupname[i][1]);
		std::cout << pqxx::to_string(responsegroupid[i][1]) << std::endl;
	}

	jsonString = QString::fromStdString(std::string(groupsdata.dump()));

	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_6_2);

	QString flag("FLAG_RECEIVE_DIALOG_TYPE");

	out << flag;
	out << QString("");
	out << quint16(0);
	out << jsonString;

	clientSocket->write(Data);
}

void Server::SendMessageToClient(QString nameofsender, quint16 groupid, std::string message)
{
	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_6_2);

	QString flag("FLAG_MESSAGE");

	out << flag;
	out << nameofsender;
	out << groupid;
	out << QString::fromStdString(message);

	for (int i = 0; i < Sockets.size(); i++)
	{
		pqxx::result response = DatabaseQuery::ExecuteWithResult(std::format("SELECT * FROM group_members WHERE groupid={0} AND userid={1}", groupid, users[Sockets[i]]->getUserid()));
		for (size_t idx = 0; idx < response.size(); idx++)
		{
			Sockets[i]->write(Data);
		}
	}
}

void Server::call(QTcpSocket* sender, std::string functionName, std::vector<std::string> args)
{
	try {
		functions[functionName](sender, args);
	}
	catch (const std::exception& e) {
		spdlog::error(e.what());
	}
}

//==========================Methods To Call From Client======================================
void Server::test(QTcpSocket* sender, std::vector<std::string> args)
{
	for (auto start = args.begin(); start != args.end(); start++) {
		spdlog::info(*start);
	}

	spdlog::info("{} has entered to test function", sender->socketDescriptor());
}

void Server::onUserAuth(QTcpSocket* sender, std::vector<std::string> args)
{
	std::string query = std::format("SELECT * FROM users WHERE login='{0}' AND password='{1}'", args[0], args[1]);
	pqxx::result response = DatabaseQuery::ExecuteWithResult(query);

	int userid{};

	for (size_t i = 0; i < response.size(); i++)
	{
		CallRemoteFunctionIntoClient(sender, "onAuthSuccess");
		userid = std::stoi(pqxx::to_string(response[i][0]));

		User* _newuser = new User(args[0], args[1], userid);
		users[sender] = _newuser;

		spdlog::info("{} succesfully authorized", _newuser->getUsername());
		//SendDialogInfoToClient(sender);
		break;
	}
}

void Server::queryToInitGroupData(QTcpSocket* sender, std::vector<std::string> args)
{
	SendDialogInfoToClient(sender);
}

void Server::onGroupSelect(QTcpSocket* sender, std::vector<std::string> args)
{
	std::string query = std::format("SELECT * FROM messages WHERE groupid='{0}'", args[0]);
	pqxx::result response = DatabaseQuery::ExecuteWithResult(query);

	nlohmann::json messages;

	for (size_t i = 0; i < response.size(); i++)
	{
		messages[std::to_string(i)]["message"] = pqxx::to_string(response[i][1]);
		messages[std::to_string(i)]["sender"] = pqxx::to_string(response[i][3]);
	}

	CallRemoteFunctionIntoClient(sender, "onMessagesInfoReceived", messages.dump());
}
