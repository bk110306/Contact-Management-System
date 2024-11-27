#include <iostream>
#include <string>
#include <fstream> // For file handling
using namespace std;

// Contact node class
class Contact {
public:
    string name;
    string phone;
    string email;
    Contact* next;

    Contact(string n, string p, string e) : name(n), phone(p), email(e), next(nullptr) {}
};

// Contact Manager class
class ContactManager {
private:
    Contact* head;

    // Save contacts to file
    void saveToFile() const {
        ofstream file("contacts.txt");
        if (!file) {
            cerr << "Error opening file for saving!\n";
            return;
        }
        Contact* current = head;
        while (current != nullptr) {
            file << current->name << "|" << current->phone << "|" << current->email << "\n";
            current = current->next;
        }
        file.close();
    }

    // Load contacts from file
    void loadFromFile() {
        ifstream file("contacts.txt");
        if (!file) {
            cout << "No existing contact file found. Starting fresh.\n";
            return;
        }
        string name, phone, email, line;
        while (getline(file, line)) {
            size_t pos1 = line.find("|");
            size_t pos2 = line.find("|", pos1 + 1);
            name = line.substr(0, pos1);
            phone = line.substr(pos1 + 1, pos2 - pos1 - 1);
            email = line.substr(pos2 + 1);
            addContact(name, phone, email); // Add each contact to the list
        }
        file.close();
    }

public:
    ContactManager() : head(nullptr) {
        loadFromFile(); // Load contacts when the program starts
    }

    // Add a new contact in alphabetical order
    void addContact(string name, string phone, string email) {
        Contact* newContact = new Contact(name, phone, email);

        if (head == nullptr || head->name > name) {
            newContact->next = head;
            head = newContact;
        } else {
            Contact* current = head;
            while (current->next != nullptr && current->next->name < name) {
                current = current->next;
            }
            newContact->next = current->next;
            current->next = newContact;
        }
        cout << "Contact added successfully!\n";
        saveToFile(); // Save updated list to file
    }

    // View all contacts
    void viewContacts() const {
        if (head == nullptr) {
            cout << "No contacts available.\n";
            return;
        }

        cout << "\nContact List:\n";
        Contact* current = head;
        while (current != nullptr) {
            cout << "Name: " << current->name << ", Phone: " << current->phone << ", Email: " << current->email << "\n";
            current = current->next;
        }
        cout << endl;
    }

    // Search for a contact by name
    void searchContact(string name) const {
        Contact* current = head;
        while (current != nullptr) {
            if (current->name == name) {
                cout << "Contact found:\n";
                cout << "Name: " << current->name << ", Phone: " << current->phone << ", Email: " << current->email << "\n";
                return;
            }
            current = current->next;
        }
        cout << "No contact found with the name " << name << ".\n";
    }

    // Delete a contact by name
    void deleteContact(string name) {
        if (head == nullptr) {
            cout << "No contacts to delete.\n";
            return;
        }

        if (head->name == name) {
            Contact* temp = head;
            head = head->next;
            delete temp;
            cout << "Contact deleted successfully.\n";
            saveToFile(); // Save updated list to file
            return;
        }

        Contact* current = head;
        while (current->next != nullptr && current->next->name != name) {
            current = current->next;
        }

        if (current->next == nullptr) {
            cout << "No contact found with the name " << name << ".\n";
        } else {
            Contact* temp = current->next;
            current->next = current->next->next;
            delete temp;
            cout << "Contact deleted successfully.\n";
            saveToFile(); // Save updated list to file
        }
    }

    // Count total contacts
    void countContacts() const {
        int count = 0;
        Contact* current = head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        cout << "Total contacts: " << count << "\n";
    }

    // Destructor to free memory
    ~ContactManager() {
        while (head != nullptr) {
            Contact* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Main function
int main() {
    ContactManager contactManager;
    int choice;
    string name, phone, email;

    do {
        cout << "\nContact Management System:\n";
        cout << "1. Add Contact\n";
        cout << "2. View Contacts\n";
        cout << "3. Search Contact\n";
        cout << "4. Delete Contact\n";
        cout << "5. Count Contacts\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter contact name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter contact phone: ";
                getline(cin, phone);
                cout << "Enter contact email: ";
                getline(cin, email);
                contactManager.addContact(name, phone, email);
                break;
            case 2:
                contactManager.viewContacts();
                break;
            case 3:
                cout << "Enter contact name to search: ";
                cin.ignore();
                getline(cin, name);
                contactManager.searchContact(name);
                break;
            case 4:
                cout << "Enter contact name to delete: ";
                cin.ignore();
                getline(cin, name);
                contactManager.deleteContact(name);
                break;
            case 5:
                contactManager.countContacts();
                break;
            case 6:
                cout << "Exiting application.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
