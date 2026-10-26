//Name: Haili Frowein
// Date: 8/6/2026
//Program: VitalDesk- Healthcare IT Ticketing System Final Project
//Description: This program is a healthcare IT ticketing system that allows users to create, view, and manage IT support tickets for healthcare organizations. It provides a user-friendly interface for submitting tickets, tracking their status, and communicating with IT support staff. The program also includes features for prioritizing tickets, assigning them to specific IT personnel, and generating reports on ticket resolution times and trends.

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>

using namespace std;
const string FILE_NAME = "vitaldesk_tickets.txt";
// Ticket column positions
const int ID = 0;
const int EMPLOYEE = 1;
const int DEPARTMENT = 2;
const int ISSUE = 3;
const int PRIORITY = 4;
const int IMPACT = 5;
const int RESPONSE = 6;
const int STATUS = 7;

// Function prototypes
void displayHeader();
void displayMenu();
int getValidatedInt(const string& prompt, int minimum, int maximum);
string getRequiredText(const string& prompt);
string chooseDepartment();
string chooseIssueType();
string priorityName(int priority);
int estimateResponseMinutes(int priority);
int estimateResponseMinutes(int priority, bool patientCareImpact);
int generateTicketID(const vector<vector<string>>& tickets);
void createTicket(vector<vector<string>>& tickets);
void displayTickets(const vector<vector<string>>& tickets);
void closeTicket(vector<vector<string>>& tickets);
void displaySummary(const vector<vector<string>>& tickets);
void saveTickets(const vector<vector<string>>& tickets);
void loadTickets(vector<vector<string>>& tickets);
vector<string> splitline(const string& line, char delimiter);
vector<string> split(string line, char delimiter);

int main()
{
    vector<vector<string>> tickets; int menuChoice;
    loadTickets(tickets);
    do
    {
        displayHeader();
        displayMenu();
        menuChoice = getValidatedInt("Enter your choice: ", 1, 5);
        switch (menuChoice)
        {
        case 1:
            createTicket(tickets);
            break;
        case 2:
            displayTickets(tickets);
            break;
        case 3:
            closeTicket(tickets);
            break;
        case 4:
            displaySummary(tickets);
            break;
        case 5:
            saveTickets(tickets);
            cout << "n\VitalDesk data saved successfully.\n";
            break;
        }
        if (menuChoice != 5)
        {
			cout << "\nPress enter to return to the main menu...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
        }

    } while (menuChoice != 5);
    return 0;
}
void displayHeader()
{
    cout << "\n**********************************************\n";
	cout << "*         VitalDesk Healthcare IT           *\n";
    cout << "*       Healthcare IT Ticketing System               *\n";
	cout << "*************************************************\n";
}
void displayMenu()
{
    cout << "1. Create a new support ticket\n";
    cout << "2. Display all support tickets\n";
	cout << "3. Close a support ticket\n";
	cout << "4. View summary of support tickets\n";
	cout << "5. Save and exit\n";
	cout << "**********************************************\n";
}
int getValidatedInt(const string& prompt, int minimum, int maximum)
{
    int value;
    while (true)
    {
        cout << prompt;
        cin >> value;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>:: max(), '\n');
            cout << "Invalid entry. Please enter a number.\n";
        }
        else if (value < minimum || value > maximum)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number from" << minimum << " through " << maximum << ".\n";
        }
        else
         {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }

}
string getRequiredText(const string& prompt)
{
    string value;
    do
    {
        cout << prompt;
        getline(cin, value);
        if (value.empty())
        {
            cout << "This field cannot be blank.\n";
        }
    } while (value.empty());
    return value;
}
string chooseDepartment()
{
    cout << "\nDepartments\n";
	cout << "1. Emergency Room\n";
	cout << "2. Radiology\n";
	cout << "3. Cardiology\n";
	cout << "4.Pharmacy\n";
    cout << "5. Labratory\n";
	cout << "6. Nursing\n";
    int choice = getValidatedInt("Select a deprtment: ", 1, 6);
    switch (choice)
    {
    case 1: return "Emergency Room";
    case 2: return "Radiology";
    case 3: return "Cardiology";
    case 4: return "Pharmacy";
    case 5: return "Laboratory";
    default: return "Nursing";
    }
}
string chooseIssueType()
{
    string issues[] =
    {
    "EHR/Login",
    "Printer",
    "Network",
    "Hardware",
    "Software",
    "Password Reset",
    "Other"
    };
    cout << "\nIssue Types\n";
    for (int i = 0; i < 7; i++)
    {
        cout << i + 1 << ". " << issues[i] << '\n';
    }
    int choice = getValidatedInt("Select an issue type: ", 1, 7);
    return issues[choice - 1];
}
string priorityName(int priority)
{
	string priorities[] = { "Low", "Medium", "High", "Critical" };
        return priorities[priority - 1];
}
// Overload function
int estimateResponseMinutes(int priority)
{
    int times[] = { 240, 120, 60, 30 };
    return times[priority - 1];
}
// Overload function
int estimateResponseMinutes(int priority, bool patientCareImpact)
{
    int minutes = estimateResponseMinutes(priority);
    if (patientCareImpact)
    {
        minutes = static_cast<int>(ceil(minutes * 0.50));
    }
    return minutes;
}
int generateTicketID(const vector<vector<string>>& tickets)
{
    int highestID = 1000;
    for (const vector<string>& ticket : tickets)
    {
        int currentID = stoi(ticket[ID]);
        if (currentID > highestID)
        {
            highestID = currentID;
        }
    }
    return highestID + 1;
}
void createTicket(vector<vector<string>>& tickets)
{
	cout << "************CREATE TCIKET************\n";
    string employee = getRequiredText("Employee name : ");
    string department = chooseDepartment();
    string issue = chooseIssueType();

    cout << "\nPriority Levels\n";
    cout << "1. Low\n";
    cout << "2. Medium\n";
    cout << "3. High\n";
    cout << "4. Critical\n";
    int priority = getValidatedInt("Select priority: ", 1, 4);
    cout << "\nDoes this issue directly impact patient care?\n";
    cout << "1. Yes\n";
    cout << "2. No\n";
    bool patientCareImpact =
        getValidatedInt("Enter choice: ", 1, 2) == 1;
    // Nested if/else logic
    if (patientCareImpact)
    {
        priority = 3;
        cout << "Priority increased to High.\n";
    }
    else
    {
        if (priority == 4)
        {
            cout << "Critical priority selected.\n";
        }
    }
    int ticketID = generateTicketID(tickets);
    int responseTime = 
        estimateResponseMinutes(priority, patientCareImpact);
    vector<string> newTicket =
    {
    to_string(ticketID),
    employee,
    department,
    issue,
    priorityName(priority),
    patientCareImpact ? "Yes" : "No",
    to_string(responseTime),
    "Open"
    };
    tickets.push_back(newTicket);
    saveTickets(tickets);
    cout << "\nTicket created successfully.\n";
    cout << "Ticket ID: " << ticketID << '\n';
	cout << "Response time: " << responseTime << " minutes\n";
}
void displayTickets(const vector<vector<string>>& tickets)
{
	cout << "************ALL TICKETS************\n";
    if (tickets.empty())
    {
        cout << "No tickets found.\n";
        return;
    }
    string headings[] =
    {
        "ID",
        "Employee",
        "Department",
        "Issue",
        "Priority",
        "Impact",
        "Minutes",
        "Status"
    };
    int widths[] =
    {
    8, 18, 22, 17, 11, 9, 10, 10
    };
    for (int i = 0; i < 8; i++)
    {
        cout << left << setw(widths[i]) << headings[i];
    }
	cout << '\n' << string(105, '-') << '\n';
    for (const vector<string>& ticket : tickets)
    {
        for (int column = 0; column < 8; column++)
        {
            cout << left << setw(widths[column]) << ticket[column];
        }
        cout << '\n';
    }
}
void closeTicket(vector<vector<string>>& tickets)
{
    if (tickets.empty())
    {
        cout << "\nNo tickets available.\n";
        return;
    }
    int ticketID = getValidatedInt("\nEnter ticket ID: ", 1001, 9999);
    bool found = false;
    for (vector<string>& ticket : tickets)
    {
        if (stoi(ticket[ID]) == ticketID)
        {
            found = true;
            if (ticket[STATUS] == "Closed")
            {
                cout << "Ticket is already closed.\n";
            }
            else
            { 
				ticket[STATUS] == "Closed";
                saveTickets(tickets);
                cout << "Ticket #" << ticketID << "closed.\n";
            }
            break;
        }
    }
    if (!found)
    {
		cout << "Ticket not found.\n";
    }
}
void displaySummary(const vector<vector<string>>& tickets)

{
    int open = 0;
    int closed = 0;
    int critical = 0;
    int totalMinutes = 0;
    for (const vector<string>& ticket : tickets)
    {
        if (ticket[STATUS] == "Open")
        { 
            open++;
        }
        else
        {
            closed++;
        }
        if (ticket[PRIORITY] == "Crtitical")
        {
			critical++;
        }
        totalMinutes += stoi(ticket[RESPONSE]);
    }
    double average = 0;
    if (!tickets.empty())
    {
        average = static_cast<double>(totalMinutes) / tickets.size();
    }
	cout << "\n************TICKET SUMMARY************\n";
	cout << left << setw(26) << "Total tickes:" << tickets.size() << '\n';
	cout << setw(26) << "Open tickets:" << open << '\n';
	cout << setw(26) << "Closed tickets:" << closed << '\n';
    cout << setw(26) << "Critical tickets:" << critical << '\n';
    cout << setw(26) << "Average response time:" << fixed << setprecision(1) << average << "minutes\n";
}
void saveTickets(const vector<vector<string>>& tickets)
{
    ofstream file(FILE_NAME);
    if (!file)
    {
        cout << "Error saving file.\n";
        return;
    }
    for (const vector<string>& ticket : tickets)
    {
        for (int i = 0; i < 8; i++)
        {
            file << ticket[i];
            if (i < 7)
            {
                file << '|';
            }
        }
        file << '\n';
    }
}
void loadTickets(vector<vector<string>>& tickets)
{
    ifstream file(FILE_NAME);
    string line;
    while (getline(file, line))
    {
        vector<string> ticket =
            split(line, '|');
        if (ticket.size() == 8)
        {
            tickets.push_back(ticket);
        }
    }
}
vector<string> split(string line, char delimiter)
{
    vector<string> values;
    string item;
    stringstream stream(line);
    while (getline(stream, item, delimiter))
    {
        values.push_back(item);
    }
    return values;
}