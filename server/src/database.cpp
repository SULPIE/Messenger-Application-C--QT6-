#include "database.h"

void DatabaseQuery::Execute(std::string queryText)
{
	try
	{
		pqxx::work worker(connectionObject);
		worker.exec(queryText);
		worker.commit();
	}
	catch (const std::exception& e)
	{
		spdlog::critical(e.what());
	}
}

pqxx::result DatabaseQuery::ExecuteWithResult(std::string queryText)
{
	try 
	{
		pqxx::work worker(connectionObject);
		return worker.exec(queryText);
	}
	catch (const std::exception& e)
	{
		spdlog::critical(e.what());
	}
}
