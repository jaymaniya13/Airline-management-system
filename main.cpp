#include <iostream>
#include <fstream>
#include <cstring>
#include <queue>
#include <stack>
#include <vector>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <regex>

using namespace std;

///////////////////////////
// Passenger Class
///////////////////////////
class Passenger
{
private:
    string firstName, lastName, phone, email, passportID, meal, travelClass, pnr, status;
    int age, row, col;
    double price;

public:
    Passenger(string fn = "", string ln = "", int a = 0, string ph = "", string em = "", string pass = "", string cls = "", string m = "")
        : firstName(fn), lastName(ln), age(a), phone(ph), email(em), passportID(pass), row(-1), col(-1), travelClass(cls), meal(m), price(0), status("Active")
    {
        generatePNR();
    }

    void assignSeat(int r, int c)
    {
        row = r;
        col = c;
    }
    void setPrice(double p) { price = p; }
    double getPrice() { return price; }
    string getFullName() { return firstName + " " + lastName; }
    int getRow() { return row; }
    int getCol() { return col; }
    string getPNR() { return pnr; }
    string getClass() { return travelClass; }
    string getMeal() { return meal; }
    string getStatus() { return status; }
    void setStatus(string s) { status = s; }

private:
    void generatePNR()
    {
        string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        pnr = "";
        srand(time(0) + rand());
        for (int i = 0; i < 2; i++)
            pnr += chars[rand() % chars.size()];
        for (int i = 0; i < 2; i++)
            pnr += to_string(rand() % 10);
        for (int i = 0; i < 1; i++)
            pnr += chars[rand() % chars.size()];
        for (int i = 0; i < 3; i++)
            pnr += to_string(rand() % 10);
    }
};

///////////////////////////
// Linked List Node
///////////////////////////
struct PassengerNode
{
    Passenger data;
    PassengerNode *next;
    PassengerNode(Passenger p) : data(p), next(nullptr) {}
};

///////////////////////////
// Flight Class
///////////////////////////
class Flight
{
private:
    string flightID, from, to, depTime, arrTime;
    const int rows = 27, cols = 10;
    int seats[27][10];
    PassengerNode *passengerHead;
    stack<pair<int, int>> undoStack;
    double revenue = 0;
    int totalPassengers = 0;
    string status;

public:
    Flight(string fid = "", string f = "", string t = "", string d = "", string a = "") : flightID(fid), from(f), to(t), depTime(d), arrTime(a), passengerHead(nullptr), status("On-Time")
    {
        memset(seats, 0, sizeof(seats));
    }

    string getID() { return flightID; }
    string getRoute() { return from + " → " + to; }
    string getTime() { return depTime + " - " + arrTime; }
    string getStatus() { return status; }
    void setStatus(string s) { status = s; }

    void displaySeatMap()
    {
        cout << "Seats Map: [O]=Available [X]=Booked | First=1-9, Business=10-18, Economy=19-27\n";
        cout << "    A  B  C     D  E  F  G     H  I  J\n";
        for (int r = 0; r < rows; r++)
        {
            if (r == 9 || r == 18)
                cout << "\n";
            cout << setw(2) << r + 1 << " ";
            for (int c = 0; c < cols; c++)
            {
                cout << "[" << (seats[r][c] ? "X" : "O") << "]";
                if (c == 2 || c == 6)
                    cout << "   ";
            }
            cout << "\n";
        }
    }

    bool bookSeat(Passenger &p, int r, int c)
    {
        if (r < 0 || r >= rows || c < 0 || c >= cols || seats[r][c] == 1)
            return false;
        seats[r][c] = 1;
        p.assignSeat(r, c);
        undoStack.push({r, c});
        // Add price
        double basePrice = 5000; // example
        if (r <= 8)
            p.setPrice(basePrice * 1.75);
        else if (r <= 17)
            p.setPrice(basePrice * 1.4);
        else
            p.setPrice(basePrice);

        revenue += p.getPrice();
        totalPassengers++;

        PassengerNode *node = new PassengerNode(p);
        node->next = passengerHead;
        passengerHead = node;
        return true;
    }

    Passenger *findPassengerByPNR(const string &pnr)
    {
        PassengerNode *curr = passengerHead;
        while (curr)
        {
            if (curr->data.getPNR() == pnr)
                return &curr->data;
            curr = curr->next;
        }
        return nullptr;
    }

    double cancelBookingByPNR(const string &pnr)
    {
        PassengerNode *curr = passengerHead;
        while (curr)
        {
            if (curr->data.getPNR() == pnr)
            {
                // Mark as cancelled
                curr->data.setStatus("Cancelled");

                // Free the seat in seat map
                int r = curr->data.getRow();
                int c = curr->data.getCol();
                seats[r][c] = 0;

                // Reset seat assignment
                curr->data.assignSeat(-1, -1);

                // Calculate refund
                double refund = 0.8 * curr->data.getPrice();
                revenue -= refund;
                totalPassengers--;

                return refund;
            }
            curr = curr->next;
        }
        return -1;
    }

    void displayPassengerList()
    {
        if (!passengerHead)
        {
            cout << "No passengers booked yet.\n";
            return;
        }

        cout << left
             << setw(6) << "ID" << "|"
             << setw(25) << "Name" << "|"
             << setw(8) << "Seat" << "|"
             << setw(10) << "Class" << "|"
             << setw(12) << "Meal" << "|"
             << setw(12) << "PNR" << "|"
             << setw(12) << "Status" << "\n";

        PassengerNode *curr = passengerHead;
        int id = 1;

        while (curr)
        {
            cout << left
                 << setw(6) << id << "|"
                 << setw(25) << curr->data.getFullName() << "|"
                 << setw(8) << (curr->data.getStatus() == "Cancelled" ? "--" : string(1, char('A' + curr->data.getCol())) + to_string(curr->data.getRow() + 1)) << "|"
                 << setw(10) << curr->data.getClass() << "|"
                 << setw(12) << curr->data.getMeal() << "|"
                 << setw(12) << curr->data.getPNR() << "|"
                 << setw(12) << curr->data.getStatus() << "\n";
            curr = curr->next;
            id++;
        }
    }

    void savePassengerList()
    {
        ofstream fout(flightID + "_passenger_list.txt");
        fout << left
             << setw(6) << "ID" << "|"
             << setw(25) << "Name" << "|"
             << setw(8) << "Seat" << "|"
             << setw(10) << "Class" << "|"
             << setw(12) << "Meal" << "|"
             << setw(12) << "PNR" << "|"
             << setw(12) << "Status" << "\n";

        PassengerNode *curr = passengerHead;
        int id = 1;
        while (curr)
        {
            fout << left
                 << setw(6) << id << "|"
                 << setw(25) << curr->data.getFullName() << "|"
                 << setw(8) << (curr->data.getStatus() == "Cancelled" ? "--" : string(1, char('A' + curr->data.getCol())) + to_string(curr->data.getRow() + 1)) << "|"
                 << setw(10) << curr->data.getClass() << "|"
                 << setw(12) << curr->data.getMeal() << "|"
                 << setw(12) << curr->data.getPNR() << "|"
                 << setw(12) << curr->data.getStatus() << "\n";
            curr = curr->next;
            id++;
        }
        fout.close();
    }

    double getRevenue() { return revenue; }
    int getTotalPassengers() { return totalPassengers; }
};

///////////////////////////
// Global Flights & Cities
///////////////////////////
vector<string> cities = {"Ahmedabad", "Mumbai", "Delhi", "Chennai"};
vector<Flight> flights;

///////////////////////////
// Validation Helpers
///////////////////////////
bool validatePhone(const string &phone)
{
    return regex_match(phone, regex("^[0-9]{10}$"));
}
bool validateEmail(const string &email)
{
    return regex_match(email, regex("^[\\w.-]+@[\\w.-]+\\.[A-Za-z]{2,}$"));
}
bool validatePassport(const string &passport)
{
    return regex_match(passport, regex("^[A-Z][0-9]{7}$"));
}

///////////////////////////
// Flight selection
///////////////////////////
Flight *selectFlight()
{
    cout << "\nSelect Departure City:\n";
    for (int i = 0; i < cities.size(); i++)
        cout << i + 1 << ". " << cities[i] << endl;
    int dep;
    cin >> dep;
    dep--;

    cout << "Select Destination City:\n";
    for (int i = 0; i < cities.size(); i++)
    {
        if (i == dep)
            continue;
        cout << i + 1 << ". " << cities[i] << endl;
    }
    int dest;
    cin >> dest;
    dest--;

    vector<Flight *> options;
    for (auto &f : flights)
    {
        if (f.getRoute() == cities[dep] + " → " + cities[dest])
            options.push_back(&f);
    }
    if (options.empty())
    {
        cout << "No flights available between these cities.\n";
        return nullptr;
    }
    cout << "\nAvailable Flights:\n";
    for (int i = 0; i < options.size(); i++)
        cout << i + 1 << ". " << options[i]->getID() << " | " << options[i]->getTime() << " | " << options[i]->getStatus() << endl;
    int choice;
    cin >> choice;
    return options[choice - 1];
}

///////////////////////////
// MAIN
///////////////////////////
int main()
{
    queue<Passenger> bookingQueue;
    int menu = 0;

    // Sample flights
    flights.push_back(Flight("EY0641", "Ahmedabad", "Mumbai", "06:00", "08:00"));
    flights.push_back(Flight("AI2035", "Ahmedabad", "Mumbai", "09:30", "11:30"));
    flights.push_back(Flight("AI3042", "Mumbai", "Ahmedabad", "07:00", "09:00"));
    flights.push_back(Flight("6E2020", "Mumbai", "Ahmedabad", "12:00", "14:00"));

    while (menu != 5)
    {
    optionsmenu:
        cout << "\n==============================================\n";
        cout << "               ETIHAD AIRWAYS\n";
        cout << "==============================================\n";
        cout << "1. Book Flights\n";
        cout << "2. Manage Booking\n";
        cout << "3. View All Flights\n";
        cout << "4. Admin Panel\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> menu;

        switch (menu)
        {
        case 1:
        {
            Flight *flight = selectFlight();
            if (!flight)
                break;

            int num;
            cout << "Enter number of passengers: ";
            cin >> num;

            for (int i = 0; i < num; i++)
            {
                string fn, ln, ph, em, pass, cls, meal;
                int age;
                cout << "\nPassenger " << i + 1 << " details:\n";
                cout << "First Name: ";
                cin >> fn;
                cout << "Last Name: ";
                cin >> ln;
                cout << "Age: ";
                cin >> age;

                do
                {
                    cout << "Phone (10 digits): ";
                    cin >> ph;
                } while (!validatePhone(ph));
                do
                {
                    cout << "Email: ";
                    cin >> em;
                } while (!validateEmail(em));
                do
                {
                    cout << "Passport ID (X1234567): ";
                    cin >> pass;
                } while (!validatePassport(pass));

                cout << "Select Travel Class (Economy/Business/First): ";
                cin >> cls;
                cout << "Select Meal (Veg/Non-Veg/Jain): ";
                cin >> meal;
                bookingQueue.push(Passenger(fn, ln, age, ph, em, pass, cls, meal));
            }

            while (!bookingQueue.empty())
            {
                Passenger p = bookingQueue.front();
                bookingQueue.pop();
                flight->displaySeatMap();
                cout << "Select seat for " << p.getFullName() << " (e.g. 2A): ";
                string seatInput;
                cin >> seatInput;
                int r = stoi(seatInput.substr(0, seatInput.size() - 1)) - 1;
                char colChar = seatInput.back();
                int c = toupper(colChar) - 'A';

                if (flight->bookSeat(p, r, c))
                {
                    cout << "Ticket booked successfully! PNR: " << p.getPNR() << endl;
                    cout << "Price: " << p.getPrice() << endl;
                }
                else
                {
                    cout << "Seat unavailable, try again.\n";
                    bookingQueue.push(p);
                }
            }
            flight->savePassengerList();
            break;
        }
        case 2:
        {
            cout << "Manage Booking:\n1. View My Booking\n2. Cancel & Refund\nEnter choice: ";
            int mchoice;
            cin >> mchoice;
            if (mchoice == 1)
            {
                string pnr;
                cout << "Enter your PNR: ";
                cin >> pnr;
                bool found = false;
                for (auto &f : flights)
                {
                    Passenger *p = f.findPassengerByPNR(pnr);
                    if (p)
                    {
                        found = true;
                        cout << "Booking Details:\n";
                        cout << "Name: " << p->getFullName() << endl;
                        cout << "Class: " << p->getClass() << endl;
                        cout << "Meal: " << p->getMeal() << endl;
                        cout << "PNR: " << p->getPNR() << endl;
                        cout << "Status: " << p->getStatus() << endl;
                        break;
                    }
                }
                if (!found)
                    cout << "PNR not found.\n";
            }
            else if (mchoice == 2)
            {
                string pnr;
                cout << "Enter PNR to cancel: ";
                cin >> pnr;
                bool found = false;
                for (auto &f : flights)
                {
                    Passenger *p = f.findPassengerByPNR(pnr);
                    if (p)
                    {
                        found = true;
                        cout << "Refund amount will be 80% of ticket price: " << 0.8 * p->getPrice() << endl;
                        cout << "Confirm cancellation? (Y/N): ";
                        char conf;
                        cin >> conf;
                        if (toupper(conf) == 'Y')
                        {
                            double refund = f.cancelBookingByPNR(pnr);
                            cout << "Booking cancelled. Refund Amount: " << refund << endl;
                            f.savePassengerList();
                        }
                        else
                        {
                            cout << "Cancellation aborted.\n";
                        }
                        break;
                    }
                }
                if (!found)
                    cout << "PNR not found.\n";
            }
            break;
        }
        case 3:
        {
            cout << "\nAll Flights Status:\n";
            for (auto &f : flights)
            {
                cout << f.getID() << " | " << f.getRoute() << " | " << f.getTime() << " | " << f.getStatus() << endl;
            }
            break;
        }
        case 4:
        {
            int Try = 5;
        admincase:
            if (Try)
            {
                cout << "Admin Access:" << endl;
                cout << "Enter Password:";
                string adminpassword = "Admin123";
                string confirmpassword;
                cin >> confirmpassword;
                if (adminpassword == confirmpassword)
                {
                    cout << "Enter Flight ID: ";
                    string fid;
                    cin >> fid;
                    bool found = false;
                    for (auto &f : flights)
                    {
                        if (f.getID() == fid)
                        {
                        adminchoices:
                            found = true;
                            int achoice = 0;
                            cout << "Admin Menu:\n1. View Seat Map\n2. View Passenger List\n3. Add Flight\n4. Booking Statistics\n5. Update Flight Status\n6. Exit\nEnter choice: ";
                            cin >> achoice;
                            if (achoice == 1)
                                f.displaySeatMap();
                            else if (achoice == 2)
                                f.displayPassengerList();
                            else if (achoice == 3)
                            {
                                string id, from, to, dep, arr;
                                cout << "Flight ID: ";
                                cin >> id;
                                cout << "From: ";
                                cin >> from;
                                cout << "To: ";
                                cin >> to;
                                cout << "Departure Time: ";
                                cin >> dep;
                                cout << "Arrival Time: ";
                                cin >> arr;
                                flights.push_back(Flight(id, from, to, dep, arr));
                                cout << "Flight added.\n";
                            }
                            else if (achoice == 4)
                            {
                                cout << "Total Passengers: " << f.getTotalPassengers() << endl;
                                cout << "Revenue: " << f.getRevenue() << endl;
                            }
                            else if (achoice == 5)
                            {
                                string status;
                                cout << "Enter new status (On-Time/Delayed/Cancelled): ";
                                cin >> status;
                                f.setStatus(status);
                                cout << "Flight status updated.\n";
                            }
                            else if (achoice == 6)
                            {

                                break;
                            }
                            goto adminchoices;
                        }
                    }
                    if (!found)
                        cout << "Flight not found.\n";
                    break;
                }
                else
                {
                    Try--;
                    goto admincase;
                }
            }
            else
            {
                goto optionsmenu;
            }
        }
        case 5:
            cout << "Thank you for using Airline Reservation System!\n";
            break;
        default:
            cout << "Invalid choice, try again.\n";
        }
    }
    return 0;
}

