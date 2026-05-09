#ifndef UTILITIES_H
#define UTILITIES_H

#include<iostream>
#include<limits>

using namespace std;

//Colors
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

void clearScreen() {
    cout<<"\033[2J\033[H";
}
void printLine(char c = '-', int width = 60) {
    cout<<DIM;
    for(int i = 0; i<width; i++) cout<<c;
    cout<<RESET<<"\n";
}
void pressEnter() {
    cout<<DIM<<"\nPress Enter to continue..."<<RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getInt(string message, int low, int high) {
    int x;

    while (true) {
        cout << message;
        if (cin >> x && x >= low && x <= high) {
            cin.ignore();
            return x;
        }
        cin.clear();
        cin.ignore(1000, '\n');

        cout<<RED<<"Invalid input.\n"<<RESET;
    }
}
float getFloat(string message) {
    float x;

    while (true){
        cout<<message;

        if (cin>>x && x>0) {
            cin.ignore();
            return x;
        }
        cin.clear();
        cin.ignore(1000, '\n');

        cout<<RED<<"Invalid amount.\n"<<RESET;
    }
}
#endif
