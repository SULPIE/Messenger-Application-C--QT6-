#ifndef CALLREMOTE_H
#define CALLREMOTE_H

#include <iostream>
#include <map>
#include <string>
#include <functional>
#include "spdlog/spdlog.h"
#include <QObject>

class FuncCaller : public QObject
{
	Q_OBJECT

public slots:
	void call(std::string functionName, std::vector<std::string> args);
public:
	static void test(std::vector<std::string> args);

	std::map<std::string, std::function<void(std::vector<std::string>)>> functions {
		{"test", test}
	};
};

#endif
