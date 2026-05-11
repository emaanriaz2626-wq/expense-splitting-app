#ifndef EXPENSE_H
#define EXPENSE_H

#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

//base class for all expense types
class Expense {
protected:
    string description;
    float amount;
    string paidBy;

public:
    Expense(string d,float a,string p){
        description=d;
        amount=a;
        paidBy=p;
    } 

    virtual void calculate(vector<string>& members,vector<float>& balances)=0; //polymorphism

    virtual void display() const{
        cout<<description<<" - $"<<fixed<<setprecision(2)<<amount<<" paid by "<<paidBy;
    }

    virtual string getType() const=0;

    virtual ~Expense(){} //virtual destructor for safe cleanup
};

#endif