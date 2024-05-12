#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <spdlog/spdlog.h>
#include <pqxx/pqxx>
#include <database.h>
#include <nlohmann/json.hpp>
#include <QThread>
#include "user.h"

class Server : public QTcpServer
{
	Q_OBJECT

public:
	Server();
	inline static QTcpSocket* socket = nullptr;

private:
	inline static QVector <QTcpSocket*> Sockets;
	inline static QByteArray Data;
	quint16 messageBlockSize = 0;

	inline static std::map<QTcpSocket*, User*> users;

	template <typename... T> void static CallRemoteFunctionIntoClient(QTcpSocket* socket, std::string functionName, T ...args);
	void SendMessageToClient(QString nameofsender, quint16 groupid, std::string message);
	static void SendDialogInfoToClient(QTcpSocket* clientSocket);
	void call(QTcpSocket* sender, std::string functionName, std::vector<std::string> args);

	//Methods to call from client
	static void test(QTcpSocket* sender, std::vector<std::string> args);
	static void onUserAuth(QTcpSocket* sender, std::vector<std::string> args);
	static void queryToInitGroupData(QTcpSocket* sender, std::vector<std::string> args);
	static void onGroupSelect(QTcpSocket* sender, std::vector<std::string> args);

	std::map<std::string, std::function<void(QTcpSocket*, std::vector<std::string>)>> functions {
		{"test", test},
		{"onUserAuth", onUserAuth},
		{"queryToInitGroupData", queryToInitGroupData},
		{"onGroupSelect", onGroupSelect}
	};

public slots:
	void incomingConnection(qintptr socketDescriptor);
	void slotReadyRead();
	void disconnectedFromHost();

signals:
	void functionCalled(std::string functionname, std::vector<std::string> args);
};
