#include "user.h"
using namespace std;

User::User() : infected_(false), found_(false)
{

}

User::User(int id) : id_(id), infected_(false), found_(false)
{

}

int User::id()
{
	return id_;
}

void User::infect()
{
	infected_ = true;
}

bool User::infected()
{
	return infected_;
}

void User::set_found(bool found)
{
	found_ = found;
}

bool User::found()
{
	return found_;
}

void User::add_connection(int user_id)
{
	if(!connected_to(user_id))
	{
		connections_.push_back(user_id);
	}
}

bool User::connected_to(int user_id) const
{
	for(unsigned int i = 0; i < connections_.size(); ++i)
	{
		if(connections_[i] == user_id)
		{
			return true;
		}
	}

	return false;
}

int User::num_connections()
{
	return connections_.size();
}

int User::connections_index(int index)
{
	return connections_[index];
}
