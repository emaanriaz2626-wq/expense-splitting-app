#ifndef EXPENSE_TYPES_H
#define EXPENSE_TYPES_H

#include "Expense.h"

//colors for CLI
#define RESET "\033[0m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
#define YELLOW "\033[33m"

//──────────── Equal Expense ────────────
class EqualExpense:public Expense {
private:
    vector<string> participants;

public:
    EqualExpense(string d,float a,string p,vector<string> people):Expense(d,a,p){participants=people;}

    void calculate(vector<string>& members,vector<float>& balances){

        float share=amount/participants.size(); //equal split logic

        for(int i=0;i<members.size();i++)
            if(members[i]==paidBy) balances[i]+=amount;

        for(int i=0;i<participants.size();i++)
            for(int j=0;j<members.size();j++)
                if(participants[i]==members[j]) balances[j]-=share;
    }

    void display() const override {
        cout<<CYAN<<"[Equal] "<<RESET;
        Expense::display();
    }

    string getType() const override {return "Equal";}
};

//──────────── Percent Expense ────────────
class PercentExpense:public Expense {
private:
    vector<string> participants;
    vector<float> percentages;

public:
    PercentExpense(string d,float a,string p,vector<string> people,vector<float> perc)
    :Expense(d,a,p){participants=people;percentages=perc;}

    void calculate(vector<string>& members,vector<float>& balances){

        for(int i=0;i<members.size();i++)
            if(members[i]==paidBy) balances[i]+=amount;

        for(int i=0;i<participants.size();i++){
            float share=amount*percentages[i]/100;

            for(int j=0;j<members.size();j++)
                if(participants[i]==members[j]) balances[j]-=share;
        }
    }

    void display() const{
        cout<<MAGENTA<<"[Percent] "<<RESET;
        Expense::display();
    }

    string getType() const{
        return "Percent";
    }
};

//──────────── Exact Expense ────────────
class ExactExpense:public Expense {
private:
    vector<string> participants;
    vector<float> exactAmounts;

public:
    ExactExpense(string d,float a,string p,vector<string> people,vector<float> exact)
    :Expense(d,a,p){participants=people;exactAmounts=exact;}

    void calculate(vector<string>& members,vector<float>& balances){

        for(int i=0;i<members.size();i++)
            if(members[i]==paidBy) balances[i]+=amount;

        for(int i=0;i<participants.size();i++)
            for(int j=0;j<members.size();j++)
                if(participants[i]==members[j]) balances[j]-=exactAmounts[i];
    }

    void display() const{
        cout<<YELLOW<<"[Exact] "<<RESET;
        Expense::display();
    }

    string getType() const{
        return "Exact";
    }
};

#endif