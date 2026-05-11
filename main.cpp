#include "ExpenseManager.h"

int main(){
    ExpenseManager app;

    app.loadData(); //load saved users and groups from files
    app.mainMenu(); //start the app
}