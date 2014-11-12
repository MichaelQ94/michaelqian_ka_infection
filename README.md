Compiling and running
-----------
    g++ -o total_infection total_infection.cpp user.cpp
    total_infection
or  

    g++ -o limited_infection limited_infection.cpp user.cpp
    limited_infection

Instructions
-----------
* `total_infection`
	1. The program will prompt for a text file containing user information (e.g., test1.txt). This will be used to construct the list of users.
	2. When all of the user information is read in, the program will display the information
	3. The program will then prompt for the id of a user to infect
	4. After infecting the selected user and allowing the infection to spread, the program will list the id's of all infected users
* `limited_infection`
	1. The program will prompt for a text file containing user information (e.g., test1.txt). This will be used to construct the list of users.
	2. When all of the user information is read in, the program will display the information
	3. The program will also display the list of all islands (see section "islands" in the notes below). Each island will have:
		* an identification number
		* a number in parenthesis indicating the number of users in the island
		* the list of users in the island
	4. The program will now prompt the user for an island to infect. **This indirectly implements the optional method of infecting _exactly_ a given number of users since whoever is running the program can simply refrain from choosing an island if none of them have the desired size.**
	5. Finally, the program will infect one of the users in the selected island, allow the infection to propagate to the other users in the island, and then display the list of infected users

Example run of total_infection:  
	
	Please enter the name of the file containing the users' information:
	test1.txt

	Users:
	1: 2,
	2: 1, 3,
	3: 2,
	4: 5,
	5: 4,

	Enter a user id to infect:
	2

	Infected 3 users:
	1, 2, 3,


Example run of limited_infection:

	Please enter the name of the file containing the users' information:
	test1.txt

	Users:
	1: 2,
	2: 1, 3,
	3: 2,
	4: 5,
	5: 4,

	User islands:
	1 (3): 1, 2, 3,
	2 (2): 4, 5,

	Select an island to infect:
	1

	Infected 3 users:
	1, 2, 3,

Test cases:
-----------

* The program expects input in the form of a text file where each line contains the id number of a user and the id numbers of the users to whom the user is connected
* The id of the user should be the first number, followed by a colon
* Then, the id's of the connected users should be listed with commas and no spaces
* If a user is listed as being connected to another user who doesn't exist in the system, that user will be created
* For example, to indicate that user 1 is connected to users 2, 3, 4, and 5:

		1:2,3,4,5

* If any of the users 2, 3, 4, or 5 do not exist in the system at the time that user 1's information is processed, they will be created.

* If the following scenario occurs:
	
		1:2
		2:

* Even though user 2 does not explicitly list user 1 as a connection, the program will infer from user 1's data that the two are connected and both users will list the other as a connection

Notes:
-----------
* User
    * The user class contains:
        * a user id for identification
        * a boolean to indicate infection
        * a boolean to indicate inclusion in an "island" (more on this later)
        * a vector containing the id's of the users to whom this user is connected
    * Since the infection spreads from teacher to student as well as from student to teacher, the connection vector does not distinguish between different types of relationships.

* Infecting users
    * There are 2 methods written for infecting users
    * `infect_user()` is meant to be used on the "seed" user from whom the infection starts
    * `spread_infection()` is called by `infect_user()` as well as recursively by itself in order to propagate the infection across all users who share mutual connections

* "Islands"
    * In order to implement `limited_infection`, I included functionality for grouping mutually-connected users into clusters called "islands" 
    * In essence, if a certain user is chosen to be a seed for an infection, the set of all users to whom the infection could spread is considered to be a single island
    * The methods for generating the islands, `add_to_islands()` and `add_from_seed()`, are nearly identical to the infection methods
    * Islands are used to calculate the size of a potential cluster of users for limited_infection 