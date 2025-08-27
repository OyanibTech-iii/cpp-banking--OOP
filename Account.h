#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
#include <memory>
#include <chrono>

// Forward declaration
class Transaction;

class Account {
protected:
    std::string accountNumber;
    std::string accountType;
    double balance;
    double interestRate;
    bool accountActive;
    std::string dateCreated;
    std::string customerId;
    std::vector<std::shared_ptr<Transaction>> transactions;
    double minimumBalance;
    double dailyWithdrawalLimit;
    double monthlyWithdrawalLimit;
    double dailyWithdrawn;
    double monthlyWithdrawn;
    std::string lastTransactionDate;

public:
    // Constructors
    Account();
    Account(const std::string& customerId, const std::string& accountType, double initialBalance = 0.0);
    virtual ~Account();

    // Getters
    std::string getAccountNumber() const;
    std::string getAccountType() const;
    double getBalance() const;
    double getInterestRate() const;
    bool isActive() const;
    std::string getDateCreated() const;
    std::string getCustomerId() const;
    double getMinimumBalance() const;
    double getDailyWithdrawalLimit() const;
    double getMonthlyWithdrawalLimit() const;

    // Setters
    void setAccountNumber(const std::string& number);
    void setAccountType(const std::string& type);
    void setBalance(double amount);
    void setInterestRate(double rate);
    void setActive(bool active);
    void setDateCreated(const std::string& date);
    void setCustomerId(const std::string& id);
    void setMinimumBalance(double amount);
    void setDailyWithdrawalLimit(double limit);
    void setMonthlyWithdrawalLimit(double limit);

    // Transaction methods
    virtual bool deposit(double amount);
    virtual bool withdraw(double amount);
    virtual bool transfer(Account& targetAccount, double amount);
    void addTransaction(std::shared_ptr<Transaction> transaction);
    std::vector<std::shared_ptr<Transaction>> getTransactions() const;
    void displayTransactionHistory() const;

    // Interest calculation
    virtual double calculateInterest() const;
    virtual void applyInterest();

    // Validation methods
    virtual bool canWithdraw(double amount) const;
    virtual bool canTransfer(double amount) const;
    bool isWithinDailyLimit(double amount) const;
    bool isWithinMonthlyLimit(double amount) const;

    // Virtual methods for polymorphism
    virtual std::string getAccountDetails() const;
    virtual void displayInfo() const;
    virtual std::string toFileString() const;
    virtual void fromFileString(const std::string& data);

    // Utility methods
    void generateAccountNumber();
    std::string getCurrentDateTime();
    void resetDailyLimits();
    void resetMonthlyLimits();
    void updateLimits();
};

#endif // ACCOUNT_H
