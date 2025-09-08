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
Report Module-Complete
*/

struct Participant
{
    string id;
    string name;
    string role;
    double amountDue;
    bool paid;
    string paymentMethod;
    string paymentDate;
    string email;
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
    string organizerName;
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



struct PersonalInfo {
    string fullName;
    string email;
    string phone;
};

struct UserProfile {
    string username;
    string password;
    string secQ;
    string secA;
    string role;
    PersonalInfo info;
};

struct EventAd {
    string eventTitle;
    string marketingText;
    string startDate;
    string endDate;
    string status;
};

//Please sort your functions into modules here to easily know

//Validation
string getValidInput(const string& title);
string getValidDateTime(const string& title);
string getValidDateline(const string& title);
char getValidYesNoChoice();
string getValidCreditCardNumber();
string getValidExpiryDate();
string getValidCVV();
string getValidBankAccount();
string getValidBankName();
string getValidEmail();
string getValidName();
string getValidPaymentMethod();
double getValidAmount();
int getValidRoleChoice();
int getValidRating(int min, int max);

//Payment
void userProcessPayment(vector<Booking>& bookings, const string& organizerName);
void displayPaymentSummary(const vector<Participant>& participants);
void showPaymentMenu(vector<Participant>& participants, vector<Booking>& bookings);
void generatePaymentReceipt(const Participant& participant, double amountDue, const string& paymentMethod, const string& userEmail, const Booking& eventInfo);vector<Booking> getOrganizerEvents(const vector<Booking>& b, const string& organizerName);
void viewAllPaymentsSummary(vector <Booking>& bookings,const string&organizerName );
void organizerPaymentMenu(vector<Booking>& bookings,const string& organizerName);
void userViewandMakePayment(vector<Booking>& bookings, const string& organizerName);
void processAllPayments(vector<Booking>& bookings, const string& organizerName);
string getCurrentDate();

//Registration
void registrationMenu(vector<Booking>& bookings);
void addParticipants(vector<Booking>& bookings,const string& organizerName);
void viewParticipants(const vector<Booking>& bookings,const string&organizerName);
void userJoinEvent(vector<Booking>& bookings, const string& organizerName);
void viewJoinedEvents(const vector<Booking>& bookings, const string& organizerName);
void JoinedEventMenu(vector<Booking>& bookings, const string& organizerName);

//Booking
Booking createBooking(int id, vector<Booking>& bookings,const string& organizerName);
time_t deadline(const string& deadline);
void saveBookings(vector<Booking>& bookings, const string& file);
void saveParticipants(vector<Booking>& bookings, const string& file);
void loadBookings(vector<Booking>& bookings, const string& file);
void loadParticipants(vector<Booking>& bookings, const string& file);
void checkDeadlines(vector<Booking>& bookings);
void destroyEvent(vector<Booking>& bookings, int eventId, const string& bookFile, const string& partFile);

//Reminder

//Marketing
string computeStatus(string startDate, string endDate);
bool isValidDate(const string& date);
void loadAds(vector<EventAd>& ads);
void saveAds(vector<EventAd>& ads);
void createAd(vector<EventAd>& ads);
void displayAds(vector<EventAd>& ads);
void showAllAds(vector<EventAd>& ads);
void deleteAd(vector<EventAd>& ads);
void marketingModule(vector<EventAd>& ads);

//User/Login
string normalize(string s);
bool usernameExists(vector<UserProfile>& users, string uname);
bool isStrongPassword(string pwd);
bool isValidEmail(string email);
bool isValidPhone(string phone);
void saveUsers(vector<UserProfile>& users);
void loadUsers(vector<UserProfile>& users);
UserProfile* login(vector<UserProfile>& users, string uname, string pwd);
void regAccount(vector<UserProfile>& users);
void forgotPwd(vector<UserProfile>& users, string uname);
void changePwd(UserProfile& user);
void updateInfo(UserProfile& user);
void deleteAccount(vector<UserProfile>& users, UserProfile*& user);
void createDefaultOrg(vector<UserProfile>& users);
void showAllU(const vector<UserProfile>& users);
void orgDeleteU(vector<UserProfile>& users);
void orgUpdateU(vector<UserProfile>& users);
void manageUsers(vector<UserProfile>& users);
void loginModule(vector<UserProfile>& users,vector<EventAd>&ads,vector<Booking>&b);

//Monitor
Review createComment(EventState& e);
void startMonitor(Booking& b);
EventState convertBookingToEventState(const Booking& b);
void printQuickNotes(const EventState& e);
void monitorEvent(vector<Booking>& bookings);

//Reporting
void generateReport(EventState e);
void readReport(const string& filename);


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
string generateParticipantId(const Booking& event, const string& participantName, int participantCount) {
    string eventPrefix = "";
    for (int i = 0; i < min(3, (int)event.eventName.length()); i++) {
        if (isalpha(event.eventName[i])) {
            eventPrefix += toupper(event.eventName[i]);
        }
    }


    string namePrefix = "";
    for (int i = 0; i < min(3, (int)participantName.length()); i++) {
        if (isalpha(participantName[i])) {
            namePrefix += toupper(participantName[i]);
        }
    }

    string participantId = eventPrefix + to_string(event.eventId) + namePrefix + to_string(participantCount + 1);

    return participantId;
}

string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);

    char buffer[11];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d",
             1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);

    return string(buffer);
}

void addParticipants(vector<Booking>& bookings, const string& organizerName) {
    cout << "\n" << string(60, '=') << endl;
    cout << "           ADD PARTICIPANTS TO EVENT" << endl;
    cout << string(60, '=') << endl;

    vector<int> organizerEventIndices;
    for (size_t i = 0; i < bookings.size(); i++) {
        if (bookings[i].organizerName == organizerName) {
            organizerEventIndices.push_back(i);
        }
    }

    if (organizerEventIndices.empty()) {
        cout << "No events found for organizer: " << organizerName << endl;
        return;
    }

    cout << "\nAvailable Events :\n";
    cout << string(60, '-') << endl;
    for (size_t i = 0; i < organizerEventIndices.size(); i++) {
        int eventIndex = organizerEventIndices[i];
        cout << "Event " << (i + 1) << ":" << endl;
        cout << "  Name: " << bookings[eventIndex].eventName << endl;
        cout << "  Type: " << bookings[eventIndex].eventType << endl;
        cout << "  Venue: " << bookings[eventIndex].venue << endl;
        cout << "  Date & Time: " << bookings[eventIndex].dateTime << endl;
        cout << "  Current Participants: " << bookings[eventIndex].participants.size() << "/" << bookings[i].guestCount << endl;
        cout << string(60, '-') << endl;
    }

    int eventChoice;
    cout << "Select an event to add participants" << endl;
    cout << "--------------------------------" << endl;
    cout << "Enter event number (1-" << organizerEventIndices.size() << "): ";

    string input;
    getline(cin, input);

    try {
        eventChoice = stoi(input);
        if (eventChoice < 1 || eventChoice > static_cast<int>(organizerEventIndices.size())) {
            cout << "\nError: Invalid event selection. Please enter a number between 1 and " << bookings.size() << "." << endl;
            return;
        }
    }
    catch (...) {
        cout << "\nError: Invalid input. Please enter a valid number." << endl;
        return;
    }

    int selectedEventIndex = organizerEventIndices[eventChoice - 1];
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

        ptcp.role = "Customer";

        ptcp.id = generateParticipantId(selectedEvent,ptcp.name,selectedEvent.participants.size() );

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

void regAccount(vector<UserProfile>& users) {
    UserProfile u;

    cout << "Enter username: ";
    getline(cin, u.username);

    while (usernameExists(users, u.username)) {
        cout << "Username already exist, please re-enter: ";
        getline(cin, u.username);
    }

    do {
        cout << "Enter password (at least 6 chars, must include digit, uppercase, lowercase): ";
        getline(cin, u.password);
    } while (!isStrongPassword(u.password));

    cout << "\nSelect Security Question\n";
    cout << "1. What is your pet's name?\n";
    cout << "2. What is your favorite color?\n";
    cout << "3. What city were you born in?\n";
    cout << "Your choice: ";

    int qChoice;
    cin >> qChoice;
    cin.ignore();

    switch (qChoice) {
    case 1:
        u.secQ = "What is your pet's name?";
        break;
    case 2:
        u.secQ = "What is your favorite color?";
        break;
    case 3:
        u.secQ = "What city were you born in?";
        break;
    default:
        u.secQ = "What is your pet's name?";
        break;
    }

    cout << "Enter your answer: ";
    getline(cin, u.secA);

    cout << "Enter your full name: ";
    getline(cin, u.info.fullName);

    do {
        cout << "Enter email: ";
        getline(cin, u.info.email);
    } while (!isValidEmail(u.info.email));

    do {
        cout << "Enter phone number: ";
        getline(cin, u.info.phone);
    } while (!isValidPhone(u.info.phone));

    cout << "\nSelect account type:\n";
    cout << "1. User (Event Participant)\n";
    cout << "2. Organizer (Event Management)\n";
    cout << "Enter choice (1-2): ";

    int roleChoice;
    string input;
    do {
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
            cout << "Error: Please enter only numbers (1-2)." << endl;
            continue;
        }

        roleChoice = stoi(input);
        if (roleChoice == 1 || roleChoice == 2) {
            break;
        } else {
            cout << "Error: Please enter 1 or 2 only." << endl;
        }
    } while (true);

    u.role = (roleChoice == 1) ? "user" : "organizer";
    users.push_back(u);
    cout << "\nAccount created successfully as " << u.role << "!\n";
    saveUsers(users);
}

void viewParticipants(const vector<Booking>& bookings,const string&organizerName) {
    cout << "\n" << string(60, '=') << endl;
    cout << "           VIEW PARTICIPANTS" << endl;
    cout << string(60, '=') << endl;

    cout << "\nAvailable Events to View Participants:\n";
    cout << string(60, '-') << endl;
    int count = 0;
    for (size_t i = 0; i < bookings.size(); i++) {
        if (bookings[i].organizerName == organizerName) {
            count++;
            cout << "Event " << (i + 1) << ":" << endl;
            cout << "  Name: " << bookings[i].eventName << endl;
            cout << "  Type: " << bookings[i].eventType << endl;
            cout << "  Venue: " << bookings[i].venue << endl;
            cout << "  Date & Time: " << bookings[i].dateTime << endl;
            cout << "  Current Participants: " << bookings[i].participants.size() << "/" << bookings[i].guestCount << endl;
            cout << string(60, '-') << endl;
        }
    }

    int eventChoice;
    cout << "Select an event to view participants" << endl;
    cout << "--------------------------------" << endl;
    cout << "Enter event number (1-" << count << "): ";
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

void userProcessPayment(Participant& participant,const Booking& selectedEvent) {
    if (participant.paid) {
        cout << "\nYou have already paid for this event." << endl;
        return;
    }

    cout << "\n" << string(50, '=') << endl;
    cout << "           PAYMENT PROCESSING" << endl;
    cout << string(50, '=') << endl;
    cout << "Event Participant: " << participant.name << endl;
    cout << "Amount Due: RM" << fixed << setprecision(2) << participant.amountDue << endl;
    cout << string(50, '-') << endl;

    cout << "Proceed with payment? (y/n): ";
    if (getValidYesNoChoice() != 'y') {
        cout << "Payment cancelled." << endl;
        return;
    }

    string paymentMethod = getValidPaymentMethod();
    double paymentAmount = participant.amountDue;
    bool paymentSuccess = false;

    if (paymentMethod == "Credit Card / Debit Card") {
        cout << "\n--- Credit Card Payment ---" << endl;
        cout << "Amount to charge: RM" << fixed << setprecision(2) << participant.amountDue << endl;
        string cardNumber = getValidCreditCardNumber();
        string expiryDate = getValidExpiryDate();
        string cvv = getValidCVV();
        cout << "Processing payment..." << endl;
        paymentSuccess = true;

    } else if (paymentMethod == "Bank Transfer") {
        cout << "\n--- Bank Transfer Payment ---" << endl;
        cout << "Amount to transfer: RM" << fixed << setprecision(2) << participant.amountDue << endl;
        string bankName = getValidBankName();
        string accountNumber = getValidBankAccount();
        cout << "Processing transfer..." << endl;
        paymentSuccess = true;

    } else if (paymentMethod == "Cash") {
        cout << "\n--- Cash Payment ---" << endl;
        cout << "Required amount: RM" << fixed << setprecision(2) << participant.amountDue << endl;
        paymentAmount = getValidAmount();

        if (paymentAmount < participant.amountDue) {
            cout << "Insufficient payment amount. Payment cancelled." << endl;
            return;
        }
        paymentSuccess = true;
    }

    if (paymentSuccess) {
        double originalAmount = participant.amountDue;
        participant.paid = true;
        participant.amountDue = 0.0;
        participant.paymentMethod = paymentMethod;
        participant.paymentDate = getCurrentDate();

        cout << "\n" << string(50, '=') << endl;
        cout << "           PAYMENT SUCCESSFUL!" << endl;
        cout << string(50, '=') << endl;
        cout << "Amount Paid: RM" << fixed << setprecision(2) << paymentAmount << endl;

        if (paymentMethod == "Cash" && paymentAmount > originalAmount) {
            cout << "Change: RM" << fixed << setprecision(2)
                 << (paymentAmount - originalAmount) << endl;
        }

        generatePaymentReceipt(participant, originalAmount, paymentMethod,participant.email,selectedEvent);
    }
}

vector<Booking> getOrganizerEvents(const vector<Booking>& b, const string& organizerName) {
    cout << "DEBUG: Looking for organizer: '" << organizerName << "'" << endl;
    cout << "DEBUG: Total bookings: " << b.size() << endl;

    vector<Booking> organizerEvents;
    for (size_t i = 0; i < b.size(); i++) {
        cout << "DEBUG: Booking " << i << " organizer: '" << b[i].organizerName << "'" << endl;
        cout << "DEBUG: Event name: '" << b[i].eventName << "'" << endl;
        cout << "DEBUG: String comparison result: " << (b[i].organizerName == organizerName) << endl;

        if (b[i].organizerName == organizerName) {
            organizerEvents.push_back(b[i]);
        }
    }
    cout << "DEBUG: Found " << organizerEvents.size() << " events" << endl;
    return organizerEvents;
}

void userJoinEvent(vector<Booking>& bookings, UserProfile& currentUser) {
    cout << "\n" << string(60, '=') << endl;
    cout << "           JOIN AN EVENT" << endl;
    cout << string(60, '=') << endl;

    vector<int> availableEvents;
    cout << "\nAvailable Events:\n";
    cout << string(60, '-') << endl;

    for (size_t i = 0; i < bookings.size(); i++) {
        if (bookings[i].status == "Open" &&
            bookings[i].participants.size() < bookings[i].guestCount) {

            bool alreadyJoined = false;
            for (const auto& p : bookings[i].participants) {
                if (p.name == currentUser.info.fullName) {
                    alreadyJoined = true;
                    break;
                }
            }

            if (!alreadyJoined) {
                availableEvents.push_back(i);
                cout << "Event " << availableEvents.size() << ":" << endl;
                cout << "  Name: " << bookings[i].eventName << endl;
                cout << "  Type: " << bookings[i].eventType << endl;
                cout << "  Venue: " << bookings[i].venue << endl;
                cout << "  Date & Time: " << bookings[i].dateTime << endl;
                cout << "  Available Spots: " << (bookings[i].guestCount - bookings[i].participants.size())
                     << "/" << bookings[i].guestCount << endl;
                cout << string(60, '-') << endl;
            }
        }
    }

    if (availableEvents.empty()) {
        cout << "No events available for you to join." << endl;
        return;
    }

    int eventChoice;
    cout << "Select an event to join (1-" << availableEvents.size() << "): ";
    string input;
    getline(cin, input);

    try {
        eventChoice = stoi(input);
        if (eventChoice < 1 || eventChoice > static_cast<int>(availableEvents.size())) {
            cout << "Invalid selection." << endl;
            return;
        }
    } catch (...) {
        cout << "Invalid input." << endl;
        return;
    }

    int selectedEventIndex = availableEvents[eventChoice - 1];
    Booking& selectedEvent = bookings[selectedEventIndex];

    cout << "\nSelected Event: " << selectedEvent.eventName << endl;
    cout << "Do you want to join this event? (y/n): ";
    if (getValidYesNoChoice() != 'y') {
        cout << "Registration cancelled." << endl;
        return;
    }

    Participant participant;
    participant.name = currentUser.info.fullName;
    participant.email = currentUser.info.email;
    participant.role = "Customer";
    participant.amountDue = 50.0;
    participant.paid = false;
    participant.paymentMethod = "";
    participant.paymentDate = "";

    participant.id = generateParticipantId(selectedEvent,participant.name,selectedEvent.participants.size());

    selectedEvent.participants.push_back(participant);

    cout << "\nSuccessfully joined the event!" << endl;
    cout << "Your Participant ID: " << participant.id << endl;
    cout << "Amount Due: RM" << fixed << setprecision(2) << participant.amountDue << endl;

    cout << "\nWould you like to pay now? (y/n): ";
    if (getValidYesNoChoice() == 'y') {
        for (auto& p : selectedEvent.participants) {
            if (p.id == participant.id) {
                userProcessPayment(p,selectedEvent);
                break;
            }
        }
    }

    saveBookings(bookings, "bookings.txt");
    saveParticipants(bookings, "participants.txt");
    cout << "Registration saved successfully!" << endl;
}



void viewJoinedEvents(const vector<Booking>& bookings, const UserProfile& currentUser) {
    cout << "\n" << string(60, '=') << endl;
    cout << "           MY EVENTS" << endl;
    cout << string(60, '=') << endl;

    bool foundEvents = false;

    for (const auto& booking : bookings) {
        for (const auto& participant : booking.participants) {
            if (participant.name == currentUser.info.fullName) {
                foundEvents = true;
                cout << "\nEvent: " << booking.eventName << endl;
                cout << "Type: " << booking.eventType << endl;
                cout << "Venue: " << booking.venue << endl;
                cout << "Date & Time: " << booking.dateTime << endl;
                cout << "Status: " << booking.status << endl;
                cout << "My Participant ID: " << participant.id << endl;
                cout << "Payment Status: " << (participant.paid ? "PAID" : "PENDING") << endl;
                if (!participant.paid) {
                    cout << "Amount Due: RM" << fixed << setprecision(2) << participant.amountDue << endl;
                }
                cout << string(60, '-') << endl;
                break;
            }
        }
    }

    if (!foundEvents) {
        cout << "You haven't joined any events yet." << endl;
        cout << "Use 'Join Event' option to register for events." << endl;
    }
}

void generateBulkPaymentReceipt(const vector<Booking>& bookings,
                               const vector<pair<int, int>>& paidEvents,
                               double totalAmount, const string& paymentMethod,
                               const string& userEmail, const string& userName) {
    cout << "\n" << string(70, '=') << endl;
    cout << "                    BULK PAYMENT RECEIPT" << endl;
    cout << string(70, '=') << endl;

    cout << left << setw(25) << "Receipt Date:" << getCurrentDate() << endl;
    cout << left << setw(25) << "Receipt No:" << "BULK-" << time(0) << endl;
    cout << left << setw(25) << "Transaction Time:" << getCurrentDate() << endl;

    cout << string(70, '-') << endl;
    cout << "                  PARTICIPANT DETAILS" << endl;
    cout << string(70, '-') << endl;

    cout << left << setw(25) << "Participant Name:" << userName << endl;
    cout << left << setw(25) << "Email Address:" << userEmail << endl;
    cout << left << setw(25) << "Payment Method:" << paymentMethod << endl;
    cout << left << setw(25) << "Payment Date:" << getCurrentDate() << endl;

    cout << string(70, '-') << endl;
    cout << "                    EVENTS PAID" << endl;
    cout << string(70, '-') << endl;

    double runningTotal = 0;
    for (size_t i = 0; i < paidEvents.size(); i++) {
        int bookingIdx = paidEvents[i].first;
        int participantIdx = paidEvents[i].second;
        const Booking& event = bookings[bookingIdx];
        const Participant& participant = bookings[bookingIdx].participants[participantIdx];

        cout << "\n" << (i + 1) << ". Event: " << event.eventName << endl;
        cout << "   Type: " << event.eventType << endl;
        cout << "   Venue: " << event.venue << endl;
        cout << "   Date & Time: " << event.dateTime << endl;
        cout << "   Participant ID: " << participant.id << endl;
        cout << "   Amount: RM " << fixed << setprecision(2) << 50.0 << endl;
        cout << string(70, '-') << endl;

        runningTotal += 50.0;
    }

    cout << string(70, '-') << endl;
    cout << "                   PAYMENT SUMMARY" << endl;
    cout << string(70, '-') << endl;
    cout << left << setw(25) << "Total Events:" << paidEvents.size() << endl;
    cout << left << setw(25) << "Total Amount Paid:" << "RM " << fixed << setprecision(2) << totalAmount << endl;
    cout << left << setw(25) << "Payment Status:" << "COMPLETED" << endl;
    cout << left << setw(25) << "Balance Due:" << "RM 0.00" << endl;

    cout << string(70, '=') << endl;
    cout << "              THANK YOU FOR YOUR BULK PAYMENT!" << endl;
    cout << "    This receipt has been sent to: " << userEmail << endl;
    cout << "       Please keep this receipt for your records." << endl;
    cout << string(70, '=') << endl << endl;
}

void processAllUserPayments(vector<Booking>& bookings, const vector<pair<int, int>>& unpaidEvents) {
    cout << "\n" << string(70, '=') << endl;
    cout << "           PAY ALL OUTSTANDING EVENTS" << endl;
    cout << string(70, '=') << endl;

    double totalAmount = 0;

    cout << "\nEvents to be paid:" << endl;
    cout << string(70, '-') << endl;

    for (size_t i = 0; i < unpaidEvents.size(); i++) {
        int bookingIdx = unpaidEvents[i].first;
        int participantIdx = unpaidEvents[i].second;
        const Booking& event = bookings[bookingIdx];
        const Participant& participant = bookings[bookingIdx].participants[participantIdx];

        cout << "\n" << (i + 1) << ". Event: " << event.eventName << endl;
        cout << "   Type: " << event.eventType << endl;
        cout << "   Venue: " << event.venue << endl;
        cout << "   Date & Time: " << event.dateTime << endl;
        cout << "   Your Participant ID: " << participant.id << endl;
        cout << "   Amount Due: RM" << fixed << setprecision(2) << participant.amountDue << endl;
        cout << string(70, '-') << endl;

        totalAmount += participant.amountDue;
    }

    cout << "\n" << string(70, '=') << endl;
    cout << "PAYMENT SUMMARY:" << endl;
    cout << "Total events: " << unpaidEvents.size() << endl;
    cout << "Total amount to pay: RM" << fixed << setprecision(2) << totalAmount << endl;
    cout << string(70, '=') << endl;

    cout << "\nConfirm payment for all " << unpaidEvents.size() << " events? (y/n): ";

    if (getValidYesNoChoice() != 'y') {
        cout << "Payment cancelled." << endl;
        return;
    }

    string paymentMethod = getValidPaymentMethod();
    bool paymentSuccess = false;

    if (paymentMethod == "Credit Card / Debit Card") {
        cout << "\n--- Credit Card Payment ---" << endl;
        cout << "Total charge: RM" << fixed << setprecision(2) << totalAmount << endl;
        string cardNumber = getValidCreditCardNumber();
        string expiryDate = getValidExpiryDate();
        string cvv = getValidCVV();
        cout << "Processing payment..." << endl;
        paymentSuccess = true;

    } else if (paymentMethod == "Bank Transfer") {
        cout << "\n--- Bank Transfer Payment ---" << endl;
        cout << "Total transfer: RM" << fixed << setprecision(2) << totalAmount << endl;
        string bankName = getValidBankName();
        string accountNumber = getValidBankAccount();
        cout << "Processing transfer..." << endl;
        paymentSuccess = true;

    } else if (paymentMethod == "Cash") {
        cout << "\n--- Cash Payment ---" << endl;
        cout << "Total required: RM" << fixed << setprecision(2) << totalAmount << endl;
        double paidAmount = getValidAmount();

        if (paidAmount < totalAmount) {
            cout << "Insufficient amount. Payment cancelled." << endl;
            return;
        }

        if (paidAmount > totalAmount) {
            cout << "Change: RM" << fixed << setprecision(2) << (paidAmount - totalAmount) << endl;
        }
        paymentSuccess = true;
    }

    if (paymentSuccess) {
        string userEmail = "";
        string userName = "";

        for (const auto& event : unpaidEvents) {
            int bookingIdx = event.first;
            int participantIdx = event.second;

            Participant& participant = bookings[bookingIdx].participants[participantIdx];

            if (userEmail.empty()) {
                userEmail = participant.email;
                userName = participant.name;
            }

            participant.paid = true;
            participant.amountDue = 0.0;
            participant.paymentMethod = paymentMethod;
            participant.paymentDate = getCurrentDate();

            cout << "✓ Paid for: " << bookings[bookingIdx].eventName << endl;
        }

        generateBulkPaymentReceipt(bookings, unpaidEvents, totalAmount, paymentMethod, userEmail, userName);

        cout << "\n" << string(70, '=') << endl;
        cout << "           ALL PAYMENTS COMPLETED!" << endl;
        cout << string(70, '=') << endl;
        cout << "Events paid: " << unpaidEvents.size() << endl;
        cout << "Total amount: RM" << fixed << setprecision(2) << totalAmount << endl;
        cout << "Payment method: " << paymentMethod << endl;
        cout << string(70, '=') << endl;
    }
}

void processPaymentTransaction(Participant& participant,const Booking& eventInfo) {
    if (participant.paid) {
        cout << "\nThis payment has already been completed." << endl;
        return;
    }

    cout << "\n" << string(50, '=') << endl;
    cout << "           PAYMENT PROCESSING" << endl;
    cout << string(50, '=') << endl;
    cout << "Participant: " << participant.name << " (ID: " << participant.id << ")" << endl;
    cout << "Amount Due: RM" << fixed << setprecision(2) << participant.amountDue << endl;
    cout << string(50, '-') << endl;

    cout << "Proceed with payment? (y/n): ";
    if (getValidYesNoChoice() != 'y') {
        cout << "Payment cancelled." << endl;
        return;
    }

    string paymentMethod = getValidPaymentMethod();
    participant.paymentMethod = paymentMethod;

    double paymentAmount = participant.amountDue;
    bool paymentSuccess = false;

    if (paymentMethod == "Credit Card / Debit Card") {
        cout << "\n--- Credit Card Payment ---" << endl;
        string cardNumber = getValidCreditCardNumber();
        string expiryDate = getValidExpiryDate();
        string cvv = getValidCVV();
        cout << "Processing payment..." << endl;
        paymentSuccess = true;

    } else if (paymentMethod == "Bank Transfer") {
        cout << "\n--- Bank Transfer Payment ---" << endl;
        string bankName = getValidBankName();
        string accountNumber = getValidBankAccount();
        cout << "Processing transfer..." << endl;
        paymentSuccess = true;

    } else if (paymentMethod == "Cash") {
        cout << "\n--- Cash Payment ---" << endl;
        cout << "Required amount: RM" << fixed << setprecision(2) << participant.amountDue << endl;
        paymentAmount = getValidAmount();

        if (paymentAmount < participant.amountDue) {
            cout << "Insufficient payment amount. Payment cancelled." << endl;
            return;
        }
        paymentSuccess = true;
    }

    if (paymentSuccess) {
        participant.paid = true;
        double originalAmount = participant.amountDue;
        participant.amountDue = 0.0;
        participant.paymentDate = getCurrentDate();

        cout << "\n" << string(50, '=') << endl;
        cout << "           PAYMENT SUCCESSFUL!" << endl;
        cout << string(50, '=') << endl;
        cout << "Amount Paid: RM" << fixed << setprecision(2) << paymentAmount << endl;

        if (paymentMethod == "Cash" && paymentAmount > originalAmount) {
            cout << "Change: RM" << fixed << setprecision(2) << (paymentAmount - originalAmount) << endl;
        }

        generatePaymentReceipt(participant, originalAmount, paymentMethod,participant.email,eventInfo);
    }
}

void viewAllPaymentsSummary( vector<Booking>& b,const string& organizerName) {
    cout << "\n" << string(70, '=') << endl;
    cout << "                   PAYMENT SUMMARY" << endl;
    cout << string(70, '=') << endl;

    double totalRevenue = 0;
    double totalPending = 0;
    int totalPaid = 0;
    int totalUnpaid = 0;
    bool hasEvents = false;

    for (const auto& booking : b) {
        if (booking.organizerName != organizerName) continue;

        hasEvents = true;
        if (booking.participants.empty()) continue;

        cout << "\nEvent: " << booking.eventName << " (" << booking.dateTime << ")" << endl;
        cout << string(70, '-') << endl;
        cout << left << setw(25) << "Participant"
             << setw(12) << "Amount"
             << setw(8) << "Status"
             << setw(15) << "Method" << endl;
        cout << string(70, '-') << endl;

        for (const auto& p : booking.participants) {
            cout << left << setw(25) << p.name
                 << setw(12) << ("RM" + to_string((int)(p.paid ? 0 : p.amountDue)))
                 << setw(8) << (p.paid ? "PAID" : "PENDING")
                 << setw(15) << (p.paid ? p.paymentMethod : "N/A") << endl;

            if (p.paid) {
                totalPaid++;
                totalRevenue += 50.0;
            } else {
                totalUnpaid++;
                totalPending += p.amountDue;
            }
        }
    }

    if (!hasEvents) {
        cout << "No events found for organizer: " << organizerName << endl;
        return;
    }

    cout << "\n" << string(70, '=') << endl;
    cout << "OVERALL SUMMARY:" << endl;
    cout << "Paid Participants: " << totalPaid << endl;
    cout << "Unpaid Participants: " << totalUnpaid << endl;
    cout << "Total Revenue Collected: RM" << fixed << setprecision(2) << totalRevenue << endl;
    cout << "Total Pending: RM" << fixed << setprecision(2) << totalPending << endl;
    cout << string(70, '=') << endl;
}

void processIndividualPayment(vector<Booking>& bookings,const string& organizerName) {
    vector<pair<int, int>> allParticipants;

    cout << "\n" << string(60, '=') << endl;
    cout << "         PROCESS INDIVIDUAL PAYMENT" << endl;
    cout << string(60, '=') << endl;

    cout << "All Participants:" << endl;
    cout << string(60, '-') << endl;
    cout << left << setw(5) << "No" << setw(20) << "Name"
         << setw(20) << "Event" << setw(10) << "Status" << endl;
    cout << string(60, '-') << endl;

    int counter = 1;
    for (size_t i = 0; i < bookings.size(); i++) {
        if (bookings[i].organizerName == organizerName) {
            for (size_t j = 0; j < bookings[i].participants.size(); j++) {
                const auto& p = bookings[i].participants[j];
                cout << left << setw(5) << counter
                     << setw(20) << p.name
                     << setw(20) << bookings[i].eventName
                     << setw(10) << (p.paid ? "PAID" : "PENDING") << endl;

                allParticipants.push_back(make_pair(i, j));
                counter++;
            }
        }
    }

    if (allParticipants.empty()) {
        cout << "No participants found." << endl;
        return;
    }

    cout << "\nSelect participant to process payment (1-" << allParticipants.size() << "): ";
    string input;
    getline(cin, input);

    try {
        int selection = stoi(input);
        if (selection < 1 || selection > static_cast<int>(allParticipants.size())) {
            cout << "Invalid selection." << endl;
            return;
        }

        int bookingIdx = allParticipants[selection - 1].first;
        int participantIdx = allParticipants[selection - 1].second;

        processPaymentTransaction(bookings[bookingIdx].participants[participantIdx],bookings[bookingIdx]);

        saveBookings(bookings, "bookings.txt");
        saveParticipants(bookings, "participants.txt");

    } catch (...) {
        cout << "Invalid input." << endl;
    }
}

void processEventBulkPayments(vector<Booking>& b,const string& organizerName) {
    cout << "\n" << string(60, '=') << endl;
    cout << "         EVENT BULK PAYMENT PROCESSING" << endl;
    cout << string(60, '=') << endl;

    vector<int> eventsWithUnpaid;
    for (size_t i = 0; i < b.size(); i++) {
        if (b[i].organizerName == organizerName) {
            int unpaidCount = 0;
            for (const auto& p : b[i].participants) {
                if (!p.paid) unpaidCount++;
            }
            if (unpaidCount > 0) {
                eventsWithUnpaid.push_back(i);
                cout << eventsWithUnpaid.size() << ". " << b[i].eventName
                     << " (Unpaid: " << unpaidCount << ")" << endl;
            }
        }
    }

    if (eventsWithUnpaid.empty()) {
        cout << "No events with unpaid participants." << endl;
        return;
    }

    cout << "\nSelect event for bulk processing (1-" << eventsWithUnpaid.size() << "): ";
    string input;
    getline(cin, input);

    try {
        int eventChoice = stoi(input);
        if (eventChoice < 1 || eventChoice > static_cast<int>(eventsWithUnpaid.size())) {
            cout << "Invalid selection." << endl;
            return;
        }

        int selectedEventIdx = eventsWithUnpaid[eventChoice - 1];
        Booking& selectedEvent = b[selectedEventIdx];

        double totalAmount = 0;
        int unpaidCount = 0;
        for (const auto& p : selectedEvent.participants) {
            if (!p.paid) {
                totalAmount += p.amountDue;
                unpaidCount++;
            }
        }

        cout << "\nBulk Payment Summary for: " << selectedEvent.eventName << endl;
        cout << "Unpaid Participants: " << unpaidCount << endl;
        cout << "Total Amount: RM" << fixed << setprecision(2) << totalAmount << endl;
        cout << "\nMark all as paid? (y/n): ";

        if (getValidYesNoChoice() == 'y') {
            for (auto& p : selectedEvent.participants) {
                if (!p.paid) {
                    p.paid = true;
                    p.amountDue = 0.0;
                    p.paymentMethod = "Bulk Processing";
                    p.paymentDate = getCurrentDate();
                    cout << "✓ Marked as paid: " << p.name << endl;
                }
            }

            cout << "\n" << string(50, '=') << endl;
            cout << "BULK PROCESSING COMPLETED!" << endl;
            cout << "Participants processed: " << unpaidCount << endl;
            cout << "Total amount: RM" << fixed << setprecision(2) << totalAmount << endl;
            cout << string(50, '=') << endl;

            saveBookings(b, "bookings.txt");
            saveParticipants(b, "participants.txt");
        }

    } catch (...) {
        cout << "Invalid input." << endl;
    }
}

void processUserPaymentSelection(vector<Booking>& bookings,
                                const vector<pair<int, int>>& unpaidEvents,
                                const UserProfile& currentUser) {

    if (unpaidEvents.size() == 1) {
        int bookingIdx = unpaidEvents[0].first;
        int participantIdx = unpaidEvents[0].second;

        cout << "\nProcessing payment for: " << bookings[bookingIdx].eventName << endl;
        Participant& participant = bookings[bookingIdx].participants[participantIdx];
        if (participant.email.empty()) {
            participant.email = currentUser.info.email;
        }
        processPaymentTransaction(participant,bookings[bookingIdx]);

    } else if (unpaidEvents.size() > 1) {
        cout << "\nSelect payment option:" << endl;
        cout << string(50, '-') << endl;

        for (size_t i = 0; i < unpaidEvents.size(); i++) {
            int bookingIdx = unpaidEvents[i].first;
            int participantIdx = unpaidEvents[i].second;

            cout << (i + 1) << ". " << bookings[bookingIdx].eventName
                 << " - RM" << fixed << setprecision(2)
                 << bookings[bookingIdx].participants[participantIdx].amountDue << endl;
        }

        cout << (unpaidEvents.size() + 1) << ". Pay All Outstanding Events" << endl;
        cout << "\nEnter choice (1-" << (unpaidEvents.size() + 1) << "): ";

        string input;
        getline(cin, input);

        try {
            int choice = stoi(input);

            if (choice >= 1 && choice <= static_cast<int>(unpaidEvents.size())) {
                int bookingIdx = unpaidEvents[choice - 1].first;
                int participantIdx = unpaidEvents[choice - 1].second;

                cout << "\nProcessing payment for: " << bookings[bookingIdx].eventName << endl;
                processPaymentTransaction(bookings[bookingIdx].participants[participantIdx],bookings[bookingIdx]);

            } else if (choice == static_cast<int>(unpaidEvents.size()) + 1) {
                cout << "\nProcessing payment for all outstanding events..." << endl;
                processAllUserPayments(bookings, unpaidEvents);
            } else {
                cout << "Invalid selection. Please enter a number between 1 and "
                     << (unpaidEvents.size() + 1) << "." << endl;
                return;
            }
        } catch (...) {
            cout << "Invalid input. Please enter a valid number." << endl;
            return;
        }
    } else {
        cout << "No unpaid events found." << endl;
        return;
    }

    saveBookings(bookings, "bookings.txt");
    saveParticipants(bookings, "participants.txt");
    cout << "\nPayment information updated successfully!" << endl;
}

void userViewAndMakePayments(vector<Booking>& bookings, const UserProfile& currentUser) {
    cout << "\n" << string(60, '=') << endl;
    cout << "           MY PAYMENT STATUS" << endl;
    cout << string(60, '=') << endl;

    vector<pair<int, int>> myPayments;
    bool foundPayments = false;

    for (size_t i = 0; i < bookings.size(); i++) {
        for (size_t j = 0; j < bookings[i].participants.size(); j++) {
            if (bookings[i].participants[j].name == currentUser.info.fullName) {
                myPayments.push_back(make_pair(i, j));
                foundPayments = true;
            }
        }
    }

    if (!foundPayments) {
        cout << "You haven't joined any events yet." << endl;
        return;
    }

    cout << "\nYour Events and Payment Status:" << endl;
    cout << string(60, '-') << endl;

    vector<pair<int, int>> unpaidEvents;

    for (size_t i = 0; i < myPayments.size(); i++) {
        int bookingIdx = myPayments[i].first;
        int participantIdx = myPayments[i].second;

        const Booking& booking = bookings[bookingIdx];
        const Participant& participant = booking.participants[participantIdx];

        cout << "\n" << (i + 1) << ". Event: " << booking.eventName << endl;
        cout << "   Date: " << booking.dateTime << endl;
        cout << "   Status: " << (participant.paid ? "PAID " : "PENDING ") << endl;

        if (participant.paid) {
            cout << "   Payment Method: " << participant.paymentMethod << endl;
            cout << "   Payment Date: " << participant.paymentDate << endl;
        } else {
            cout << "   Amount Due: RM" << fixed << setprecision(2) << participant.amountDue << endl;
            unpaidEvents.push_back(make_pair(bookingIdx, participantIdx));
        }
        cout << string(60, '-') << endl;
    }

    if (!unpaidEvents.empty()) {
        cout << "\nYou have " << unpaidEvents.size() << " unpaid event(s)." << endl;
        cout << "Would you like to make a payment now? (y/n): ";

        if (getValidYesNoChoice() == 'y') {
            processUserPaymentSelection(bookings, unpaidEvents, currentUser);
        }
    } else {
        cout << "\nAll your payments are up to date! " << endl;
    }
}



void organizerPaymentMenu(vector<Booking>& bookings,const string& organizerName) {
    int choice;
    do {
        cout << "\n" << string(50, '=') << endl;
        cout << "         PAYMENT MANAGEMENT" << endl;
        cout << string(50, '=') << endl;
        cout << "1. View Payment Summary" << endl;
        cout << "2. Process Individual Payment" << endl;
        cout << "3. Process Event Bulk Payments" << endl;
        cout << "4. Back to Organizer Menu" << endl;
        cout << "Enter choice (1-4): ";

        string input;
        getline(cin, input);

        try {
            choice = stoi(input);
        } catch (...) {
            cout << "Invalid input. Please enter 1-4." << endl;
            continue;
        }

        vector<Booking> orgEvents = getOrganizerEvents(bookings, organizerName);

        switch (choice) {
            case 1:
                viewAllPaymentsSummary(bookings,organizerName);
                break;
            case 2:
                processIndividualPayment(bookings,organizerName);
                break;
            case 3:
                processEventBulkPayments(bookings,organizerName);
                break;
            case 4:
                cout << "Returning to organizer menu..." << endl;
                break;
            default:
                cout << "Please enter 1-4 only." << endl;
                break;
        }
    } while (choice != 4);
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

void generatePaymentReceipt(const Participant& participant, double amountDue,
                           const string& paymentMethod, const string& userEmail,
                           const Booking& eventInfo) {
    cout << "\n" << string(70, '=') << endl;
    cout << "                    PAYMENT RECEIPT" << endl;
    cout << string(70, '=') << endl;

    cout << left << setw(25) << "Receipt Date:" << getCurrentDate() << endl;
    cout << left << setw(25) << "Receipt No:" << "RCP-" << participant.id << "-" << time(0) << endl;
    cout << left << setw(25) << "Transaction Time:" << participant.paymentDate << endl;

    cout << string(70, '-') << endl;
    cout << "                    EVENT DETAILS" << endl;
    cout << string(70, '-') << endl;

    cout << left << setw(25) << "Event Name:" << eventInfo.eventName << endl;
    cout << left << setw(25) << "Event Type:" << eventInfo.eventType << endl;
    cout << left << setw(25) << "Venue:" << eventInfo.venue << endl;
    cout << left << setw(25) << "Date & Time:" << eventInfo.dateTime << endl;
    cout << left << setw(25) << "Organizer:" << eventInfo.organizerName << endl;

    cout << string(70, '-') << endl;
    cout << "                  PARTICIPANT DETAILS" << endl;
    cout << string(70, '-') << endl;

    cout << left << setw(25) << "Participant Name:" << participant.name << endl;
    cout << left << setw(25) << "Participant ID:" << participant.id << endl;
    cout << left << setw(25) << "Role:" << participant.role << endl;
    cout << left << setw(25) << "Email Address:" << userEmail << endl;

    cout << string(70, '-') << endl;
    cout << "                   PAYMENT DETAILS" << endl;
    cout << string(70, '-') << endl;

    cout << left << setw(25) << "Payment Method:" << paymentMethod << endl;
    cout << left << setw(25) << "Amount Due:" << "RM " << fixed << setprecision(2) << amountDue << endl;
    cout << left << setw(25) << "Amount Paid:" << "RM " << fixed << setprecision(2) << amountDue << endl;
    cout << left << setw(25) << "Payment Status:" << "COMPLETED" << endl;
    cout << left << setw(25) << "Payment Date:" << participant.paymentDate << endl;

    cout << string(70, '-') << endl;
    cout << "                   PAYMENT SUMMARY" << endl;
    cout << string(70, '-') << endl;
    cout << left << setw(25) << "Total Amount Paid:" << "RM " << fixed << setprecision(2) << amountDue << endl;
    cout << left << setw(25) << "Balance Due:" << "RM 0.00" << endl;
    cout << left << setw(25) << "Status:" << "PAID IN FULL" << endl;

    cout << string(70, '=') << endl;
    cout << "                  THANK YOU FOR YOUR PAYMENT!" << endl;
    cout << "    This receipt has been sent to: " << userEmail << endl;
    cout << "       Please keep this receipt for your records." << endl;
    cout << string(70, '=') << endl << endl;
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

Booking createBooking(int id, vector<Booking>& bookings,const string& organizerName) {
    Booking b;
    b.eventId = id;
    b.organizerName = organizerName;

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
        outFile << "Organizer: " << b.organizerName << "\n";
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
        else if (line.find("Organizer: ") == 0) {
            b.organizerName = line.substr(11);
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
                    p.id = line.substr(idPos, idEnd - idPos);


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
        time_t deadlineTime = deadline(bookings[i].deadline);

        if (deadlineTime != -1 && deadlineTime < now) {
            bookings[i].status = "Closed";  // deadline passed
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

//Marketing
string computeStatus(string startDate, string endDate) {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    // Build current date as YYYY-MM-DD
    char buffer[11];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", 1900 + ltm.tm_year, 1 + ltm.tm_mon, ltm.tm_mday);
    string today = buffer;

    if (today < startDate) return "Upcoming";
    else if (today >= startDate && today <= endDate) return "Active";
    else return "Expired";
}

bool isValidDate(const string& date) {
    regex pattern(R"(^\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01])$)");
    return regex_match(date, pattern);
}

void loadAds(vector<EventAd>& ads) {
    ifstream fin("ads.txt");
    string line;

    while (getline(fin, line)) {
        EventAd ad;
        size_t pos = 0, prev = 0;
        vector<string> parts;

        while ((pos = line.find('|', prev)) != string::npos) {
            parts.push_back(line.substr(prev, pos - prev));
            prev = pos + 1;
        }
        parts.push_back(line.substr(prev));
        if (parts.size() == 4) {
            ad.eventTitle = parts[0];
            ad.marketingText = parts[1];
            ad.startDate = parts[2];
            ad.endDate = parts[3];
            ad.status = computeStatus(ad.startDate, ad.endDate);

            ads.push_back(ad);
        }
    }
    fin.close();
}

void saveAds(vector<EventAd>& ads) {
    ofstream fout("ads.txt");
    for (auto& ad : ads) {
        fout << ad.eventTitle << "|" << ad.marketingText << "|"
            << ad.startDate << "|" << ad.endDate << endl;
    }
    fout.close();
}

void createAd(vector<EventAd>& ads) {
    EventAd ad;
    cout << "Enter Event Title: ";
    getline(cin, ad.eventTitle);

    cout << "Enter Marketing Text: ";
    getline(cin, ad.marketingText);

    do {
        cout << "Enter Start Date (YYYY-MM-DD): ";
        getline(cin, ad.startDate);
        if (!isValidDate(ad.startDate)) {
            cout << "Invalid date format! Please enter again.\n";
        }
    } while (!isValidDate(ad.startDate));

    do {
        cout << "Enter End Date (YYYY-MM-DD): ";
        getline(cin, ad.endDate);
        if (!isValidDate(ad.endDate)) {
            cout << "Invalid date format! Please enter again.\n";
        }
    } while (!isValidDate(ad.endDate));

    ad.status = computeStatus(ad.startDate, ad.endDate);
    ads.push_back(ad);

    saveAds(ads);
    cout << "Advertisement created successfully!\n";
}

// active advertisements
void displayAds(vector<EventAd>& ads) {
    cout << "\n\t\t\t=== Current Event Advertisements ===\n\n";
    cout << left << setw(15) << "Event" << setw(30)
        << "Message" << setw(12) << "Start Date"
        << setw(12) << "End Date" << setw(10) << "Status" << endl;
    cout << string(80, '-') << endl;

    for (auto& ad : ads) {
        if (ad.status == "Active") {
            cout << left << setw(15) << ad.eventTitle
                << setw(30) << ad.marketingText
                << setw(12) << ad.startDate
                << setw(12) << ad.endDate
                << setw(10) << ad.status << endl;
        }
    }
}

// all advertisements
void showAllAds(vector<EventAd>& ads) {
    cout << "\n\t\t\t=== All Event Advertisements ===\n\n";
    cout << left << setw(15) << "Event" << setw(30)
        << "Message" << setw(12) << "Start Date"
        << setw(12) << "End Date" << setw(10) << "Status" << endl;
    cout << string(80, '-') << endl;

    for (auto& ad : ads) {
        cout << left << setw(15) << ad.eventTitle
            << setw(30) << ad.marketingText
            << setw(12) << ad.startDate
            << setw(12) << ad.endDate
            << setw(10) << ad.status << endl;
    }
}

void deleteAd(vector<EventAd>& ads) {
    if (ads.empty()) {
        cout << "No advertisements available to delete.\n";
        return;
    }

    cout << "\n=== Delete Advertisement ===\n";
    for (size_t i = 0; i < ads.size(); i++) {
        cout << i + 1 << ". " << ads[i].eventTitle
            << " (" << ads[i].startDate << " to " << ads[i].endDate << ")\n";
    }

    string input;
    int choice = 0;
    regex numRegex(R"(^\d+$)");

    do {
        cout << "Enter the number of the advertisement to delete: ";
        getline(cin, input);

        if (!regex_match(input, numRegex)) {
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        stringstream ss(input);
        ss >> choice;

        if (choice < 1 || choice >(int)ads.size()) {
            cout << "Invalid choice. Enter a number between 1 and " << ads.size() << ".\n";
            choice = 0;
        }

    } while (choice == 0);

    ads.erase(ads.begin() + (choice - 1));
    saveAds(ads);

    cout << "Advertisement deleted successfully!\n";
}

void marketingModule(vector<EventAd>& ads) {
    int choice;
    do {
        cout << "\n\n=== Marketing Module ===\n";
        cout << "1. Create Advertisement\n";
        cout << "2. Show All Advertisement\n";
        cout << "3. Delete Advertisement\n";
        cout << "4. Exit to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            createAd(ads);
            break;

        case 2:
            showAllAds(ads);
            break;

        case 3:
            deleteAd(ads);
            break;

        case 4:
            cout << "Returning to Main Menu...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 4);
}


//User/Login
string normalize(string s) {
    string result;
    for (char c : s) {
        if (c != ' ') result += tolower(c);
    }
    return result;
}

bool usernameExists(vector<UserProfile>& users, string uname) {
    for (auto& u : users) {
        if (u.username == uname)
            return true;
    }
    return false;
}

// check password strength
bool isStrongPassword(string pwd) {
    regex pattern("^(?=.*[0-9])(?=.*[a-z])(?=.*[A-Z]).{6,}$");
    return regex_match(pwd, pattern);
}

bool isValidEmail(string email) {
    regex pattern("^[a-zA-Z0-9._%+-]+@gmail\\.com$");
    return regex_match(email, pattern);
}

bool isValidPhone(string phone) {
    regex pattern("^\\+?[0-9]{1,3}[ ]?[0-9]{2,4}-?[0-9]{6,8}$");
    return regex_match(phone, pattern);
}

void saveUsers(vector<UserProfile>& users) {
    ofstream fout("users.txt");
    for (auto& u : users) {
        fout << u.username << "|" 
            << u.password << "|" 
            << u.secQ << "|" 
            << u.secA << "|" 
            << u.role << "|" 
            << u.info.fullName 
            << "|" << u.info.email 
            << "|" << u.info.phone 
            << endl;
    }
    fout.close();
}

void loadUsers(vector<UserProfile>& users) {
    ifstream fin("users.txt");
    string line;
    while (getline(fin, line)) {
        UserProfile u;
        size_t pos = 0, prev = 0;
        vector<string> parts;
        while ((pos = line.find('|', prev)) != string::npos) {
            parts.push_back(line.substr(prev, pos - prev));
            prev = pos + 1;
        }
        parts.push_back(line.substr(prev));

        if (parts.size() == 8) {
            u.username = parts[0];
            u.password = parts[1];
            u.secQ = parts[2];
            u.secA = parts[3];
            u.role = parts[4];
            u.info.fullName = parts[5];
            u.info.email = parts[6];
            u.info.phone = parts[7];
            users.push_back(u);
        }
    }
    fin.close();
}

UserProfile* login(vector<UserProfile>& users, string uname, string pwd) {
    for (auto& u : users) {
        if (u.username == uname) {
            while (u.password != pwd) {
                cout << "Incorrect password. Please try again: ";
                getline(cin, pwd);
            }
            return &u;
        }
    }
    cout << "Username not found.\n";
    return nullptr;
}

//u forgot password
void forgotPwd(vector<UserProfile>& users, string uname) {
    for (auto& u : users) {
        if (u.username == uname) {
            cout << "Security Questions: " << u.secQ << endl;
            cout << "Answer: ";
            string ans;
            getline(cin, ans);

            if (normalize(ans) == normalize(u.secA)) {
                string newPwd;
                do {
                    cout << "Enter new password (must include digit, uppercase, lowercase, at least 6 chars): ";
                    getline(cin, newPwd);
                    if (!isStrongPassword(newPwd)) {
                        cout << "New password is not strong enough. Please try again.\n";
                    }
                } while (!isStrongPassword(newPwd));

                u.password = newPwd;
                cout << "Password reset successfully.\n";
            }
            else {
                cout << "Incorrect answer.";
            }
            return;
        }
    }
    cout << "Username not found.\n";
}

//u changed password
void changePwd(UserProfile& user) {
    cout << "Enter current password: ";
    string current;
    getline(cin, current);

    if (current == user.password) {
        string newPwd;
        do {
            cout << "Enter new password (must include digit, uppercase, lowercase, min 6 chars): ";
            getline(cin, newPwd);

            if (!isStrongPassword(newPwd)) {
                cout << "Password too weak. Please try again.\n";
            }
        } while (!isStrongPassword(newPwd));

        user.password = newPwd;
        cout << "Password changed successfully.\n";
    }
    else {
        cout << "Incorrect current password.\n";
    }
}

//u update info
void updateInfo(UserProfile& user) {
    cout << "Update full name: ";
    getline(cin, user.info.fullName);

    do {
        cout << "Update email: ";
        getline(cin, user.info.email);
    } while (!isValidEmail(user.info.email));

    cout << "Update phone: ";
    getline(cin, user.info.phone);

    cout << "Personal info updated successfully!\n";
}

//u delete acc
void deleteAccount(vector<UserProfile>& users, UserProfile*& user) {
    cout << "Are you sure you want to delete your account? (Y/N): ";
    char confirm;
    cin >> confirm;
    cin.ignore();

    if (toupper(confirm) == 'Y') {
        for (auto it = users.begin(); it != users.end(); ++it) {
            if (it->username == user->username) {
                users.erase(it);
                cout << "Account deleted successfully!\n";
                user = nullptr;
                saveUsers(users);
                return;
            }
        }
    }
    else {
        cout << "Account deletion cancelled.\n";
    }
}


void createDefaultOrg(vector<UserProfile>& users) {
    if (!usernameExists(users, "organizer")) {
        UserProfile org;
        org.username = "organizer";
        org.password = "Admin123"; //default pwd
        org.secQ = "Default";
        org.secA = "Default";
        org.role = "organizer";
        org.info.fullName = "System Organizer";
        org.info.email = "organizer@gmail.com";
        org.info.phone = "+60123456789";
        users.push_back(org);
        saveUsers(users);
    }
}

//name list for org
void showAllU(const vector<UserProfile>& users) {
    cout << "\n\t\t=== Registered Users ===\n";
    cout << left << setw(15) << "Username"
        << setw(25) << "Full Name"
        << setw(25) << "Email"
        << setw(15) << "Phone"
        << setw(10) << "Role" << endl;
    cout << string(90, '-') << endl;

    for (auto& u : users) {
        if (u.role != "organizer") { // hide org acc
            cout << left << setw(15) << u.username
                << setw(25) << u.info.fullName
                << setw(25) << u.info.email
                << setw(15) << u.info.phone
                << setw(10) << u.role << endl;
        }
    }
}

// delete u by username
void orgDeleteU(vector<UserProfile>& users) {
    string uname;
    cout << "Enter the username to delete: ";
    getline(cin, uname);

    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->username == uname && it->role != "organizer") {
            users.erase(it);
            cout << "User " << uname << " deleted successfully.\n";
            saveUsers(users);
            return;
        }
    }
    cout << "User not found.\n";
}

// org update u info
void orgUpdateU(vector<UserProfile>& users) {
    string uname;
    cout << "Enter the username to update: ";
    getline(cin, uname);

    for (auto& u : users) {
        if (u.username == uname && u.role != "organizer") {
            cout << "Update full name: ";
            getline(cin, u.info.fullName);

            do {
                cout << "Update email: ";
                getline(cin, u.info.email);
            } while (!isValidEmail(u.info.email));

            cout << "Update phone: ";
            getline(cin, u.info.phone);

            cout << "User info updated successfully!\n";
            saveUsers(users);
            return;
        }
    }
    cout << "User not found.\n";
}



void manageUsers(vector<UserProfile>& users) {
    int choice;
    do {
        cout << "\n=== Manage Users (Organizer) ===\n";
        cout << "1. Show All Users\n";
        cout << "2. Delete a User\n";
        cout << "3. Update a User's Info\n";
        cout << "4. Back\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            showAllU(users);
            break;

        case 2:
            orgDeleteU(users);
            break;

        case 3:
            orgUpdateU(users);
            break;

        case 4:
            cout << "Returning to Organizer Menu...\n";
            break;

        default:
            cout << "Invalid choice.";
        }

    } while (choice != 4);
}


void registrationMenu(vector<Booking>& bookings,const string& organizerName) {
    int choice;
    do {
        cout << "\n=== Registration Management ===\n";
        cout << "1. Add Participants to Event\n";
        cout << "2. View Participants\n";
        cout << "4. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                addParticipants(bookings,organizerName);
                break;
            case 2:
                viewParticipants(bookings,organizerName);
                break;
            case 3:
                cout << "Returning to main menu...\n";
                break;
        }
    } while (choice != 4);
}

void loginModule(vector<UserProfile>& users,vector<EventAd>& ads,vector<Booking>&b) {
    int choice;

    do {
        cout << "\n=== User/Organizer Login ===\n";
        cout << "1. Register New Account\n";
        cout << "2. Login\n";
        cout << "3. Forgot Password\n";
        cout << "4. Exit to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            regAccount(users);
        }
        else if (choice == 2) {
            string uname, pwd;
            cout << "Username: ";
            getline(cin, uname);
            cout << "Password: ";
            getline(cin, pwd);

            UserProfile* user = login(users, uname, pwd);
            if (user) {
                cout << "Login successful! Welcome " << user->info.fullName << "\n";

                if (user->role == "organizer") {
                    string organizerName = user->info.fullName;
                    int orgChoice;
                    do {
                        cout << "\n=== Organizer Menu ===\n";
                        cout << "1. Manage Users\n";
                        cout << "2. Event Monitoring\n";
                        cout << "3. Booking Management\n";
                        cout << "4. Marketing Management\n";
                        cout << "5. Registration\n";
                        cout << "6. Payment Management\n";
                        cout << "7. Reporting\n";
                        cout << "8. Logout\n";
                        cout << "Choice: ";
                        cin >> orgChoice;
                        cin.ignore();

                        switch (orgChoice) {
                        case 1:
                            manageUsers(users);
                            break;

                        case 2:
                            //monitoring
                            break;

                        case 3:
                            //booking
                            break;

                        case 4:
                            marketingModule(ads);
                            break;

                        case 5:
                            registrationMenu(b,organizerName);
                            break;

                                case 6 :
                                organizerPaymentMenu(b,organizerName);
                                break;
                        case 7:
                            //reporting
                            break;

                        case 8:
                            cout << "Logging out...\n";
                            break;

                        default: cout << "Invalid choice.\n";
                        }

                    } while (orgChoice != 7);
                }
                else {
                    int subChoice;
                    do {
                        cout << "\n--- Account Menu ---\n";
                        cout << "1. Join Event\n";
                        cout << "2. View Joined Event\n";
                        cout << "3. View Payment\n";
                        cout << "4. Change Password\n";
                        cout << "5. Update Personal Info\n";
                        cout << "6. Delete Account\n";
                        cout << "7. Logout\n";
                        cout << "Choice: ";
                        cin >> subChoice;
                        cin.ignore();

                        if (subChoice == 1) {
                            userJoinEvent(b,*user);
                       }
                        else if (subChoice == 2) {
                            viewJoinedEvents(b,*user);

                        }
                        else if (subChoice == 3) {
                            userViewAndMakePayments(b,*user);
                        }
                        else if (subChoice == 4) {
                            changePwd(*user);
                        }
                        else if (subChoice == 5) {
                            updateInfo(*user);
                        }
                        else if (subChoice == 6) {
                            deleteAccount(users, user);
                            break;
                        }

                    } while (subChoice != 7 && user != nullptr);
                }
            }
            else {
                cout << "Invalid username or password.\n";
            }
        }
        else if (choice == 3) {
            string uname;
            cout << "Enter your username: ";
            getline(cin, uname);
            forgotPwd(users, uname);
        }

    } while (choice != 4);
}



const int PROBLEM_TITLE = 0;
const int ORG_PROBLEM = 1;
const int PROBLEM_RATING = 2;

void startMonitoring(Booking& b) {
    EventState e = convertBookingToEventState(b);

    bool continueMonitoring = true;

    while (continueMonitoring) {
        cout << "\n--- Monitoring Event: " << e.booking.eventName << " ---\n";
        cout << "1. Add Participant review\n";
        cout << "2. Log Technical Problem\n";
        cout << "3. Generate Report and Exiting\n";
        cout << "Enter choice: ";

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
            cout << "Error: Please enter only numbers (1-3)." << endl;
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

        if (choice < 1 || choice > 3) {
            cout << "Error: Please enter 1-3 only." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            if (e.review.empty()) {
                Review r = createComment(e);
                e.review.push_back(r);
            }
            break;

        case 2: {
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
            system("cls");
            printQuickNotes(e);
            break;
        }
        case 3: {
            generateReport(e);
            string filename = "EventReport_" + to_string(e.booking.eventId) + ".txt";
            readReport(filename);

            continueMonitoring = false;
            break;
        }
        default:
            cout << "Invalid option.\n";
        }
    }

}
void monitorEvent(vector<Booking>& bookings) {
    cout << "\n" << string(60, '=') << endl;
    cout << "         EVENT MONITOR" << endl;
    cout << string(60, '=') << endl;
    size_t i;

    cout << "\nAvailable Events for monitor:\n";
    cout << string(60, '-') << endl;
    for ( i = 0; i < bookings.size(); i++) {
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
    cout << "  Type: " << bookings[i].eventType << endl;
    cout << "  Venue: " << bookings[i].venue << endl;
    cout << "  Date & Time: " << bookings[i].dateTime << endl;
    cout << "  Current Participants: " << bookings[i].participants.size() << "/" << bookings[i].guestCount << endl;
    cout << string(60, '-') << endl;

    cout << "Selected Event? (Y/N) : " << endl;
    if (getValidYesNoChoice() == 'y') {
        startMonitoring(selectedEvent);
    }
    else {
        system("cls");
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
    int rating = -1;
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

void generateReport(EventState e) {
    e.booking.status = "finished";

    string filename = "EventReport_" + to_string(e.booking.eventId) + ".txt";

    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error creating report file: " << filename << endl;
        return;
    }

    auto printLine = [&](ostream& os, char c = '=', int n = 60) {
        os << string(n, c) << "\n";
        };


    auto writeBoth = [&](const string& line) {
        cout << line << endl;
        outFile << line << "\n";
        };

    //Header
    printLine(cout); printLine(outFile);
    writeBoth("           EVENT REPORT");
    printLine(cout); printLine(outFile);

    //Event Info
    writeBoth("Event ID     : " + to_string(e.booking.eventId));
    writeBoth("Name         : " + e.booking.eventName);
    writeBoth("Type         : " + e.booking.eventType);
    writeBoth("Venue        : " + e.booking.venue);
    writeBoth("Date & Time  : " + e.booking.dateTime);
    writeBoth("Deadline     : " + e.booking.deadline);
    writeBoth("Status       : " + e.booking.status);
    writeBoth("Guest Limit  : " + to_string(e.booking.guestCount));
    writeBoth("Participants : " + to_string(e.booking.participants.size()) + "/" + to_string(e.booking.guestCount));
    printLine(cout, '-'); printLine(outFile, '-');

    //Participants
    writeBoth("Participants:");
    if (e.booking.participants.empty()) {
        writeBoth("  (No participants)");
    }
    else {
        for (const auto& p : e.booking.participants) {
            writeBoth("  ID: " + p.id + " | Name: " + p.name + " | Role: " + p.role);
            writeBoth("     Amount Due: " + to_string(p.amountDue) +
                " | Paid: " + (p.paid ? "Yes" : "No") +
                " | Method: " + p.paymentMethod +
                " | Date: " + p.paymentDate);
        }
    }
    printLine(cout, '-'); printLine(outFile, '-');

    //Reviews
    writeBoth("Reviews:");
    if (e.review.empty()) {
        writeBoth("  (No reviews submitted)");
    }
    else {
        for (const auto& r : e.review) {
            writeBoth("  Reviewer: " + r.name);
            writeBoth("  Title   : " + r.title);
            writeBoth("  Comment : " + r.comment);
            writeBoth("  Rating  : " + to_string(r.rating) + "/5");
            writeBoth(string(30, '-'));
        }
    }
    printLine(cout, '-'); printLine(outFile, '-');

    //Problems / Quick Notes
    writeBoth("Logged Problems:");
    bool hasProblems = false;
    for (int i = 0; i < 50; i++) {
        if (!e.quickNote[ORG_PROBLEM][i].empty()) {
            hasProblems = true;
            writeBoth("  Title : " + e.quickNote[PROBLEM_TITLE][i]);
            writeBoth("  Detail: " + e.quickNote[ORG_PROBLEM][i]);
            writeBoth("  Rating: " + e.quickNote[PROBLEM_RATING][i] + "/10");
            writeBoth(string(30, '-'));
        }
    }
    if (!hasProblems) {
        writeBoth("  (No problems logged)");
    }
    printLine(cout); printLine(outFile);

    outFile.close();
    cout << "\nReport successfully saved to: " << filename << endl;
}

void readReport(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error opening report file: " << filename << endl;
        return;
    }

    cout << "\n" << string(60, '=') << endl;
    cout << "           READING EVENT REPORT" << endl;
    cout << string(60, '=') << endl;

    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }

    cout << string(60, '=') << endl;
    cout << "       END OF REPORT (" << filename << ")" << endl;
    cout << string(60, '=') << endl;

    inFile.close();
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

void addBookingReminders(const Booking &b, int minutesBefore) { //if need to set reminder jst call this and it will set all reminders
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

    vector<UserProfile> users;
    vector<Booking> bookings;
    vector<EventAd> ads;

    loadUsers(users);
    createDefaultOrg(users);
    saveUsers(users);

    loadBookings(bookings, "bookings.txt");
    loadParticipants(bookings, "participants.txt");
    checkDeadlines(bookings);

    loginModule(users,ads,bookings);

    loadAds(ads);
    marketingModule(ads);
    saveAds(ads);

    return 0;
}

