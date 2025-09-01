#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <ctime>
#include <regex>

using namespace std;
/*
To-Do
(JQ)
Payment Module - Complete
Registration Module
(Zetton)
Booking Module - Complete
Reminder Module
(Heng Mien)
Marketing Module
User/Login Module
(HanYuan)
Monitor Module
Report Module
*/

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

struct Booking
{
    string eventName;
    string eventType;
    string venue;
    string dateTime;
    string deadline;
    string status;
    vector<int> partId;
    int guestCount;
    int eventId;
};

//Please sort your functions into modules here to easily know

//Validation
string getValidInput(const string &title);
string getValidDateTime(const string &title);
string getValidDateline(const string &title);
char getValidYesNoChoice();

//Payment
string getValidName();
string getValidPaymentMethod();
double getValidAmount();
void proceedPayment(Participant& participant);
void processAllPayments(vector<Participant>& participants);
void displayPaymentSummary(const vector<Participant>& participants);
void showPaymentMenu(vector<Participant>& participants);
void processSinglePayment(vector<Participant>& participants);

//Registration
int getValidRoleChoice();
void addParticipants(vector<Participant>& participants);

//Booking
Booking createBooking(int id);
time_t deadline(const string &deadline);
void saveBookings(vector<Booking> &bookings, const string &file);
void saveParticipants(vector<Participant> &part, const string &file);
void loadBookings(vector<Booking> &bookings, const string &file);
void loadParticipants(vector<Booking> &bookings, const string &file);
void checkDeadlines(vector<Booking> &bookings);
void destroyEvent(vector<Booking> &bookings, int eventId, const string &bookFile, const string &partFile);

//Reminder

//Marketing

//User/Login

//Monitor

//Reporting


string getValidName() {
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
 
int getValidRoleChoice() {
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
 
char getValidYesNoChoice() {
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
        
        choice = getValidYesNoChoice();
        
     } while (choice == 'y');
     
    
 }
 
string getValidPaymentMethod() {
    string input;
    bool isValidChoice = false;
    do {
        cout << "Payment Methods:" << endl;
        cout << "1. Credit Card" << endl;
        cout << "2. Debit Card" << endl;
        cout << "3. Bank Transfer" << endl;
        cout << "4. Cash" << endl;
        cout << "Enter payment method (1-4): ";
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
            cout << "Error: Please enter only numbers (1-4)." << endl;
            continue;
        }
        
        int choice = stoi(input);
        if (choice >= 1 && choice <= 4) {
            isValidChoice = true;
            switch (choice) {
                case 1: return "Credit Card";
                case 2: return "Debit Card";
                case 3: return "Bank Transfer";
                case 4: return "Cash";
            }
        } else {
            cout << "Error: Please enter 1-4 only." << endl;
        }
    } while (!isValidChoice);
}

double getValidAmount() {
    string input;
    bool isValidAmount = false;
    do {
        cout << "Enter payment amount: $";
        getline(cin, input);
        
        if (input.empty()) {
            cout << "Error: Amount cannot be empty." << endl;
            continue;
        }
        
        bool isValidNumber = true;
        bool hasDecimal = false;
        for (size_t i = 0; i < input.length(); i++) {
            if (i == 0 && input[i] == '-') continue; 
            if (input[i] == '.' && !hasDecimal) {
                hasDecimal = true;
                continue;
            }
            if (!isdigit(input[i])) {
                isValidNumber = false;
                break;
            }
        }
        
        if (!isValidNumber) {
            cout << "Error: Please enter a valid amount." << endl;
            continue;
        }
        
        double amount = stod(input);
        if (amount > 0) {
            isValidAmount = true;
            return amount;
        } else {
            cout << "Error: Amount must be greater than 0." << endl;
        }
    } while (!isValidAmount);
}

void proceedPayment(Participant& participant) {
    if (participant.paid) {
        cout << "Participant " << participant.name << " has already paid." << endl;
        return;
    }
    
    cout << "Processing Payment " << endl;
    cout << "------------------" << endl;
    cout << "Participant: " << participant.name << " (ID: " << participant.id << ")" << endl;
    cout << "Role: " << participant.role << endl;
    cout << "Amount Due: RM" << fixed << setprecision(2) << participant.amountDue << endl;
    
    char proceed = getValidYesNoChoice();
    if (proceed != 'y') {
        cout << "Payment cancelled." << endl;
        return;
    }
    
    participant.paymentMethod = getValidPaymentMethod();
    
    double paymentAmount = getValidAmount();
    
    if (paymentAmount >= participant.amountDue) {
        participant.paid = true;
        participant.amountDue = 0.0;
        cout << endl;
        cout << "Payment successful!" << endl;
        cout << "Amount paid: RM" << fixed << setprecision(2) << paymentAmount << endl;
        if (paymentAmount > participant.amountDue) {
            double change = paymentAmount - participant.amountDue;
            cout << "Change: RM" << fixed << setprecision(2) << change << endl;
        }
    } else {
        participant.amountDue -= paymentAmount;
        cout << endl;
        cout << "Partial payment received." << endl;
        cout << "Amount paid: RM" << fixed << setprecision(2) << paymentAmount << endl;
        cout << "Remaining balance: RM" << fixed << setprecision(2) << participant.amountDue << endl;
    }
    
    participant.paymentDate = "Today";
}

void processAllPayments(vector<Participant>& participants) {
    if (participants.empty()) {
        cout << "No participants to process payments for." << endl;
        return;
    }
    
    cout << "Payment Processing" << endl;
    cout << "----------------" << endl;
    int processed = 0;
    
    for (auto& participant : participants) {
        if (!participant.paid) {
            cout << endl;
            cout << "Processing payment for: " << participant.name << endl;
             proceedPayment(participant);
             processed++;
            
            if (processed < participants.size()) {
                cout << "Continue with next participant? (y/n): ";
                char contProcess = getValidYesNoChoice();
                if (contProcess != 'y') break;
            }
        }
    }
    
}

void displayPaymentSummary(const vector<Participant>& participants) {
    if (participants.empty()) {
        cout << "No participants to display." << endl;
        return;
    }
    
    cout << "Payment Summary" << endl;
    cout << "----------------" << endl;
    cout << setw(5) << "ID" << setw(20) << "Name" << setw(15) << "Role" 
         << setw(12) << "Amount Due" << setw(8) << "Paid" << setw(15) << "Payment Method" << endl;
    cout << string(85, '-') << endl;
    
    double totalDue = 0.0;
    double totalPaid = 0.0;
    int paidCount = 0;
    
    for (const auto& participant : participants) {
        cout << setw(5) << participant.id 
             << setw(20) << participant.name 
             << setw(15) << participant.role
             << setw(12) << fixed << setprecision(2) << participant.amountDue
             << setw(8) << (participant.paid ? "Yes" : "No")
             << setw(15) << (participant.paid ? participant.paymentMethod : "N/A") << endl;
        
        totalDue += participant.amountDue;
        if (participant.paid) {
            totalPaid += participant.amountDue;
            paidCount++;
        }
    }
    
    cout << string(85, '-') << endl;
    cout << "Total Participants: " << participants.size() << endl;
    cout << "Paid: " << paidCount << " | Unpaid: " << (participants.size() - paidCount) << endl;
    cout << "Total Amount Due: RM" << fixed << setprecision(2) << totalDue << endl;
    cout << "Total Amount Paid: RM" << fixed << setprecision(2) << totalPaid << endl;
    cout << "Outstanding: RM" << fixed << setprecision(2) << (totalDue - totalPaid) << endl;
}

void showPaymentMenu(vector<Participant>& participants) {
    if (participants.empty()) {
        cout << "No participants to process payments for." << endl;
        return;
    }
    
    bool contMenu = true;
    while (contMenu) {
        cout << "Payment Menu" << endl;
        cout << "------------" << endl;
        cout << "1. Process payment for one participant" << endl;
        cout << "2. Process payments for all participants" << endl;
        cout << "3. Show payment summary" << endl;
        cout << "4. Back to main menu" << endl;
        cout << "Enter your choice (1-4): ";
        
        string input;
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
            cout << "Error: Please enter only numbers (1-4)." << endl;
            continue;
        }
        
        int choice = stoi(input);
        
        switch (choice) {
            case 1:
                processSinglePayment(participants);
                break;
            case 2:
                processAllPayments(participants);
                break;
            case 3:
                displayPaymentSummary(participants);
                break;
            case 4:
                contMenu = false;
                cout << "Return to main menu" << endl;
                break;
            default:
                cout << "Error: Please enter 1-4 only." << endl;
                break;
        }
    }
}

void processSinglePayment(vector<Participant>& participants) {
    if (participants.empty()) {
        cout << "No participants to process payments for." << endl;
        return;
    }
    
    cout << "Available Participants" << endl;
    cout << "----------------" << endl;
    cout << setw(5) << "ID" << setw(20) << "Name" << setw(15) << "Role" 
         << setw(12) << "Amount Due" << setw(8) << "Paid" << endl;
    cout << string(70, '-') << endl;
    
         for (const auto& participant : participants) {
        cout << setw(5) << participant.id 
             << setw(20) << participant.name 
             << setw(15) << participant.role
             << setw(12) << fixed << setprecision(2) << participant.amountDue
             << setw(8) << (participant.paid ? "Yes" : "No") << endl;
    }
    cout << endl;
    cout << "Enter participant ID to process payment: ";
    string input;
    getline(cin, input);
    
    if (input.empty()) {
        cout << "Error: Please enter a participant ID." << endl;
        return;
    }
    
    bool isValidNumber = true;
    for (char i : input) {
        if (!isdigit(i)) {
            isValidNumber = false;
            break;
        }
    }
    
    if (!isValidNumber) {
        cout << "Error: Please enter a valid participant ID." << endl;
        return;
    }
    
    int participantId = stoi(input);
    
    Participant* selectedParticipant = nullptr;
         for (auto& participant : participants) {
        if (participant.id == participantId) {
            selectedParticipant = &participant;
            break;
        }
    }
    
    if (selectedParticipant == nullptr) {
        cout << "Error: Participant with ID " << participantId << " not found." << endl;
        return;
    }
    
    if (selectedParticipant->paid) {
        cout << "Participant " << selectedParticipant->name << " has already paid." << endl;
        return;
    }
    
    proceedPayment(*selectedParticipant);
}

string getValidInput(const string &title) {
    string input;
    do {
        cout << title;
        getline(cin, input);
        if (input.empty()) {
            cout << "Input cannot be empty. Try again.\n";
        }
    } while (input.empty());
    return input;
}

string getValidDateTime(const string &title) {
    string input;
    regex pattern{"\d{4}-\d{2}-\d{2} \d{2}:\d{2}"};
    do {
        cout << title;
        getline(cin, input);
        if (input.empty()) {
            cout << "Input cannot be empty. Try again.\n";
        }else if (!regex_match(input, pattern)) {
            cout << "Input format incorrect. Try again.\n";
        }
    } while (input.empty());
    return input;
}

string getValidDateline(const string &title) {
    string input;
    regex pattern{"\d{4}-\d{2}-\d{2}"};
    do {
        cout << title;
        getline(cin, input);
        if (input.empty()) {
            cout << "Input cannot be empty. Try again.\n";
        }else if (!regex_match(input, pattern)) {
            cout << "Input format incorrect. Try again.\n";
        }
    } while (input.empty());
    return input;
}

Booking createBooking(int id) {
    Booking b;
    b.eventId = id;

    cout << "\n--- Create a New Event Booking ---\n";
    b.eventName = getValidInput("Enter event name: ");
    b.eventType = getValidInput("Enter event type: ");
    b.venue = getValidInput("Enter venue: ");
    b.dateTime = getValidDateTime("Enter date & time (YYYY-MM-DD HH:MM): ");
    b.deadline = getValidDateline("Enter registration deadline (YYYY-MM-DD): ");
    b.status = "Open";
    cout << "Enter maximum number of guests: ";
    while (!(cin >> b.guestCount) || b.guestCount < 0) {
        cout << "Invalid number. Enter a non-negative value: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    cin.ignore();

    return b;
}

void saveBookings(vector<Booking> &bookings, const string &file) {
    ofstream outFile(file);
    if (!outFile) {
        cerr << "Error opening file: " << file << endl;
        return;
    }

    for (Booking &b : bookings) {
        outFile << "Event ID: " << b.eventId << "\n";
        outFile << "Name: " << b.eventName << "\n";
        outFile << "Type: " << b.eventType << "\n";
        outFile << "Venue: " << b.venue << "\n";
        outFile << "Date & Time: " << b.dateTime << "\n";
        outFile << "Deadline: " << b.deadline << "\n";
        outFile << "Status: " << b.status << "\n";
        outFile << "Guest Limit: " << b.guestCount << "\n";
        outFile << string(50, '-') << "\n";
    }

    outFile.close();
}

void saveParticipants(vector<Booking> &bookings, const string &file) {
    ofstream outFile(file);
    if (!outFile) {
        cerr << "Error opening file: " << file << endl;
        return;
    }
    for (Booking &b : bookings) {
        outFile << "Event ID: " << b.eventId << "\n";
        outFile << "Participant ID: ";
        for (int id: b.partId) {
            outFile << id << " ";
        }
        outFile << "\n";
    }

    outFile.close();
}

void loadBookings(vector<Booking> &bookings, const string &file) {
    ifstream inFile(file);
    if (!inFile) {
        cout << "No booking found.\n";
        return;
    }

    Booking b;
    string line;

    while (getline(inFile, line)) {
        if (line.find("Event ID: ") == 0) {
            b.eventId = stoi(line.substr(10));
        }
        else if (line.find("Name: ") == 0) {
            b.eventName = line.substr(6);
        }
        else if (line.find("Type: ") == 0) {
            b.eventType = line.substr(6);
        }
        else if (line.find("Venue: ") == 0) {
            b.venue = line.substr(7);
        }
        else if (line.find("Date & Time: ") == 0) {
            b.dateTime = line.substr(13);
        }
        else if (line.find("Deadline: ") == 0) {
            b.deadline = line.substr(10);
        }
        else if (line.find("Status: ") == 0) {
            b.status = line.substr(8);
        }
        else if (line.find("Guest Limit: ") == 0) {
            b.guestCount = stoi(line.substr(13));
        }
        else if (line.find("--------------------------------------------------") == 0) {
            // Finished one booking entry
            bookings.push_back(b);
        }
    }

    inFile.close();
    cout << "Bookings loaded successfully (" << bookings.size() << " events).\n";
}

void loadParticipants(vector<Booking> &bookings, const string &file) {
    ifstream inFile(file);
    if (!inFile) {
        cerr << "Error opening file: " << file << endl;
        return;
    }

    string line;
    int currentEventId = -1;

    while (getline(inFile, line)) {
        if (line.find("Event ID: ") == 0) {
            currentEventId = stoi(line.substr(10));
        }
        else if (line.find("Participant ID: ") == 0 && currentEventId != -1) {
            istringstream iss(line.substr(15));
            int id;
            while (iss >> id) {
                for (size_t i = 0; i < bookings.size(); i++) {
                    if (bookings[i].eventId == currentEventId) {
                        bookings[i].partId.push_back(id);
                        break;
                    }
                }
            }
        }
    }

    inFile.close();
    cout << "Participants loaded successfully.\n";
}

time_t deadline(const string &deadline) {
    tm t = {};
    int year, month, day;

    if (sscanf(deadline.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
        cerr << "Invalid deadline format: " << deadline << endl;
        return -1;
    }

    t.tm_year = year - 1900; // years since 1900
    t.tm_mon = month - 1;    // months 0-11
    t.tm_mday = day;
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;

    return mktime(&t);
}

void checkDeadlines(vector<Booking> &bookings) {
    time_t now = time(0); // current system time

    for (int i = 0; i < (int)bookings.size(); i++) {
        time_t deadlineTime = deadline(bookings[i].deadline);

        if (deadlineTime != -1 && deadlineTime < now) {
            bookings[i].status = "Closed";  // deadline passed
        } else {
            bookings[i].status = "Open";    // still valid
        }
    }
}

void destroyEvent(vector<Booking> &bookings, int eventId, const string &bookFile, const string &partFile) {
    for (int i = 0; i < (int)bookings.size(); i++) {
        if (bookings[i].eventId == eventId) {
            bookings.erase(bookings.begin() + i); // remove booking from memory
            cout << "Booking with Event ID " << eventId << " and its participants have been deleted.\n";
            break;
        }
    }

    saveBookings(bookings, bookFile);
    saveParticipants(bookings, partFile);
}


int main(){}

