#include <iostream>
#include <iomanip>

#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <limits>
#include <cmath>

using namespace std;\n
#include <initializer_list>

template <typename T>
class DynamicArray {
private:
    T* data;
    size_t capacity_;
    size_t size_;

    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity_ = new_capacity;
    }

public:
    DynamicArray() : data(nullptr), capacity_(0), size_(0) {}
    
    DynamicArray(std::initializer_list<T> init) : capacity_(init.size()), size_(init.size()) {
        if (capacity_ > 0) {
            data = new T[capacity_];
            size_t i = 0;
            for (const T& val : init) {
                data[i++] = val;
            }
        } else {
            data = nullptr;
        }
    }

    DynamicArray(const DynamicArray& other) : capacity_(other.capacity_), size_(other.size_) {
        if (capacity_ > 0) {
            data = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data[i] = other.data[i];
            }
        } else {
            data = nullptr;
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            capacity_ = other.capacity_;
            size_ = other.size_;
            if (capacity_ > 0) {
                data = new T[capacity_];
                for (size_t i = 0; i < size_; ++i) {
                    data[i] = other.data[i];
                }
            } else {
                data = nullptr;
            }
        }
        return *this;
    }

    ~DynamicArray() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data[size_++] = value;
    }

    void erase(T* it) {
        size_t index = it - data;
        for (size_t i = index; i < size_ - 1; ++i) {
            data[i] = data[i + 1];
        }
        size_--;
    }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }

    T* begin() { return data; }
    const T* begin() const { return data; }
    T* end() { return data + size_; }
    const T* end() const { return data + size_; }
};


// ─── ANSI Color Codes ───────────────────────────────────────────────────────
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

// ─── Data Structures ────────────────────────────────────────────────────────
struct Expense {
    string description;
    double amount;
    string paidBy;
    DynamicArray<string> splitAmong; // empty = split among all
};

struct Settlement {
    string from;
    string to;
    double amount;
};

// ─── Global State ────────────────────────────────────────────────────────────
DynamicArray<string> members;
DynamicArray<Expense> expenses;
string groupName = "My Group";

// ─── Utility Functions ───────────────────────────────────────────────────────
void clearScreen() {
    cout << "\033[2J\033[H";
}

void printLine(char c = '-', int width = 60) {
    cout << DIM;
    for (int i = 0; i < width; ++i) cout << c;
    cout << RESET << "\n";
}

void printHeader() {
    clearScreen();
    printLine('=');
    cout << BOLD << CYAN
              << "  EXPENSE SPLITTER  |  " << groupName
              << RESET << "\n";
    printLine('=');
    cout << "\n";
}

bool memberExists(const string& name) {
    return find(members.begin(), members.end(), name) != members.end();
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

double getDouble(const string& prompt) {
    double val;
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);
        istringstream ss(line);
        if (ss >> val && val > 0) return val;
        cout << RED << "  Please enter a positive number.\n" << RESET;
    }
}

int getInt(const string& prompt, int lo, int hi) {
    int val;
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);
        istringstream ss(line);
        if (ss >> val && val >= lo && val <= hi) return val;
        cout << RED << "  Enter a number between " << lo << " and " << hi << ".\n" << RESET;
    }
}

void pressEnter() {
    cout << DIM << "\n  Press Enter to continue..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ─── Balance & Settlement Engine ────────────────────────────────────────────
map<string, double> computeBalances() {
    map<string, double> balance;
    for (const auto& m : members) balance[m] = 0.0;

    for (const auto& e : expenses) {
        const auto& participants = e.splitAmong.empty() ? members : e.splitAmong;
        double share = e.amount / (double)participants.size();

        balance[e.paidBy] += e.amount;
        for (const auto& p : participants)
            balance[p] -= share;
    }
    return balance;
}

DynamicArray<Settlement> minimizeSettlements() {
    auto balance = computeBalances();
    DynamicArray<pair<string, double>> creditors, debtors;

    for (const auto& [name, bal] : balance) {
        if (bal > 0.005)  creditors.push_back({name, bal});
        else if (bal < -0.005) debtors.push_back({name, -bal});
    }

    sort(creditors.begin(), creditors.end(),
              [](auto& a, auto& b){ return a.second > b.second; });
    sort(debtors.begin(), debtors.end(),
              [](auto& a, auto& b){ return a.second > b.second; });

    DynamicArray<Settlement> settlements;
    size_t i = 0, j = 0;
    while (i < creditors.size() && j < debtors.size()) {
        double amount = min(creditors[i].second, debtors[j].second);
        settlements.push_back({debtors[j].first, creditors[i].first,
                               round(amount * 100.0) / 100.0});
        creditors[i].second -= amount;
        debtors[j].second   -= amount;
        if (creditors[i].second < 0.005) ++i;
        if (debtors[j].second   < 0.005) ++j;
    }
    return settlements;
}

// ─── Menu Actions ────────────────────────────────────────────────────────────
void setGroupName() {
    printHeader();
    cout << BOLD << "  Set Group Name\n" << RESET;
    printLine();
    cout << "  Current name: " << CYAN << groupName << RESET << "\n\n";
    cout << "  New name: ";
    string name;
    getline(cin, name);
    name = trim(name);
    if (!name.empty()) {
        groupName = name;
        cout << GREEN << "\n  Group name updated.\n" << RESET;
    } else {
        cout << YELLOW << "\n  Name unchanged.\n" << RESET;
    }
    pressEnter();
}

void addMember() {
    printHeader();
    cout << BOLD << "  Add Member\n" << RESET;
    printLine();

    if (!members.empty()) {
        cout << "  Current members:\n";
        for (const auto& m : members)
            cout << "    " << CYAN << "· " << m << RESET << "\n";
        cout << "\n";
    }

    cout << "  Member name (or blank to cancel): ";
    string name;
    getline(cin, name);
    name = trim(name);

    if (name.empty()) return;
    if (memberExists(name)) {
        cout << RED << "\n  '" << name << "' already exists.\n" << RESET;
    } else {
        members.push_back(name);
        cout << GREEN << "\n  " << name << " added.\n" << RESET;
    }
    pressEnter();
}

void removeMember() {
    printHeader();
    cout << BOLD << "  Remove Member\n" << RESET;
    printLine();

    if (members.empty()) {
        cout << YELLOW << "  No members to remove.\n" << RESET;
        pressEnter(); return;
    }

    for (size_t i = 0; i < members.size(); ++i)
        cout << "  " << CYAN << "[" << i+1 << "]" << RESET
                  << "  " << members[i] << "\n";
    cout << "  " << DIM << "[0]  Cancel\n" << RESET;

    int choice = getInt("\n  Select member: ", 0, (int)members.size());
    if (choice == 0) return;

    string name = members[choice - 1];

    bool hasExpenses = false;
    for (const auto& e : expenses) {
        if (e.paidBy == name) { hasExpenses = true; break; }
        for (const auto& p : e.splitAmong)
            if (p == name) { hasExpenses = true; break; }
    }

    if (hasExpenses) {
        cout << RED << "\n  Cannot remove '" << name
                  << "' - they are part of existing expenses.\n" << RESET;
    } else {
        members.erase(members.begin() + choice - 1);
        cout << GREEN << "\n  " << name << " removed.\n" << RESET;
    }
    pressEnter();
}

void listMembers() {
    printHeader();
    cout << BOLD << "  Members (" << members.size() << ")\n" << RESET;
    printLine();
    if (members.empty()) {
        cout << DIM << "  No members yet.\n" << RESET;
    } else {
        for (const auto& m : members)
            cout << "  " << CYAN << "· " << RESET << m << "\n";
    }
    pressEnter();
}

void addExpense() {
    printHeader();
    cout << BOLD << "  Add Expense\n" << RESET;
    printLine();

    if (members.size() < 2) {
        cout << YELLOW << "  Add at least 2 members first.\n" << RESET;
        pressEnter(); return;
    }

    string desc;
    while (desc.empty()) {
        cout << "  Description: ";
        getline(cin, desc);
        desc = trim(desc);
    }

    double amount = getDouble("  Amount: $");

    cout << "\n  Who paid?\n";
    for (size_t i = 0; i < members.size(); ++i)
        cout << "  " << CYAN << "[" << i+1 << "]" << RESET
                  << "  " << members[i] << "\n";
    int payerIdx = getInt("\n  Payer: ", 1, (int)members.size());
    string payer = members[payerIdx - 1];

    cout << "\n  Split among:\n";
    cout << "  " << CYAN << "[1]" << RESET << "  Everyone\n";
    cout << "  " << CYAN << "[2]" << RESET << "  Select specific people\n";
    int splitMode = getInt("\n  Choice: ", 1, 2);

    DynamicArray<string> splitAmong;
    if (splitMode == 2) {
        cout << "\n  Select participants (number then Enter, blank when done):\n";
        for (size_t i = 0; i < members.size(); ++i)
            cout << "  " << CYAN << "[" << i+1 << "]" << RESET
                      << "  " << members[i] << "\n";

        while (true) {
            cout << "  Add: ";
            string line;
            getline(cin, line);
            line = trim(line);
            if (line.empty()) break;
            istringstream ss(line);
            int idx;
            if (ss >> idx && idx >= 1 && idx <= (int)members.size()) {
                const string& chosen = members[idx - 1];
                if (find(splitAmong.begin(), splitAmong.end(), chosen) == splitAmong.end()) {
                    splitAmong.push_back(chosen);
                    cout << GREEN << "  Added: " << chosen << RESET << "\n";
                }
            }
        }
        if (splitAmong.empty()) {
            cout << YELLOW << "\n  No one selected - splitting among everyone.\n" << RESET;
        }
    }

    expenses.push_back({desc, amount, payer, splitAmong});

    const auto& parts = splitAmong.empty() ? members : splitAmong;
    double share = amount / parts.size();

    cout << GREEN << "\n  Expense added!\n" << RESET;
    cout << DIM << "  $" << fixed << setprecision(2) << amount
              << " paid by " << payer << ", split "
              << parts.size() << " ways ($" << share << " each)\n" << RESET;
    pressEnter();
}

void listExpenses() {
    printHeader();
    cout << BOLD << "  Expenses (" << expenses.size() << ")\n" << RESET;
    printLine();

    if (expenses.empty()) {
        cout << DIM << "  No expenses yet.\n" << RESET;
        pressEnter(); return;
    }

    double total = 0;
    for (size_t i = 0; i < expenses.size(); ++i) {
        const auto& e = expenses[i];
        total += e.amount;

        cout << "  " << CYAN << "[" << setw(2) << i+1 << "]" << RESET
                  << "  " << BOLD << e.description << RESET << "\n";
        cout << "       " << GREEN << "$" << fixed << setprecision(2)
                  << e.amount << RESET
                  << "  paid by " << YELLOW << e.paidBy << RESET;
        if (e.splitAmong.empty())
            cout << "  " << DIM << "(all)" << RESET;
        else {
            cout << "  " << DIM << "(";
            for (size_t j = 0; j < e.splitAmong.size(); ++j) {
                if (j) cout << ", ";
                cout << e.splitAmong[j];
            }
            cout << ")" << RESET;
        }
        cout << "\n";
    }

    printLine();
    cout << "  " << BOLD << "Total: " << GREEN << "$"
              << fixed << setprecision(2) << total << RESET << "\n";
    pressEnter();
}

void removeExpense() {
    printHeader();
    cout << BOLD << "  Remove Expense\n" << RESET;
    printLine();

    if (expenses.empty()) {
        cout << YELLOW << "  No expenses to remove.\n" << RESET;
        pressEnter(); return;
    }

    for (size_t i = 0; i < expenses.size(); ++i)
        cout << "  " << CYAN << "[" << i+1 << "]" << RESET
                  << "  " << expenses[i].description
                  << "  " << DIM << "($" << fixed << setprecision(2)
                  << expenses[i].amount << ")" << RESET << "\n";
    cout << "  " << DIM << "[0]  Cancel\n" << RESET;

    int choice = getInt("\n  Remove expense: ", 0, (int)expenses.size());
    if (choice == 0) return;

    string name = expenses[choice - 1].description;
    expenses.erase(expenses.begin() + choice - 1);
    cout << GREEN << "\n  Removed: " << name << "\n" << RESET;
    pressEnter();
}

void showBalances() {
    printHeader();
    cout << BOLD << "  Balances\n" << RESET;
    printLine();

    if (members.empty()) {
        cout << DIM << "  No members yet.\n" << RESET;
        pressEnter(); return;
    }

    auto balances = computeBalances();

    for (const auto& m : members) {
        double b = balances[m];
        cout << "  " << left << setw(18) << m;
        if (b > 0.005)
            cout << GREEN << "+$" << fixed << setprecision(2) << b
                      << "  (is owed)" << RESET;
        else if (b < -0.005)
            cout << RED << "-$" << fixed << setprecision(2) << -b
                      << "  (owes)" << RESET;
        else
            cout << DIM << " $0.00  (settled)" << RESET;
        cout << "\n";
    }
    pressEnter();
}

void showSettlements() {
    printHeader();
    cout << BOLD << "  Settlement Plan\n" << RESET;
    printLine();

    if (members.empty() || expenses.empty()) {
        cout << DIM << "  Nothing to settle yet.\n" << RESET;
        pressEnter(); return;
    }

    auto settlements = minimizeSettlements();

    if (settlements.empty()) {
        cout << GREEN << "  Everyone is settled up!\n" << RESET;
    } else {
        cout << "  " << settlements.size() << " payment(s) needed:\n\n";
        for (size_t i = 0; i < settlements.size(); ++i) {
            const auto& s = settlements[i];
            cout << "  " << i+1 << ". "
                      << YELLOW << s.from << RESET
                      << "  ->  "
                      << GREEN << s.to << RESET
                      << "    " << BOLD << "$"
                      << fixed << setprecision(2) << s.amount
                      << RESET << "\n";
        }
    }
    pressEnter();
}

void showSummary() {
    printHeader();
    cout << BOLD << "  Group Summary\n" << RESET;
    printLine();

    double total = 0;
    for (const auto& e : expenses) total += e.amount;

    cout << "  Group:    " << CYAN << groupName << RESET << "\n";
    cout << "  Members:  " << CYAN << members.size() << RESET << "\n";
    cout << "  Expenses: " << CYAN << expenses.size() << RESET << "\n";
    cout << "  Total:    " << GREEN << "$" << fixed
              << setprecision(2) << total << RESET << "\n";

    if (!members.empty() && total > 0) {
        cout << "  Per head: " << YELLOW << "$"
                  << fixed << setprecision(2)
                  << total / members.size() << RESET << "\n";
    }

    if (!expenses.empty()) {
        map<string, double> paid;
        for (const auto& e : expenses) paid[e.paidBy] += e.amount;
        auto top = max_element(paid.begin(), paid.end(),
                   [](auto& a, auto& b){ return a.second < b.second; });
        cout << "\n  Top payer: " << MAGENTA << top->first
                  << RESET << " ($" << fixed << setprecision(2)
                  << top->second << ")\n";
    }

    pressEnter();
}

// ─── Main Menu ───────────────────────────────────────────────────────────────
void showMenu() {
    printHeader();
    cout << BOLD << "  MEMBERS\n" << RESET;
    cout << "  " << CYAN << "[1]" << RESET << "  Add member\n";
    cout << "  " << CYAN << "[2]" << RESET << "  Remove member\n";
    cout << "  " << CYAN << "[3]" << RESET << "  List members\n";
    cout << "\n" << BOLD << "  EXPENSES\n" << RESET;
    cout << "  " << CYAN << "[4]" << RESET << "  Add expense\n";
    cout << "  " << CYAN << "[5]" << RESET << "  List expenses\n";
    cout << "  " << CYAN << "[6]" << RESET << "  Remove expense\n";
    cout << "\n" << BOLD << "  REPORTS\n" << RESET;
    cout << "  " << CYAN << "[7]" << RESET << "  Show balances\n";
    cout << "  " << CYAN << "[8]" << RESET << "  Settlement plan\n";
    cout << "  " << CYAN << "[9]" << RESET << "  Group summary\n";
    cout << "\n" << BOLD << "  SETTINGS\n" << RESET;
    cout << "  " << CYAN << "[10]" << RESET << " Set group name\n";
    cout << "  " << CYAN << "[0]" << RESET << "  Quit\n";
    printLine();
}

int main() {
    // Demo data
    groupName = "Weekend Trip";
    members = {"Alice", "Bob", "Carol"};
    expenses = {
        {"Hotel", 300.0, "Alice", {}},
        {"Dinner", 90.0, "Bob", {}},
        {"Taxi", 45.0, "Carol", {"Alice", "Carol"}}
    };

    while (true) {
        showMenu();
        int choice = getInt("  -> ", 0, 10);

        switch (choice) {
            case 0:
                clearScreen();
                cout << GREEN << "\n  Goodbye!\n\n" << RESET;
                return 0;
            case 1:  addMember();       break;
            case 2:  removeMember();    break;
            case 3:  listMembers();     break;
            case 4:  addExpense();      break;
            case 5:  listExpenses();    break;
            case 6:  removeExpense();   break;
            case 7:  showBalances();    break;
            case 8:  showSettlements(); break;
            case 9:  showSummary();     break;
            case 10: setGroupName();    break;
        }
    }
}
