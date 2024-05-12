#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <spdlog/spdlog.h>
#include <pqxx/pqxx>
#include "server_cfg.h"

class DatabaseQuery
{
private:
	DatabaseQuery& operator=(const DatabaseQuery&) = delete;

	static inline std::string connectionString = std::string("host=") + ServerCfg::getDbHost() +
		std::string(" port=") + ServerCfg::getDbPort() +
		std::string(" dbname=") + ServerCfg::getDbName() +
		std::string(" user=") + ServerCfg::getDbUser() +
		std::string(" password=") + ServerCfg::getDbPassword();

	static inline pqxx::connection connectionObject{ connectionString.c_str() };

public:
	void static Execute(std::string queryText);
	static pqxx::result ExecuteWithResult(std::string queryText);
};

#endif