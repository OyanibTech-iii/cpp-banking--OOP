#ifndef BANKING_SYSTEM_H
#define BANKING_SYSTEM_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <fstream>

#include "Customer.h"
#include "Account.h"
#include "Transaction.h"
#include "Loan.h"
#include "SavingsAccount.h"

class BankingSystem {
private:
    std::vector<std::shared_ptr<Customer>> customers;
    std::vector<std::shared_ptr<Account>> accounts;
    std::vector<std::shared_ptr<Transaction>> transactions;
    std::vector<std::shared_ptr<Loan>> loans;
    
    // File paths for data persistence
    std::string customersFile;
    std::string accountsFile;
    std::string transactionsFile;
    std::string loansFile;
    
    // System statistics
    double totalDeposits;
    double totalWithdrawals;
    double totalLoans;
    int totalCustomers;
    int totalAccounts;
    int totalTransactions;

public:
    // Constructor and destructor
    BankingSystem();
    ~BankingSystem();

    // Customer management
    std::shared_ptr<Customer> createCustomer(const std::string& name, const std::string& email, 
                                            const std::string& phone, const std::string& address, 
                                            const std::string& password);
    std::shared_ptr<Customer> findCustomer(const std::string& accountNumber);
    std::shared_ptr<Customer> authenticateUser(const std::string& accountNumber, const std::string& password);
    void displayAllCustomers() const;
    void deleteCustomer(const std::string& accountNumber);

    // Account management
    std::shared_ptr<Account> createAccount(const std::string& customerId, const std::string& accountType, 
                                          double initialBalance = 0.0);
    std::shared_ptr<Account> findAccount(const std::string& accountNumber);
    void displayAllAccounts() const;
    void deleteAccount(const std::string& accountNumber);

    // Transaction management
    bool processDeposit(std::shared_ptr<Customer> customer);
    bool processWithdrawal(std::shared_ptr<Customer> customer);
    bool processTransfer(std::shared_ptr<Customer> customer);
    void displayTransactionHistory(std::shared_ptr<Customer> customer) const;
    void displayAllTransactions() const;

    // Loan management
    std::shared_ptr<Loan> applyForLoan(std::shared_ptr<Customer> customer);
    void processLoanApplications();
    void viewLoanStatus(std::shared_ptr<Customer> customer) const;
    bool makeLoanPayment(std::shared_ptr<Customer> customer);
    void displayAllLoans() const;

    // Account operations
    void displayAccountBalance(std::shared_ptr<Customer> customer) const;
    void changePassword(std::shared_ptr<Customer> customer);
    void calculateInterest();
    void applyInterestToAllAccounts();

    // System operations
    void createNewAccount();
    void backupDatabase();
    void displaySystemStatistics() const;
    void loadData();
    void saveData();
    void clearData();

    // Utility methods
    void updateSystemStatistics();
    std::string generateReport() const;
    bool validateAccountNumber(const std::string& accountNumber) const;
    bool validateAmount(double amount) const;
    void displayWelcomeMessage() const;
    void displaySystemInfo() const;

private:
    // Helper methods
    void loadCustomersFromFile();
    void loadAccountsFromFile();
    void loadTransactionsFromFile();
    void loadLoansFromFile();
    void saveCustomersToFile();
    void saveAccountsToFile();
    void saveTransactionsToFile();
    void saveLoansToFile();
    std::string getCurrentDateTime() const;
    void createSampleData();
};

#endif // BANKING_SYSTEM_H
