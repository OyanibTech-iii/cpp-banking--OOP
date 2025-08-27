#include "Transaction.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <random>

Transaction::Transaction() : amount(0.0), balance(0.0), status("Pending") {
    generateTransactionId();
    date = getCurrentDateTime();
}

Transaction::Transaction(const std::string& accountNumber, const std::string& type, 
                         double amount, double balance, const std::string& date)
    : accountNumber(accountNumber), type(type), amount(amount), balance(balance), 
      date(date), status("Completed") {
    generateTransactionId();
}

Transaction::Transaction(const std::string& accountNumber, const std::string& type, 
                         double amount, double balance, const std::string& date, 
                         const std::string& description)
    : accountNumber(accountNumber), type(type), amount(amount), balance(balance), 
      date(date), description(description), status("Completed") {
    generateTransactionId();
}

Transaction::~Transaction() {}

// Getters
std::string Transaction::getTransactionId() const { return transactionId; }
std::string Transaction::getAccountNumber() const { return accountNumber; }
std::string Transaction::getType() const { return type; }
double Transaction::getAmount() const { return amount; }
double Transaction::getBalance() const { return balance; }
std::string Transaction::getDate() const { return date; }
std::string Transaction::getDescription() const { return description; }
std::string Transaction::getStatus() const { return status; }

// Setters
void Transaction::setTransactionId(const std::string& id) { transactionId = id; }
void Transaction::setAccountNumber(const std::string& accountNum) { accountNumber = accountNum; }
void Transaction::setType(const std::string& type) { this->type = type; }
void Transaction::setAmount(double amount) { this->amount = amount; }
void Transaction::setBalance(double balance) { this->balance = balance; }
void Transaction::setDate(const std::string& date) { this->date = date; }
void Transaction::setDescription(const std::string& description) { this->description = description; }
void Transaction::setStatus(const std::string& status) { this->status = status; }

// Utility methods
void Transaction::generateTransactionId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000000, 999999999);
    transactionId = "TXN" + std::to_string(dis(gen));
}

std::string Transaction::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void Transaction::displayInfo() const {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                    TRANSACTION DETAILS\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "Transaction ID: " << transactionId << "\n";
    std::cout << "Account Number: " << accountNumber << "\n";
    std::cout << "Type: " << type << "\n";
    std::cout << "Amount: " << getFormattedAmount() << "\n";
    std::cout << "Balance After: $" << std::fixed << std::setprecision(2) << balance << "\n";
    std::cout << "Date: " << date << "\n";
    std::cout << "Status: " << status << "\n";
    if (!description.empty()) {
        std::cout << "Description: " << description << "\n";
    }
    std::cout << "══════════════════════════════════════════════════════════════\n";
}

std::string Transaction::toFileString() const {
    std::ostringstream oss;
    oss << transactionId << "|" << accountNumber << "|" << type << "|" << amount << "|"
        << balance << "|" << date << "|" << description << "|" << status;
    return oss.str();
}

void Transaction::fromFileString(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(iss, token, '|')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() >= 8) {
        transactionId = tokens[0];
        accountNumber = tokens[1];
        type = tokens[2];
        amount = std::stod(tokens[3]);
        balance = std::stod(tokens[4]);
        date = tokens[5];
        description = tokens[6];
        status = tokens[7];
    }
}

bool Transaction::isCredit() const {
    return amount > 0;
}

bool Transaction::isDebit() const {
    return amount < 0;
}

std::string Transaction::getFormattedAmount() const {
    std::ostringstream oss;
    if (isCredit()) {
        oss << "+$" << std::fixed << std::setprecision(2) << amount;
    } else {
        oss << "-$" << std::fixed << std::setprecision(2) << std::abs(amount);
    }
    return oss.str();
}
