#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>

using namespace std;

class FileManager; 

//represents a single user account
class User{
private:
    int id;
    string username;
    string password;

    static int nextId; 

public:
    User();
    User(string u,string p); 

    string getUsername() const;
    bool login(string pass) const; //checks if password matches
    int getId() const;

    friend class FileManager; //lets FileManager access private members for file i/o
};

#endif