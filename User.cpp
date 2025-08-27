#include "User.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <random>
#include <algorithm>
#include <chrono>

User::User() : isActive(true) {
    generateUserId();
    generateAccountNumber();
    dateCreated = getCurrentDateTime();
}

User::User(const std::string& name, const std::string& email, const std::string& phone, 
           const std::string& address, const std::string& password)
    : name(name), email(email), phone(phone), address(address), password(password), isActive(true) {
    generateUserId();
    generateAccountNumber();
    dateCreated = getCurrentDateTime();
}

User::~User() {}

// Getters
std::string User::getUserId() const { return userId; }
std::string User::getName() const { return name; }
std::string User::getEmail() const { return email; }
std::string User::getPhone() const { return phone; }
std::string User::getAddress() const { return address; }
std::string User::getAccountNumber() const { return accountNumber; }
bool User::getIsActive() const { return isActive; }
std::string User::getDateCreated() const { return dateCreated; }

// Setters
void User::setUserId(const std::string& id) { userId = id; }
void User::setName(const std::string& name) { this->name = name; }
void User::setEmail(const std::string& email) { this->email = email; }
void User::setPhone(const std::string& phone) { this->phone = phone; }
void User::setAddress(const std::string& address) { this->address = address; }
void User::setAccountNumber(const std::string& accountNum) { accountNumber = accountNum; }
void User::setIsActive(bool active) { isActive = active; }
void User::setDateCreated(const std::string& date) { dateCreated = date; }

// Authentication
bool User::authenticate(const std::string& password) const {
    return this->password == password;
}

void User::changePassword(const std::string& newPassword) {
    password = newPassword;
}

std::string User::getPassword() const {
    return password;
}

// Virtual methods
void User::displayInfo() const {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                    USER INFORMATION\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "User ID: " << userId << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Email: " << email << "\n";
    std::cout << "Phone: " << phone << "\n";
    std::cout << "Address: " << address << "\n";
    std::cout << "Account Number: " << accountNumber << "\n";
    std::cout << "Status: " << (isActive ? "Active" : "Inactive") << "\n";
    std::cout << "Date Created: " << dateCreated << "\n";
    std::cout << "User Type: " << getUserType() << "\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
}

std::string User::toFileString() const {
    std::ostringstream oss;
    oss << userId << "|" << name << "|" << email << "|" << phone << "|" 
        << address << "|" << password << "|" << accountNumber << "|" 
        << (isActive ? "1" : "0") << "|" << dateCreated << "|" << getUserType();
    return oss.str();
}

void User::fromFileString(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(iss, token, '|')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() >= 9) {
        userId = tokens[0];
        name = tokens[1];
        email = tokens[2];
        phone = tokens[3];
        address = tokens[4];
        password = tokens[5];
        accountNumber = tokens[6];
        isActive = (tokens[7] == "1");
        dateCreated = tokens[8];
    }
}

// Utility methods
void User::generateUserId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);
    userId = "U" + std::to_string(dis(gen));
}

void User::generateAccountNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000000, 999999999);
    accountNumber = std::to_string(dis(gen));
}

std::string User::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
