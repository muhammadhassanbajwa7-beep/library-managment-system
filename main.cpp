#include<iostream>
#include<fstream>
#include<string>
using namespace std;

struct book {
    int id;
    string name;
    string author;
    string issue_date;
    int lendedTo = -1;
};

struct student {
    int enrollment;
    string name;
    int borrowedBookID[3] = {-1, -1, -1};
};

void printLine(char c = '-', int len = 60) {
    for (int i = 0; i < len; i++) cout << c;
    cout << "\n";
}

void printHeader(string title) {
    cout << "\n";
    printLine('=');
    cout << "  " << title << "\n";
    printLine('=');
}

void printSuccess(string msg) {
    cout << "\n  [OK] " << msg << "\n";
}

void printError(string msg) {
    cout << "\n  [!!] " << msg << "\n";
}

//  save and load students
void saveStudents(student st[], int studentCount) {
    ofstream file("data/students.txt");
    if(!file.is_open()) { printError("Error opening students file for writing!"); return; }
    
    file << studentCount << "\n";
    for (int i = 0; i < studentCount; i++) {
        file << st[i].enrollment << "\n" 
             << st[i].name << "\n" 
             << st[i].borrowedBookID[0] << " " 
             << st[i].borrowedBookID[1] << " " 
             << st[i].borrowedBookID[2] << "\n";
    }
    cout << "  Data is saved in ./data/students.txt\n";
}

void loadStudents(student st[], int &studentCount) {
    ifstream file("data/students.txt");
    if(!file.is_open()) return;
    
    file >> studentCount; 
    file.ignore();
    
    for(int i = 0; i < studentCount; i++) {
        file >> st[i].enrollment;
        file.ignore();
        getline(file, st[i].name);
        file >> st[i].borrowedBookID[0] >> st[i].borrowedBookID[1] >> st[i].borrowedBookID[2];
        file.ignore(); 
    }
    cout << "  Data is loaded from ./data/students.txt\n";
}

//  save and load books
void saveBooks(book books[], int bookCount) {
    ofstream file("data/books.txt");
    if(!file.is_open()) return;
    
    file << bookCount << "\n";
    for(int i = 0; i < bookCount; i++) {
        file << books[i].id << "\n" 
             << books[i].name << "\n" 
             << books[i].author << "\n" 
             << books[i].issue_date << "\n" 
             << books[i].lendedTo << "\n";
    }
    cout << "  Data is Saved in ./data/books.txt\n";
}

void loadBooks(book books[], int &bookCount) {
    ifstream file("data/books.txt");
    if(!file.is_open()) return;
    
    file >> bookCount;
    file.ignore();
    
    for(int i = 0; i < bookCount; i++) {
        file >> books[i].id;
        file.ignore();
        getline(file, books[i].name);
        getline(file, books[i].author);
        getline(file, books[i].issue_date);
        file >> books[i].lendedTo;
        file.ignore();
    }
    cout << "  Data is Loaded from ./data/books.txt\n";
}

//  Data Entry Menus for students and books
void studentEntryMenu(student st[], int &studentCount) {
    int choice;
    do {
        if (studentCount >= 99) {
            printError("Can't Input more Data. Array full!");
            break;
        }
        printHeader("Student Entry  [Record #" + to_string(studentCount + 1) + "]");
        cout << "  Name       : ";
        cin.ignore();
        getline(cin, st[studentCount].name);
        
        cout << "  Enrollment : ";
        cin >> st[studentCount].enrollment;
        
        studentCount++;
        printSuccess("Student registered successfully.");
        
        cout << "\n  Register another Student? (1-Yes / 0-No): ";
        cin >> choice;
    } while (choice == 1);
}

void BookEntryMenu(book books[], int &bookCount) {
    int choice;
    do {
        if (bookCount >= 99) {
            printError("Can't Input more Data. Array full!");
            break;
        }
        printHeader("Book Entry  [Record #" + to_string(bookCount + 1) + "]");
        cout << "  Book ID    : ";
        cin  >> books[bookCount].id;
        cin.ignore();
        
        cout << "  Title      : ";
        getline(cin, books[bookCount].name);
        cout << "  Author     : ";
        getline(cin, books[bookCount].author);
        cout << "  Issue Date : ";
        getline(cin, books[bookCount].issue_date);
        
        bookCount++;
        printSuccess("Book registered successfully.");
        
        cout << "\n  Register another Book? (1-Yes / 0-No): ";
        cin >> choice;
    } while (choice == 1);
}

//  Display functions
void displayStudents(student st[], int studentCount) {
    if (studentCount == 0) { printError("No registered students found."); return; }

    printHeader("Registered Students");
    cout << "  " << left;
    cout.width(14); cout << "Enrollment";
    cout.width(24); cout << "Name";
    cout << "Borrowed Book IDs\n";
    printLine('-');
    for (int i = 0; i < studentCount; i++) {
        cout << "  ";
        cout.width(14); cout << st[i].enrollment;
        cout.width(24); cout << st[i].name;
        bool anyBook = false;
        for (int j = 0; j < 3; j++) {
            if (st[i].borrowedBookID[j] != -1) {
                cout << (anyBook ? ", " : "") << st[i].borrowedBookID[j];
                anyBook = true;
            }
        }
        if (!anyBook) cout << "None";
        cout << "\n";
    }
    printLine('-');
    cout << "  Total students: " << studentCount << "\n";
}

void displayBooks(book books[], int bookCount) {
    if (bookCount == 0) { printError("No registered books found."); return; }

    printHeader("Book Shelf");
    for (int i = 0; i < bookCount; i++) {
        printLine('-');
        cout << "  ID         : " << books[i].id << "\n";
        cout << "  Title      : " << books[i].name << "\n";
        cout << "  Author     : " << books[i].author << "\n";
        cout << "  Issue Date : " << books[i].issue_date << "\n";
        cout << "  Status     : ";
        if (books[i].lendedTo == -1)
            cout << "Available\n";
        else
            cout << "Lent to Enrollment " << books[i].lendedTo << "\n";
    }
    printLine('-');
    cout << "  Total books: " << bookCount << "\n";
}

//  Search book by title
void searchBookByTitle(book books[], int bookCount) {
    if (bookCount == 0) { printError("No registered books found."); return; }

    printHeader("Search Book by Title");
    cout << "  Enter title to search: ";
    cin.ignore();
    string query;
    getline(cin, query);

    // convert query to lowercase for case-insensitive match
    string lowerQuery = query;
    for (int i = 0; i < (int)lowerQuery.size(); i++)
        lowerQuery[i] = tolower(lowerQuery[i]);

    int found = 0;
    for (int i = 0; i < bookCount; i++) {
        string lowerName = books[i].name;
        for (int j = 0; j < (int)lowerName.size(); j++)
            lowerName[j] = tolower(lowerName[j]);

        if (lowerName.find(lowerQuery) != string::npos) {
            if (found == 0) {
                cout << "\n  Search results for \"" << query << "\":\n";
                printLine('-');
            }
            cout << "  ID         : " << books[i].id << "\n";
            cout << "  Title      : " << books[i].name << "\n";
            cout << "  Author     : " << books[i].author << "\n";
            cout << "  Issue Date : " << books[i].issue_date << "\n";
            cout << "  Status     : ";
            if (books[i].lendedTo == -1)
                cout << "Available\n";
            else
                cout << "Lent to Enrollment " << books[i].lendedTo << "\n";
            printLine('-');
            found++;
        }
    }
    if (found == 0)
        printError("No book found matching \"" + query + "\".");
    else
        cout << "  " << found << " result(s) found.\n";
}

//  Edit functions
void editStudent(student students[], int studentCount) {
    if (studentCount == 0) { printError("No registered students found."); return; }

    displayStudents(students, studentCount);
    printHeader("Edit Student");
    cout << "  Enter enrollment of student to edit: ";
    int enrollment;
    cin >> enrollment;
    cin.ignore();

    int idx = -1;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].enrollment == enrollment) { idx = i; break; }
    }
    if (idx == -1) { printError("Student not found!"); return; }

    cout << "\n  Current Name       : " << students[idx].name << "\n";
    cout << "  Current Enrollment : " << students[idx].enrollment << "\n";
    cout << "\n  Enter new Name (leave blank to keep current): ";
    string newName;
    getline(cin, newName);
    if (newName != "") students[idx].name = newName;

    cout << "  Enter new Enrollment (0 to keep current): ";
    int newEnroll;
    cin >> newEnroll;
    if (newEnroll != 0) students[idx].enrollment = newEnroll;

    printSuccess("Student record updated.");
}

void editBook(book books[], int bookCount) {
    if (bookCount == 0) { printError("No registered books found."); return; }

    displayBooks(books, bookCount);
    printHeader("Edit Book");
    cout << "  Enter ID of book to edit: ";
    int bookID;
    cin >> bookID;
    cin.ignore();

    int idx = -1;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == bookID) { idx = i; break; }
    }
    if (idx == -1) { printError("Book not found!"); return; }

    cout << "\n  Leave any field blank to keep current value.\n";
    cout << "  Current Title      : " << books[idx].name << "\n";
    cout << "  New Title          : ";
    string tmp;
    getline(cin, tmp);
    if (tmp != "") books[idx].name = tmp;

    cout << "  Current Author     : " << books[idx].author << "\n";
    cout << "  New Author         : ";
    getline(cin, tmp);
    if (tmp != "") books[idx].author = tmp;

    cout << "  Current Issue Date : " << books[idx].issue_date << "\n";
    cout << "  New Issue Date     : ";
    getline(cin, tmp);
    if (tmp != "") books[idx].issue_date = tmp;

    cout << "  Current ID         : " << books[idx].id << "\n";
    cout << "  New ID (0 to keep) : ";
    int newID;
    cin >> newID;
    if (newID != 0) books[idx].id = newID;

    printSuccess("Book record updated.");
}

//  Delete functions
void deleteStudent(student students[], int &studentCount, book books[], int bookCount) {
    if (studentCount == 0) { printError("No registered students found."); return; }

    displayStudents(students, studentCount);
    printHeader("Delete Student");
    cout << "  Enter enrollment of student to delete: ";
    int enrollment;
    cin >> enrollment;
    cin.ignore();

    int idx = -1;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].enrollment == enrollment) { idx = i; break; }
    }
    if (idx == -1) { printError("Student not found!"); return; }

    // free any books lended to this student
    for (int j = 0; j < 3; j++) {
        int bid = students[idx].borrowedBookID[j];
        if (bid != -1) {
            for (int k = 0; k < bookCount; k++) {
                if (books[k].id == bid) { books[k].lendedTo = -1; break; }
            }
        }
    }

    cout << "\n  Deleting: " << students[idx].name << " (Enrollment: " << students[idx].enrollment << ")\n";
    cout << "  Are you sure? (1-Yes / 0-No): ";
    int confirm;
    cin >> confirm;
    if (confirm != 1) { cout << "  Deletion cancelled.\n"; return; }

    // shift array
    for (int i = idx; i < studentCount - 1; i++)
        students[i] = students[i + 1];
    studentCount--;

    printSuccess("Student deleted successfully.");
    saveStudents(students, studentCount);
    saveBooks(books, bookCount);
}

void deleteBook(book books[], int &bookCount, student students[], int studentCount) {
    if (bookCount == 0) { printError("No registered books found."); return; }

    displayBooks(books, bookCount);
    printHeader("Delete Book");
    cout << "  Enter ID of book to delete: ";
    int bookID;
    cin >> bookID;
    cin.ignore();

    int idx = -1;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == bookID) { idx = i; break; }
    }
    if (idx == -1) { printError("Book not found!"); return; }

    if (books[idx].lendedTo != -1) {
        printError("Cannot delete! Book is currently lent to Enrollment " + to_string(books[idx].lendedTo) + ". Return it first.");
        return;
    }

    cout << "\n  Deleting: " << books[idx].name << " by " << books[idx].author << "\n";
    cout << "  Are you sure? (1-Yes / 0-No): ";
    int confirm;
    cin >> confirm;
    if (confirm != 1) { cout << "  Deletion cancelled.\n"; return; }

    // remove book ID from any student records (safety)
    for (int i = 0; i < studentCount; i++)
        for (int j = 0; j < 3; j++)
            if (students[i].borrowedBookID[j] == bookID)
                students[i].borrowedBookID[j] = -1;

    // shift array
    for (int i = idx; i < bookCount - 1; i++)
        books[i] = books[i + 1];
    bookCount--;

    printSuccess("Book deleted successfully.");
    saveBooks(books, bookCount);
    saveStudents(students, studentCount);
}

//  Update functions
void lendBook(book books[], int bookCount, student students[], int studentCount) {
    if (studentCount == 0) { printError("No registered students found."); return; }
    
    // 1. Pick student
    displayStudents(students, studentCount);
    printHeader("Lend Book");
    cout << "  Enter enrollment of student to lend book to: ";
    int enrollment;
    cin >> enrollment;
    cin.ignore();

    int studentIndex = -1;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].enrollment == enrollment) { studentIndex = i; break; }
    }
    if (studentIndex == -1) { printError("Student not found!"); return; }

    // 2. Check 3-book limit
    int borrowedCount = 0, emptySlot = -1;
    for (int j = 0; j < 3; j++) {
        if (students[studentIndex].borrowedBookID[j] != -1) borrowedCount++;
        else if (emptySlot == -1) emptySlot = j;
    }
    if (borrowedCount >= 3) {
        printError("Student has already borrowed the maximum of 3 books!");
        return;
    }

    if (bookCount == 0) { printError("No registered books found in the system."); return; }

    // 3. Pick book
    displayBooks(books, bookCount);
    cout << "\n  Enter ID of book to lend: ";
    int bookID;
    cin >> bookID;
    cin.ignore();

    int bookIndex = -1;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == bookID) { bookIndex = i; break; }
    }
    if (bookIndex == -1) { printError("Book not found!"); return; }

    // 4. Check availability
    if (books[bookIndex].lendedTo != -1) {
        printError("Book is already lent to Enrollment " + to_string(books[bookIndex].lendedTo) + "!");
        return;
    }

    // 5. Assign
    students[studentIndex].borrowedBookID[emptySlot] = bookID;
    books[bookIndex].lendedTo = enrollment;

    printSuccess("Book \"" + books[bookIndex].name + "\" successfully lent to " + students[studentIndex].name + "!");

    saveStudents(students, studentCount);
    saveBooks(books, bookCount);
}

void returnBook(book books[], int bookCount, student students[], int studentCount) {
    if (studentCount == 0) { printError("No registered students found."); return; }

    displayStudents(students, studentCount);
    printHeader("Return Book");
    cout << "  Enter enrollment of student returning the book: ";
    int enrollment;
    cin >> enrollment;
    cin.ignore();

    int studentIndex = -1;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].enrollment == enrollment) { studentIndex = i; break; }
    }
    if (studentIndex == -1) { printError("Student not found!"); return; }

    bool hasBorrowed = false;
    cout << "\n  Books currently borrowed by " << students[studentIndex].name << ":\n";
    printLine('-');
    for (int j = 0; j < 3; j++) {
        int bid = students[studentIndex].borrowedBookID[j];
        if (bid != -1) {
            for (int k = 0; k < bookCount; k++) {
                if (books[k].id == bid) {
                    cout << "    ID: " << bid << "   Title: " << books[k].name << "\n";
                    hasBorrowed = true;
                    break;
                }
            }
        }
    }
    if (!hasBorrowed) { printError("This student has no borrowed books."); return; }
    printLine('-');

    cout << "\n  Enter ID of book to return: ";
    int bookID;
    cin >> bookID;
    cin.ignore();

    int slotIndex = -1;
    for (int j = 0; j < 3; j++) {
        if (students[studentIndex].borrowedBookID[j] == bookID) { slotIndex = j; break; }
    }
    if (slotIndex == -1) {
        printError("This student did not borrow Book ID " + to_string(bookID) + "!");
        return;
    }

    int bookIndex = -1;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == bookID) { bookIndex = i; break; }
    }
    if (bookIndex == -1) { printError("Book record not found!"); return; }

    students[studentIndex].borrowedBookID[slotIndex] = -1;
    books[bookIndex].lendedTo = -1;

    printSuccess("Book \"" + books[bookIndex].name + "\" successfully returned by " + students[studentIndex].name + "!");

    saveStudents(students, studentCount);
    saveBooks(books, bookCount);
}

void studentManageMenu(student students[], int &studentCount, book books[], int bookCount) {
    int choice;
    do {
        printHeader("Manage Students");
        cout << "  1. Edit Student\n";
        cout << "  2. Delete Student\n";
        cout << "  0. Back\n";
        printLine('-');
        cout << "  Your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: editStudent(students, studentCount); break;
            case 2: deleteStudent(students, studentCount, books, bookCount); break;
            case 0: break;
            default: printError("Invalid choice!");
        }
    } while (choice != 0);
}

void bookManageMenu(book books[], int &bookCount, student students[], int studentCount) {
    int choice;
    do {
        printHeader("Manage Books");
        cout << "  1. Edit Book\n";
        cout << "  2. Delete Book\n";
        cout << "  0. Back\n";
        printLine('-');
        cout << "  Your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: editBook(books, bookCount); saveBooks(books, bookCount); break;
            case 2: deleteBook(books, bookCount, students, studentCount); break;
            case 0: break;
            default: printError("Invalid choice!");
        }
    } while (choice != 0);
}

int main() {
    student students[99];
    book books[99];
    int studentCount = 0, bookCount = 0, choice;

    printHeader("Library Management System  -  Starting Up");
    loadStudents(students, studentCount);
    loadBooks(books, bookCount);

    do {
        printHeader("Main Menu  |  Library Management System");
        cout << "   0.  Exit\n";
        cout << "   1.  Add Students\n";
        cout << "   2.  Add Books\n";
        cout << "   3.  Display Books\n";
        cout << "   4.  Display Students\n";
        cout << "   5.  Lend Book\n";
        cout << "   6.  Return Book\n";
        cout << "   7.  Search Book by Title\n";
        cout << "   8.  Manage Students  (Edit / Delete)\n";
        cout << "   9.  Manage Books     (Edit / Delete)\n";
        printLine('-');
        cout << "  Your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 0:
                saveBooks(books, bookCount);
                saveStudents(students, studentCount);
                printHeader("Goodbye!");
                cout << "  Thanks for using the Library System..... Bye!\n\n";
                break;
            case 1:
                studentEntryMenu(students, studentCount);
                break;
            case 2:
                BookEntryMenu(books, bookCount);
                break;
            case 3:
                displayBooks(books, bookCount);
                break;
            case 4:
                displayStudents(students, studentCount);
                break;
            case 5:
                lendBook(books, bookCount, students, studentCount);
                break;
            case 6:
                returnBook(books, bookCount, students, studentCount);    
                break;
            case 7:
                searchBookByTitle(books, bookCount);
                break;
            case 8:
                studentManageMenu(students, studentCount, books, bookCount);
                break;
            case 9:
                bookManageMenu(books, bookCount, students, studentCount);
                break;
            default:
                printError("Invalid choice!!!");
                break;
        }
    } while (choice != 0);
    return 0;
}