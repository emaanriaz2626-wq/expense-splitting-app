#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>

using namespace std;

class FileManager;

class User{
private:
    int id;
    string username;
    string password;

    static int nextId;

public:
    User();
    User(string u,string p); 

    string getUsername() const; //getter
    bool login(string pass) const; //auth check
    int getId() const; //returns ID

    friend class FileManager; //file access permission
};

#endif