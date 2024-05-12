#include "server_cfg.h"

ServerCfg::ServerCfg()
{
	std::ifstream cfgFile(cfgName);

	if (cfgFile.is_open())
	{
		try
		{
			cfgFile >> cfgFileJson;
			InitDataFromJson();
		}
		catch (nlohmann::json::type_error e)
		{
			spdlog::critical("{0} in {1} file", e.what(), cfgName);
		}
	}
	else
	{
		spdlog::info("Cfg file does not exist. Creating the new one...");
		std::ofstream cfgFileTheNew(cfgName);

		cfgFileJson = {
			{dbHostJsonField, dbHost},
			{dbPortJsonField, dbPort},
			{dbNameJsonField, dbName},
			{dbUserJsonField, dbUser},
			{dbPasswordJsonField, dbPassword},
			{serverPortJsonField, serverPort},
			{serverIpJsonField, serverIp}
		};

		cfgFileTheNew << std::setw(4) << cfgFileJson;

		//std::string s = cfgFileJson;
		//std::cout << s << std::endl;
	}

	spdlog::info("Server started at {0} port, DataBase started at {1} port", getServerPort(), getDbPort());
}

void ServerCfg::InitDataFromJson()
{
	dbHost = cfgFileJson[dbHostJsonField];
	dbPort = cfgFileJson[dbPortJsonField];
	dbName = cfgFileJson[dbNameJsonField];
	dbUser = cfgFileJson[dbUserJsonField];
	dbPassword = cfgFileJson[dbPasswordJsonField];
	serverPort = cfgFileJson[serverPortJsonField];
	serverIp = cfgFileJson[serverIpJsonField];
}