#ifndef USER_H
#define USER_H

#include <iostream>

class User
{
private:
	std::string username;
	std::string password;
	int userid;

public:
	User(std::string _username, std::string _password, int _userid);

	std::string getUsername() { return username; };
	std::string getPassword() { return password; };
	int getUserid() { return userid; };
};

#endif //USER_H