#include <iostream>
#include <cstring>   
using namespace std;
struct BloodRequest {
int requestId;
int recipientId;
char bloodGroup[5];
int units;
char status[15]; 
};
int toIntFromChar(char ch) {
return (int)(ch - '0'); 
}

void printLine() {
cout << "--------------------------------------------------\n";
}
class User {
protected:
int id;
char name[30];
char role[12];
bool verified;

public:
User() {
id = 0;
strcpy(name, "Unknown");
strcpy(role, "User");
verified = false;
}

User(int _id, const char _name[], const char _role[]) {  
    id = _id;  
    strcpy(name, _name);  
    strcpy(role, _role);  
    verified = false;  
}  

virtual void showMenu() {
    cout << "Base User Menu\n";  
}  

void displayProfile() {  
    cout << "ID: " << id  
         << "\nName: " << name  
         << "\nRole: " << role  
         << "\nVerified: " << (verified ? "Yes" : "No")  
         << "\n";  
}  

int getId() { return id; }  
bool isVerified() { return verified; }  

friend class Admin; 

};

class Donor : public User {
private:
char bloodGroup[5];
bool available;

public:
Donor() : User() {
strcpy(role, "Donor");
strcpy(bloodGroup, "N/A");
available = false;
}

Donor(int _id, const char _name[], const char _bg[])  
    : User(_id, _name, "Donor") {  
    strcpy(bloodGroup, _bg);  
    available = true;  
}  

void setAvailability(bool a) { available = a; }  
bool isAvailable() { return available; }  
const char* getBloodGroup() { return bloodGroup; }  

void showMenu() {  
    cout << "\nDonor Menu:\n";  
    cout << "1) View Profile\n";  
    cout << "2) Update Availability\n";  
    cout << "0) Back\n";  
}

};

class Recipient : public User {
public:
Recipient() : User() { strcpy(role, "Recipient"); }
Recipient(int _id, const char _name[]) : User(_id, _name, "Recipient") {}

void showMenu() {  
    cout << "\nRecipient Menu:\n";  
    cout << "1) View Profile\n";  
    cout << "2) Create Blood Request\n";  
    cout << "3) Check My Requests\n";  
    cout << "4) Search Donor by Blood Group\n";   
    cout << "0) Back\n";  
}

};
class BloodStock {
private:
char bloodGroup[5];
int units;

public:
BloodStock() {
strcpy(bloodGroup, "N/A");
units = 0;
}

BloodStock(const char bg[], int u) {  
    strcpy(bloodGroup, bg);  
    units = u;  
}  

void showStock() { cout << bloodGroup << " : " << units << " units\n"; }  
const char* getGroup() { return bloodGroup; }  
int getUnits() { return units; }  

BloodStock operator+(int addUnits) {  
    BloodStock temp(bloodGroup, units + addUnits);  
    return temp;  
}  

BloodStock operator-(int removeUnits) {  
    BloodStock temp(bloodGroup, units - removeUnits);  
    if (temp.units < 0) temp.units = 0;  
    return temp;  
}

};

class Appointment {
private:
int donorId;
char dateTime[25];

public:
Appointment() {
donorId = 0;
strcpy(dateTime, "Not Scheduled");
}

void schedule(int dId, const char dt[]) {  
    donorId = dId;  
    strcpy(dateTime, dt);  
}  

void show() {  
    cout << "Donor ID: " << donorId << " | Appointment: " << dateTime << "\n";  
}

};

class BloodBank {
private:
BloodStock *stockPtr; 
int stockCount;

Appointment appt;     

public:
BloodBank() {
stockPtr = NULL;
stockCount = 0;
}

void attachStock(BloodStock *ptr, int count) {  
    stockPtr = ptr;  
    stockCount = count;  
}  

void viewStock() {  
    if (stockPtr == NULL) { cout << "Stock not attached.\n"; return; }  
    cout << "\n--- Blood Stock ---\n";  
    for (int i = 0; i < stockCount; i++) stockPtr[i].showStock();  
}  

void updateStock() {  
    if (stockPtr == NULL) return;  

    char bg[5];  
    int add;  
    cout << "Enter blood group (e.g., A+, O-): ";  
    cin >> bg;  
    cout << "Enter units to add: ";  
    cin >> add;  

    for (int i = 0; i < stockCount; i++) {  
        if (strcmp(stockPtr[i].getGroup(), bg) == 0) {  
            stockPtr[i] = stockPtr[i] + add; 
            cout << "Stock updated!\n";  
            return;  
        }  
    }  
    cout << "Blood group not found.\n";  
}  

void scheduleAppointment() {  
    int dId;  
    char dt[25];  
    cout << "Enter Donor ID: ";  
    cin >> dId;  
    cout << "Enter Date-Time (e.g., 15-Dec-2025 10AM): ";  
    cin.ignore();  
    cin.getline(dt, 25);  

    appt.schedule(dId, dt);  
    cout << "Appointment scheduled.\n";  
}  

void viewAppointment() { appt.show(); }

};
class Admin : public User {
public:
Admin() : User(999, "Admin", "Admin") { verified = true; }

void showMenu() {  
    cout << "\nAdmin Menu:\n";  
    cout << "1) View All Users\n";  
    cout << "2) Verify User\n";  
    cout << "3) View All Requests\n";  
    cout << "4) Update Stock\n";  
    cout << "5) View Stock\n";  
    cout << "6) Schedule Appointment\n";  
    cout << "7) View Appointment\n";  
    cout << "8) Accept/Cancel a Request\n";    
    cout << "9) Search Donor by Blood Group\n";   
    cout << "0) Back\n";  
}  
void verifyUser(User *u) { 
    u->verified = true;  
    cout << "User verified successfully.\n";  
}
};
const int MAX_DONORS = 10;
const int MAX_RECIPIENTS = 10;
const int MAX_REQUESTS = 20;

Donor donors[MAX_DONORS];
Recipient recipients[MAX_RECIPIENTS];
BloodRequest requests[MAX_REQUESTS];

int donorCount = 0;
int recipientCount = 0;
int requestCount = 0;
void seedStock(BloodStock stock[], int &count) {
count = 4;
stock[0] = BloodStock("A+", 10);
stock[1] = BloodStock("B+", 7);
stock[2] = BloodStock("O+", 12);
stock[3] = BloodStock("O-", 4);
}

void listAllUsers() {
printLine();
cout << "All Donors:\n";
for (int i = 0; i < donorCount; i++) {
donors[i].displayProfile();
cout << "Blood Group: " << donors[i].getBloodGroup()
<< " | Available: " << (donors[i].isAvailable() ? "Yes" : "No") << "\n";
printLine();
}

cout << "All Recipients:\n";  
for (int i = 0; i < recipientCount; i++) {  
    recipients[i].displayProfile();  
    printLine();  
}

}

void createBloodRequest(int recId) {
if (requestCount >= MAX_REQUESTS) {
cout << "Request limit reached.\n";
return;
}

BloodRequest br;  
br.requestId = requestCount + 1;  
br.recipientId = recId;  

cout << "Enter required blood group: ";  
cin >> br.bloodGroup;  

cout << "Enter units: ";  
cin >> br.units;  

strcpy(br.status, "Pending");  

requests[requestCount] = br;  
requestCount++;  

cout << "Blood request created. Status = Pending\n";

}

void showMyRequests(int recId) {
cout << "\n--- My Requests ---\n";
bool found = false;
for (int i = 0; i < requestCount; i++) {
if (requests[i].recipientId == recId) {
cout << "Request ID: " << requests[i].requestId
<< " | Group: " << requests[i].bloodGroup
<< " | Units: " << requests[i].units
<< " | Status: " << requests[i].status << "\n";
found = true;
}
}
if (!found) cout << "No requests found.\n";
}
void searchDonorByBloodGroup() {
char bg[5];
cout << "Enter blood group to search: ";
cin >> bg;

cout << "\n--- Matching Donors ---\n";  
bool found = false;  

for (int i = 0; i < donorCount; i++) {  
    if (strcmp(donors[i].getBloodGroup(), bg) == 0) {   
        if (donors[i].isVerified() && donors[i].isAvailable()) {  
            cout << "Donor Name: " << donors[i].getBloodGroup()  
                 << " | Blood Group: " << donors[i].getBloodGroup()  
                 << " | Available: Yes\n";  
            found = true;  
        }  
    }  
}  

if (!found) {  
    cout << "No verified & available donor found for " << bg << ".\n";  
}

}

void adminUpdateRequestStatus() {
if (requestCount == 0) {
cout << "No requests available.\n";
return;
}

int rid;  
cout << "Enter Request ID: ";  
cin >> rid;  

int index = -1;  
for (int i = 0; i < requestCount; i++) {  
    if (requests[i].requestId == rid) {  
        index = i;  
        break;  
    }  
}  

if (index == -1) {  
    cout << "Request not found.\n";  
    return;  
}  

cout << "Current Status: " << requests[index].status << "\n";  
cout << "1) Accept Request\n";  
cout << "2) Cancel Request\n";  
cout << "Enter choice: ";  

int choice;  
cin >> choice;  

if (choice == 1) {  
    strcpy(requests[index].status, "Accepted");  
    cout << "Request accepted.\n";  
} else if (choice == 2) {  
    strcpy(requests[index].status, "Cancelled");  
    cout << "Request cancelled.\n";  
} else {  
    cout << "Invalid choice.\n";  
}

}
int main() {
Admin admin;

BloodStock stock[10];  
int stockCount = 0;  
seedStock(stock, stockCount);  

BloodBank bank;  
bank.attachStock(stock, stockCount);  

int mainChoice;  

do {  
    printLine();  
    cout << "BLOOD DONATION MANAGEMENT SYSTEM (C++)\n";  
    cout << "1) Register Donor\n";  
    cout << "2) Register Recipient\n";  
    cout << "3) Donor Login\n";  
    cout << "4) Recipient Login\n";  
    cout << "5) Admin Panel\n";  
    cout << "0) Exit\n";  
    printLine();  
    cout << "Enter choice: ";  
    cin >> mainChoice;  

    if (mainChoice == 1) {  
        if (donorCount >= MAX_DONORS) { cout << "Donor limit reached.\n"; continue; }  

        int id;  
        char name[30], bg[5];  
        cout << "Enter Donor ID: ";  
        cin >> id;  
        cout << "Enter Name: ";  
        cin.ignore();  
        cin.getline(name, 30);  
        cout << "Enter Blood Group: ";  
        cin >> bg;  

        donors[donorCount] = Donor(id, name, bg);  
        donorCount++;  
        cout << "Donor Registered (Pending Verification).\n";  
    }  
    else if (mainChoice == 2) {  
        if (recipientCount >= MAX_RECIPIENTS) { cout << "Recipient limit reached.\n"; continue; }  

        int id;  
        char name[30];  
        cout << "Enter Recipient ID: ";  
        cin >> id;  
        cout << "Enter Name: ";  
        cin.ignore();  
        cin.getline(name, 30);  

        recipients[recipientCount] = Recipient(id, name);  
        recipientCount++;  
        cout << "Recipient Registered (Pending Verification).\n";  
    }  
    else if (mainChoice == 3) {  
        int id;  
        cout << "Enter Donor ID: ";  
        cin >> id;  

        int i = 0;  
        while (i < donorCount && donors[i].getId() != id) i++;  

        if (i == donorCount) {  
            cout << "Donor not found.\n";  
        } else {  
            User *uPtr = &donors[i];  

            int c;  
            do {  
                uPtr->showMenu();  
                cout << "Choice: ";  
                cin >> c;  

                if (c == 1) donors[i].displayProfile();  
                else if (c == 2) {  
                    cout << "1) Available  0) Not Available: ";  
                    char ch;  
                    cin >> ch;  
                    int val = toIntFromChar(ch);  
                    donors[i].setAvailability(val == 1);  
                    cout << "Availability updated.\n";  
                }  

            } while (c != 0);  
        }  
    }  
    else if (mainChoice == 4) {  
        int id;  
        cout << "Enter Recipient ID: ";  
        cin >> id;  

        int idx = -1;  
        for (int i = 0; i < recipientCount; i++) {  
            if (recipients[i].getId() == id) { idx = i; break; }  
        }  

        if (idx == -1) {  
            cout << "Recipient not found.\n";  
        } else {  
            User *uPtr = &recipients[idx];  

            int c;  
            do {  
                uPtr->showMenu();  
                cout << "Choice: ";  
                cin >> c;  

                if (c == 1) recipients[idx].displayProfile();  
                else if (c == 2) createBloodRequest(recipients[idx].getId());  
                else if (c == 3) showMyRequests(recipients[idx].getId());  
                else if (c == 4) searchDonorByBloodGroup(); 

            } while (c != 0);  
        }  
    }  
    else if (mainChoice == 5) {  
        int c;  
        do {  
            admin.showMenu();  
            cout << "Choice: ";  
            cin >> c;  

            if (c == 1) listAllUsers();  
            else if (c == 2) {  
                int uid;  
                cout << "Enter user ID to verify (donor/recipient): ";  
                cin >> uid;  

                bool done = false;  

                for (int i = 0; i < donorCount; i++) {  
                    if (donors[i].getId() == uid) {  
                        admin.verifyUser(&donors[i]); 
                        done = true;  
                        break;  
                    }  
                }  

                if (!done) {  
                    for (int i = 0; i < recipientCount; i++) {  
                        if (recipients[i].getId() == uid) {  
                            admin.verifyUser(&recipients[i]);  
                            done = true;  
                            break;  
                        }  
                    }  
                }  

                if (!done) cout << "User ID not found.\n";  
            }  
            else if (c == 3) {  
                cout << "\n--- All Requests ---\n";  
                for (int i = 0; i < requestCount; i++) {  
                    cout << "Req#" << requests[i].requestId  
                         << " RecID:" << requests[i].recipientId  
                         << " Group:" << requests[i].bloodGroup  
                         << " Units:" << requests[i].units  
                         << " Status:" << requests[i].status << "\n";  
                }  
                if (requestCount == 0) cout << "No requests.\n";  
            }  
            else if (c == 4) bank.updateStock();  
            else if (c == 5) bank.viewStock();  
            else if (c == 6) bank.scheduleAppointment();  
            else if (c == 7) bank.viewAppointment();  
            else if (c == 8) adminUpdateRequestStatus(); 
            else if (c == 9) searchDonorByBloodGroup();     

        } while (c != 0);  
    }  

} while (mainChoice != 0);  

cout << "Exiting... Thank you!\n";  
return 0;

}

