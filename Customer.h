#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include <vector>
#include <memory>

class Account;
class Loan;

class Customer : public User {
private:
    std::vector<std::shared_ptr<Account>> accounts;
    std::vector<std::shared_ptr<Loan>> loans;
    double creditScore;
    std::string customerType; // "Regular", "Premium", "VIP"

public:
    // Constructors
    Customer();
    Customer(const std::string& name, const std::string& email, const std::string& phone, 
             const std::string& address, const std::string& password);
    ~Customer();

    // Account management
    void addAccount(std::shared_ptr<Account> account);
    void removeAccount(const std::string& accountNumber);
    std::shared_ptr<Account> getAccount(const std::string& accountNumber);
    std::vector<std::shared_ptr<Account>> getAllAccounts() const;
    int getAccountCount() const;

    // Loan management
    void addLoan(std::shared_ptr<Loan> loan);
    std::vector<std::shared_ptr<Loan>> getAllLoans() const;
    int getLoanCount() const;

    // Credit score management
    double getCreditScore() const;
    void setCreditScore(double score);
    void updateCreditScore();

    // Customer type management
    std::string getCustomerType() const;
    void setCustomerType(const std::string& type);
    void upgradeCustomerType();

    // Overridden methods
    std::string getUserType() const override;
    void displayInfo() const override;
    std::string toFileString() const override;
    void fromFileString(const std::string& data) override;

    // Customer-specific methods
    double getTotalBalance() const;
    void displayAccountSummary() const;
    bool hasActiveLoans() const;
    bool isEligibleForLoan() const;
};

#endif // CUSTOMER_H
