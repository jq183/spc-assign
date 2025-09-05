#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <ctime>
#include <regex>
#include <sstream>
#include <cstdlib>

using namespace std;
/*
To-Do
(JQ)
Payment Module - Complete
Registration Module -Complete
(Zetton)
Booking Module - Complete
Reminder Module - Complete
(Heng Mien)
Marketing Module
User/Login Module
(HanYuan)
Monitor Module-Complete
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
    vector<Participant> participants;
    int guestCount;
    int eventId;
};

struct Review
{
    string name;
    string title;
    string comment;
    int rating;
};

struct EventState {
    Booking booking;
    string quickNote[3][50]; // [type][index]
    vector<Review> review;
};

const int PROBLEM_TITLE = 0;
const int ORG_PROBLEM = 1;
const int PROBLEM_RATING = 2;

//Please sort your functions into modules here to easily know

//Vital Functions
void showMainMenu(vector<Booking>& bookings);

//Validation
string getValidInput(const string& title);
string getValidDateTime(const string& title);
string getValidDateline(const string& title);
string getValidCreditCardNumber();
string getValidExpiryDate();
string getValidCVV();
string getValidBankAccount();
string getValidBankName();
string getValidEmail();
string getValidName();
string getValidPaymentMethod();
double getValidAmount();
char getValidYesNoChoice();
int getValidRoleChoice();
int getValidRating(int min, int max);

//Payment
void proceedPayment(Participant& participant, vector<Booking>& bookings);
void processAllPayments(vector<Participant>& participants, vector<Booking>& bookings);
void displayPaymentSummary(const vector<Participant>& participants);
void showPaymentMenu(vector<Participant>& participants, vector<Booking>& bookings);
void processSinglePayment(vector<Participant>& participants, vector<Booking>& bookings);
void showPaymentMenuForEvents(vector<Booking>& bookings);
void generatePaymentReceipt(const Participant& participant, double amountDue, const string& paymentMethod);

//Registration
void addParticipants(vector<Participant>& participants, vector<Booking>& bookings);
void addParticipantsToEvent(vector<Booking>& bookings);
void viewParticipants(const vector<Booking>& bookings);

//Booking
Booking createBooking(int id, vector<Booking>& bookings);
time_t deadline(const string& deadline);
void saveBookings(vector<Booking>& bookings, const string& file);
void saveParticipants(vector<Booking>& bookings, const string& file);
void loadBookings(vector<Booking>& bookings, const string& file);
void loadParticipants(vector<Booking>& bookings, const string& file);
void checkDeadlines(vector<Booking>& bookings);
void destroyEvent(vector<Booking>& bookings, int eventId, const string& bookFile, const string& partFile);

//Reminder
time_t stringToDateTime(const string &dateTime);
time_t stringToDate(const string &dateStr);
string formatTimeHHMM(time_t t);
string formatDateYMD(time_t t);
void scheduleReminder(const string &taskName, const string &date, const string &time, const string &message);
void checkBookingStatus(vector<Booking> &bookings);
void addBookingReminders(const Booking &b, int minutesBefore);

//Marketing

//User/Login

//Monitor
Review createComment(EventState& e);
void startMonitor(Booking& b);
EventState convertBookingToEventState(const Booking& b);
void printQuickNotes(const EventState& e);
void monitorEvent(vector<Booking>& bookings);

//Reporting


//Code starts
void showMainMenu(vector<Booking>& bookings);

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
    return "";
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
        }
        else {
            cout << "Error: Please enter 1 or 2 only." << endl;
        }
    } while (!isValidChoice);
    return 0;
}

char getValidYesNoChoice() {
    char choice;
    string input;
    bool isValidChoice = false;
    do {

        getline(cin, input);
        if (input.empty()) {
            cout << "Error: Please enter 'y' or 'n' only." << endl;
            continue;
        }
        else if (input.length() > 1) {
            cout << "Error: Please enter 'y' or 'n' only." << endl;
            continue;
        }
        else {
            choice = tolower(input[0]);
            if (choice == 'y' || choice == 'n') {
                isValidChoice = true;
                return choice;
            }
            else {
                cout << "Error: Please enter 'y' or 'n' only." << endl;
            }
        }
    } while (!isValidChoice);
    return 'n';
}

string getValidCreditCardNumber() {
    string cardNumber;
    bool isValid = false;
    do {
        cout << "Enter credit card number (16 digits): ";
        getline(cin, cardNumber);

        if (cardNumber.empty()) {
            cout << "Error: Card number cannot be empty." << endl;
            continue;
        }

        // Remove spaces and dashes
        string cleanNumber;
        for (char c : cardNumber) {
            if (isdigit(c)) {
                cleanNumber += c;
            }
        }

        if (cleanNumber.length() != 16) {
            cout << "Error: Card number must be exactly 16 digits." << endl;
            continue;
        }

        isValid = true;
        return "****-****-****-" + cleanNumber.substr(12);

    } while (!isValid);
    return "";
}

string getValidExpiryDate() {
    string expiryDate;
    bool isValid = false;
    do {
        cout << "Enter expiry date (MM/YY): ";
        getline(cin, expiryDate);

        if (expiryDate.empty()) {
            cout << "Error: Expiry date cannot be empty." << endl;
            continue;
        }

        if (expiryDate.length() != 5 || expiryDate[2] != '/') {
            cout << "Error: Please use MM/YY format (e.g., 12/25)." << endl;
            continue;
        }

        string monthStr = expiryDate.substr(0, 2);
        string yearStr = expiryDate.substr(3, 2);

        if (!isdigit(monthStr[0]) || !isdigit(monthStr[1]) ||
            !isdigit(yearStr[0]) || !isdigit(yearStr[1])) {
            cout << "Error: Please enter only numbers." << endl;
            continue;
        }

        int month = stoi(monthStr);
        int year = stoi(yearStr);

        if (month < 1 || month > 12) {
            cout << "Error: Month must be between 01 and 12." << endl;
            continue;
        }

        int currentYear = 25;
        if (year < currentYear || (year == currentYear && month < 1)) {
            cout << "Error: Card appears to be expired." << endl;
            continue;
        }

        isValid = true;
    } while (!isValid);
    return expiryDate;
}

string getValidCVV() {
    string cvv;
    bool isValid = false;
    do {
        cout << "Enter CVV (3-4 digits): ";
        getline(cin, cvv);

        if (cvv.empty()) {
            cout << "Error: CVV cannot be empty." << endl;
            continue;
        }

        if (cvv.length() < 3 || cvv.length() > 4) {
            cout << "Error: CVV must be 3 or 4 digits." << endl;
            continue;
        }

        bool isAllDigits = true;
        for (char c : cvv) {
            if (!isdigit(c)) {
                isAllDigits = false;
                break;
            }
        }

        if (!isAllDigits) {
            cout << "Error: CVV must contain only numbers." << endl;
            continue;
        }

        isValid = true;
    } while (!isValid);
    return "***";
}

string getValidBankAccount() {
    string accountNumber;
    bool isValid = false;
    do {
        cout << "Enter bank account number (8-12 digits): ";
        getline(cin, accountNumber);

        if (accountNumber.empty()) {
            cout << "Error: Account number cannot be empty." << endl;
            continue;
        }

        if (accountNumber.length() < 8 || accountNumber.length() > 12) {
            cout << "Error: Account number must be 8-12 digits." << endl;
            continue;
        }

        bool isAllDigits = true;
        for (char c : accountNumber) {
            if (!isdigit(c)) {
                isAllDigits = false;
                break;
            }
        }

        if (!isAllDigits) {
            cout << "Error: Account number must contain only numbers." << endl;
            continue;
        }

        isValid = true;
        return "****" + accountNumber.substr(accountNumber.length() - 4);
    } while (!isValid);
    return "";
}

string getValidBankName() {
    string bankName;
    bool isValid = false;
    do {
        cout << "Enter bank name: ";
        getline(cin, bankName);

        if (bankName.empty()) {
            cout << "Error: Bank name cannot be empty." << endl;
            continue;
        }

        if (bankName.length() < 2) {
            cout << "Error: Bank name must be at least 2 characters." << endl;
            continue;
        }

        isValid = true;
    } while (!isValid);
    return bankName;
}

void addParticipants(vector<Participant>& participants, vector<Booking>& bookings) {
    char choice;

    do {
        Participant ptcp;

        cout << "Add New Participant" << endl;
        cout << "--------------------" << endl;
        cout << "Enter participant name: ";
        ptcp.name = getValidName();

        int roleChoice = getValidRoleChoice();
        ptcp.role = (roleChoice == 1) ? "Host/Vendor" : "Customer";

        ptcp.id = static_cast<int>(participants.size()) + 1;

        participants.push_back(ptcp);

        proceedPayment(ptcp, bookings);

        cout << "Continue? (y/n): ";

        choice = getValidYesNoChoice();

    } while (choice == 'y');


}

void addParticipantsToEvent(vector<Booking>& bookings) {
    cout << "\n" << string(60, '=') << endl;
    cout << "           ADD PARTICIPANTS TO EVENT" << endl;
    cout << string(60, '=') << endl;

    cout << "\nAvailable Events for Adding Participants:\n";
    cout << string(60, '-') << endl;
    for (size_t i = 0; i < bookings.size(); i++) {
        cout << "Event " << (i + 1) << ":" << endl;
        cout << "  Name: " << bookings[i].eventName << endl;
        cout << "  Type: " << bookings[i].eventType << endl;
        cout << "  Venue: " << bookings[i].venue << endl;
        cout << "  Date & Time: " << bookings[i].dateTime << endl;
        cout << "  Current Participants: " << bookings[i].participants.size() << "/" << bookings[i].guestCount << endl;
        cout << string(60, '-') << endl;
    }

    int eventChoice;
    cout << "Select an event to add participants" << endl;
    cout << "--------------------------------" << endl;
    cout << "Enter event number (1-" << bookings.size() << "): ";

    string input;
    getline(cin, input);

    try {
        eventChoice = stoi(input);
        if (eventChoice < 1 || eventChoice > static_cast<int>(bookings.size())) {
            cout << "\nError: Invalid event selection. Please enter a number between 1 and " << bookings.size() << "." << endl;
            return;
        }
    }
    catch (...) {
        cout << "\nError: Invalid input. Please enter a valid number." << endl;
        return;
    }

    int selectedEventIndex = eventChoice - 1;
    Booking& selectedEvent = bookings[selectedEventIndex];

    cout << "\n" << string(60, '=') << endl;
    cout << "SELECTED EVENT: " << selectedEvent.eventName << endl;
    cout << string(60, '=') << endl;
    cout << "Event Type: " << selectedEvent.eventType << endl;
    cout << "Venue: " << selectedEvent.venue << endl;
    cout << "Date & Time: " << selectedEvent.dateTime << endl;
    cout << "Current Participants: " << selectedEvent.participants.size() << "/" << selectedEvent.guestCount << endl;
    cout << string(60, '=') << endl;

    if (selectedEvent.participants.size() >= selectedEvent.guestCount) {
        cout << "\nThis event is full. Cannot add more participants." << endl;
        return;
    }

    char continueAdding;
    do {
        cout << "\n--- Add New Participant ---\n";

        Participant ptcp;
        cout << "Enter participant name: ";
        ptcp.name = getValidName();

        int roleChoice = getValidRoleChoice();
        ptcp.role = (roleChoice == 1) ? "Host/Vendor" : "Customer";

        int maxId = 0;
        for (const auto& booking : bookings) {
            for (const auto& participant : booking.participants) {
                if (participant.id > maxId) maxId = participant.id;
            }
        }
        ptcp.id = maxId + 1;

        ptcp.amountDue = 50.0;
        ptcp.paid = false;
        ptcp.paymentMethod = "";
        ptcp.paymentDate = "";

        selectedEvent.participants.push_back(ptcp);

        cout << "Participant " << ptcp.name << " (ID: " << ptcp.id << ") added to event!" << endl;
        cout << "Current participants: " << selectedEvent.participants.size() << "/" << selectedEvent.guestCount << endl;

        if (selectedEvent.participants.size() >= selectedEvent.guestCount) {
            cout << "Event is now full!" << endl;
            break;
        }

        cout << "Continue? (y/n): ";
        continueAdding = getValidYesNoChoice();

    } while (continueAdding == 'y');

    saveBookings(bookings, "bookings.txt");
    saveParticipants(bookings, "participants.txt");
    cout << "Participants saved successfully!" << endl;

    cout << "\n--- Participant List for " << selectedEvent.eventName << " ---\n";
    if (selectedEvent.participants.empty()) {
        cout << "No participants yet." << endl;
    }
    else {
        cout << "Participants: ";
        for (size_t i = 0; i < selectedEvent.participants.size(); i++) {
            cout << selectedEvent.participants[i].name << " (ID: " << selectedEvent.participants[i].id << ")";
            if (i < selectedEvent.participants.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

void viewParticipants(const vector<Booking>& bookings) {
    cout << "\n" << string(60, '=') << endl;
    cout << "           VIEW PARTICIPANTS" << endl;
    cout << string(60, '=') << endl;

    cout << "\nAvailable Events to View Participants:\n";
    cout << string(60, '-') << endl;
    for (size_t i = 0; i < bookings.size(); i++) {
        cout << "Event " << (i + 1) << ":" << endl;
        cout << "  Name: " << bookings[i].eventName << endl;
        cout << "  Type: " << bookings[i].eventType << endl;
        cout << "  Venue: " << bookings[i].venue << endl;
        cout << "  Date & Time: " << bookings[i].dateTime << endl;
        cout << "  Current Participants: " << bookings[i].participants.size() << "/" << bookings[i].guestCount << endl;
        cout << string(60, '-') << endl;
    }

    int eventChoice;
    cout << "Select an event to view participants" << endl;
    cout << "--------------------------------" << endl;
    cout << "Enter event number (1-" << bookings.size() << "): ";
    string input;
    getline(cin, input);

    try {
        eventChoice = stoi(input);
        if (eventChoice < 1 || eventChoice > static_cast<int>(bookings.size())) {
            cout << "Invalid event selection." << endl;
            return;
        }
    }
    catch (...) {
        cout << "Invalid input. Please enter a number." << endl;
        return;
    }

    int selectedEventIndex = eventChoice - 1;
    const Booking& selectedEvent = bookings[selectedEventIndex];

    cout << "\n--- Participants for " << selectedEvent.eventName << " ---\n";
    cout << "Event Type: " << selectedEvent.eventType << endl;
    cout << "Venue: " << selectedEvent.venue << endl;
    cout << "Date & Time: " << selectedEvent.dateTime << endl;
    cout << "Status: " << selectedEvent.status << endl;
    cout << "Guest Limit: " << selectedEvent.guestCount << endl;
    cout << string(50, '-') << endl;

    if (selectedEvent.participants.empty()) {
        cout << "No participants registered yet." << endl;
    }
    else {
        cout << "Registered Participants (" << selectedEvent.participants.size() << "/" << selectedEvent.guestCount << "):\n";
        for (size_t i = 0; i < selectedEvent.participants.size(); i++) {
            cout << i + 1 << ". " << selectedEvent.participants[i].name
                << " (ID: " << selectedEvent.participants[i].id << ")"
                << " - Role: " << selectedEvent.participants[i].role
                << " - Paid: " << (selectedEvent.participants[i].paid ? "Yes" : "No") << endl;
        }

        if (selectedEvent.participants.size() >= selectedEvent.guestCount) {
            cout << "\n*** This event is FULL ***" << endl;
        }
        else {
            cout << "\nSpots remaining: " << (selectedEvent.guestCount - selectedEvent.participants.size()) << endl;
        }
    }
}

string getValidPaymentMethod() {
    string input;
    bool isValidChoice = false;
    do {
        cout << "Payment Methods:" << endl;
        cout << "1. Credit Card / Debit Card" << endl;
        cout << "2. Bank Transfer" << endl;
        cout << "3. Cash" << endl;
        cout << "Enter payment method (1-3): ";
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
            cout << "Error: Please enter only numbers (1-3)." << endl;
            continue;
        }

        int choice = stoi(input);
        if (choice >= 1 && choice <= 3) {
            isValidChoice = true;
            switch (choice) {
            case 1: return "Credit Card / Debit Card";
            case 2: return "Bank Transfer";
            case 3: return "Cash";
            }
        }
        else {
            cout << "Error: Please enter 1-3 only." << endl;
        }
    } while (!isValidChoice);
    return "Cash";
}

double getValidAmount() {
    string input;
    bool isValidAmount = false;
    do {
        cout << "Enter payment amount: RM";
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
        }
        else {
            cout << "Error: Amount must be greater than 0." << endl;
        }
    } while (!isValidAmount);
    return 0.0;
}

void proceedPayment(Participant& participant, vector<Booking>& bookings) {
    if (participant.paid) {
        cout << "Participant " << participant.name << " has already paid." << endl;
        return;
    }

    cout << "Processing Payment " << endl;
    cout << "------------------" << endl;
    cout << "Participant: " << participant.name << " (ID: " << participant.id << ")" << endl;
    cout << "Role: " << participant.role << endl;
    cout << "Amount Due: RM" << fixed << setprecision(2) << participant.amountDue << endl;

    cout << "Continue? (y/n): ";
    char proceed = getValidYesNoChoice();
    if (proceed != 'y') {
        cout << "Payment cancelled." << endl;
        return;
    }

    string paymentMethod = getValidPaymentMethod();
    participant.paymentMethod = paymentMethod;

    cout << "\n" << string(40, '-') << endl;
    cout << "PAYMENT REQUIRED: RM" << fixed << setprecision(2) << participant.amountDue << endl;
    cout << string(40, '-') << endl;

    if (paymentMethod == "Credit Card / Debit Card") {
        cout << "\n--- Credit Card / Debit Card Payment Process ---" << endl;
        cout << "Please provide your card details:" << endl;

        string cardNumber = getValidCreditCardNumber();
        string expiryDate = getValidExpiryDate();
        string cvv = getValidCVV();

        cout << "\nCard Details:" << endl;
        cout << "Card Number: " << cardNumber << endl;
        cout << "Expiry Date: " << expiryDate << endl;
        cout << "CVV: " << cvv << endl;

        cout << "\nProcessing card payment..." << endl;
        cout << "Please wait while we verify your card..." << endl;

        for (int i = 0; i < 3; i++) {
            cout << ".";

        }
        cout << endl;

    }
    else if (paymentMethod == "Bank Transfer") {
        cout << "\n--- Bank Transfer Payment Process ---" << endl;
        cout << "Please provide your bank details:" << endl;

        string bankName = getValidBankName();
        string accountNumber = getValidBankAccount();

        cout << "\nBank Details:" << endl;
        cout << "Bank Name: " << bankName << endl;
        cout << "Account Number: " << accountNumber << endl;

        cout << "\nTransfer Details:" << endl;
        cout << "Amount: RM" << fixed << setprecision(2) << participant.amountDue << endl;
        cout << "Reference: PAY-" << participant.id << "-" << time(0) << endl;

        cout << "\nPlease complete the transfer using the details above." << endl;
        cout << "Payment will be confirmed once transfer is received." << endl;

    }
    else if (paymentMethod == "Cash") {
        cout << "\n--- Cash Payment Process ---" << endl;
        cout << "Please prepare cash amount: RM" << fixed << setprecision(2) << participant.amountDue << endl;
        cout << "Note: Change will be provided if you pay more than required." << endl;
    }

    double paymentAmount;

    if (paymentMethod == "Credit Card / Debit Card" || paymentMethod == "Bank Transfer") {
        paymentAmount = participant.amountDue;
        cout << "\n" << string(40, '=') << endl;
        cout << "AUTOMATIC PAYMENT PROCESSING" << endl;
        cout << string(40, '=') << endl;
        cout << "Amount will be automatically charged: RM" << fixed << setprecision(2) << paymentAmount << endl;
        cout << "No manual amount entry required." << endl;
    }
    else {
        cout << "\n" << string(40, '=') << endl;
        cout << "ENTER PAYMENT AMOUNT" << endl;
        cout << string(40, '=') << endl;
        paymentAmount = getValidAmount();

        if (paymentAmount < participant.amountDue) {
            cout << endl;
            cout << "Payment amount insufficient!" << endl;
            cout << "Amount due: RM" << fixed << setprecision(2) << participant.amountDue << endl;
            cout << "Amount paid: RM" << fixed << setprecision(2) << paymentAmount << endl;
            cout << "Please pay the full amount required." << endl;
            return;
        }
    }

    participant.paid = true;
    double originalAmountDue = participant.amountDue;
    participant.amountDue = 0.0;
    cout << endl;
    cout << "Payment successful!" << endl;
    cout << "Amount due: RM" << fixed << setprecision(2) << originalAmountDue << endl;
    cout << "Amount paid: RM" << fixed << setprecision(2) << paymentAmount << endl;

    if (paymentMethod == "Cash" && paymentAmount > originalAmountDue) {
        double change = paymentAmount - originalAmountDue;
        cout << "Change: RM" << fixed << setprecision(2) << change << endl;
    }

    generatePaymentReceipt(participant, originalAmountDue, paymentMethod);

    if (paymentMethod == "Credit Card / Debit Card") {
        cout << "Your card has been charged successfully." << endl;
        cout << "A receipt has been sent to your registered email." << endl;
    }
    else if (paymentMethod == "Bank Transfer") {
        cout << "Bank transfer confirmed. Reference: PAY-" << participant.id << "-" << time(0) << endl;
    }
    else if (paymentMethod == "Cash") {
        cout << "Cash payment received. Thank you!" << endl;
    }

    participant.paymentDate = "Today";
    saveParticipants(bookings, "participants.txt");
}

void processAllPayments(vector<Participant>& participants, vector<Booking>& bookings) {
    if (participants.empty()) {
        cout << "No participants to process payments for." << endl;
        return;
    }

    cout << "\n" << string(60, '=') << endl;
    cout << "           BULK PAYMENT PROCESSING" << endl;
    cout << string(60, '=') << endl;

    double totalAmountDue = 0.0;
    int unpaidCount = 0;

    for (auto& participant : participants) {
        if (!participant.paid) {
            totalAmountDue += participant.amountDue;
            unpaidCount++;
        }
    }

    if (unpaidCount == 0) {
        cout << "All participants have already paid!" << endl;
        return;
    }


    cout << "Payment Summary:" << endl;
    cout << "Total Participants: " << participants.size() << endl;
    cout << "Unpaid Participants: " << unpaidCount << endl;
    cout << "Total Amount Due: RM" << fixed << setprecision(2) << totalAmountDue << endl;
    cout << string(60, '-') << endl;

    cout << "Do you want to process payment for ALL unpaid participants? (y/n): ";
    char proceed = getValidYesNoChoice();
    if (proceed != 'y') {
        cout << "Bulk payment cancelled." << endl;
        return;
    }

    cout << "\nSelect payment method for bulk payment:" << endl;
    string paymentMethod = getValidPaymentMethod();

    cout << "\n" << string(60, '=') << endl;
    cout << "PAYMENT REQUIRED: RM" << fixed << setprecision(2) << totalAmountDue << endl;
    cout << string(60, '=') << endl;

    if (paymentMethod == "Credit Card / Debit Card") {
        cout << "AUTOMATIC PAYMENT PROCESSING" << endl;
        cout << "Amount will be charged: RM" << fixed << setprecision(2) << totalAmountDue << endl;

        cout << "\nEnter Credit Card / Debit Card Details:" << endl;
        cout << "----------------------------------------" << endl;

        string cardNumber = getValidCreditCardNumber();
        string expiryDate = getValidExpiryDate();
        string cvv = getValidCVV();

        cout << "\nProcessing payment..." << endl;
        cout << " Payment processed successfully!" << endl;

    }
    else if (paymentMethod == "Bank Transfer") {
        cout << "AUTOMATIC PAYMENT PROCESSING" << endl;
        cout << "Amount will be transferred: RM" << fixed << setprecision(2) << totalAmountDue << endl;

        cout << "\nEnter Bank Transfer Details:" << endl;
        cout << "----------------------------" << endl;

        string accountNumber = getValidBankAccount();
        string bankName = getValidBankName();

        cout << "\nProcessing bank transfer..." << endl;
        cout << " Bank transfer processed successfully!" << endl;

    }
    else if (paymentMethod == "Cash") {
        cout << "Enter payment amount: RM";
        double paymentAmount = getValidAmount();

        if (paymentAmount < totalAmountDue) {
            cout << "Insufficient payment amount. Payment cancelled." << endl;
            return;
        }

        if (paymentAmount > totalAmountDue) {
            double change = paymentAmount - totalAmountDue;
            cout << "Change: RM" << fixed << setprecision(2) << change << endl;
        }

        cout << "\n Cash payment received!" << endl;
    }



    cout << "\n" << string(60, '=') << endl;
    cout << "PROCESSING BULK PAYMENT" << endl;
    cout << string(60, '=') << endl;

    for (auto& participant : participants) {
        if (!participant.paid) {
            cout << "Processing payment for: " << participant.name << " - RM" << fixed << setprecision(2) << participant.amountDue << endl;

            participant.paymentMethod = paymentMethod;
            participant.paid = true;
            participant.amountDue = 0.0;
            participant.paymentDate = "Today";

            cout << " Payment processed successfully!" << endl;
        }
    }

    saveParticipants(bookings, "participants.txt");


    cout << "\n" << string(60, '=') << endl;
    cout << "BULK PAYMENT COMPLETED!" << endl;
    cout << string(60, '=') << endl;
    cout << "Total participants processed: " << unpaidCount << endl;
    cout << "Total amount collected: RM" << fixed << setprecision(2) << totalAmountDue << endl;
    cout << "Payment method used: " << paymentMethod << endl;
    cout << string(60, '=') << endl;

    cout << "\nGenerating bulk payment receipt..." << endl;
    cout << "Receipt will be sent to your email." << endl;
    string email = getValidEmail();
    cout << "Bulk payment receipt sent to: " << email << endl;
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

void showPaymentMenu(vector<Participant>& participants, vector<Booking>& bookings) {
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
            processSinglePayment(participants, bookings);
            break;
        case 2:
            processAllPayments(participants, bookings);
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

void processSinglePayment(vector<Participant>& participants, vector<Booking>& bookings) {
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

    proceedPayment(*selectedParticipant, bookings);
}

string getValidEmail() {
    string email;
    bool isValid = false;
    do {
        cout << "Enter email address: ";
        getline(cin, email);

        if (email.empty()) {
            cout << "Error: Email cannot be empty." << endl;
            continue;
        }

        bool hasAtSymbol = false;
        bool hasDotAfterAt = false;
        bool hasTextBeforeAt = false;
        bool hasTextAfterDot = false;

        for (size_t i = 0; i < email.length(); i++) {
            if (email[i] == '@') {
                if (hasAtSymbol) {
                    break;
                }
                hasAtSymbol = true;
                if (i > 0) hasTextBeforeAt = true;
            }
            else if (hasAtSymbol && email[i] == '.') {
                hasDotAfterAt = true;
                if (i < email.length() - 1) hasTextAfterDot = true;
            }
        }

        if (hasAtSymbol && hasDotAfterAt && hasTextBeforeAt && hasTextAfterDot) {
            isValid = true;
        }
        else {
            cout << "Error: Please enter a valid email format (e.g., user@domain.com)" << endl;
        }

    } while (!isValid);
    return email;
}

void generatePaymentReceipt(const Participant& participant, double amountDue, const string& paymentMethod) {
    cout << "\n" << string(50, '=') << endl;
    cout << "           PAYMENT RECEIPT" << endl;
    cout << string(50, '=') << endl;
    cout << "Date: " << participant.paymentDate << endl;
    cout << "Receipt No: RCP-" << participant.id << "-" << time(0) << endl;
    cout << string(50, '-') << endl;
    cout << "Participant Details:" << endl;
    cout << "  Name: " << participant.name << endl;
    cout << "  ID: " << participant.id << endl;
    cout << "  Role: " << participant.role << endl;
    cout << string(50, '-') << endl;
    cout << "Payment Details:" << endl;
    cout << "  Method: " << paymentMethod << endl;
    cout << "  Amount Due: RM" << fixed << setprecision(2) << amountDue << endl;
    cout << "  Amount Paid: RM" << fixed << setprecision(2) << amountDue << endl;

    cout << string(50, '-') << endl;
    cout << "Receipt Delivery:" << endl;
    string email = getValidEmail();
    cout << "Receipt will be sent to: " << email << endl;

    cout << string(50, '-') << endl;
    cout << "Status: PAID" << endl;
    cout << string(50, '=') << endl;
    cout << "Thank you for your payment!" << endl;
    cout << "A receipt has been sent to your email." << endl;
    cout << string(50, '=') << endl << endl;
}

void showPaymentMenuForEvents(vector<Booking>& bookings) {
    cout << "\n" << string(60, '=') << endl;
    cout << "           PAYMENT MANAGEMENT SYSTEM" << endl;
    cout << string(60, '=') << endl;

    cout << "\nAvailable Events for Payment Processing:\n";
    cout << string(60, '-') << endl;
    for (size_t i = 0; i < bookings.size(); i++) {
        cout << "Event " << (i + 1) << ":" << endl;
        cout << "  Name: " << bookings[i].eventName << endl;
        cout << "  Type: " << bookings[i].eventType << endl;
        cout << "  Venue: " << bookings[i].venue << endl;
        cout << "  Date & Time: " << bookings[i].dateTime << endl;
        cout << "  Participants: " << bookings[i].participants.size() << "/" << bookings[i].guestCount << endl;
        cout << string(60, '-') << endl;
    }

    int eventChoice;
    cout << "Select an event to process payments" << endl;
    cout << "--------------------------------" << endl;
    cout << "Enter event number (1-" << bookings.size() << "): ";

    string input;
    getline(cin, input);

    try {
        eventChoice = stoi(input);
        if (eventChoice < 1 || eventChoice > static_cast<int>(bookings.size())) {
            cout << "\nError: Invalid event selection. Please enter a number between 1 and " << bookings.size() << "." << endl;
            return;
        }
    }
    catch (...) {
        cout << "\nError: Invalid input. Please enter a valid number." << endl;
        return;
    }

    int selectedEventIndex = eventChoice - 1;
    Booking& selectedEvent = bookings[selectedEventIndex];

    cout << "\n" << string(60, '=') << endl;
    cout << "SELECTED EVENT: " << selectedEvent.eventName << endl;
    cout << string(60, '=') << endl;
    cout << "Event Type: " << selectedEvent.eventType << endl;
    cout << "Venue: " << selectedEvent.venue << endl;
    cout << "Date & Time: " << selectedEvent.dateTime << endl;
    cout << "Current Participants: " << selectedEvent.participants.size() << "/" << selectedEvent.guestCount << endl;
    cout << string(60, '=') << endl;

    if (selectedEvent.participants.empty()) {
        cout << "\nNo participants registered for this event yet." << endl;
        cout << "Please add participants first before processing payments." << endl;
        return;
    }

    cout << "\nProceeding to payment menu for this event's participants..." << endl;
    cout << string(60, '-') << endl;

    showPaymentMenu(selectedEvent.participants, bookings);
}

string getValidInput(const string& title) {
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

string getValidDateTime(const string& title) {
    string input;
    regex pattern{ "\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}" };
    do {
        cout << title;
        getline(cin, input);
        if (input.empty()) {
            cout << "Input cannot be empty. Try again.\n";
        }
        else if (!regex_match(input, pattern)) {
            cout << "Input format incorrect. Try again.\n";
        }
    } while (input.empty());
    return input;
}

string getValidDateline(const string& title) {
    string input;
    regex pattern{ "\\d{4}-\\d{2}-\\d{2}" };
    do {
        cout << title;
        getline(cin, input);
        if (input.empty()) {
            cout << "Input cannot be empty. Try again.\n";
        }
        else if (!regex_match(input, pattern)) {
            cout << "Input format incorrect. Try again.\n";
        }
    } while (input.empty());
    return input;
}

Booking createBooking(int id, vector<Booking>& bookings) {
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
    bookings.push_back(b);

    saveBookings(bookings, "bookings.txt");

    return b;
}

void saveBookings(vector<Booking>& bookings, const string& file) {
    ofstream outFile(file);
    if (!outFile) {
        cerr << "Error opening file: " << file << endl;
        return;
    }


    for (Booking& b : bookings) {
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

void saveParticipants(vector<Booking>& bookings, const string& file) {
    ofstream outFile(file);
    if (!outFile) {
        cerr << "Error opening file: " << file << endl;
        return;
    }
    for (Booking& b : bookings) {
        outFile << "Event ID: " << b.eventId << "\n";
        outFile << "Event Name: " << b.eventName << "\n";
        if (b.participants.empty()) {
            outFile << "Participants: None\n";
        }
        else {
            outFile << "Participants:\n";
            for (const auto& participant : b.participants) {
                outFile << "  - ID: " << participant.id
                    << ", Name: " << participant.name
                    << ", Role: " << participant.role
                    << ", Amount Due: " << participant.amountDue
                    << ", Paid: " << (participant.paid ? "Yes" : "No")
                    << ", Payment Method: " << (participant.paid ? participant.paymentMethod : "N/A")
                    << ", Payment Date: " << (participant.paid ? participant.paymentDate : "N/A") << "\n";
            }
        }
        outFile << string(30, '-') << "\n";
    }

    outFile.close();
}

void loadBookings(vector<Booking>& bookings, const string& file) {
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

void loadParticipants(vector<Booking>& bookings, const string& file) {
    ifstream inFile(file);
    if (!inFile) {
        cerr << "Error opening file: " << file << endl;
        return;
    }

    string line;
    int currentEventId = -1;
    bool inParticipantsSection = false;

    while (getline(inFile, line)) {
        if (line.find("Event ID: ") == 0) {
            currentEventId = stoi(line.substr(10));
            inParticipantsSection = false;
        }
        else if (line.find("Participants:") == 0 && currentEventId != -1) {
            if (line.find("None") != string::npos) {
                // No participants for this event
                continue;
            }
            inParticipantsSection = true;
        }
        else if (inParticipantsSection && line.find("  - ID: ") == 0 && currentEventId != -1) {
            for (size_t i = 0; i < bookings.size(); i++) {
                if (bookings[i].eventId == currentEventId) {
                    Participant p;
                    size_t idPos = line.find("ID: ") + 4;
                    size_t idEnd = line.find(",", idPos);
                    p.id = stoi(line.substr(idPos, idEnd - idPos));

                    size_t namePos = line.find("Name: ") + 6;
                    size_t nameEnd = line.find(",", namePos);
                    p.name = line.substr(namePos, nameEnd - namePos);

                    size_t rolePos = line.find("Role: ") + 6;
                    size_t roleEnd = line.find(",", rolePos);
                    p.role = line.substr(rolePos, roleEnd - rolePos);

                    size_t amountPos = line.find("Amount Due: ") + 12;
                    size_t amountEnd = line.find(",", amountPos);
                    p.amountDue = stod(line.substr(amountPos, amountEnd - amountPos));

                    size_t paidPos = line.find("Paid: ") + 6;
                    size_t paidEnd = line.find(",", paidPos);
                    string paidStr = line.substr(paidPos, paidEnd - paidPos);
                    p.paid = (paidStr == "Yes");

                    size_t methodPos = line.find("Payment Method: ") + 16;
                    size_t methodEnd = line.find(",", methodPos);
                    p.paymentMethod = line.substr(methodPos, methodEnd - methodPos);

                    size_t datePos = line.find("Payment Date: ") + 14;
                    p.paymentDate = line.substr(datePos);

                    bookings[i].participants.push_back(p);
                    break;
                }
            }
        }
    }

    inFile.close();
    cout << "Participants loaded successfully.\n";
}

time_t deadline(const string& deadline) {
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

void checkDeadlines(vector<Booking>& bookings) {
    time_t now = time(0); // current system time

    for (int i = 0; i < (int)bookings.size(); i++) {
        time_t deadlineTime = stringToDate(bookings[i].deadline);

        if (deadlineTime != -1 && deadlineTime < now) {
            if (bookings[i].status != "Closed") {
                bookings[i].status = "Closed";

                string timeStr = formatTimeHHMM(now + 20);
                string dateStr = formatDateYMD(now);

                string taskName = "ClosedReminder_" + to_string(bookings[i].eventId);
                string message = "\"Notice: Registration for " + bookings[i].eventName + " is now CLOSED.\"";

                scheduleReminder(taskName, dateStr, timeStr, message);
            }
        }
        else {
            bookings[i].status = "Open";    // still valid
        }
    }
}

void destroyEvent(vector<Booking>& bookings, int eventId, const string& bookFile, const string& partFile) {
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

void showMainMenu(vector<Booking>& bookings) {
    bool continueMenu = true;

    while (continueMenu) {
        cout << "Welcome to our Personal Event Booking System" << endl;
        cout << "------------------------------------------" << endl;
        cout << "1. Create a new event booking" << endl;
        cout << "2. Add participants " << endl;
        cout << "3. View participants " << endl;
        cout << "4. Make Payment " << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice (1-5): ";

        string input;
        getline(cin, input);

        if (input.empty()) {
            cout << "Error: Please enter a number." << endl;
            continue;
        }

        bool isAllDigits = true;
        for (char ch : input) {
            if (!isdigit(static_cast<unsigned char>(ch))) {
                isAllDigits = false;
                break;
            }
        }

        if (!isAllDigits) {
            cout << "Error: Please enter only numbers (1-5)." << endl;
            continue;
        }

        int choice = 0;
        try {
            choice = stoi(input);
        }
        catch (...) {
            cout << "Error: Invalid number." << endl;
            continue;
        }

        if (choice < 1 || choice > 5) {
            cout << "Error: Please enter 1-5 only." << endl;
            continue;
        }

        switch (choice) {
        case 1: {
            cout << "Creating a new event booking" << endl;
            break;
        }
        case 2: {
            if (bookings.empty()) {
                cout << "No events available. Please create an event first." << endl;
            }
            else {
                addParticipantsToEvent(bookings);
            }
            break;
        }
        case 3: {
            if (bookings.empty()) {
                cout << "No events available to view." << endl;
            }
            else {
                viewParticipants(bookings);
            }
            break;
        }
        case 4: {
            if (bookings.empty()) {
                cout << "No events available. Please create an event first." << endl;
            }
            else {
                showPaymentMenuForEvents(bookings);
            }
            break;
        }
        case 5:
            cout << "Thank you for using our system!" << endl;
            continueMenu = false;
            break;
        }

        cout << endl;
    }
}

void monitorEvent(vector<Booking>& bookings) {
    cout << "\n" << string(60, '=') << endl;
    cout << "         EVENT MONITOR" << endl;
    cout << string(60, '=') << endl;

    cout << "\nAvailable Events for monitor:\n";
    cout << string(60, '-') << endl;
    for (size_t i = 0; i < bookings.size(); i++) {
        cout << "Event " << (i + 1) << ":" << endl;
        cout << "  Name: " << bookings[i].eventName << endl;
        cout << "  Type: " << bookings[i].eventType << endl;
        cout << "  Venue: " << bookings[i].venue << endl;
        cout << "  Date & Time: " << bookings[i].dateTime << endl;
        cout << "  Current Participants: " << bookings[i].participants.size() << "/" << bookings[i].guestCount << endl;
        cout << string(60, '-') << endl;
    }

    int eventChoice;
    cout << "Select an event to start monitoring" << endl;
    cout << "--------------------------------" << endl;
    cout << "Enter event number (1-" << bookings.size() << "): ";

    string input;
    getline(cin, input);

    try {
        eventChoice = stoi(input);
        if (eventChoice < 1 || eventChoice > static_cast<int>(bookings.size())) {
            cout << "\nError: Invalid event selection. Please enter a number between 1 and " << bookings.size() << "." << endl;
            return;
        }
    }
    catch (...) {
        cout << "\nError: Invalid input. Please enter a valid number." << endl;
        return;
    }



    int selectedEventIndex = eventChoice - 1;
    Booking& selectedEvent = bookings[selectedEventIndex];

    cout << "Event " << (eventChoice) << ":" << endl;
    cout << "  Name: " << selectedEvent.eventName << endl;
    cout << "  Type: " << bookings[selectedEventIndex].eventType << endl;
    cout << "  Venue: " << bookings[selectedEventIndex].venue << endl;
    cout << "  Date & Time: " << bookings[selectedEventIndex].dateTime << endl;
    cout << "  Current Participants: " << bookings[selectedEventIndex].participants.size() << "/" << bookings[selectedEventIndex].guestCount << endl;
    cout << string(60, '-') << endl;

    if (getValidYesNoChoice("Selected Event?(Y/N):\n") == 'y') {
        startMonitor(selectedEvent);
    }
    else {
        system("cls");
        showMainMenu();
    }

}

void printQuickNotes(const EventState& e) {
    struct Row {
        string detail;
        string title;
        int rating;
    };

    vector<Row> rows;

    for (int i = 0; i < 50; i++) {
        if (!e.quickNote[ORG_PROBLEM][i].empty()) {
            Row r;
            r.detail = e.quickNote[ORG_PROBLEM][i];
            r.title = e.quickNote[PROBLEM_TITLE][i];
            r.rating = e.quickNote[PROBLEM_RATING][i].empty() ? 10 : stoi(e.quickNote[PROBLEM_RATING][i]);
            rows.push_back(r);
        }
    }

    // Sort by rating, then title
    sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) {
        if (a.rating == b.rating) return a.title < b.title;
        return a.rating < b.rating;
        });

    cout << "\n========== Logged Problems ==========\n";
    for (const auto& r : rows) {
        cout << "[Rating: " << r.rating << "] " << r.title << " -> " << r.detail << endl;
    }
    cout << "=====================================\n";
}

EventState convertBookingToEventState(const Booking& b) {
    EventState e;
    e.booking = b;
    return e;
}

void startMonitor(Booking& b) {
    EventState e = convertBookingToEventState(b);
    //PrintQuickNote funtional;

    cout << "\n--- Monitoring Event: " << b.eventName << " ---\n";
    cout << "1. Add Participant review\n";
    cout << "2. Log Technical Problem\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;
    cin.ignore();


    switch (choice) {
    case 1:
        if (e.review.empty()) {
            Review r = createComment(e);
            e.review.push_back(r);
        }
        break;

    case 2:
        string note, title, ratingInput;
        int rating = -1;

        cout << "Enter Problem details: ";
        getline(cin, note);

        cout << "Enter the Title (Food/Technical Problem/Other): ";
        getline(cin, title);

        cout << "Rate the severity (1 = minor, 10 = critical): ";
        getline(cin, ratingInput);

        int severity = getValidRating(1, 10);

        for (int i = 0; i < 50; i++) {
            if (e.quickNote[ORG_PROBLEM][i].empty()) {
                e.quickNote[ORG_PROBLEM][i] = note;
                e.quickNote[PROBLEM_TITLE][i] = title;
                e.quickNote[PROBLEM_RATING][i] = to_string(severity);
                break;
            }
        }
        printQuickNotes(e);
        break;

    default:
        cout << "Invalid option.\n";
    }
}

Review createComment(EventState& e) {
    Review r;
    cout << "Enter your Name: ";
    getline(cin, r.name);
    cout << "Enter Title: ";
    getline(cin, r.title);
    cout << "Enter Comment: ";
    getline(cin, r.comment);

    r.rating = getValidRating(0, 5);
    return r;
}

int getValidRating(int min, int max) {
    string input;
    rating = -1;
    bool validRating = false;

    cout << "May you giving us a good rating?" << endl;

    do {
        cout << "Enter rating(1-5): ";
        getline(cin, input);

        if (input.empty()) {
            cout << "Error: Please enter a number." << endl;
            continue;
        }

        bool isValidNumber = true;
        for (char c : input) {
            if (!isdigit(c)) {
                isValidNumber = false;
                break;
            }
        }

        if (!isValidNumber) {
            cout << "Error: Please enter only numbers (1-5)." << endl;
            continue;
        }

        rating = stoi(input);

        if (rating >= min && rating <= max) {
            validRating = true;
        }
        else {
            cout << "Error: Rating must be between 1 and 5." << endl;
        }
    } while (!validRating);

    return rating;
}

time_t stringToDateTime(const string &dateTime) {
    tm t = {};
    int year, month, day, hour, minute;
    if (sscanf(dateTime.c_str(), "%d-%d-%d %d:%d",
               &year, &month, &day, &hour, &minute) != 5) {
        cerr << "Invalid dateTime format: " << dateTime << endl;
        return -1;
    }
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = 0;
    return mktime(&t);
}

time_t stringToDate(const string &dateStr) {
    tm t = {};
    int year, month, day;
    if (sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
        cerr << "Invalid deadline format: " << dateStr << endl;
        return -1;
    }
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;
    return mktime(&t);
}

string formatTimeHHMM(time_t t) {
    tm *lt = localtime(&t);
    ostringstream out;
    out << setw(2) << setfill('0') << lt->tm_hour << ":"
        << setw(2) << setfill('0') << lt->tm_min;
    return out.str();
}

string formatDateYMD(time_t t) {
    tm *lt = localtime(&t);
    ostringstream out;
    out << (lt->tm_year + 1900) << "/"
        << setw(2) << setfill('0') << (lt->tm_mon + 1) << "/"
        << setw(2) << setfill('0') << lt->tm_mday;
    return out.str();
}

void scheduleReminder(const string &taskName, const string &date, const string &time, const string &message) {
    string command = "schtasks /create /sc once /tn \"" + taskName +
                     "\" /tr \"cmd /c msg * " + message +
                     "\" /st " + time + " /sd " + date + " /f";

    int result = system(command.c_str());

    if (result == 0) {
        cout << "Reminder scheduled: " << taskName << " at " << date << " " << time << endl;
    } else {
        cerr << "Failed to schedule reminder!" << endl;
    }
}

void addBookingReminders(const Booking &b, int minutesBefore) {
    time_t deadlineTime = stringToDate(b.deadline);
    if (deadlineTime != -1) {
        string timeStr = "09:00";
        string dateStr = formatDateYMD(deadlineTime);

        string taskName = "DeadlineReminder_" + to_string(b.eventId);
        string message = "\"Reminder: Registration deadline for " + b.eventName + " is today!\"";

        scheduleReminder(taskName, dateStr, timeStr, message);
    }

    time_t eventTime = stringToDateTime(b.dateTime);
    if (eventTime != -1) {
        time_t reminderTime = eventTime - (minutesBefore * 60); //60 is changeable
        string timeStr = formatTimeHHMM(reminderTime);
        string dateStr = formatDateYMD(reminderTime);

        string taskName = "EventReminder_" + to_string(b.eventId);
        string message = "\"Reminder: Event " + b.eventName +
                         " starts in " + to_string(minutesBefore) + " minutes!\"";

        scheduleReminder(taskName, dateStr, timeStr, message);
    }

    if (b.status == "Closed") {
        time_t now = time(0);
        string timeStr = formatTimeHHMM(now + 60);
        string dateStr = formatDateYMD(now);

        string taskName = "ClosedReminder_" + to_string(b.eventId);
        string message = "\"Notice: Event " + b.eventName + " is now CLOSED.\"";

        scheduleReminder(taskName, dateStr, timeStr, message);
    }
}

int main() {
    vector<Booking> bookings;
    loadBookings(bookings, "bookings.txt");
    loadParticipants(bookings, "participants.txt");
    checkDeadlines(bookings);
    showMainMenu(bookings);

}

