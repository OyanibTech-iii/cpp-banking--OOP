#include "Account.h"
#include "Transaction.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <random>
#include <algorithm>

Account::Account() : balance(0.0), interestRate(0.0), accountActive(true), 
                     minimumBalance(0.0), dailyWithdrawalLimit(1000.0), 
                     monthlyWithdrawalLimit(5000.0), dailyWithdrawn(0.0), 
                     monthlyWithdrawn(0.0) {
    generateAccountNumber();
    dateCreated = getCurrentDateTime();
    lastTransactionDate = getCurrentDateTime();
}

Account::Account(const std::string& customerId, const std::string& accountType, double initialBalance)
    : customerId(customerId), accountType(accountType), balance(initialBalance), 
      interestRate(0.0), accountActive(true), minimumBalance(0.0), 
      dailyWithdrawalLimit(1000.0), monthlyWithdrawalLimit(5000.0), 
      dailyWithdrawn(0.0), monthlyWithdrawn(0.0) {
    generateAccountNumber();
    dateCreated = getCurrentDateTime();
    lastTransactionDate = getCurrentDateTime();
}

Account::~Account() {}

// Getters
std::string Account::getAccountNumber() const { return accountNumber; }
std::string Account::getAccountType() const { return accountType; }
double Account::getBalance() const { return balance; }
double Account::getInterestRate() const { return interestRate; }
bool Account::isActive() const { return accountActive; }
std::string Account::getDateCreated() const { return dateCreated; }
std::string Account::getCustomerId() const { return customerId; }
double Account::getMinimumBalance() const { return minimumBalance; }
double Account::getDailyWithdrawalLimit() const { return dailyWithdrawalLimit; }
double Account::getMonthlyWithdrawalLimit() const { return monthlyWithdrawalLimit; }

// Setters
void Account::setAccountNumber(const std::string& number) { accountNumber = number; }
void Account::setAccountType(const std::string& type) { accountType = type; }
void Account::setBalance(double amount) { balance = amount; }
void Account::setInterestRate(double rate) { interestRate = rate; }
void Account::setActive(bool active) { accountActive = active; }
void Account::setDateCreated(const std::string& date) { dateCreated = date; }
void Account::setCustomerId(const std::string& id) { customerId = id; }
void Account::setMinimumBalance(double amount) { minimumBalance = amount; }
void Account::setDailyWithdrawalLimit(double limit) { dailyWithdrawalLimit = limit; }
void Account::setMonthlyWithdrawalLimit(double limit) { monthlyWithdrawalLimit = limit; }

// Transaction methods
bool Account::deposit(double amount) {
    if (amount <= 0 || !accountActive) {
        return false;
    }
    
    balance += amount;
    updateLimits();
    
    // Create transaction record
    auto transaction = std::make_shared<Transaction>(
        accountNumber, "Deposit", amount, balance, getCurrentDateTime()
    );
    addTransaction(transaction);
    
    return true;
}

bool Account::withdraw(double amount) {
    if (!canWithdraw(amount)) {
        return false;
    }
    
    balance -= amount;
    dailyWithdrawn += amount;
    monthlyWithdrawn += amount;
    updateLimits();
    
    // Create transaction record
    auto transaction = std::make_shared<Transaction>(
        accountNumber, "Withdrawal", -amount, balance, getCurrentDateTime()
    );
    addTransaction(transaction);
    
    return true;
}

bool Account::transfer(Account& targetAccount, double amount) {
    if (!canTransfer(amount) || !targetAccount.isActive()) {
        return false;
    }
    
    // Withdraw from this account
    balance -= amount;
    dailyWithdrawn += amount;
    monthlyWithdrawn += amount;
    
    // Deposit to target account
    targetAccount.balance += amount;
    
    updateLimits();
    targetAccount.updateLimits();
    
    // Create transaction records
    auto transaction1 = std::make_shared<Transaction>(
        accountNumber, "Transfer Out", -amount, balance, getCurrentDateTime()
    );
    auto transaction2 = std::make_shared<Transaction>(
        targetAccount.getAccountNumber(), "Transfer In", amount, targetAccount.getBalance(), getCurrentDateTime()
    );
    
    addTransaction(transaction1);
    targetAccount.addTransaction(transaction2);
    
    return true;
}

void Account::addTransaction(std::shared_ptr<Transaction> transaction) {
    transactions.push_back(transaction);
}

std::vector<std::shared_ptr<Transaction>> Account::getTransactions() const {
    return transactions;
}

void Account::displayTransactionHistory() const {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                TRANSACTION HISTORY\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "Account: " << accountNumber << " (" << accountType << ")\n";
    std::cout << "Current Balance: $" << std::fixed << std::setprecision(2) << balance << "\n\n";
    
    if (transactions.empty()) {
        std::cout << "No transactions found.\n";
    } else {
        std::cout << std::setw(20) << "Date" << std::setw(15) << "Type" 
                  << std::setw(15) << "Amount" << std::setw(15) << "Balance\n";
        std::cout << std::string(65, '-') << "\n";
        
        for (const auto& transaction : transactions) {
            std::cout << std::setw(20) << transaction->getDate()
                      << std::setw(15) << transaction->getType()
                      << std::setw(15) << "$" << std::fixed << std::setprecision(2) << transaction->getAmount()
                      << std::setw(15) << "$" << std::fixed << std::setprecision(2) << transaction->getBalance() << "\n";
        }
    }
    
    std::cout << "══════════════════════════════════════════════════════════════\n";
}

// Interest calculation
double Account::calculateInterest() const {
    return balance * interestRate / 100.0;
}

void Account::applyInterest() {
    double interest = calculateInterest();
    if (interest > 0) {
        balance += interest;
        
        auto transaction = std::make_shared<Transaction>(
            accountNumber, "Interest", interest, balance, getCurrentDateTime()
        );
        addTransaction(transaction);
    }
}

// Validation methods
bool Account::canWithdraw(double amount) const {
    return amount > 0 && accountActive && balance >= amount + minimumBalance && 
           isWithinDailyLimit(amount) && isWithinMonthlyLimit(amount);
}

bool Account::canTransfer(double amount) const {
    return canWithdraw(amount);
}

bool Account::isWithinDailyLimit(double amount) const {
    return dailyWithdrawn + amount <= dailyWithdrawalLimit;
}

bool Account::isWithinMonthlyLimit(double amount) const {
    return monthlyWithdrawn + amount <= monthlyWithdrawalLimit;
}

// Virtual methods
std::string Account::getAccountDetails() const {
    std::ostringstream oss;
    oss << "Account Number: " << accountNumber << "\n"
        << "Type: " << accountType << "\n"
        << "Balance: $" << std::fixed << std::setprecision(2) << balance << "\n"
        << "Interest Rate: " << interestRate << "%\n"
        << "Status: " << (accountActive ? "Active" : "Inactive") << "\n"
        << "Date Created: " << dateCreated;
    return oss.str();
}

void Account::displayInfo() const {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                    ACCOUNT INFORMATION\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << getAccountDetails() << "\n";
    std::cout << "Minimum Balance: $" << std::fixed << std::setprecision(2) << minimumBalance << "\n";
    std::cout << "Daily Withdrawal Limit: $" << std::fixed << std::setprecision(2) << dailyWithdrawalLimit << "\n";
    std::cout << "Monthly Withdrawal Limit: $" << std::fixed << std::setprecision(2) << monthlyWithdrawalLimit << "\n";
    std::cout << "Daily Withdrawn: $" << std::fixed << std::setprecision(2) << dailyWithdrawn << "\n";
    std::cout << "Monthly Withdrawn: $" << std::fixed << std::setprecision(2) << monthlyWithdrawn << "\n";
    std::cout << "Number of Transactions: " << transactions.size() << "\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
}

std::string Account::toFileString() const {
    std::ostringstream oss;
    oss << accountNumber << "|" << accountType << "|" << balance << "|" << interestRate << "|"
        << (accountActive ? "1" : "0") << "|" << dateCreated << "|" << customerId << "|"
        << minimumBalance << "|" << dailyWithdrawalLimit << "|" << monthlyWithdrawalLimit << "|"
        << dailyWithdrawn << "|" << monthlyWithdrawn << "|" << lastTransactionDate;
    return oss.str();
}

void Account::fromFileString(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(iss, token, '|')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() >= 13) {
        accountNumber = tokens[0];
        accountType = tokens[1];
        balance = std::stod(tokens[2]);
        interestRate = std::stod(tokens[3]);
        accountActive = (tokens[4] == "1");
        dateCreated = tokens[5];
        customerId = tokens[6];
        minimumBalance = std::stod(tokens[7]);
        dailyWithdrawalLimit = std::stod(tokens[8]);
        monthlyWithdrawalLimit = std::stod(tokens[9]);
        dailyWithdrawn = std::stod(tokens[10]);
        monthlyWithdrawn = std::stod(tokens[11]);
        lastTransactionDate = tokens[12];
    }
}

// Utility methods
void Account::generateAccountNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000000, 999999999);
    accountNumber = std::to_string(dis(gen));
}

std::string Account::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void Account::resetDailyLimits() {
    dailyWithdrawn = 0.0;
}

void Account::resetMonthlyLimits() {
    monthlyWithdrawn = 0.0;
}

void Account::updateLimits() {
    std::string currentDate = getCurrentDateTime();
    
    // Reset daily limits if it's a new day
    if (currentDate.substr(0, 10) != lastTransactionDate.substr(0, 10)) {
        resetDailyLimits();
    }
    
    // Reset monthly limits if it's a new month
    if (currentDate.substr(0, 7) != lastTransactionDate.substr(0, 7)) {
        resetMonthlyLimits();
    }
    
    lastTransactionDate = currentDate;
}
