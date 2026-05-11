#include "User.h"

int User::nextId=1;

User::User(){}

User::User(string u,string p){ //assign id and store credentials
    id=nextId++;
    username=u;
    password=p;
}

string User::getUsername() const{
    return username;}
bool User::login(string pass) const{
    return password==pass;}
int User::getId() const{
    return id;}