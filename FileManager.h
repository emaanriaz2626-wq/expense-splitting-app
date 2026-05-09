#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fstream>
#include "User.h"
#include "Group.h"

class FileManager {
public:

//Save Users
static void saveUsers(vector<User>& users){
    ofstream file("users.txt");

    for(int i=0;i<users.size();i++){
        file<<users[i].id<<" "<<users[i].username<<" "<<users[i].password<<"\n";
    }

    file.close();
}

//Load Users
static void loadUsers(vector<User>& users){
    ifstream file("users.txt");
    if(!file) return;

    User temp;

    while(file>>temp.id>>temp.username>>temp.password){
        users.push_back(temp);
    }

    file.close();
}

//Save Users
static void saveGroups(vector<Group>& groups){
    ofstream file("groups.txt");

    for(int i=0;i<groups.size();i++){
        file<<groups[i].getName()<<"\n";
        file<<groups[i].getCode()<<"\n";

        vector<string>& m=groups[i].getMembers();

        file<<m.size()<<"\n";

        for(int j=0;j<m.size();j++)
            file<<m[j]<<"\n";
    }

    file.close();
}
//Load Groups
static void loadGroups(vector<Group>& groups){
    ifstream file("groups.txt");
    if(!file) return;

    while(!file.eof()){
        string name,code;
        getline(file,name);

        if(name=="") break;
        getline(file,code);
        Group g(name,code);

        int count;
        file>>count;
        file.ignore();

        for(int i=0;i<count;i++){
            string member;
            getline(file,member);
            g.addMember(member);
        }
        groups.push_back(g);
    }
    file.close();
}
};

#endif