#ifndef SERVER_CFG_H
#define SERVER_CFG_H

#include <nlohmann/json.hpp>
#include <iostream>
#include "spdlog/spdlog.h"
#include <fstream>

class ServerCfg
{
private:
	static inline std::string cfgName = "servercfg.json";
	static inline nlohmann::json cfgFileJson;

	static inline std::string dbHostJsonField = "DbHost";
	static inline std::string dbPortJsonField = "DbPort";
	static inline std::string dbNameJsonField = "DbName";
	static inline std::string dbUserJsonField = "DbUser";
	static inline std::string dbPasswordJsonField = "DbPassword";
	static inline std::string serverPortJsonField = "ServerPort";
	static inline std::string serverIpJsonField = "ServerIp";

	static inline std::string dbHost = "localhost";
	static inline std::string dbPort = "8888";
	static inline std::string dbName = "postgres";
	static inline std::string dbUser = "postgres";
	static inline std::string dbPassword = "123";
	static inline std::string serverPort = "2004";
	static inline std::string serverIp;

    void InitDataFromJson();

public:
	ServerCfg();

	static const std::string getDbHost() { return dbHost; }
	static const std::string getDbPort() { return dbPort; }
	static const std::string getDbName() { return dbName; }
	static const std::string getDbUser() { return dbUser; }
	static const std::string getDbPassword() { return dbPassword; }
	static const std::string getServerPort() { return serverPort; }
	static const std::string getServerIp() { return serverIp; }
};

#endif