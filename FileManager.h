#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fstream>
#include <stdexcept>
#include "User.h"
#include "Group.h"

//handles reading and writing data to text files
class FileManager {
public:

//write all users to file
static void saveUsers(vector<User>& users){
    try{
        ofstream file("users.txt");
        if(!file) throw runtime_error("Cannot open users.txt for writing");

        for(int i=0;i<users.size();i++){
            file<<users[i].id<<" "<<users[i].username<<" "<<users[i].password<<"\n";
        }

        file.close();
    }
    catch(exception& e){
        cout<<RED<<"Error saving users: "<<e.what()<<"\n"<<RESET;
    }
}

//read all users from file
static void loadUsers(vector<User>& users){
    try{
        ifstream file("users.txt");
        if(!file) return; //first run, no file yet

        User temp;

        while(file>>temp.id>>temp.username>>temp.password){
            users.push_back(temp);
        }

        file.close();
    }
    catch(exception& e){
        cout<<RED<<"Error loading users: "<<e.what()<<"\n"<<RESET;
    }
}

//write all groups to file
static void saveGroups(vector<Group>& groups){
    try{
        ofstream file("groups.txt");
        if(!file) throw runtime_error("Cannot open groups.txt for writing");

        for(int i=0;i<groups.size();i++){
            file<<groups[i].getName()<<"\n";
            file<<groups[i].getCode()<<"\n";

            vector<string>& m=groups[i].getMembers();

            file<<m.size()<<"\n"; //how many members

            for(int j=0;j<m.size();j++)
                file<<m[j]<<"\n";
        }

        file.close();
    }
    catch(exception& e){
        cout<<RED<<"Error saving groups: "<<e.what()<<"\n"<<RESET;
    }
}
//read all groups from file
static void loadGroups(vector<Group>& groups){
    try{
        ifstream file("groups.txt");
        if(!file) return; //first run, no file yet

        while(!file.eof()){
            string name,code;
            getline(file,name);

            if(name=="") break; //end of file
            getline(file,code);
            Group g(name,code);

            int count;
            file>>count;
            file.ignore(); //skip newline after number

            for(int i=0;i<count;i++){
                string member;
                getline(file,member);
                g.addMember(member);
            }
            groups.push_back(g);
        }
        file.close();
    }
    catch(exception& e){
        cout<<RED<<"Error loading groups: "<<e.what()<<"\n"<<RESET;
    }
}
};

#endif