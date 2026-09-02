#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Helper function: Convert string to lowercase for case-insensitive search
string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}


class Book {
private:
    int bookId;
    string title;
    string author;
    bool isIssued;
    int issuedToMemberId;

public:
    Book() : bookId(0), title(""), author(""), isIssued(false), issuedToMemberId(0) {}

    Book(int id, string t, string a, bool issued = false, int memberId = 0)
        : bookId(id), title(t), author(a), isIssued(issued), issuedToMemberId(memberId) {}

    int getBookId() const { return bookId; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool getIsIssued() const { return isIssued; }
    int getIssuedToMemberId() const { return issuedToMemberId; }

    void issueBook(int memberId) {
        isIssued = true;
        issuedToMemberId = memberId;
    }

    void returnBook() {
        isIssued = false;
        issuedToMemberId = 0;
    }

    void displayBook() const {
        cout << left << setw(10) << bookId
             << setw(30) << title
             << setw(25) << author
             << setw(15) << (isIssued ? "Issued" : "Available")
             << setw(15) << (isIssued ? to_string(issuedToMemberId) : "-") << endl;
    }

    string toFileData() const {
        return to_string(bookId) + "|" + title + "|" + author + "|" + (isIssued ? "1" : "0") + "|" + to_string(issuedToMemberId);
    }
};

// ==========================================
// Class: Member
// ==========================================
class Member {
private:
    int memberId;
    string name;
    string contact;

public:
    Member() : memberId(0), name(""), contact("") {}
    Member(int id, string n, string c) : memberId(id), name(n), contact(c) {}

    int getMemberId() const { return memberId; }
    string getName() const { return name; }
    string getContact() const { return contact; }

    void displayMember() const {
        cout << left << setw(12) << memberId
             << setw(25) << name
             << setw(20) << contact << endl;
    }

    string toFileData() const {
        return to_string(memberId) + "|" + name + "|" + contact;
    }
};


class LibraryManager {
private:
    const string booksFile = "books_record.txt";
    const string membersFile = "members_record.txt";

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

    vector<Book> loadBooks() {
        vector<Book> books;
        ifstream inFile(booksFile);
        if (!inFile) return books;

        string line;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            vector<string> parts = split(line, '|');
            if (parts.size() == 5) {
                int id = stoi(parts[0]);
                string title = parts[1];
                string author = parts[2];
                bool issued = (parts[3] == "1");
                int memId = stoi(parts[4]);
                books.push_back(Book(id, title, author, issued, memId));
            }
        }
        inFile.close();
        return books;
    }

    void saveBooks(const vector<Book>& books) {
        ofstream outFile(booksFile, ios::trunc);
        for (const auto& b : books) {
            outFile << b.toFileData() << "\n";
        }
        outFile.close();
    }

    vector<Member> loadMembers() {
        vector<Member> members;
        ifstream inFile(membersFile);
        if (!inFile) return members;

        string line;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            vector<string> parts = split(line, '|');
            if (parts.size() == 3) {
                int id = stoi(parts[0]);
                string name = parts[1];
                string contact = parts[2];
                members.push_back(Member(id, name, contact));
            }
        }
        inFile.close();
        return members;
    }

    void saveMembers(const vector<Member>& members) {
        ofstream outFile(membersFile, ios::trunc);
        for (const auto& m : members) {
            outFile << m.toFileData() << "\n";
        }
        outFile.close();
    }

public:
    
    void addBook() {
        int id;
        string title, author;

        cout << "\n=== ADD NEW BOOK ===\n";
        cout << "Enter Book ID: ";
        cin >> id;

        vector<Book> books = loadBooks();
        for (const auto& b : books) {
            if (b.getBookId() == id) {
                cout << "[ERROR] Book ID already exists!\n";
                return;
            }
        }

        cin.ignore();
        cout << "Enter Book Title: ";
        getline(cin, title);
        cout << "Enter Book Author: ";
        getline(cin, author);

        books.push_back(Book(id, title, author));
        saveBooks(books);
        cout << "[SUCCESS] Book added successfully!\n";
    }

    
    void registerMember() {
        int id;
        string name, contact;

        cout << "\n=== REGISTER NEW MEMBER ===\n";
        cout << "Enter Member ID: ";
        cin >> id;

        vector<Member> members = loadMembers();
        for (const auto& m : members) {
            if (m.getMemberId() == id) {
                cout << "[ERROR] Member ID already exists!\n";
                return;
            }
        }

        cin.ignore();
        cout << "Enter Member Name: ";
        getline(cin, name);
        cout << "Enter Member Contact: ";
        getline(cin, contact);

        members.push_back(Member(id, name, contact));
        saveMembers(members);
        cout << "[SUCCESS] Member registered successfully!\n";
    }

    
    void issueBook() {
        int bId, mId;
        cout << "\n=== ISSUE BOOK ===\n";
        cout << "Enter Book ID to issue: ";
        cin >> bId;
        cout << "Enter Member ID: ";
        cin >> mId;

        vector<Member> members = loadMembers();
        bool memberExists = false;
        for (const auto& m : members) {
            if (m.getMemberId() == mId) {
                memberExists = true;
                break;
            }
        }

        if (!memberExists) {
            cout << "[ERROR] Member ID not registered!\n";
            return;
        }

        vector<Book> books = loadBooks();
        bool bookFound = false;

        for (auto& b : books) {
            if (b.getBookId() == bId) {
                bookFound = true;
                if (b.getIsIssued()) {
                    cout << "[ERROR] Book is already issued to Member ID: " << b.getIssuedToMemberId() << "\n";
                } else {
                    b.issueBook(mId);
                    saveBooks(books);
                    cout << "[SUCCESS] Book issued successfully to Member ID " << mId << "!\n";
                }
                break;
            }
        }

        if (!bookFound) {
            cout << "[ERROR] Book ID not found in library!\n";
        }
    }

    
    void returnBook() {
        int bId;
        cout << "\n=== RETURN BOOK ===\n";
        cout << "Enter Book ID to return: ";
        cin >> bId;

        vector<Book> books = loadBooks();
        bool bookFound = false;

        for (auto& b : books) {
            if (b.getBookId() == bId) {
                bookFound = true;
                if (!b.getIsIssued()) {
                    cout << "[ERROR] Book is not currently issued to anyone.\n";
                } else {
                    b.returnBook();
                    saveBooks(books);
                    cout << "[SUCCESS] Book returned successfully!\n";
                }
                break;
            }
        }

        if (!bookFound) {
            cout << "[ERROR] Book ID not found in library!\n";
        }
    }

    
    void searchBook() {
        int opt;
        cout << "\n=== SEARCH BOOKS ===\n";
        cout << "1. Search by Title\n";
        cout << "2. Search by Author\n";
        cout << "Enter choice: ";
        cin >> opt;
        cin.ignore();

        string query;
        if (opt == 1) {
            cout << "Enter Title (or part of title): ";
            getline(cin, query);
        } else if (opt == 2) {
            cout << "Enter Author Name (or part of author): ";
            getline(cin, query);
        } else {
            cout << "[ERROR] Invalid search option.\n";
            return;
        }

        string lowerQuery = toLower(query);
        vector<Book> books = loadBooks();
        vector<Book> results;

        for (const auto& b : books) {
            if (opt == 1 && toLower(b.getTitle()).find(lowerQuery) != string::npos) {
                results.push_back(b);
            } else if (opt == 2 && toLower(b.getAuthor()).find(lowerQuery) != string::npos) {
                results.push_back(b);
            }
        }

        if (results.empty()) {
            cout << "\n[INFO] No matching books found.\n";
            return;
        }

        cout << "\n" << left << setw(10) << "Book ID"
             << setw(30) << "Title"
             << setw(25) << "Author"
             << setw(15) << "Status"
             << setw(15) << "Issued To" << endl;
        cout << string(95, '-') << endl;

        for (const auto& b : results) {
            b.displayBook();
        }
    }

    
    void displayAllBooks() {
        vector<Book> books = loadBooks();
        if (books.empty()) {
            cout << "\n[INFO] No books available in library.\n";
            return;
        }

        cout << "\n================================ LIBRARY CATALOG ================================\n";
        cout << left << setw(10) << "Book ID"
             << setw(30) << "Title"
             << setw(25) << "Author"
             << setw(15) << "Status"
             << setw(15) << "Issued To" << endl;
        cout << string(95, '-') << endl;

        for (const auto& b : books) {
            b.displayBook();
        }
    }

   
    void displayAllMembers() {
        vector<Member> members = loadMembers();
        if (members.empty()) {
            cout << "\n[INFO] No members registered yet.\n";
            return;
        }

        cout << "\n======================== REGISTERED MEMBERS ========================\n";
        cout << left << setw(12) << "Member ID"
             << setw(25) << "Name"
             << setw(20) << "Contact" << endl;
        cout << string(57, '-') << endl;

        for (const auto& m : members) {
            m.displayMember();
        }
    }
};


int main() {
    LibraryManager lib;
    int choice;

    do {
        cout << "\n==========================================";
        cout << "\n        LIBRARY MANAGEMENT SYSTEM         ";
        cout << "\n==========================================";
        cout << "\n1. Add New Book";
        cout << "\n2. Register New Member";
        cout << "\n3. Issue Book";
        cout << "\n4. Return Book";
        cout << "\n5. Search Book (Title / Author)";
        cout << "\n6. Display All Books";
        cout << "\n7. Display All Members";
        cout << "\n8. Exit";
        cout << "\n==========================================";
        cout << "\nEnter your choice (1-8): ";
        cin >> choice;

        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.registerMember(); break;
            case 3: lib.issueBook(); break;
            case 4: lib.returnBook(); break;
            case 5: lib.searchBook(); break;
            case 6: lib.displayAllBooks(); break;
            case 7: lib.displayAllMembers(); break;
            case 8: cout << "\nExiting Library System. Goodbye!\n"; break;
            default: cout << "\n[ERROR] Invalid choice! Please select 1-8.\n";
        }
    } while (choice != 8);

    return 0;
}