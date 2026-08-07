#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;


class Student {
private:
    int rollNo;
    string name;
    string course;

public:
    
    Student() : rollNo(0), name(""), course("") {}
    Student(int r, string n, string c) : rollNo(r), name(n), course(c) {}


    int getRollNo() const { return rollNo; }
    string getName() const { return name; }
    string getCourse() const { return course; }

   
    void setName(const string& n) { name = n; }
    void setCourse(const string& c) { course = c; }

    
    void display() const {
        cout << left << setw(15) << rollNo 
             << setw(25) << name 
             << setw(20) << course << "\n";
    }

    
    string toCSV() const {
        return to_string(rollNo) + "," + name + "," + course;
    }
};


class StudentManager {
private:
    const string filename = "students_record.txt";

    
    vector<Student> loadStudents() {
        vector<Student> students;
        ifstream inFile(filename);
        if (!inFile) return students;

        string line, name, course, rollStr;
        while (getline(inFile, line)) {
            stringstream ss(line);
            getline(ss, rollStr, ',');
            getline(ss, name, ',');
            getline(ss, course, ',');

            if (!rollStr.empty()) {
                students.push_back(Student(stoi(rollStr), name, course));
            }
        }
        inFile.close();
        return students;
    }

    
    void saveStudents(const vector<Student>& students) {
        ofstream outFile(filename, ios::trunc); // Overwrite file
        for (const auto& student : students) {
            outFile << student.toCSV() << "\n";
        }
        outFile.close();
    }

public:
    void addStudent() {
        int roll;
        string name, course;

        cout << "\nEnter Roll Number: ";
        cin >> roll;
        cin.ignore(); 
        cout << "Enter Full Name: ";
        getline(cin, name);
        cout << "Enter Course: ";
        getline(cin, course);

        
        vector<Student> students = loadStudents();
        for (const auto& s : students) {
            if (s.getRollNo() == roll) {
                cout << "\nError: Student with Roll Number " << roll << " already exists!\n";
                return;
            }
        }

        ofstream outFile(filename, ios::app); 
        if (outFile.is_open()) {
            Student newStudent(roll, name, course);
            outFile << newStudent.toCSV() << "\n";
            outFile.close();
            cout << "\nStudent added successfully!\n";
        } else {
            cout << "\nError: Could not open file for writing.\n";
        }
    }

    void displayAll() {
        vector<Student> students = loadStudents();
        if (students.empty()) {
            cout << "\nNo records found.\n";
            return;
        }

        cout << "\n------------------------------------------------------------\n";
        cout << left << setw(15) << "Roll Number" << setw(25) << "Name" << setw(20) << "Course" << "\n";
        cout << "------------------------------------------------------------\n";
        for (const auto& student : students) {
            student.display();
        }
        cout << "------------------------------------------------------------\n";
    }

    void updateStudent() {
        int roll;
        cout << "\nEnter Roll Number to update: ";
        cin >> roll;
        cin.ignore();

        vector<Student> students = loadStudents();
        bool found = false;

        for (auto& student : students) {
            if (student.getRollNo() == roll) {
                string newName, newCourse;
                cout << "Current Name: " << student.getName() << "\n";
                cout << "Enter New Name (or press Enter to keep current): ";
                getline(cin, newName);
                if (!newName.empty()) student.setName(newName);

                cout << "Current Course: " << student.getCourse() << "\n";
                cout << "Enter New Course (or press Enter to keep current): ";
                getline(cin, newCourse);
                if (!newCourse.empty()) student.setCourse(newCourse);

                found = true;
                break;
            }
        }

        if (found) {
            saveStudents(students);
            cout << "\nRecord updated successfully!\n";
        } else {
            cout << "\nStudent record not found.\n";
        }
    }

    void deleteStudent() {
        int roll;
        cout << "\nEnter Roll Number to delete: ";
        cin >> roll;

        vector<Student> students = loadStudents();
        bool found = false;
        
        // Iterate and erase using iterator
        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->getRollNo() == roll) {
                students.erase(it);
                found = true;
                break;
            }
        }

        if (found) {
            saveStudents(students);
            cout << "\nRecord deleted successfully!\n";
        } else {
            cout << "\nStudent record not found.\n";
        }
    }
};


int main() {
    StudentManager manager;
    int choice;

    do {
        cout << "\n===================================\n";
        cout << "     STUDENT MANAGEMENT SYSTEM     \n";
        cout << "===================================\n";
        cout << "1. Add New Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Update Student Record\n";
        cout << "4. Delete Student Record\n";
        cout << "5. Exit\n";
        cout << "===================================\n";
        cout << "Enter your choice (1-5): ";
        cin >> choice;

        switch (choice) {
            case 1:
                manager.addStudent();
                break;
            case 2:
                manager.displayAll();
                break;
            case 3:
                manager.updateStudent();
                break;
            case 4:
                manager.deleteStudent();
                break;
            case 5:
                cout << "\nExiting System. Goodbye!\n";
                break;
            default:
                cout << "\nInvalid choice. Please enter a number between 1 and 5.\n";
        }
    } while (choice != 5);

    return 0;
}