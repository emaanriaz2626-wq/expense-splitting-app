#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <limits>
#include <cmath>

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
    std::string description;
    double amount;
    std::string paidBy;
    std::vector<std::string> splitAmong; // empty = split among all
};

struct Settlement {
    std::string from;
    std::string to;
    double amount;
};

// ─── Global State ────────────────────────────────────────────────────────────
std::vector<std::string> members;
std::vector<Expense> expenses;
std::string groupName = "My Group";

// ─── Utility Functions ───────────────────────────────────────────────────────
void clearScreen() {
    std::cout << "\033[2J\033[H";
}

void printLine(char c = '-', int width = 60) {
    std::cout << DIM;
    for (int i = 0; i < width; ++i) std::cout << c;
    std::cout << RESET << "\n";
}

void printHeader() {
    clearScreen();
    printLine('=');
    std::cout << BOLD << CYAN
              << "  EXPENSE SPLITTER  |  " << groupName
              << RESET << "\n";
    printLine('=');
    std::cout << "\n";
}

bool memberExists(const std::string& name) {
    return std::find(members.begin(), members.end(), name) != members.end();
}

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

double getDouble(const std::string& prompt) {
    double val;
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        std::istringstream ss(line);
        if (ss >> val && val > 0) return val;
        std::cout << RED << "  Please enter a positive number.\n" << RESET;
    }
}

int getInt(const std::string& prompt, int lo, int hi) {
    int val;
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        std::istringstream ss(line);
        if (ss >> val && val >= lo && val <= hi) return val;
        std::cout << RED << "  Enter a number between " << lo << " and " << hi << ".\n" << RESET;
    }
}

void pressEnter() {
    std::cout << DIM << "\n  Press Enter to continue..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ─── Balance & Settlement Engine ────────────────────────────────────────────
std::map<std::string, double> computeBalances() {
    std::map<std::string, double> balance;
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

std::vector<Settlement> minimizeSettlements() {
    auto balance = computeBalances();
    std::vector<std::pair<std::string, double>> creditors, debtors;

    for (const auto& [name, bal] : balance) {
        if (bal > 0.005)  creditors.push_back({name, bal});
        else if (bal < -0.005) debtors.push_back({name, -bal});
    }

    std::sort(creditors.begin(), creditors.end(),
              [](auto& a, auto& b){ return a.second > b.second; });
    std::sort(debtors.begin(), debtors.end(),
              [](auto& a, auto& b){ return a.second > b.second; });

    std::vector<Settlement> settlements;
    size_t i = 0, j = 0;
    while (i < creditors.size() && j < debtors.size()) {
        double amount = std::min(creditors[i].second, debtors[j].second);
        settlements.push_back({debtors[j].first, creditors[i].first,
                               std::round(amount * 100.0) / 100.0});
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
    std::cout << BOLD << "  Set Group Name\n" << RESET;
    printLine();
    std::cout << "  Current name: " << CYAN << groupName << RESET << "\n\n";
    std::cout << "  New name: ";
    std::string name;
    std::getline(std::cin, name);
    name = trim(name);
    if (!name.empty()) {
        groupName = name;
        std::cout << GREEN << "\n  Group name updated.\n" << RESET;
    } else {
        std::cout << YELLOW << "\n  Name unchanged.\n" << RESET;
    }
    pressEnter();
}

void addMember() {
    printHeader();
    std::cout << BOLD << "  Add Member\n" << RESET;
    printLine();

    if (!members.empty()) {
        std::cout << "  Current members:\n";
        for (const auto& m : members)
            std::cout << "    " << CYAN << "· " << m << RESET << "\n";
        std::cout << "\n";
    }

    std::cout << "  Member name (or blank to cancel): ";
    std::string name;
    std::getline(std::cin, name);
    name = trim(name);

    if (name.empty()) return;
    if (memberExists(name)) {
        std::cout << RED << "\n  '" << name << "' already exists.\n" << RESET;
    } else {
        members.push_back(name);
        std::cout << GREEN << "\n  " << name << " added.\n" << RESET;
    }
    pressEnter();
}

void removeMember() {
    printHeader();
    std::cout << BOLD << "  Remove Member\n" << RESET;
    printLine();

    if (members.empty()) {
        std::cout << YELLOW << "  No members to remove.\n" << RESET;
        pressEnter(); return;
    }

    for (size_t i = 0; i < members.size(); ++i)
        std::cout << "  " << CYAN << "[" << i+1 << "]" << RESET
                  << "  " << members[i] << "\n";
    std::cout << "  " << DIM << "[0]  Cancel\n" << RESET;

    int choice = getInt("\n  Select member: ", 0, (int)members.size());
    if (choice == 0) return;

    std::string name = members[choice - 1];

    bool hasExpenses = false;
    for (const auto& e : expenses) {
        if (e.paidBy == name) { hasExpenses = true; break; }
        for (const auto& p : e.splitAmong)
            if (p == name) { hasExpenses = true; break; }
    }

    if (hasExpenses) {
        std::cout << RED << "\n  Cannot remove '" << name
                  << "' - they are part of existing expenses.\n" << RESET;
    } else {
        members.erase(members.begin() + choice - 1);
        std::cout << GREEN << "\n  " << name << " removed.\n" << RESET;
    }
    pressEnter();
}

void listMembers() {
    printHeader();
    std::cout << BOLD << "  Members (" << members.size() << ")\n" << RESET;
    printLine();
    if (members.empty()) {
        std::cout << DIM << "  No members yet.\n" << RESET;
    } else {
        for (const auto& m : members)
            std::cout << "  " << CYAN << "· " << RESET << m << "\n";
    }
    pressEnter();
}

void addExpense() {
    printHeader();
    std::cout << BOLD << "  Add Expense\n" << RESET;
    printLine();

    if (members.size() < 2) {
        std::cout << YELLOW << "  Add at least 2 members first.\n" << RESET;
        pressEnter(); return;
    }

    std::string desc;
    while (desc.empty()) {
        std::cout << "  Description: ";
        std::getline(std::cin, desc);
        desc = trim(desc);
    }

    double amount = getDouble("  Amount: $");

    std::cout << "\n  Who paid?\n";
    for (size_t i = 0; i < members.size(); ++i)
        std::cout << "  " << CYAN << "[" << i+1 << "]" << RESET
                  << "  " << members[i] << "\n";
    int payerIdx = getInt("\n  Payer: ", 1, (int)members.size());
    std::string payer = members[payerIdx - 1];

    std::cout << "\n  Split among:\n";
    std::cout << "  " << CYAN << "[1]" << RESET << "  Everyone\n";
    std::cout << "  " << CYAN << "[2]" << RESET << "  Select specific people\n";
    int splitMode = getInt("\n  Choice: ", 1, 2);

    std::vector<std::string> splitAmong;
    if (splitMode == 2) {
        std::cout << "\n  Select participants (number then Enter, blank when done):\n";
        for (size_t i = 0; i < members.size(); ++i)
            std::cout << "  " << CYAN << "[" << i+1 << "]" << RESET
                      << "  " << members[i] << "\n";

        while (true) {
            std::cout << "  Add: ";
            std::string line;
            std::getline(std::cin, line);
            line = trim(line);
            if (line.empty()) break;
            std::istringstream ss(line);
            int idx;
            if (ss >> idx && idx >= 1 && idx <= (int)members.size()) {
                const std::string& chosen = members[idx - 1];
                if (std::find(splitAmong.begin(), splitAmong.end(), chosen) == splitAmong.end()) {
                    splitAmong.push_back(chosen);
                    std::cout << GREEN << "  Added: " << chosen << RESET << "\n";
                }
            }
        }
        if (splitAmong.empty()) {
            std::cout << YELLOW << "\n  No one selected - splitting among everyone.\n" << RESET;
        }
    }

    expenses.push_back({desc, amount, payer, splitAmong});

    const auto& parts = splitAmong.empty() ? members : splitAmong;
    double share = amount / parts.size();

    std::cout << GREEN << "\n  Expense added!\n" << RESET;
    std::cout << DIM << "  $" << std::fixed << std::setprecision(2) << amount
              << " paid by " << payer << ", split "
              << parts.size() << " ways ($" << share << " each)\n" << RESET;
    pressEnter();
}

void listExpenses() {
    printHeader();
    std::cout << BOLD << "  Expenses (" << expenses.size() << ")\n" << RESET;
    printLine();

    if (expenses.empty()) {
        std::cout << DIM << "  No expenses yet.\n" << RESET;
        pressEnter(); return;
    }

    double total = 0;
    for (size_t i = 0; i < expenses.size(); ++i) {
        const auto& e = expenses[i];
        total += e.amount;

        std::cout << "  " << CYAN << "[" << std::setw(2) << i+1 << "]" << RESET
                  << "  " << BOLD << e.description << RESET << "\n";
        std::cout << "       " << GREEN << "$" << std::fixed << std::setprecision(2)
                  << e.amount << RESET
                  << "  paid by " << YELLOW << e.paidBy << RESET;
        if (e.splitAmong.empty())
            std::cout << "  " << DIM << "(all)" << RESET;
        else {
            std::cout << "  " << DIM << "(";
            for (size_t j = 0; j < e.splitAmong.size(); ++j) {
                if (j) std::cout << ", ";
                std::cout << e.splitAmong[j];
            }
            std::cout << ")" << RESET;
        }
        std::cout << "\n";
    }

    printLine();
    std::cout << "  " << BOLD << "Total: " << GREEN << "$"
              << std::fixed << std::setprecision(2) << total << RESET << "\n";
    pressEnter();
}

void removeExpense() {
    printHeader();
    std::cout << BOLD << "  Remove Expense\n" << RESET;
    printLine();

    if (expenses.empty()) {
        std::cout << YELLOW << "  No expenses to remove.\n" << RESET;
        pressEnter(); return;
    }

    for (size_t i = 0; i < expenses.size(); ++i)
        std::cout << "  " << CYAN << "[" << i+1 << "]" << RESET
                  << "  " << expenses[i].description
                  << "  " << DIM << "($" << std::fixed << std::setprecision(2)
                  << expenses[i].amount << ")" << RESET << "\n";
    std::cout << "  " << DIM << "[0]  Cancel\n" << RESET;

    int choice = getInt("\n  Remove expense: ", 0, (int)expenses.size());
    if (choice == 0) return;

    std::string name = expenses[choice - 1].description;
    expenses.erase(expenses.begin() + choice - 1);
    std::cout << GREEN << "\n  Removed: " << name << "\n" << RESET;
    pressEnter();
}

void showBalances() {
    printHeader();
    std::cout << BOLD << "  Balances\n" << RESET;
    printLine();

    if (members.empty()) {
        std::cout << DIM << "  No members yet.\n" << RESET;
        pressEnter(); return;
    }

    auto balances = computeBalances();

    for (const auto& m : members) {
        double b = balances[m];
        std::cout << "  " << std::left << std::setw(18) << m;
        if (b > 0.005)
            std::cout << GREEN << "+$" << std::fixed << std::setprecision(2) << b
                      << "  (is owed)" << RESET;
        else if (b < -0.005)
            std::cout << RED << "-$" << std::fixed << std::setprecision(2) << -b
                      << "  (owes)" << RESET;
        else
            std::cout << DIM << " $0.00  (settled)" << RESET;
        std::cout << "\n";
    }
    pressEnter();
}

void showSettlements() {
    printHeader();
    std::cout << BOLD << "  Settlement Plan\n" << RESET;
    printLine();

    if (members.empty() || expenses.empty()) {
        std::cout << DIM << "  Nothing to settle yet.\n" << RESET;
        pressEnter(); return;
    }

    auto settlements = minimizeSettlements();

    if (settlements.empty()) {
        std::cout << GREEN << "  Everyone is settled up!\n" << RESET;
    } else {
        std::cout << "  " << settlements.size() << " payment(s) needed:\n\n";
        for (size_t i = 0; i < settlements.size(); ++i) {
            const auto& s = settlements[i];
            std::cout << "  " << i+1 << ". "
                      << YELLOW << s.from << RESET
                      << "  ->  "
                      << GREEN << s.to << RESET
                      << "    " << BOLD << "$"
                      << std::fixed << std::setprecision(2) << s.amount
                      << RESET << "\n";
        }
    }
    pressEnter();
}

void showSummary() {
    printHeader();
    std::cout << BOLD << "  Group Summary\n" << RESET;
    printLine();

    double total = 0;
    for (const auto& e : expenses) total += e.amount;

    std::cout << "  Group:    " << CYAN << groupName << RESET << "\n";
    std::cout << "  Members:  " << CYAN << members.size() << RESET << "\n";
    std::cout << "  Expenses: " << CYAN << expenses.size() << RESET << "\n";
    std::cout << "  Total:    " << GREEN << "$" << std::fixed
              << std::setprecision(2) << total << RESET << "\n";

    if (!members.empty() && total > 0) {
        std::cout << "  Per head: " << YELLOW << "$"
                  << std::fixed << std::setprecision(2)
                  << total / members.size() << RESET << "\n";
    }

    if (!expenses.empty()) {
        std::map<std::string, double> paid;
        for (const auto& e : expenses) paid[e.paidBy] += e.amount;
        auto top = std::max_element(paid.begin(), paid.end(),
                   [](auto& a, auto& b){ return a.second < b.second; });
        std::cout << "\n  Top payer: " << MAGENTA << top->first
                  << RESET << " ($" << std::fixed << std::setprecision(2)
                  << top->second << ")\n";
    }

    pressEnter();
}

// ─── Main Menu ───────────────────────────────────────────────────────────────
void showMenu() {
    printHeader();
    std::cout << BOLD << "  MEMBERS\n" << RESET;
    std::cout << "  " << CYAN << "[1]" << RESET << "  Add member\n";
    std::cout << "  " << CYAN << "[2]" << RESET << "  Remove member\n";
    std::cout << "  " << CYAN << "[3]" << RESET << "  List members\n";
    std::cout << "\n" << BOLD << "  EXPENSES\n" << RESET;
    std::cout << "  " << CYAN << "[4]" << RESET << "  Add expense\n";
    std::cout << "  " << CYAN << "[5]" << RESET << "  List expenses\n";
    std::cout << "  " << CYAN << "[6]" << RESET << "  Remove expense\n";
    std::cout << "\n" << BOLD << "  REPORTS\n" << RESET;
    std::cout << "  " << CYAN << "[7]" << RESET << "  Show balances\n";
    std::cout << "  " << CYAN << "[8]" << RESET << "  Settlement plan\n";
    std::cout << "  " << CYAN << "[9]" << RESET << "  Group summary\n";
    std::cout << "\n" << BOLD << "  SETTINGS\n" << RESET;
    std::cout << "  " << CYAN << "[10]" << RESET << " Set group name\n";
    std::cout << "  " << CYAN << "[0]" << RESET << "  Quit\n";
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
                std::cout << GREEN << "\n  Goodbye!\n\n" << RESET;
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
