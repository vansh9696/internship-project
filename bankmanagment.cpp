#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

class Account {
private:
    int accountNumber;
    string accountHolderName;
    string accountType;  
    double balance;

public:
    Account() : accountNumber(0), accountHolderName(""), accountType(""), balance(0.0) {}

    Account(int accNo, string name, string type, double initialDeposit) {
        accountNumber = accNo;
        accountHolderName = name;
        accountType = type;
        balance = initialDeposit;
    }

    
    int getAccountNumber() const { return accountNumber; }
    string getAccountHolderName() const { return accountHolderName; }
    string getAccountType() const { return accountType; }
    double getBalance() const { return balance; }

    // Core Banking Operations
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "\n[SUCCESS] Successfully deposited: Rs. " << fixed << setprecision(2) << amount << endl;
            cout << "Updated Balance: Rs. " << balance << endl;
        } else {
            cout << "\n[ERROR] Deposit amount must be positive.\n";
        }
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "\n[ERROR] Withdrawal amount must be positive.\n";
            return false;
        }
        if (amount > balance) {
            cout << "\n[ERROR] Insufficient funds! Current Balance: Rs. " << fixed << setprecision(2) << balance << endl;
            return false;
        }
        balance -= amount;
        cout << "\n[SUCCESS] Successfully withdrawn: Rs. " << fixed << setprecision(2) << amount << endl;
        cout << "Updated Balance: Rs. " << balance << endl;
        return true;
    }

    void displayAccountDetails() const {
        cout << "\n----------------------------------------";
        cout << "\nAccount Number   : " << accountNumber;
        cout << "\nAccount Holder   : " << accountHolderName;
        cout << "\nAccount Type     : " << accountType;
        cout << "\nCurrent Balance  : Rs. " << fixed << setprecision(2) << balance;
        cout << "\n----------------------------------------\n";
    }

    
    string toFileData() const {
        return to_string(accountNumber) + "|" + accountHolderName + "|" + accountType + "|" + to_string(balance);
    }
};


class BankManager {
private:
    const string fileName = "bank_records.txt";

    // Helper: Split string by delimiter
    vector<string> split(const string& str, char delimiter) {
        vector<string> tokens;
        string token;
        for (char ch : str) {
            if (ch == delimiter) {
                tokens.push_back(token);
                token = "";
            } else {
                token += ch;
            }
        }
        tokens.push_back(token);
        return tokens;
    }

    
    vector<Account> loadAccounts() {
        vector<Account> accounts;
        ifstream inFile(fileName);
        if (!inFile) {
            return accounts; // File doesn't exist yet
        }

        string line;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            vector<string> parts = split(line, '|');
            if (parts.size() == 4) {
                int accNo = stoi(parts[0]);
                string name = parts[1];
                string type = parts[2];
                double bal = stod(parts[3]);
                accounts.push_back(Account(accNo, name, type, bal));
            }
        }
        inFile.close();
        return accounts;
    }

    
    void saveAccounts(const vector<Account>& accounts) {
        ofstream outFile(fileName, ios::trunc);
        for (const auto& acc : accounts) {
            outFile << acc.toFileData() << "\n";
        }
        outFile.close();
    }

public:
    
    void createAccount() {
        int accNo;
        string name, type;
        double initialDeposit;

        cout << "\n=== CREATE NEW BANK ACCOUNT ===\n";
        cout << "Enter Account Number: ";
        cin >> accNo;

        
        vector<Account> accounts = loadAccounts();
        for (const auto& acc : accounts) {
            if (acc.getAccountNumber() == accNo) {
                cout << "\n[ERROR] Account Number already exists! Please use a unique number.\n";
                return;
            }
        }

        cin.ignore();
        cout << "Enter Account Holder Name: ";
        getline(cin, name);

        cout << "Enter Account Type (Savings/Current): ";
        getline(cin, type);

        cout << "Enter Initial Deposit Amount (Min Rs. 500): ";
        cin >> initialDeposit;

        if (initialDeposit < 500) {
            cout << "\n[ERROR] Minimum initial deposit must be Rs. 500.\n";
            return;
        }

        Account newAcc(accNo, name, type, initialDeposit);
        accounts.push_back(newAcc);
        saveAccounts(accounts);

        cout << "\n[SUCCESS] Account created successfully for " << name << "!\n";
    }

    
    void performDeposit() {
        int accNo;
        double amount;
        cout << "\n=== DEPOSIT FUNDS ===\n";
        cout << "Enter Account Number: ";
        cin >> accNo;

        vector<Account> accounts = loadAccounts();
        bool found = false;

        for (auto& acc : accounts) {
            if (acc.getAccountNumber() == accNo) {
                found = true;
                cout << "Enter Amount to Deposit: Rs. ";
                cin >> amount;
                acc.deposit(amount);
                break;
            }
        }

        if (found) {
            saveAccounts(accounts);
        } else {
            cout << "\n[ERROR] Account Number not found.\n";
        }
    }

    
    void performWithdrawal() {
        int accNo;
        double amount;
        cout << "\n=== WITHDRAW FUNDS ===\n";
        cout << "Enter Account Number: ";
        cin >> accNo;

        vector<Account> accounts = loadAccounts();
        bool found = false;

        for (auto& acc : accounts) {
            if (acc.getAccountNumber() == accNo) {
                found = true;
                cout << "Enter Amount to Withdraw: Rs. ";
                cin >> amount;
                if (acc.withdraw(amount)) {
                    saveAccounts(accounts);
                }
                break;
            }
        }

        if (!found) {
            cout << "\n[ERROR] Account Number not found.\n";
        }
    }

    
    void checkBalance() {
        int accNo;
        cout << "\n=== BALANCE ENQUIRY ===\n";
        cout << "Enter Account Number: ";
        cin >> accNo;

        vector<Account> accounts = loadAccounts();
        bool found = false;

        for (const auto& acc : accounts) {
            if (acc.getAccountNumber() == accNo) {
                acc.displayAccountDetails();
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "\n[ERROR] Account Number not found.\n";
        }
    }

    
    void displayAllAccounts() {
        vector<Account> accounts = loadAccounts();
        if (accounts.empty()) {
            cout << "\n[INFO] No accounts found in the system.\n";
            return;
        }

        cout << "\n============================== ALL REGISTERED ACCOUNTS ==============================\n";
        cout << left << setw(15) << "Account No" 
             << setw(25) << "Holder Name" 
             << setw(15) << "Type" 
             << setw(15) << "Balance (Rs.)" << endl;
        cout << "------------------------------------------------------------------------------------\n";

        for (const auto& acc : accounts) {
            cout << left << setw(15) << acc.getAccountNumber()
                 << setw(25) << acc.getAccountHolderName()
                 << setw(15) << acc.getAccountType()
                 << fixed << setprecision(2) << setw(15) << acc.getBalance() << endl;
        }
        cout << "====================================================================================\n";
    }
};


int main() {
    BankManager bank;
    int choice;

    do {
        cout << "\n==========================================";
        cout << "\n      BANK MANAGEMENT APPLICATION        ";
        cout << "\n==========================================";
        cout << "\n1. Open New Account";
        cout << "\n2. Deposit Money";
        cout << "\n3. Withdraw Money";
        cout << "\n4. Balance Inquiry & Account Details";
        cout << "\n5. Display All Accounts";
        cout << "\n6. Exit";
        cout << "\n==========================================";
        cout << "\nEnter your choice (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1:
                bank.createAccount();
                break;
            case 2:
                bank.performDeposit();
                break;
            case 3:
                bank.performWithdrawal();
                break;
            case 4:
                bank.checkBalance();
                break;
            case 5:
                bank.displayAllAccounts();
                break;
            case 6:
                cout << "\nThank you for using the Banking Application. Have a great day!\n";
                break;
            default:
                cout << "\n[ERROR] Invalid selection! Please enter a number between 1 and 6.\n";
        }
    } while (choice != 6);

    return 0;
}