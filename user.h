#ifndef USER_H
#define USER_H

#include <vector>

class User
{
private:
	int id_;
	bool infected_;
	bool found_;
	std::vector<int> connections_;

public:
	User();
	//default constructor

	User(int id);
	//initialize with id

	int id();
	//accessor for the user id number

	void infect();
	//infect the user

	bool infected();
	//check to see if the user is already infected

	void set_found(bool found);

	bool found();

	void add_connection(int user_id);
	//add a connection

	bool connected_to(int user_id) const;
	//test to see if this user is connected to the user
	//with the given id

	int num_connections();
	//return the number of connections

	int connections_index(int index);
	//return the id stored at connections_[index]
};

#endif
