#include <cstdlib>
#include <climits>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include "user.h"

using namespace std;

void find_islands(vector< set<int> > &islands, map<int, User> &users);
//fill the set of islands with user id's by searching through all users

void add_to_islands(User &user, vector< set<int> > &islands,
	map<int, User> &users);
//if the user has not already been accounted for, create a new island
//and add the user as well as all of the user's connections to it

void add_from_seed(int user_id, int source, set<int> &island,
	map<int, User> &users);
//recursively add users to a given island by searching through the
//users' connections

void infect_user(int user_id, map<int, User> &users);
//infect the user with the specified id
//pass the map of all users to spread the infection

void spread_infection(int user_id, int source, map<int, User> &users);
//recursively spread the infection from a source user to a new user
//pass the map of all users to further spread the infection

void parse_file(map<int, User> &users);
//read in user data from the provided text file

int main()
{
	map<int, User> users;
	/*
	Maps the id of a user to the corresponding User object.
	Since each user contains a vector of the user id's
	to which it is connected, this is also functions as the 
	adjacency list for the network of users.
	*/

	vector< set<int> > islands;
	/*
	Each set within islands contains the id's of all users which are
	mutually connected.
	*/

	//read in user information
	parse_file(users);

	//generate islands
	find_islands(islands, users);

	//display user information
	cout << endl << "Users:" << endl;
	for(map<int, User>::iterator it = users.begin(); 
		it != users.end(); ++it)
	{
		cout << it->second.id() << ": ";
		for(int i = 0; i < it->second.num_connections(); ++i)
		{
			cout << it->second.connections_index(i) << ", ";
		}
		cout << endl;
	}
	
	//display island information
	cout << endl << "User islands:" << endl;

	for(unsigned int i = 0; i < islands.size(); ++i)
	{
		cout << (i+1) <<" (" << islands[i].size() << "): ";
		for(set<int>::iterator it = islands[i].begin();
			it != islands[i].end(); ++it)
		{
			cout << *it << ", ";
		}
		cout << endl;
	}

	//prompt for infection
	unsigned int infect_island;
	cout << endl << "Select an island to infect:" << endl;
	while(!(cin >> infect_island) || infect_island < 1
		|| infect_island > islands.size())
	{
		cout << "Invalid island number. Please try again:" << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
	}

	infect_user(*(islands[infect_island-1].begin()), users);

	//display infected users
	cout << endl << "Infected " << islands[infect_island-1].size()
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

void find_islands(vector< set<int> > &islands, map<int, User> &users)
{
	for(map<int, User>::iterator user_it = users.begin();
		user_it != users.end(); ++user_it)
	{
		if(!(user_it->second.found()))
		{
			add_to_islands(user_it->second, islands, users);
		}
	}
}

void add_to_islands(User &user, vector< set<int> > &islands, 
	map<int, User> &users)
{
	user.set_found(true);	

	islands.push_back(set<int>());
	islands[islands.size()-1].insert(user.id());

	for(int i = 0; i < user.num_connections(); ++i)
	{
		add_from_seed(user.connections_index(i), user.id(),
			islands[islands.size()-1], users);
	}

}

void add_from_seed(int user_id, int source, set<int> &island, 
	map<int, User> &users)
{

	map<int, User>::iterator user_it = users.find(user_id);

	//stop of the user has already been scanned (base case)
	if(user_it->second.found())
	{
		return;
	}

	user_it->second.set_found(true);

	island.insert(user_id);

	for(int i = 0; i < user_it->second.num_connections(); ++i)
	{
		if(user_it->second.connections_index(i) != source)
		{
			add_from_seed(user_it->second.connections_index(i), user_id,
				island, users);
		}
	}
}

void infect_user(int user_id, map<int, User> &users)
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

		//spread the infection to all of the user's connections
		for(int i = 0; i < user_it->second.num_connections(); ++i)
		{
			spread_infection(user_it->second.connections_index(i), 
				user_it->second.id(), users);
		}
	}
}

void spread_infection(int user_id, int source, map<int, User> &users)
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

		//spread the infection to all of the user's connections
		for(int i = 0; i < user_it->second.num_connections(); ++i)
		{
			//no need to infect the user that the infection is coming from
			if(user_it->second.connections_index(i) != source)
			{
				spread_infection(user_it->second.connections_index(i), 
					user_it->second.id(), users);
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
