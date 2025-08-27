#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>
using namespace std;

void proceedPayment(Participant& participant);

struct Participant {
    int id;
    string name;
    string role;
    double amountDue;
    bool paid;
    string paymentMethod;
    string paymentDate;
};


static string getValidPaymentMethod() {
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

static double getValidAmount() {
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

static char getValidYesNoChoice() {
    char choice;
    string input;
    bool isValidChoice = false;
    do {
        cout << "Proceed with payment? (y/n): ";
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
    
    bool continueMenu = true;
    while (continueMenu) {
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
                continueMenu = false;
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


