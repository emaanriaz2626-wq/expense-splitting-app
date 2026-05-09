#include "ExpenseManager.h"

int main(){
    ExpenseManager app;

    app.loadData(); //load saved users + groups
    app.mainMenu(); 
}