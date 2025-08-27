#ifndef SAVINGS_ACCOUNT_H
#define SAVINGS_ACCOUNT_H

#include "Account.h"
#include "Transaction.h"

class SavingsAccount : public Account {
private:
    double minimumBalance;
    int monthlyTransactions;
    int maxMonthlyTransactions;
    double annualInterestRate;

public:
    // Constructors
    SavingsAccount();
    SavingsAccount(const std::string& customerId, double initialBalance = 0.0);
    ~SavingsAccount();

    // Overridden methods
    bool withdraw(double amount) override;
    bool transfer(Account& targetAccount, double amount) override;
    double calculateInterest() const override;
    void applyInterest() override;
    bool canWithdraw(double amount) const override;
    bool canTransfer(double amount) const override;
    
    std::string getAccountDetails() const override;
    void displayInfo() const override;
    std::string toFileString() const override;
    void fromFileString(const std::string& data) override;

    // Savings-specific methods
    void setMinimumBalance(double amount);
    double getMinimumBalance() const;
    int getMonthlyTransactions() const;
    int getMaxMonthlyTransactions() const;
    void setMaxMonthlyTransactions(int max);
    double getAnnualInterestRate() const;
    void setAnnualInterestRate(double rate);
    bool canMakeTransaction() const;
    void incrementTransactionCount();
    void resetMonthlyTransactions();
};

#endif // SAVINGS_ACCOUNT_H
