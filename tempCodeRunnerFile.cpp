
    void displayBook() const {
        cout << left << setw(10) << bookId
             << setw(30) << title
             << setw(25) << author
             << setw(15) << (isIssued ? "Issued" : "Available")
             << setw(15) << (isIssued ? to_string(issuedToMemberId) : "-") << endl;
    }
