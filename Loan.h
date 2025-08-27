#ifndef LOAN_H
#define LOAN_H

#include <string>
#include <vector>
#include <memory>
#include <chrono>

class Transaction;

class Loan {
private:
    std::string loanId;
    std::string customerId;
    std::string loanType; // "Personal", "Home", "Business", "Education"
    double amount;
    double interestRate;
    int termMonths;
    std::string status; // "Pending", "Approved", "Active", "Paid", "Defaulted"
    std::string dateApplied;
    std::string dateApproved;
    std::string dateDisbursed;
    double monthlyPayment;
    double remainingBalance;
    std::vector<std::shared_ptr<Transaction>> payments;
    std::string description;
    double creditScore;

public:
    // Constructors
    Loan();
    Loan(const std::string& customerId, const std::string& loanType, double amount, 
         int termMonths, double creditScore);
    ~Loan();

    // Getters
    std::string getLoanId() const;
    std::string getCustomerId() const;
    std::string getLoanType() const;
    double getAmount() const;
    double getInterestRate() const;
    int getTermMonths() const;
    std::string getStatus() const;
    std::string getDateApplied() const;
    std::string getDateApproved() const;
    std::string getDateDisbursed() const;
    double getMonthlyPayment() const;
    double getRemainingBalance() const;
    std::string getDescription() const;
    double getCreditScore() const;

    // Setters
    void setLoanId(const std::string& id);
    void setCustomerId(const std::string& id);
    void setLoanType(const std::string& type);
    void setAmount(double amount);
    void setInterestRate(double rate);
    void setTermMonths(int months);
    void setStatus(const std::string& status);
    void setDateApproved(const std::string& date);
    void setDateDisbursed(const std::string& date);
    void setMonthlyPayment(double payment);
    void setRemainingBalance(double balance);
    void setDescription(const std::string& description);
    void setCreditScore(double score);

    // Loan management methods
    void approve();
    void disburse();
    bool makePayment(double amount);
    void calculateMonthlyPayment();
    void calculateRemainingBalance();
    double getTotalPaid() const;
    int getPaymentsMade() const;
    bool isOverdue() const;
    int getDaysOverdue() const;
    double getLateFee() const;

    // Payment management
    void addPayment(std::shared_ptr<Transaction> payment);
    std::vector<std::shared_ptr<Transaction>> getPayments() const;
    void displayPaymentHistory() const;

    // Utility methods
    void generateLoanId();
    std::string getCurrentDateTime();
    void displayInfo() const;
    std::string toFileString() const;
    void fromFileString(const std::string& data);
    bool isEligible() const;
    double getLoanToValueRatio() const;
    std::string getNextPaymentDate() const;
};

#endif // LOAN_H
