#include "SavingsAccount.h"
#include <iostream>
#include <sstream>
#include <iomanip>

SavingsAccount::SavingsAccount() : minimumBalance(500.0), monthlyTransactions(0), 
                                   maxMonthlyTransactions(6), annualInterestRate(2.5) {
    setAccountType("Savings");
    setInterestRate(annualInterestRate);
    setMinimumBalance(minimumBalance);
    setDailyWithdrawalLimit(2000.0);
    setMonthlyWithdrawalLimit(10000.0);
}

SavingsAccount::SavingsAccount(const std::string& customerId, double initialBalance)
    : Account(customerId, "Savings", initialBalance), minimumBalance(500.0), 
      monthlyTransactions(0), maxMonthlyTransactions(6), annualInterestRate(2.5) {
    setInterestRate(annualInterestRate);
    setMinimumBalance(minimumBalance);
    setDailyWithdrawalLimit(2000.0);
    setMonthlyWithdrawalLimit(10000.0);
}

SavingsAccount::~SavingsAccount() {}

// Overridden methods
bool SavingsAccount::withdraw(double amount) {
    if (!canWithdraw(amount) || !canMakeTransaction()) {
        return false;
    }
    
    bool success = Account::withdraw(amount);
    if (success) {
        incrementTransactionCount();
    }
    return success;
}

bool SavingsAccount::transfer(Account& targetAccount, double amount) {
    if (!canTransfer(amount) || !canMakeTransaction()) {
        return false;
    }
    
    bool success = Account::transfer(targetAccount, amount);
    if (success) {
        incrementTransactionCount();
    }
    return success;
}

double SavingsAccount::calculateInterest() const {
    return getBalance() * annualInterestRate / 100.0 / 12.0; // Monthly interest
}

void SavingsAccount::applyInterest() {
    double interest = calculateInterest();
    if (interest > 0) {
        setBalance(getBalance() + interest);
        
        // Create transaction record for interest
        auto transaction = std::make_shared<Transaction>(
            getAccountNumber(), "Interest", interest, getBalance(), getCurrentDateTime()
        );
        addTransaction(transaction);
    }
}

bool SavingsAccount::canWithdraw(double amount) const {
    return Account::canWithdraw(amount) && 
           getBalance() - amount >= minimumBalance &&
           canMakeTransaction();
}

bool SavingsAccount::canTransfer(double amount) const {
    return canWithdraw(amount);
}

std::string SavingsAccount::getAccountDetails() const {
    std::ostringstream oss;
    oss << Account::getAccountDetails() << "\n"
        << "Minimum Balance Required: $" << std::fixed << std::setprecision(2) << minimumBalance << "\n"
        << "Monthly Transactions: " << monthlyTransactions << "/" << maxMonthlyTransactions << "\n"
        << "Annual Interest Rate: " << annualInterestRate << "%";
    return oss.str();
}

void SavingsAccount::displayInfo() const {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                SAVINGS ACCOUNT INFORMATION\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << getAccountDetails() << "\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
}

std::string SavingsAccount::toFileString() const {
    std::ostringstream oss;
    oss << Account::toFileString() << "|" << minimumBalance << "|" 
        << monthlyTransactions << "|" << maxMonthlyTransactions << "|" << annualInterestRate;
    return oss.str();
}

void SavingsAccount::fromFileString(const std::string& data) {
    Account::fromFileString(data);
    
    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(iss, token, '|')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() >= 18) {
        minimumBalance = std::stod(tokens[14]);
        monthlyTransactions = std::stoi(tokens[15]);
        maxMonthlyTransactions = std::stoi(tokens[16]);
        annualInterestRate = std::stod(tokens[17]);
    }
}

// Savings-specific methods
void SavingsAccount::setMinimumBalance(double amount) {
    minimumBalance = amount;
}

double SavingsAccount::getMinimumBalance() const {
    return minimumBalance;
}

int SavingsAccount::getMonthlyTransactions() const {
    return monthlyTransactions;
}

int SavingsAccount::getMaxMonthlyTransactions() const {
    return maxMonthlyTransactions;
}

void SavingsAccount::setMaxMonthlyTransactions(int max) {
    maxMonthlyTransactions = max;
}

double SavingsAccount::getAnnualInterestRate() const {
    return annualInterestRate;
}

void SavingsAccount::setAnnualInterestRate(double rate) {
    annualInterestRate = rate;
    setInterestRate(rate);
}

bool SavingsAccount::canMakeTransaction() const {
    return monthlyTransactions < maxMonthlyTransactions;
}

void SavingsAccount::incrementTransactionCount() {
    monthlyTransactions++;
}

void SavingsAccount::resetMonthlyTransactions() {
    monthlyTransactions = 0;
}
