#ifndef EXPENSE_MANAGER_H
#define EXPENSE_MANAGER_H

#include "FileManager.h"
#include "Utilities.h"

//handles user interaction
class ExpenseManager{
private:
    vector<User> users;
    vector<Group> groups;
    string currentUser; //who's logged in right now

public:
void loadData(){
    FileManager::loadUsers(users);
    FileManager::loadGroups(groups);
}
void saveData(){
    FileManager::saveUsers(users);
    FileManager::saveGroups(groups);
}

void mainMenu(){ 
    while (true){

        clearScreen();

        printLine('=');
        cout << BOLD << CYAN
             << "  EXPENSE SPLITTER"
             << RESET << "\n";
        printLine('=');

        cout<<CYAN<<"[1] "<<RESET<<"Register\n";
        cout<<CYAN<<"[2] "<<RESET<<"Login\n";
        cout<<CYAN <<"[0] "<<RESET<<"Exit\n";

        int choice = getInt("\nChoice: ", 0, 2);
        switch(choice){

        case 1:{
            registerUser();
            break;
        }
        case 2:{
            if (loginUser()) {
                dashboard();
            }
            break;
        }
        case 0:{
            clearScreen();
            cout<<GREEN<<"Closing App..\n"<<RESET;
            saveData();
            return;
        }
        }
    }
}
//creates a new account
void registerUser(){

    clearScreen();
    string username, password;

    printLine();
    cout << BOLD << "Register\n" << RESET;
    printLine();

    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);
    users.push_back(User(username, password));

    saveData();
    cout << GREEN << "\nAccount created successfully!\n" << RESET;
    pressEnter();
}
//checks credentials and logs in
bool loginUser(){

    clearScreen();
    string username, password;

    printLine();
    cout << BOLD << "Login\n" << RESET;
    printLine();

    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);

    for (int i = 0; i < users.size(); i++) {

        if (users[i].getUsername() == username &&
            users[i].login(password)){
            currentUser = username;

            cout << GREEN << "\nLogin successful!\n" << RESET;
            pressEnter();

            return true;
        }
    }
    cout << RED << "\nInvalid credentials.\n" << RESET;
    pressEnter();

    return false;
}
//menu after login
void dashboard(){

    while (true) {

        clearScreen();

        printLine('=');
        cout<<BOLD<<CYAN<<"  DASHBOARD"<<RESET<<"\n";
        printLine('=');
        cout<<"Logged in as: "<<YELLOW<<currentUser<<RESET <<"\n\n";

        cout<<CYAN<<"[1] "<<RESET<<"Create Group\n";
        cout<<CYAN<<"[2] "<<RESET<<"Join Group\n";
        cout<<CYAN<<"[3] "<< RESET<<"Open Group\n";
        cout<<CYAN<<"[0] "<<RESET<<"Logout\n";

        int choice = getInt("\nChoice: ", 0, 3);

        switch(choice){

        case 1: createGroup(); break;
        case 2: joinGroup(); break;
        case 3: openGroup(); break;
        case 0: return; //logout
        }
    }
}
//sets up a new group with an invite code
void createGroup(){

    clearScreen();
    string name, code;

    printLine();
    cout << BOLD << "Create Group\n" << RESET;
    printLine();

    cout << "Group name: ";
    getline(cin, name);
    cout << "Invite code: ";
    getline(cin, code);

    Group g(name, code);
    g.addMember(currentUser); //creator joins automatically
    groups.push_back(g);

    saveData();
    cout << GREEN << "\nGroup created!\n" << RESET;
    pressEnter();
}
//join an existing group using its code
void joinGroup(){

    clearScreen();
    string code;

    printLine();
    cout << BOLD << "Join Group\n" << RESET;
    printLine();
    cout << "Enter invite code: ";
    getline(cin, code);

    for (int i = 0; i < groups.size(); i++) {
        if (groups[i].getCode() == code) {
            if (!groups[i].memberExists(currentUser)) {
                groups[i].addMember(currentUser);
            }
            saveData();
            cout<<GREEN<<"\nJoined successfully!\n"<<RESET;
            pressEnter();
            return;
        }
    }
    cout<<RED<<"\nInvalid code.\n"<<RESET;
    pressEnter();
}
//pick a group to open
void openGroup() {
    clearScreen();
    if (groups.size() == 0) {
        cout<<RED<<"No groups available.\n"<<RESET;
        pressEnter();
        return;
    }

    printLine();
    cout<<BOLD<<"Groups\n"<<RESET;
    printLine();

    for (int i = 0; i < groups.size(); i++) {
        cout<<CYAN<<"["<<i+1<<"] "<<RESET<<groups[i].getName()<<"\n";
    }

    int choice = getInt("\nSelect group: ", 1, groups.size());
    groupMenu(groups[choice-1]);
}
//inside a specific group
void groupMenu(Group& g){

    while (true){
        clearScreen();

        printLine();
        cout<<BOLD<<g.getName()<<RESET<<"\n";
        printLine();

        cout<<CYAN<<"[1] "<<RESET<<"Add Expense\n";
        cout<<CYAN<<"[2] "<<RESET<<"Show Expenses\n";
        cout<<CYAN<<"[3] "<<RESET<<"Show Balances\n";
        cout<<CYAN<<"[4] "<<RESET<<"Settlement Plan\n";
        cout<<CYAN<<"[5] "<<RESET<<"List Members\n";
        cout<<CYAN<<"[0] "<<RESET<<"Back\n";

        int choice = getInt("\nChoice: ", 0, 5);
        switch (choice){

        case 1: addExpenseToGroup(g); break;
        case 2:{
            clearScreen();
            g.showExpenses();
            pressEnter();
            break;
        }
        case 3:{
            clearScreen();
            g.showBalances();
            pressEnter();
            break;
        }
        case 4:{
            clearScreen();
            g.settleUp();
            pressEnter();
            break;
        }
        case 5:{
            clearScreen();
            g.listMembers();
            pressEnter();
            break;
        }
        case 0:{
            saveData();
            return;
        }
        }
    }
}
//walks user through adding an expense to the group
void addExpenseToGroup(Group& g){

    clearScreen();

    string desc;
    float amount;

    printLine();
    cout << BOLD << "Add Expense\n" << RESET;
    printLine();

    cout << "Description: ";
    getline(cin, desc);
    amount = getFloat("Amount: $");
    vector<string>& members = g.getMembers();

    //ask who paid
    cout << "\nWho paid?\n";
    for (int i = 0; i < members.size(); i++) {
        cout << CYAN << "[" << i + 1 << "] " << RESET
             << members[i] << "\n";
    }

    int payerIndex = getInt("\nChoice: ", 1, members.size());
    string payer = members[payerIndex - 1];

    //choose how to split
    cout<<"\nSplit Type\n";
    cout<<CYAN<<"[1] "<<RESET<<"Equal\n";
    cout<<CYAN<<"[2] "<< RESET<<"Percent\n";
    cout<<CYAN<<"[3] "<<RESET<<"Exact\n";

    int type = getInt("\nChoice: ", 1, 3);

    if (type == 1){
        Expense* e = new EqualExpense(desc, amount, payer, members);
        g.addExpense(e);
    }
    else if(type == 2){
        vector<float> perc;

        //get each person's percentage
        for (int i = 0; i < members.size(); i++)
            perc.push_back(getFloat(members[i] + ": "));

        Expense* e = new PercentExpense(desc, amount, payer, members, perc);
        g.addExpense(e);
    }
    else{
        vector<float> exact;

        //get each person's exact amount
        for (int i = 0; i < members.size(); i++)
            exact.push_back(getFloat(members[i] + ": $"));

        Expense* e = new ExactExpense(desc, amount, payer, members, exact);
        g.addExpense(e);
    }
    saveData();
    cout<<GREEN<<"\nExpense added successfully!\n"<<RESET;
    pressEnter();
}
};
#endif