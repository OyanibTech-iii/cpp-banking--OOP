#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <memory>

class Account;

class User {
protected:
    std::string userId;
    std::string name;
    std::string email;
    std::string phone;
    std::string address;
    std::string password;
    std::string accountNumber;
    bool isActive;
    std::string dateCreated;

public:
    // Constructors
    User();
    User(const std::string& name, const std::string& email, const std::string& phone, 
         const std::string& address, const std::string& password);
    virtual ~User();

    // Getters
    std::string getUserId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getPhone() const;
    std::string getAddress() const;
    std::string getAccountNumber() const;
    bool getIsActive() const;
    std::string getDateCreated() const;

    // Setters
    void setUserId(const std::string& id);
    void setName(const std::string& name);
    void setEmail(const std::string& email);
    void setPhone(const std::string& phone);
    void setAddress(const std::string& address);
    void setAccountNumber(const std::string& accountNum);
    void setIsActive(bool active);
    void setDateCreated(const std::string& date);

    // Authentication
    virtual bool authenticate(const std::string& password) const;
    void changePassword(const std::string& newPassword);
    std::string getPassword() const;

    // Virtual methods for polymorphism
    virtual std::string getUserType() const = 0;
    virtual void displayInfo() const;
    virtual std::string toFileString() const;
    virtual void fromFileString(const std::string& data);

    // Utility methods
    void generateUserId();
    void generateAccountNumber();
    std::string getCurrentDateTime();
};

#endif // USER_H
