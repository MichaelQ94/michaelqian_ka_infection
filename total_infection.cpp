#include <cstdlib>
#include <climits>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "user.h"

using namespace std;

void infect_user(int user_id, map<int, User> &users,
	int &infect_count);
//infect the user with the specified id
//pass the map of all users to spread the infection

void spread_infection(int user_id, int source, map<int, 
	User> &users, int &infect_count);
//spread the infection from a source user to a new user
//pass the map of all users to further spread the infection

void parse_file(map<int, User> &users);
//read in user data from the provided text file

int main()
{
	map<int, User> users;
	/*
	Maps the id of a User to the corresponding User object.
	Since each User contains a vector of the User objects
	to which it is connected, this is also the adjacency list
	for the network of users.
	*/

	//read in user information
	parse_file(users);

	cout << endl << "Users:" << endl;
	for(map<int, User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		cout << it->second.id() << ": ";
		for(int i = 0; i < it->second.num_connections(); ++i)
		{
			cout << it->second.connections_index(i) << ", ";
		}
		cout << endl;
	}

	//prompt for infection
	int infect_id;
	cout << endl << "Enter a user id to infect:" << endl;
	while(!(cin >> infect_id) || users.find(infect_id) == users.end())
	{
		cout << "Invalid id. Please try again:" << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
	}

	int infect_count = 0;
	infect_user(infect_id, users, infect_count);

	//display infected users
	cout << endl << "Infected " << infect_count
		<< " users:" << endl;

	for(map<int, User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if(it->second.infected())
		{
			cout << it->second.id() << ", ";
		}
	}
	cout << endl;

	return 0;
}

void infect_user(int user_id, map<int, User> &users,
	int &infect_count)
{
	map<int, User>::iterator user_it = users.find(user_id);

	//make sure the user exists
	if(user_it != users.end())
	{
		//stop if the user is already infected (base case)
		if(user_it->second.infected())
		{
			return;
		}

		user_it->second.infect();
		++infect_count;

		//spread the infection to all of the user's connections
		for(int i = 0; i < user_it->second.num_connections(); ++i)
		{
			spread_infection(user_it->second.connections_index(i), 
				user_it->second.id(), users, infect_count);
		}
	}
}

void spread_infection(int user_id, int source, map<int, 
	User> &users, int &infect_count)
{
	map<int, User>::iterator user_it = users.find(user_id);

	if(user_it != users.end())
	{
		//stop if the user is already infected (base case)
		if(user_it->second.infected())
		{
			return;
		}

		user_it->second.infect();
		++infect_count;

		//spread the infection to all of the user's connections
		for(int i = 0; i < user_it->second.num_connections(); ++i)
		{
			//no need to infect the user that the infection is coming from
			if(user_it->second.connections_index(i) != source)
			{
				spread_infection(user_it->second.connections_index(i), 
					user_it->second.id(), users, infect_count);
			}
		}
	}
}

void parse_file(map<int, User> &users)
{
	//read in file name
	string filename;

	cout << "Please enter the name of the file containing the users\' "
		<< "information:" << endl;
	cin >> filename;

	//open the file
	ifstream inputfile(filename.c_str());

	//if the file failed to open, reprompt for filename
	while(!inputfile)
	{
		inputfile.clear();
		cout << "Error opening file, please try again:" << endl;
		cin >> filename;
		inputfile.open(filename.c_str());
	}

	//read in data from the file
	stringstream extraction_buffer;
	string inputline, input;
	int input_id;
	map<int, User>::iterator new_user;
	map<int, User>::iterator other_user;

	while(getline(inputfile, inputline))
	{
		extraction_buffer << inputline;

		while(getline(extraction_buffer, input, ':'))
		{
			input_id = atoi(input.c_str());

			//create/locate the user with the specified id
			new_user = (users.insert(pair<int, User>(input_id, 
			User(input_id)))).first;

			while(getline(extraction_buffer, input, ','))
			{
				input_id = atoi(input.c_str());

				new_user->second.add_connection(input_id);

				//add the connection for the other user as well
				other_user = users.find(input_id);
				if(other_user != users.end())
				{
					other_user->second.add_connection(new_user->second.id());
				}
				else
				{
					//if the other user doesn't exist, create it
					User user(input_id);
					user.add_connection(new_user->second.id());
					users.insert(pair<int, User>(input_id, user));
				}
			}
		}

		extraction_buffer.str(string());
		extraction_buffer.clear();
	}
}
