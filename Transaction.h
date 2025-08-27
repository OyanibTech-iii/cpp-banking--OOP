#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <chrono>

class Transaction {
private:
    std::string transactionId;
    std::string accountNumber;
    std::string type; // "Deposit", "Withdrawal", "Transfer", "Interest", "Loan"
    double amount;
    double balance;
    std::string date;
    std::string description;
    std::string status; // "Pending", "Completed", "Failed", "Cancelled"

public:
    // Constructors
    Transaction();
    Transaction(const std::string& accountNumber, const std::string& type, 
                double amount, double balance, const std::string& date);
    Transaction(const std::string& accountNumber, const std::string& type, 
                double amount, double balance, const std::string& date, 
                const std::string& description);
    ~Transaction();

    // Getters
    std::string getTransactionId() const;
    std::string getAccountNumber() const;
    std::string getType() const;
    double getAmount() const;
    double getBalance() const;
    std::string getDate() const;
    std::string getDescription() const;
    std::string getStatus() const;

    // Setters
    void setTransactionId(const std::string& id);
    void setAccountNumber(const std::string& accountNum);
    void setType(const std::string& type);
    void setAmount(double amount);
    void setBalance(double balance);
    void setDate(const std::string& date);
    void setDescription(const std::string& description);
    void setStatus(const std::string& status);

    // Utility methods
    void generateTransactionId();
    std::string getCurrentDateTime();
    void displayInfo() const;
    std::string toFileString() const;
    void fromFileString(const std::string& data);
    bool isCredit() const;
    bool isDebit() const;
    std::string getFormattedAmount() const;
};

#endif // TRANSACTION_H
