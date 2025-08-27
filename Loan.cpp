#include "Loan.h"
#include "Transaction.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <random>
#include <cmath>

Loan::Loan() : amount(0.0), interestRate(0.0), termMonths(0), monthlyPayment(0.0), 
               remainingBalance(0.0), creditScore(0.0) {
    generateLoanId();
    dateApplied = getCurrentDateTime();
    status = "Pending";
}

Loan::Loan(const std::string& customerId, const std::string& loanType, double amount, 
           int termMonths, double creditScore)
    : customerId(customerId), loanType(loanType), amount(amount), termMonths(termMonths), 
      creditScore(creditScore), monthlyPayment(0.0), remainingBalance(amount) {
    generateLoanId();
    dateApplied = getCurrentDateTime();
    status = "Pending";
    
    // Set interest rate based on credit score and loan type
    if (creditScore >= 750) {
        interestRate = 5.0; // Excellent credit
    } else if (creditScore >= 700) {
        interestRate = 7.0; // Good credit
    } else if (creditScore >= 650) {
        interestRate = 9.0; // Fair credit
    } else {
        interestRate = 12.0; // Poor credit
    }
    
    // Adjust rate based on loan type
    if (loanType == "Home") {
        interestRate -= 1.0; // Lower rate for home loans
    } else if (loanType == "Education") {
        interestRate -= 0.5; // Lower rate for education loans
    }
    
    calculateMonthlyPayment();
}

Loan::~Loan() {}

// Getters
std::string Loan::getLoanId() const { return loanId; }
std::string Loan::getCustomerId() const { return customerId; }
std::string Loan::getLoanType() const { return loanType; }
double Loan::getAmount() const { return amount; }
double Loan::getInterestRate() const { return interestRate; }
int Loan::getTermMonths() const { return termMonths; }
std::string Loan::getStatus() const { return status; }
std::string Loan::getDateApplied() const { return dateApplied; }
std::string Loan::getDateApproved() const { return dateApproved; }
std::string Loan::getDateDisbursed() const { return dateDisbursed; }
double Loan::getMonthlyPayment() const { return monthlyPayment; }
double Loan::getRemainingBalance() const { return remainingBalance; }
std::string Loan::getDescription() const { return description; }
double Loan::getCreditScore() const { return creditScore; }

// Setters
void Loan::setLoanId(const std::string& id) { loanId = id; }
void Loan::setCustomerId(const std::string& id) { customerId = id; }
void Loan::setLoanType(const std::string& type) { loanType = type; }
void Loan::setAmount(double amount) { this->amount = amount; }
void Loan::setInterestRate(double rate) { interestRate = rate; }
void Loan::setTermMonths(int months) { termMonths = months; }
void Loan::setStatus(const std::string& status) { this->status = status; }
void Loan::setDateApproved(const std::string& date) { dateApproved = date; }
void Loan::setDateDisbursed(const std::string& date) { dateDisbursed = date; }
void Loan::setMonthlyPayment(double payment) { monthlyPayment = payment; }
void Loan::setRemainingBalance(double balance) { remainingBalance = balance; }
void Loan::setDescription(const std::string& description) { this->description = description; }
void Loan::setCreditScore(double score) { creditScore = score; }

// Loan management methods
void Loan::approve() {
    status = "Approved";
    dateApproved = getCurrentDateTime();
}

void Loan::disburse() {
    if (status == "Approved") {
        status = "Active";
        dateDisbursed = getCurrentDateTime();
        remainingBalance = amount;
    }
}

bool Loan::makePayment(double paymentAmount) {
    if (status != "Active" || paymentAmount <= 0) {
        return false;
    }
    
    if (paymentAmount >= remainingBalance) {
        paymentAmount = remainingBalance;
        status = "Paid";
    }
    
    remainingBalance -= paymentAmount;
    
    // Create payment transaction
    auto payment = std::make_shared<Transaction>(
        customerId, "Loan Payment", -paymentAmount, remainingBalance, getCurrentDateTime(),
        "Loan payment for " + loanId
    );
    addPayment(payment);
    
    return true;
}

void Loan::calculateMonthlyPayment() {
    if (termMonths > 0 && interestRate > 0) {
        double monthlyRate = interestRate / 100.0 / 12.0;
        monthlyPayment = amount * (monthlyRate * pow(1 + monthlyRate, termMonths)) / 
                        (pow(1 + monthlyRate, termMonths) - 1);
    }
}

void Loan::calculateRemainingBalance() {
    double totalPaid = getTotalPaid();
    remainingBalance = amount - totalPaid;
    if (remainingBalance <= 0) {
        remainingBalance = 0;
        status = "Paid";
    }
}

double Loan::getTotalPaid() const {
    double total = 0.0;
    for (const auto& payment : payments) {
        total += std::abs(payment->getAmount());
    }
    return total;
}

int Loan::getPaymentsMade() const {
    return payments.size();
}

bool Loan::isOverdue() const {
    if (status != "Active") return false;
    
    // Simple overdue calculation (30 days past due)
    // In a real system, you'd track payment due dates
    return getDaysOverdue() > 30;
}

int Loan::getDaysOverdue() const {
    // Simplified calculation - in real system, track actual due dates
    return 0; // Placeholder
}

double Loan::getLateFee() const {
    if (isOverdue()) {
        return monthlyPayment * 0.05; // 5% late fee
    }
    return 0.0;
}

// Payment management
void Loan::addPayment(std::shared_ptr<Transaction> payment) {
    payments.push_back(payment);
}

std::vector<std::shared_ptr<Transaction>> Loan::getPayments() const {
    return payments;
}

void Loan::displayPaymentHistory() const {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                    LOAN PAYMENT HISTORY\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "Loan ID: " << loanId << "\n";
    std::cout << "Loan Type: " << loanType << "\n";
    std::cout << "Original Amount: $" << std::fixed << std::setprecision(2) << amount << "\n";
    std::cout << "Remaining Balance: $" << std::fixed << std::setprecision(2) << remainingBalance << "\n";
    std::cout << "Total Paid: $" << std::fixed << std::setprecision(2) << getTotalPaid() << "\n";
    std::cout << "Payments Made: " << getPaymentsMade() << "\n\n";
    
    if (payments.empty()) {
        std::cout << "No payments recorded.\n";
    } else {
        std::cout << std::setw(20) << "Date" << std::setw(15) << "Amount" 
                  << std::setw(15) << "Remaining\n";
        std::cout << std::string(50, '-') << "\n";
        
        for (const auto& payment : payments) {
            std::cout << std::setw(20) << payment->getDate()
                      << std::setw(15) << "$" << std::fixed << std::setprecision(2) << std::abs(payment->getAmount())
                      << std::setw(15) << "$" << std::fixed << std::setprecision(2) << payment->getBalance() << "\n";
        }
    }
    
    std::cout << "══════════════════════════════════════════════════════════════\n";
}

// Utility methods
void Loan::generateLoanId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);
    loanId = "LOAN" + std::to_string(dis(gen));
}

std::string Loan::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void Loan::displayInfo() const {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                        LOAN INFORMATION\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "Loan ID: " << loanId << "\n";
    std::cout << "Customer ID: " << customerId << "\n";
    std::cout << "Loan Type: " << loanType << "\n";
    std::cout << "Amount: $" << std::fixed << std::setprecision(2) << amount << "\n";
    std::cout << "Interest Rate: " << interestRate << "%\n";
    std::cout << "Term: " << termMonths << " months\n";
    std::cout << "Monthly Payment: $" << std::fixed << std::setprecision(2) << monthlyPayment << "\n";
    std::cout << "Remaining Balance: $" << std::fixed << std::setprecision(2) << remainingBalance << "\n";
    std::cout << "Status: " << status << "\n";
    std::cout << "Credit Score: " << creditScore << "\n";
    std::cout << "Date Applied: " << dateApplied << "\n";
    if (!dateApproved.empty()) {
        std::cout << "Date Approved: " << dateApproved << "\n";
    }
    if (!dateDisbursed.empty()) {
        std::cout << "Date Disbursed: " << dateDisbursed << "\n";
    }
    if (!description.empty()) {
        std::cout << "Description: " << description << "\n";
    }
    std::cout << "══════════════════════════════════════════════════════════════\n";
}

std::string Loan::toFileString() const {
    std::ostringstream oss;
    oss << loanId << "|" << customerId << "|" << loanType << "|" << amount << "|"
        << interestRate << "|" << termMonths << "|" << status << "|" << dateApplied << "|"
        << dateApproved << "|" << dateDisbursed << "|" << monthlyPayment << "|"
        << remainingBalance << "|" << description << "|" << creditScore;
    return oss.str();
}

void Loan::fromFileString(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(iss, token, '|')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() >= 14) {
        loanId = tokens[0];
        customerId = tokens[1];
        loanType = tokens[2];
        amount = std::stod(tokens[3]);
        interestRate = std::stod(tokens[4]);
        termMonths = std::stoi(tokens[5]);
        status = tokens[6];
        dateApplied = tokens[7];
        dateApproved = tokens[8];
        dateDisbursed = tokens[9];
        monthlyPayment = std::stod(tokens[10]);
        remainingBalance = std::stod(tokens[11]);
        description = tokens[12];
        creditScore = std::stod(tokens[13]);
    }
}

bool Loan::isEligible() const {
    return creditScore >= 600 && amount > 0 && termMonths > 0;
}

double Loan::getLoanToValueRatio() const {
    // Simplified LTV calculation
    return amount / 100000.0; // Assuming $100k as base value
}

std::string Loan::getNextPaymentDate() const {
    // Simplified next payment date calculation
    // In a real system, you'd track actual payment schedules
    return "Next payment due: Monthly";
}
