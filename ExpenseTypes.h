#ifndef EXPENSE_TYPES_H
#define EXPENSE_TYPES_H

#include "Expense.h"
#include <stdexcept>

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

        float share=amount/participants.size(); //split evenly among all

        //credit the payer
        for(int i=0;i<members.size();i++)
            if(members[i]==paidBy) balances[i]+=amount;

        //deduct each person's share
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
    :Expense(d,a,p){
        participants=people;percentages=perc;
        float total=0;
        for(int i=0;i<perc.size();i++) total+=perc[i];
        if(total<99.9||total>100.1) throw runtime_error("Percentages must add up to 100"); //validate input
    }

    void calculate(vector<string>& members,vector<float>& balances){

        //credit the payer
        for(int i=0;i<members.size();i++)
            if(members[i]==paidBy) balances[i]+=amount;

        //deduct each person's percentage
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
    :Expense(d,a,p){
        participants=people;exactAmounts=exact;
        float total=0;
        for(int i=0;i<exact.size();i++) total+=exact[i];
        if(total<a-0.01||total>a+0.01) throw runtime_error("Exact amounts must add up to the total"); //validate input
    }

    void calculate(vector<string>& members,vector<float>& balances){

        //credit the payer
        for(int i=0;i<members.size();i++)
            if(members[i]==paidBy) balances[i]+=amount;

        //deduct each person's exact amount
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