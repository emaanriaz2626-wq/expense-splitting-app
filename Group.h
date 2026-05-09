#ifndef GROUP_H
#define GROUP_H

#include "ExpenseTypes.h"
#include "Utilities.h"

class Group {
private:
    string groupName;
    string inviteCode;

    vector<string> members;
    vector<Expense*> expenses;

public:
    Group(){}
    Group(string n,string c){
        groupName=n;
        inviteCode=c;
    } 

    string getName() const{
        return groupName;
    }
    string getCode() const{
        return inviteCode;
    }

    void addMember(string name){
        members.push_back(name);
    }

    vector<string>& getMembers(){
        return members;
    } //composition access

    bool memberExists(string name){
        for(int i=0;i<members.size();i++)
            if(members[i]==name) return true;
        return false;
    }
    void listMembers() {
        printLine();
        cout << BOLD << "Members\n" << RESET;
        printLine();

        for (int i = 0; i < members.size(); i++) {
            cout << CYAN << i + 1 << ". " << RESET
                 << members[i] << "\n";
        }
    }
    void addExpense(Expense* e){
        expenses.push_back(e);
    } //polymorphic storage

    vector<Expense*>& getExpenses(){
        return expenses;
    }
    void showExpenses() {
        printLine();
        cout << BOLD << "Expenses\n" << RESET;
        printLine();

        if (expenses.size() == 0){
            cout << DIM << "No expenses yet.\n" << RESET;
            return;
        }

        for (int i = 0; i < expenses.size(); i++){
            cout << i + 1 << ". ";
            expenses[i]->display();
            cout << "\n";
        }
    }

    vector<float> calculateBalances(){
        vector<float> balances(members.size(), 0);

        for (int i = 0; i < expenses.size(); i++) {
            expenses[i]->calculate(members, balances);
        }

        return balances;
    }

    void showBalances() {
        vector<float> balances = calculateBalances();

        printLine();
        cout << BOLD << "Balances\n" << RESET;
        printLine();

        for (int i = 0; i < members.size(); i++){

            cout<<left<<setw(15)<<members[i];

            if (balances[i] > 0){
                cout<<GREEN<<"+$"<<fixed<<setprecision(2)<<balances[i]<<" (gets back)"<<RESET;
            }
            else if (balances[i] < 0){
                cout<<RED<<"-$"<<fixed<<setprecision(2)<<-balances[i]<<" (owes)"<<RESET;
            }
            else{
                cout<<DIM<<"$0.00"<<RESET;
            }
            cout << "\n";
        }
    }

    void settleUp() {
        vector<float> balances = calculateBalances();

        printLine();
        cout<<BOLD<<"Settlement Plan\n"<<RESET;
        printLine();

        bool settled = true;

        for (int i = 0; i < members.size(); i++){
            if (balances[i] < 0){
                for (int j = 0; j < members.size(); j++) {
                    if (balances[j] > 0){
                        float amount = min(-balances[i], balances[j]);

                        if (amount > 0.01){
                            cout<<YELLOW<<members[i]<<RESET<<" pays "<<GREEN<<members[j]<<RESET<<" $"<<fixed<<setprecision(2)<<amount<< "\n";

                            balances[i] += amount;
                            balances[j] -= amount;
                            settled = false;
                        }
                    }
                }
            }
        }
        if (settled){
            cout << GREEN << "Everyone is settled up!\n" << RESET;
        }
    }
    ~Group(){ //cleanup memory
        for(int i=0;i<expenses.size();i++){
            delete expenses[i];
        }
    }
};

#endif