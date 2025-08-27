#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
using namespace std;

void proceedPayment(Participant& participant);

 struct Participant
 {
    int id;
    string name;
    string role;
    double amountDue;
    bool paid;
    string paymentMethod;
    string paymentDate;
 };


static string getValidName() {
   string name;
   bool isValidName = false;
   do {
      getline(cin, name);
      if (!name.empty()) {
        isValidName = true;
         return name;
      }
      cout << "Error: Name cannot be empty. Please try again." << endl;
   } while (!isValidName);
}

 static int getValidRoleChoice() {
     string input;
     int choice;
     bool isValidChoice = false;
     do {
        cout << "Role:" << endl;
        cout << "1. Host/Vendor" << endl;
        cout << "2. Customer" << endl;
        cout << "Enter role (1/2): ";
        getline(cin, input);
        
        if (input.empty()) {
           cout << "Error: Please enter a number." << endl;
           continue;
        }
        
        bool isValidNumber = true;
        for (char i : input) {
           if (!isdigit(i)) {
              isValidNumber = false;
              break;
           }
        }
        
        if (!isValidNumber) {
           cout << "Error: Please enter only numbers (1 or 2)." << endl;
           continue;
        }
        
        choice = stoi(input);
        
        if (choice == 1 || choice == 2) {
           isValidChoice = true;
           return choice;
        } else {
           cout << "Error: Please enter 1 or 2 only." << endl;
        }
     } while (!isValidChoice);
 }

 static char getValidYesNoChoice() {
    char choice;
    string input;
    bool isValidChoice = false;
    do {
       cout << "Continue? (y/n): ";
       getline(cin, input);
       if (input.empty()) {
          cout << "Error: Please enter 'y' or 'n' only." << endl;
          continue;
       } else if (input.length() > 1) {
          cout << "Error: Please enter 'y' or 'n' only." << endl;
          continue;
       } else {
          choice = tolower(input[0]);
          if (choice == 'y' || choice == 'n') {
             isValidChoice = true;
             return choice;
          } else {
             cout << "Error: Please enter 'y' or 'n' only." << endl;
          }
       }
    } while (!isValidChoice);
 }

void addParticipants(vector<Participant>& participants) {
    char choice;
    
    do {
       Participant ptcp;
       
       cout << "Add New Participant" << endl;
       cout << "--------------------"<<endl;
       cout << "Enter participant name: ";
       ptcp.name = getValidName();
       
       int roleChoice = getValidRoleChoice();
       ptcp.role = (roleChoice == 1) ? "Host/Vendor" : "Customer";
       
       ptcp.id = static_cast<int>(participants.size()) + 1;
       
               participants.push_back(ptcp);

        proceedPayment(ptcp);
       
       choice = getValidYesNoChoice("Add another participant? (y/n): ");
       
    } while (choice == 'y');
    
   
}

