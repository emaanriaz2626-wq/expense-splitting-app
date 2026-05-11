# Expense Splitter

A CLI-based expense splitting application built with C++. Create groups, add shared expenses, and figure out who owes whom.

## Features

- **User Registration & Login** — create an account and log in with username/password
- **Groups** — create groups with invite codes and let others join
- **Add Expenses** — log expenses with a description, amount, and who paid
- **Split Types** — split costs three ways:
  - Equal — divided evenly among all members
  - Percentage — each member pays a custom percentage
  - Exact — specify the exact amount each member owes
- **Balance Tracking** — view how much each member owes or is owed
- **Settlement Plan** — generates a simple plan showing who should pay whom
- **File Persistence** — user and group data is saved to `.txt` files automatically

## Project Structure

```
expense-splitting-app/
├── main.cpp            # Entry point
├── ExpenseManager.h    # App controller (menus, user flow)
├── User.h / User.cpp   # User class (registration, login)
├── Group.h             # Group class (members, expenses, balances)
├── Expense.h           # Abstract base class for expenses
├── ExpenseTypes.h      # EqualExpense, PercentExpense, ExactExpense
├── FileManager.h       # Save/load data to text files
├── Utilities.h         # Input helpers and CLI formatting
├── users.txt           # Stored user data
└── groups.txt          # Stored group data
```

## How to Run

**Requirements:** Any C++ compiler (g++, MSVC, etc.)

```bash
g++ main.cpp User.cpp -o app
./app
```

On Windows:
```
g++ main.cpp User.cpp -o app.exe
app.exe
```

## Usage

1. **Register** — pick option `[1]` from the main menu, enter a username and password
2. **Login** — pick option `[2]`, enter your credentials to reach the dashboard
3. **Create a Group** — from the dashboard select `[1]`, give it a name and an invite code
4. **Join a Group** — select `[2]` and enter the invite code shared by the group creator
5. **Open a Group** — select `[3]`, then choose a group from the list

Inside a group you can:

| Option | What it does |
|---|---|
| `[1]` Add Expense | Enter a description, amount, who paid, and choose a split type (Equal / Percent / Exact) |
| `[2]` Show Expenses | Lists all recorded expenses in the group |
| `[3]` Show Balances | Shows how much each member owes or is owed |
| `[4]` Settlement Plan | Shows who should pay whom to settle all debts |
| `[5]` List Members | Displays all members in the group |
| `[0]` Back | Return to the dashboard |

All data is saved automatically to `users.txt` and `groups.txt`.
