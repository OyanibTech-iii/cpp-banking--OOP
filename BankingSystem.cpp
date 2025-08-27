#include "BankingSystem.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <algorithm>
#include <filesystem>

BankingSystem::BankingSystem() : totalDeposits(0.0), totalWithdrawals(0.0), totalLoans(0.0),
                                 totalCustomers(0), totalAccounts(0), totalTransactions(0) {
    customersFile = "customers.txt";
    accountsFile = "accounts.txt";
    transactionsFile = "transactions.txt";
    loansFile = "loans.txt";
}

BankingSystem::~BankingSystem() {
    saveData();
}

// Customer management
std::shared_ptr<Customer> BankingSystem::createCustomer(const std::string& name, const std::string& email, 
                                                        const std::string& phone, const std::string& address, 
                                                        const std::string& password) {
    auto customer = std::make_shared<Customer>(name, email, phone, address, password);
    customers.push_back(customer);
    totalCustomers++;
    return customer;
}

std::shared_ptr<Customer> BankingSystem::findCustomer(const std::string& accountNumber) {
    for (const auto& customer : customers) {
        if (customer->getAccountNumber() == accountNumber) {
            return customer;
        }
    }
    return nullptr;
}

std::shared_ptr<Customer> BankingSystem::authenticateUser(const std::string& accountNumber, const std::string& password) {
    auto customer = findCustomer(accountNumber);
    if (customer && customer->authenticate(password)) {
        return customer;
    }
    return nullptr;
}

void BankingSystem::displayAllCustomers() const {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                    ALL CUSTOMERS\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    
    if (customers.empty()) {
        std::cout << "No customers found.\n";
    } else {
        std::cout << std::setw(15) << "Account No." << std::setw(20) << "Name" 
                  << std::setw(25) << "Email" << std::setw(15) << "Status\n";
        std::cout << std::string(75, '-') << "\n";
        
        for (const auto& customer : customers) {
            std::cout << std::setw(15) << customer->getAccountNumber()
                      << std::setw(20) << customer->getName()
                      << std::setw(25) << customer->getEmail()
                      << std::setw(15) << (customer->getIsActive() ? "Active" : "Inactive") << "\n";
        }
    }
    
    std::cout << "══════════════════════════════════════════════════════════════\n";
}

void BankingSystem::deleteCustomer(const std::string& accountNumber) {
    customers.erase(
        std::remove_if(customers.begin(), customers.end(),
            [&accountNumber](const std::shared_ptr<Customer>& customer) {
                return customer->getAccountNumber() == accountNumber;
            }),
        customers.end()
    );
    totalCustomers--;
}

// Account management
std::shared_ptr<Account> BankingSystem::createAccount(const std::string& customerId, const std::string& accountType, 
                                                      double initialBalance) {
    std::shared_ptr<Account> account;
    
    if (accountType == "Savings") {
        account = std::make_shared<SavingsAccount>(customerId, initialBalance);
    } else {
        account = std::make_shared<Account>(customerId, accountType, initialBalance);
    }
    
    accounts.push_back(account);
    totalAccounts++;
    
    // Add account to customer
    auto customer = findCustomer(customerId);
    if (customer) {
        customer->addAccount(account);
    }
    
    return account;
}

std::shared_ptr<Account> BankingSystem::findAccount(const std::string& accountNumber) {
    for (const auto& account : accounts) {
        if (account->getAccountNumber() == accountNumber) {
            return account;
        }
    }
    return nullptr;
}

void BankingSystem::displayAllAccounts() const {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                    ALL ACCOUNTS\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    
    if (accounts.empty()) {
        std::cout << "No accounts found.\n";
    } else {
        std::cout << std::setw(15) << "Account No." << std::setw(15) << "Type" 
                  << std::setw(15) << "Balance" << std::setw(15) << "Customer ID" 
                  << std::setw(15) << "Status\n";
        std::cout << std::string(75, '-') << "\n";
        
        for (const auto& account : accounts) {
            std::cout << std::setw(15) << account->getAccountNumber()
                      << std::setw(15) << account->getAccountType()
                      << std::setw(15) << "$" << std::fixed << std::setprecision(2) << account->getBalance()
                      << std::setw(15) << account->getCustomerId()
                      << std::setw(15) << (account->isActive() ? "Active" : "Inactive") << "\n";
        }
    }
    
    std::cout << "══════════════════════════════════════════════════════════════\n";
}

void BankingSystem::deleteAccount(const std::string& accountNumber) {
    accounts.erase(
        std::remove_if(accounts.begin(), accounts.end(),
            [&accountNumber](const std::shared_ptr<Account>& account) {
                return account->getAccountNumber() == accountNumber;
            }),
        accounts.end()
    );
    totalAccounts--;
}

// Transaction management
bool BankingSystem::processDeposit(std::shared_ptr<Customer> customer) {
    std::cout << "\nEnter amount to deposit: $";
    double amount;
    std::cin >> amount;
    
    if (!validateAmount(amount)) {
        std::cout << "Invalid amount.\n";
        return false;
    }
    
    auto accounts = customer->getAllAccounts();
    if (accounts.empty()) {
        std::cout << "No accounts found for this customer.\n";
        return false;
    }
    
    std::cout << "Select account to deposit to:\n";
    for (size_t i = 0; i < accounts.size(); ++i) {
        std::cout << i + 1 << ". " << accounts[i]->getAccountNumber() 
                  << " (" << accounts[i]->getAccountType() << ") - $"
                  << std::fixed << std::setprecision(2) << accounts[i]->getBalance() << "\n";
    }
    
    int choice;
    std::cout << "Enter choice: ";
    std::cin >> choice;
    
    if (choice > 0 && choice <= static_cast<int>(accounts.size())) {
        auto account = accounts[choice - 1];
        if (account->deposit(amount)) {
            totalDeposits += amount;
            totalTransactions++;
            std::cout << "Deposit successful! New balance: $"
                      << std::fixed << std::setprecision(2) << account->getBalance() << "\n";
            return true;
        } else {
            std::cout << "Deposit failed.\n";
            return false;
        }
    }
    
    std::cout << "Invalid choice.\n";
    return false;
}

bool BankingSystem::processWithdrawal(std::shared_ptr<Customer> customer) {
    std::cout << "\nEnter amount to withdraw: $";
    double amount;
    std::cin >> amount;
    
    if (!validateAmount(amount)) {
        std::cout << "Invalid amount.\n";
        return false;
    }
    
    auto accounts = customer->getAllAccounts();
    if (accounts.empty()) {
        std::cout << "No accounts found for this customer.\n";
        return false;
    }
    
    std::cout << "Select account to withdraw from:\n";
    for (size_t i = 0; i < accounts.size(); ++i) {
        std::cout << i + 1 << ". " << accounts[i]->getAccountNumber() 
                  << " (" << accounts[i]->getAccountType() << ") - $"
                  << std::fixed << std::setprecision(2) << accounts[i]->getBalance() << "\n";
    }
    
    int choice;
    std::cout << "Enter choice: ";
    std::cin >> choice;
    
    if (choice > 0 && choice <= static_cast<int>(accounts.size())) {
        auto account = accounts[choice - 1];
        if (account->withdraw(amount)) {
            totalWithdrawals += amount;
            totalTransactions++;
            std::cout << "Withdrawal successful! New balance: $"
                      << std::fixed << std::setprecision(2) << account->getBalance() << "\n";
            return true;
        } else {
            std::cout << "Withdrawal failed. Check balance and limits.\n";
            return false;
        }
    }
    
    std::cout << "Invalid choice.\n";
    return false;
}

bool BankingSystem::processTransfer(std::shared_ptr<Customer> customer) {
    std::cout << "\nEnter amount to transfer: $";
    double amount;
    std::cin >> amount;
    
    if (!validateAmount(amount)) {
        std::cout << "Invalid amount.\n";
        return false;
    }
    
    std::cout << "Enter target account number: ";
    std::string targetAccountNumber;
    std::cin >> targetAccountNumber;
    
    auto sourceAccounts = customer->getAllAccounts();
    auto targetAccount = findAccount(targetAccountNumber);
    
    if (sourceAccounts.empty()) {
        std::cout << "No accounts found for this customer.\n";
        return false;
    }
    
    if (!targetAccount) {
        std::cout << "Target account not found.\n";
        return false;
    }
    
    std::cout << "Select source account:\n";
    for (size_t i = 0; i < sourceAccounts.size(); ++i) {
        std::cout << i + 1 << ". " << sourceAccounts[i]->getAccountNumber() 
                  << " (" << sourceAccounts[i]->getAccountType() << ") - $"
                  << std::fixed << std::setprecision(2) << sourceAccounts[i]->getBalance() << "\n";
    }
    
    int choice;
    std::cout << "Enter choice: ";
    std::cin >> choice;
    
    if (choice > 0 && choice <= static_cast<int>(sourceAccounts.size())) {
        auto sourceAccount = sourceAccounts[choice - 1];
        if (sourceAccount->transfer(*targetAccount, amount)) {
            totalTransactions++;
            std::cout << "Transfer successful!\n";
            std::cout << "Source account balance: $"
                      << std::fixed << std::setprecision(2) << sourceAccount->getBalance() << "\n";
            std::cout << "Target account balance: $"
                      << std::fixed << std::setprecision(2) << targetAccount->getBalance() << "\n";
            return true;
        } else {
            std::cout << "Transfer failed. Check balance and limits.\n";
            return false;
        }
    }
    
    std::cout << "Invalid choice.\n";
    return false;
}

void BankingSystem::displayTransactionHistory(std::shared_ptr<Customer> customer) const {
    auto accounts = customer->getAllAccounts();
    if (accounts.empty()) {
        std::cout << "No accounts found for this customer.\n";
        return;
    }
    
    std::cout << "Select account to view transactions:\n";
    for (size_t i = 0; i < accounts.size(); ++i) {
        std::cout << i + 1 << ". " << accounts[i]->getAccountNumber() 
                  << " (" << accounts[i]->getAccountType() << ")\n";
    }
    
    int choice;
    std::cout << "Enter choice: ";
    std::cin >> choice;
    
    if (choice > 0 && choice <= static_cast<int>(accounts.size())) {
        accounts[choice - 1]->displayTransactionHistory();
    } else {
        std::cout << "Invalid choice.\n";
    }
}

void BankingSystem::displayAllTransactions() const {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                    ALL TRANSACTIONS\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    
    if (transactions.empty()) {
        std::cout << "No transactions found.\n";
    } else {
        std::cout << std::setw(20) << "Date" << std::setw(15) << "Account" 
                  << std::setw(15) << "Type" << std::setw(15) << "Amount" 
                  << std::setw(15) << "Balance\n";
        std::cout << std::string(80, '-') << "\n";
        
        for (const auto& transaction : transactions) {
            std::cout << std::setw(20) << transaction->getDate()
                      << std::setw(15) << transaction->getAccountNumber()
                      << std::setw(15) << transaction->getType()
                      << std::setw(15) << transaction->getFormattedAmount()
                      << std::setw(15) << "$" << std::fixed << std::setprecision(2) << transaction->getBalance() << "\n";
        }
    }
    
    std::cout << "══════════════════════════════════════════════════════════════\n";
}

// Loan management
std::shared_ptr<Loan> BankingSystem::applyForLoan(std::shared_ptr<Customer> customer) {
    if (!customer->isEligibleForLoan()) {
        std::cout << "You are not eligible for a loan at this time.\n";
        return nullptr;
    }
    
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                        LOAN APPLICATION\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    
    std::cout << "Loan Types Available:\n";
    std::cout << "1. Personal Loan\n";
    std::cout << "2. Home Loan\n";
    std::cout << "3. Business Loan\n";
    std::cout << "4. Education Loan\n";
    
    int loanTypeChoice;
    std::cout << "Select loan type: ";
    std::cin >> loanTypeChoice;
    
    std::string loanType;
    switch (loanTypeChoice) {
        case 1: loanType = "Personal"; break;
        case 2: loanType = "Home"; break;
        case 3: loanType = "Business"; break;
        case 4: loanType = "Education"; break;
        default: 
            std::cout << "Invalid choice.\n";
            return nullptr;
    }
    
    std::cout << "Enter loan amount: $";
    double amount;
    std::cin >> amount;
    
    if (!validateAmount(amount)) {
        std::cout << "Invalid amount.\n";
        return nullptr;
    }
    
    std::cout << "Enter loan term (in months): ";
    int termMonths;
    std::cin >> termMonths;
    
    if (termMonths <= 0 || termMonths > 360) {
        std::cout << "Invalid term. Must be between 1 and 360 months.\n";
        return nullptr;
    }
    
    auto loan = std::make_shared<Loan>(customer->getUserId(), loanType, amount, termMonths, customer->getCreditScore());
    loans.push_back(loan);
    customer->addLoan(loan);
    totalLoans += amount;
    
    std::cout << "Loan application submitted successfully!\n";
    std::cout << "Loan ID: " << loan->getLoanId() << "\n";
    std::cout << "Status: " << loan->getStatus() << "\n";
    
    return loan;
}

void BankingSystem::processLoanApplications() {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                PROCESSING LOAN APPLICATIONS\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    
    int pendingCount = 0;
    for (const auto& loan : loans) {
        if (loan->getStatus() == "Pending") {
            pendingCount++;
            std::cout << "\nLoan ID: " << loan->getLoanId() << "\n";
            std::cout << "Customer ID: " << loan->getCustomerId() << "\n";
            std::cout << "Amount: $" << std::fixed << std::setprecision(2) << loan->getAmount() << "\n";
            std::cout << "Type: " << loan->getLoanType() << "\n";
            std::cout << "Credit Score: " << loan->getCreditScore() << "\n";
            
            std::cout << "Approve this loan? (y/n): ";
            char choice;
            std::cin >> choice;
            
            if (choice == 'y' || choice == 'Y') {
                loan->approve();
                std::cout << "Loan approved!\n";
            } else {
                std::cout << "Loan rejected.\n";
            }
        }
    }
    
    if (pendingCount == 0) {
        std::cout << "No pending loan applications.\n";
    }
}

void BankingSystem::viewLoanStatus(std::shared_ptr<Customer> customer) const {
    auto customerLoans = customer->getAllLoans();
    if (customerLoans.empty()) {
        std::cout << "No loans found for this customer.\n";
        return;
    }
    
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                        LOAN STATUS\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    
    for (const auto& loan : customerLoans) {
        loan->displayInfo();
    }
}

bool BankingSystem::makeLoanPayment(std::shared_ptr<Customer> customer) {
    auto customerLoans = customer->getAllLoans();
    if (customerLoans.empty()) {
        std::cout << "No loans found for this customer.\n";
        return false;
    }
    
    std::cout << "Select loan to make payment:\n";
    for (size_t i = 0; i < customerLoans.size(); ++i) {
        std::cout << i + 1 << ". " << customerLoans[i]->getLoanId() 
                  << " (" << customerLoans[i]->getLoanType() << ") - $"
                  << std::fixed << std::setprecision(2) << customerLoans[i]->getRemainingBalance() << "\n";
    }
    
    int choice;
    std::cout << "Enter choice: ";
    std::cin >> choice;
    
    if (choice > 0 && choice <= static_cast<int>(customerLoans.size())) {
        auto loan = customerLoans[choice - 1];
        if (loan->getStatus() != "Active") {
            std::cout << "This loan is not active.\n";
            return false;
        }
        
        std::cout << "Enter payment amount: $";
        double amount;
        std::cin >> amount;
        
        if (loan->makePayment(amount)) {
            std::cout << "Payment successful! Remaining balance: $"
                      << std::fixed << std::setprecision(2) << loan->getRemainingBalance() << "\n";
            return true;
        } else {
            std::cout << "Payment failed.\n";
            return false;
        }
    }
    
    std::cout << "Invalid choice.\n";
    return false;
}

void BankingSystem::displayAllLoans() const {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                        ALL LOANS\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    
    if (loans.empty()) {
        std::cout << "No loans found.\n";
    } else {
        std::cout << std::setw(15) << "Loan ID" << std::setw(15) << "Customer ID" 
                  << std::setw(15) << "Type" << std::setw(15) << "Amount" 
                  << std::setw(15) << "Status\n";
        std::cout << std::string(75, '-') << "\n";
        
        for (const auto& loan : loans) {
            std::cout << std::setw(15) << loan->getLoanId()
                      << std::setw(15) << loan->getCustomerId()
                      << std::setw(15) << loan->getLoanType()
                      << std::setw(15) << "$" << std::fixed << std::setprecision(2) << loan->getAmount()
                      << std::setw(15) << loan->getStatus() << "\n";
        }
    }
    
    std::cout << "══════════════════════════════════════════════════════════════\n";
}

// Account operations
void BankingSystem::displayAccountBalance(std::shared_ptr<Customer> customer) const {
    customer->displayAccountSummary();
}

void BankingSystem::changePassword(std::shared_ptr<Customer> customer) {
    std::cout << "Enter new password: ";
    std::string newPassword;
    std::cin >> newPassword;
    
    customer->changePassword(newPassword);
    std::cout << "Password changed successfully!\n";
}

void BankingSystem::calculateInterest() {
    std::cout << "\nCalculating interest for all accounts...\n";
    for (auto& account : accounts) {
        if (account->isActive() && account->getBalance() > 0) {
            double interest = account->calculateInterest();
            if (interest > 0) {
                std::cout << "Account " << account->getAccountNumber() 
                          << ": Interest = $" << std::fixed << std::setprecision(2) << interest << "\n";
            }
        }
    }
    std::cout << "Interest calculation completed.\n";
}

void BankingSystem::applyInterestToAllAccounts() {
    std::cout << "\nApplying interest to all accounts...\n";
    for (auto& account : accounts) {
        if (account->isActive() && account->getBalance() > 0) {
            account->applyInterest();
        }
    }
    std::cout << "Interest applied to all accounts.\n";
}

// System operations
void BankingSystem::createNewAccount() {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                    CREATE NEW ACCOUNT\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    
    std::string name, email, phone, address, password;
    
    std::cout << "Enter full name: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    
    std::cout << "Enter email: ";
    std::cin >> email;
    
    std::cout << "Enter phone: ";
    std::cin >> phone;
    
    std::cout << "Enter address: ";
    std::cin.ignore();
    std::getline(std::cin, address);
    
    std::cout << "Enter password: ";
    std::cin >> password;
    
    auto customer = createCustomer(name, email, phone, address, password);
    
    std::cout << "Select account type:\n";
    std::cout << "1. Savings Account\n";
    std::cout << "2. Checking Account\n";
    
    int choice;
    std::cout << "Enter choice: ";
    std::cin >> choice;
    
    std::string accountType;
    switch (choice) {
        case 1: accountType = "Savings"; break;
        case 2: accountType = "Checking"; break;
        default: 
            std::cout << "Invalid choice. Creating savings account.\n";
            accountType = "Savings";
    }
    
    std::cout << "Enter initial deposit amount: $";
    double initialBalance;
    std::cin >> initialBalance;
    
    auto account = createAccount(customer->getUserId(), accountType, initialBalance);
    
    std::cout << "\nAccount created successfully!\n";
    std::cout << "Customer ID: " << customer->getUserId() << "\n";
    std::cout << "Account Number: " << customer->getAccountNumber() << "\n";
    std::cout << "Account Type: " << accountType << "\n";
    std::cout << "Initial Balance: $" << std::fixed << std::setprecision(2) << initialBalance << "\n";
}

void BankingSystem::backupDatabase() {
    std::cout << "\nCreating backup of database...\n";
    saveData();
    std::cout << "Backup completed successfully!\n";
}

void BankingSystem::displaySystemStatistics() const {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                    SYSTEM STATISTICS\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "Total Customers: " << totalCustomers << "\n";
    std::cout << "Total Accounts: " << totalAccounts << "\n";
    std::cout << "Total Transactions: " << totalTransactions << "\n";
    std::cout << "Total Loans: " << loans.size() << "\n";
    std::cout << "Total Deposits: $" << std::fixed << std::setprecision(2) << totalDeposits << "\n";
    std::cout << "Total Withdrawals: $" << std::fixed << std::setprecision(2) << totalWithdrawals << "\n";
    std::cout << "Total Loans Amount: $" << std::fixed << std::setprecision(2) << totalLoans << "\n";
    
    double totalBalance = 0.0;
    for (const auto& account : accounts) {
        totalBalance += account->getBalance();
    }
    std::cout << "Total Bank Balance: $" << std::fixed << std::setprecision(2) << totalBalance << "\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
}

void BankingSystem::loadData() {
    loadCustomersFromFile();
    loadAccountsFromFile();
    loadTransactionsFromFile();
    loadLoansFromFile();
    updateSystemStatistics();
}

void BankingSystem::saveData() {
    saveCustomersToFile();
    saveAccountsToFile();
    saveTransactionsToFile();
    saveLoansToFile();
}

void BankingSystem::clearData() {
    customers.clear();
    accounts.clear();
    transactions.clear();
    loans.clear();
    totalCustomers = totalAccounts = totalTransactions = 0;
    totalDeposits = totalWithdrawals = totalLoans = 0.0;
}

// Utility methods
void BankingSystem::updateSystemStatistics() {
    totalCustomers = customers.size();
    totalAccounts = accounts.size();
    totalTransactions = transactions.size();
    
    totalDeposits = 0.0;
    totalWithdrawals = 0.0;
    totalLoans = 0.0;
    
    for (const auto& transaction : transactions) {
        if (transaction->getAmount() > 0) {
            totalDeposits += transaction->getAmount();
        } else {
            totalWithdrawals += std::abs(transaction->getAmount());
        }
    }
    
    for (const auto& loan : loans) {
        totalLoans += loan->getAmount();
    }
}

std::string BankingSystem::generateReport() const {
    std::ostringstream oss;
    oss << "Banking System Report\n";
    oss << "Generated: " << getCurrentDateTime() << "\n";
    oss << "Total Customers: " << totalCustomers << "\n";
    oss << "Total Accounts: " << totalAccounts << "\n";
    oss << "Total Transactions: " << totalTransactions << "\n";
    return oss.str();
}

bool BankingSystem::validateAccountNumber(const std::string& accountNumber) const {
    return accountNumber.length() >= 8 && accountNumber.length() <= 12;
}

bool BankingSystem::validateAmount(double amount) const {
    return amount > 0 && amount <= 1000000.0; // Max $1M per transaction
}

void BankingSystem::displayWelcomeMessage() const {
    std::cout << "\nWelcome to Oyanib Banking System!\n";
    std::cout << "Your trusted financial partner.\n";
}

void BankingSystem::displaySystemInfo() const {
    std::cout << "\nOyanib Banking System v2.0\n";
    std::cout << "Advanced C++ Implementation\n";
    std::cout << "Features: Multi-account types, Loan management, Interest calculation\n";
}

// Helper methods
void BankingSystem::loadCustomersFromFile() {
    std::ifstream file(customersFile);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            auto customer = std::make_shared<Customer>();
            customer->fromFileString(line);
            customers.push_back(customer);
        }
        file.close();
    }
}

void BankingSystem::loadAccountsFromFile() {
    std::ifstream file(accountsFile);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            auto account = std::make_shared<Account>();
            account->fromFileString(line);
            accounts.push_back(account);
        }
        file.close();
    }
}

void BankingSystem::loadTransactionsFromFile() {
    std::ifstream file(transactionsFile);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            auto transaction = std::make_shared<Transaction>();
            transaction->fromFileString(line);
            transactions.push_back(transaction);
        }
        file.close();
    }
}

void BankingSystem::loadLoansFromFile() {
    std::ifstream file(loansFile);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            auto loan = std::make_shared<Loan>();
            loan->fromFileString(line);
            loans.push_back(loan);
        }
        file.close();
    }
}

void BankingSystem::saveCustomersToFile() {
    std::ofstream file(customersFile);
    if (file.is_open()) {
        for (const auto& customer : customers) {
            file << customer->toFileString() << "\n";
        }
        file.close();
    }
}

void BankingSystem::saveAccountsToFile() {
    std::ofstream file(accountsFile);
    if (file.is_open()) {
        for (const auto& account : accounts) {
            file << account->toFileString() << "\n";
        }
        file.close();
    }
}

void BankingSystem::saveTransactionsToFile() {
    std::ofstream file(transactionsFile);
    if (file.is_open()) {
        for (const auto& transaction : transactions) {
            file << transaction->toFileString() << "\n";
        }
        file.close();
    }
}

void BankingSystem::saveLoansToFile() {
    std::ofstream file(loansFile);
    if (file.is_open()) {
        for (const auto& loan : loans) {
            file << loan->toFileString() << "\n";
        }
        file.close();
    }
}

std::string BankingSystem::getCurrentDateTime() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void BankingSystem::createSampleData() {
    // Create sample customer
    auto customer = createCustomer("John Doe", "john@example.com", "1234567890", 
                                  "123 Main St, City, State", "password123");
    
    // Create sample accounts
    createAccount(customer->getUserId(), "Savings", 5000.0);
    createAccount(customer->getUserId(), "Checking", 2500.0);
    
    std::cout << "Sample data created successfully!\n";
}
