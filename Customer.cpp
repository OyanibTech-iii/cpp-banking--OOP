#include "Customer.h"
#include "Account.h"
#include "Loan.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <sstream>

Customer::Customer() : creditScore(650.0), customerType("Regular") {}

Customer::Customer(const std::string& name, const std::string& email, const std::string& phone, 
                   const std::string& address, const std::string& password)
    : User(name, email, phone, address, password), creditScore(650.0), customerType("Regular") {}

Customer::~Customer() {}

// Account management
void Customer::addAccount(std::shared_ptr<Account> account) {
    accounts.push_back(account);
}

void Customer::removeAccount(const std::string& accountNumber) {
    accounts.erase(
        std::remove_if(accounts.begin(), accounts.end(),
            [&accountNumber](const std::shared_ptr<Account>& account) {
                return account->getAccountNumber() == accountNumber;
            }),
        accounts.end()
    );
}

std::shared_ptr<Account> Customer::getAccount(const std::string& accountNumber) {
    for (const auto& account : accounts) {
        if (account->getAccountNumber() == accountNumber) {
            return account;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Account>> Customer::getAllAccounts() const {
    return accounts;
}

int Customer::getAccountCount() const {
    return accounts.size();
}

// Loan management
void Customer::addLoan(std::shared_ptr<Loan> loan) {
    loans.push_back(loan);
}

std::vector<std::shared_ptr<Loan>> Customer::getAllLoans() const {
    return loans;
}

int Customer::getLoanCount() const {
    return loans.size();
}

// Credit score management
double Customer::getCreditScore() const {
    return creditScore;
}

void Customer::setCreditScore(double score) {
    creditScore = std::max(300.0, std::min(850.0, score));
}

void Customer::updateCreditScore() {
    // Simple credit score calculation based on account balance and loan history
    double baseScore = 650.0;
    double balanceBonus = getTotalBalance() / 10000.0 * 50.0; // +50 points per $10k
    double loanPenalty = hasActiveLoans() ? -50.0 : 0.0;
    
    creditScore = baseScore + balanceBonus + loanPenalty;
    creditScore = std::max(300.0, std::min(850.0, creditScore));
}

// Customer type management
std::string Customer::getCustomerType() const {
    return customerType;
}

void Customer::setCustomerType(const std::string& type) {
    customerType = type;
}

void Customer::upgradeCustomerType() {
    double totalBalance = getTotalBalance();
    if (totalBalance >= 100000.0 && creditScore >= 750.0) {
        customerType = "VIP";
    } else if (totalBalance >= 50000.0 && creditScore >= 700.0) {
        customerType = "Premium";
    } else {
        customerType = "Regular";
    }
}

// Overridden methods
std::string Customer::getUserType() const {
    return "Customer";
}

void Customer::displayInfo() const {
    User::displayInfo();
    std::cout << "Credit Score: " << creditScore << "\n";
    std::cout << "Customer Type: " << customerType << "\n";
    std::cout << "Number of Accounts: " << getAccountCount() << "\n";
    std::cout << "Number of Loans: " << getLoanCount() << "\n";
    std::cout << "Total Balance: $" << std::fixed << std::setprecision(2) << getTotalBalance() << "\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
}

std::string Customer::toFileString() const {
    std::ostringstream oss;
    oss << User::toFileString() << "|" << creditScore << "|" << customerType;
    return oss.str();
}

void Customer::fromFileString(const std::string& data) {
    User::fromFileString(data);
    
    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(iss, token, '|')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() >= 12) {
        creditScore = std::stod(tokens[10]);
        customerType = tokens[11];
    }
}

// Customer-specific methods
double Customer::getTotalBalance() const {
    return std::accumulate(accounts.begin(), accounts.end(), 0.0,
        [](double sum, const std::shared_ptr<Account>& account) {
            return sum + account->getBalance();
        });
}

void Customer::displayAccountSummary() const {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                    ACCOUNT SUMMARY\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "Customer: " << getName() << " (" << getAccountNumber() << ")\n";
    std::cout << "Customer Type: " << customerType << "\n";
    std::cout << "Credit Score: " << creditScore << "\n";
    std::cout << "Total Balance: $" << std::fixed << std::setprecision(2) << getTotalBalance() << "\n";
    std::cout << "Number of Accounts: " << getAccountCount() << "\n";
    std::cout << "Number of Loans: " << getLoanCount() << "\n\n";
    
    if (!accounts.empty()) {
        std::cout << "Accounts:\n";
        std::cout << std::setw(15) << "Account No." << std::setw(15) << "Type" 
                  << std::setw(15) << "Balance" << std::setw(15) << "Status\n";
        std::cout << std::string(60, '-') << "\n";
        
        for (const auto& account : accounts) {
            std::cout << std::setw(15) << account->getAccountNumber()
                      << std::setw(15) << account->getAccountType()
                      << std::setw(15) << "$" << std::fixed << std::setprecision(2) << account->getBalance()
                      << std::setw(15) << (account->isActive() ? "Active" : "Inactive") << "\n";
        }
    }
    
    if (!loans.empty()) {
        std::cout << "\nLoans:\n";
        std::cout << std::setw(15) << "Loan ID" << std::setw(15) << "Amount" 
                  << std::setw(15) << "Status" << std::setw(15) << "Type\n";
        std::cout << std::string(60, '-') << "\n";
        
        for (const auto& loan : loans) {
            std::cout << std::setw(15) << loan->getLoanId()
                      << std::setw(15) << "$" << std::fixed << std::setprecision(2) << loan->getAmount()
                      << std::setw(15) << loan->getStatus()
                      << std::setw(15) << loan->getLoanType() << "\n";
        }
    }
    
    std::cout << "══════════════════════════════════════════════════════════════\n";
}

bool Customer::hasActiveLoans() const {
    return std::any_of(loans.begin(), loans.end(),
        [](const std::shared_ptr<Loan>& loan) {
            return loan->getStatus() == "Active";
        });
}

bool Customer::isEligibleForLoan() const {
    return creditScore >= 600.0 && !hasActiveLoans() && getTotalBalance() > 1000.0;
}
