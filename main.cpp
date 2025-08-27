#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <limits>
#include <algorithm>
#include <cctype>
#include <random>
#include <chrono>
#include <thread>

#include "BankingSystem.h"
#include "Account.h"
#include "Transaction.h"
#include "User.h"

using namespace std;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void showWelcomeScreen() {
    clearScreen();
    cout << "\n";
    cout << "╔══════════════════════════════════════════════════════════════╗\n";
    cout << "║                    OYANIB BANKING SYSTEM                    ║\n";
    cout << "║                        Advanced Edition                     ║\n";
    cout << "╚══════════════════════════════════════════════════════════════╝\n";
    cout << "\n";
    cout << "                    Welcome to Oyanib Bank!\n";
    cout << "              Your Trusted Financial Partner\n\n";
    cout << "══════════════════════════════════════════════════════════════════\n\n";
}

void showMainMenu() {
    cout << "┌───────────────── MAIN MENU ─────────────────┐\n";
    cout << "│ 1. Customer Login                            │\n";
    cout << "│ 2. Create New Account                        │\n";
    cout << "│ 3. Admin Panel                               │\n";
    cout << "│ 4. About Oyanib Bank                         │\n";
    cout << "│ 5. Exit                                      │\n";
    cout << "└─────────────────────────────────────────────┘\n";
    cout << "Enter your choice: ";
}

void showCustomerMenu() {
    cout << "\n┌───────────────── CUSTOMER MENU ────────────────┐\n";
    cout << "│ 1. View Account Balance                       │\n";
    cout << "│ 2. Deposit Money                              │\n";
    cout << "│ 3. Withdraw Money                             │\n";
    cout << "│ 4. Transfer Money                             │\n";
    cout << "│ 5. View Transaction History                   │\n";
    cout << "│ 6. Apply for Loan                             │\n";
    cout << "│ 7. View Loan Status                           │\n";
    cout << "│ 8. Change Password                            │\n";
    cout << "│ 9. Logout                                     │\n";
    cout << "└─────────────────────────────────────────────┘\n";
    cout << "Enter your choice: ";
}

void showAdminMenu() {
    cout << "\n┌───────────────── ADMIN MENU ──────────────────┐\n";
    cout << "│ 1. View All Accounts                          │\n";
    cout << "│ 2. View All Transactions                      │\n";
    cout << "│ 3. Process Loan Applications                  │\n";
    cout << "│ 4. Calculate Interest                         │\n";
    cout << "│ 5. System Statistics                          │\n";
    cout << "│ 6. Backup Database                            │\n";
    cout << "│ 7. Return to Main Menu                        │\n";
    cout << "└─────────────────────────────────────────────┘\n";
    cout << "Enter your choice: ";
}

void showAbout() {
    clearScreen();
    cout << "\n";
    cout << "╔══════════════════════════════════════════════════════════════╗\n";
    cout << "║                        ABOUT OYANIB BANK                    ║\n";
    cout << "╚══════════════════════════════════════════════════════════════╝\n";
    cout << "\n";
    cout << "Oyanib Bank is a modern banking system designed to provide\n";
    cout << "secure and efficient financial services to our customers.\n\n";
    cout << "Features:\n";
    cout << "• Multiple Account Types (Savings, Checking, Credit)\n";
    cout << "• Secure Authentication System\n";
    cout << "• Real-time Transaction Processing\n";
    cout << "• Loan Management System\n";
    cout << "• Interest Calculation\n";
    cout << "• Comprehensive Transaction History\n";
    cout << "• Admin Panel for System Management\n\n";
    cout << "Version: 2.0 Advanced Edition\n";
    cout << "Developed with modern C++ standards\n\n";
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main() {
    BankingSystem bank;
    
    // Load existing data
    bank.loadData();
    
    int choice;
    bool running = true;
    
    while (running) {
        showWelcomeScreen();
        showMainMenu();
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            this_thread::sleep_for(chrono::seconds(2));
            continue;
        }
        
        switch (choice) {
            case 1: {
                // Customer Login
                string accountNumber, password;
                cout << "\nEnter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Password: ";
                cin >> password;
                
                auto user = bank.authenticateUser(accountNumber, password);
                if (user) {
                    cout << "\nLogin successful! Welcome, " << user->getName() << "!\n";
                    this_thread::sleep_for(chrono::seconds(2));
                    
                    // Customer session
                    bool customerSession = true;
                    while (customerSession) {
                        clearScreen();
                        cout << "\nWelcome, " << user->getName() << "!\n";
                        showCustomerMenu();
                        
                        int customerChoice;
                        if (!(cin >> customerChoice)) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input.\n";
                            this_thread::sleep_for(chrono::seconds(2));
                            continue;
                        }
                        
                        switch (customerChoice) {
                            case 1:
                                bank.displayAccountBalance(user);
                                break;
                            case 2:
                                bank.processDeposit(user);
                                break;
                            case 3:
                                bank.processWithdrawal(user);
                                break;
                            case 4:
                                bank.processTransfer(user);
                                break;
                            case 5:
                                bank.displayTransactionHistory(user);
                                break;
                            case 6:
                                bank.applyForLoan(user);
                                break;
                            case 7:
                                bank.viewLoanStatus(user);
                                break;
                            case 8:
                                bank.changePassword(user);
                                break;
                            case 9:
                                customerSession = false;
                                cout << "Logged out successfully.\n";
                                this_thread::sleep_for(chrono::seconds(2));
                                break;
                            default:
                                cout << "Invalid choice.\n";
                                this_thread::sleep_for(chrono::seconds(2));
                        }
                    }
                } else {
                    cout << "Invalid credentials. Please try again.\n";
                    this_thread::sleep_for(chrono::seconds(2));
                }
                break;
            }
            case 2: {
                // Create New Account
                bank.createNewAccount();
                break;
            }
            case 3: {
                // Admin Panel
                string adminPassword;
                cout << "\nEnter Admin Password: ";
                cin >> adminPassword;
                
                if (adminPassword == "admin123") { // In real system, use proper authentication
                    cout << "Admin access granted.\n";
                    this_thread::sleep_for(chrono::seconds(1));
                    
                    bool adminSession = true;
                    while (adminSession) {
                        clearScreen();
                        cout << "\nAdmin Panel\n";
                        showAdminMenu();
                        
                        int adminChoice;
                        if (!(cin >> adminChoice)) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input.\n";
                            this_thread::sleep_for(chrono::seconds(2));
                            continue;
                        }
                        
                        switch (adminChoice) {
                            case 1:
                                bank.displayAllAccounts();
                                break;
                            case 2:
                                bank.displayAllTransactions();
                                break;
                            case 3:
                                bank.processLoanApplications();
                                break;
                            case 4:
                                bank.calculateInterest();
                                break;
                            case 5:
                                bank.displaySystemStatistics();
                                break;
                            case 6:
                                bank.backupDatabase();
                                break;
                            case 7:
                                adminSession = false;
                                break;
                            default:
                                cout << "Invalid choice.\n";
                                this_thread::sleep_for(chrono::seconds(2));
                        }
                    }
                } else {
                    cout << "Invalid admin password.\n";
                    this_thread::sleep_for(chrono::seconds(2));
                }
                break;
            }
            case 4: {
                showAbout();
                break;
            }
            case 5: {
                cout << "\nThank you for using Oyanib Banking System!\n";
                cout << "Saving data and exiting...\n";
                bank.saveData();
                running = false;
                break;
            }
            default: {
                cout << "Invalid choice. Please try again.\n";
                this_thread::sleep_for(chrono::seconds(2));
            }
        }
    }
    
    return 0;
}
