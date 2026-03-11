# Airline Reservation System (C++)

A console-based **Airline Reservation System** built in C++ using core Data Structures. Simulates real-world flight booking with passenger management, seat selection, cancellations, and an admin panel.

---

## Features

###  Booking System
- Book flights between multiple cities (Ahmedabad, Mumbai, Delhi, Chennai)
- Multi-passenger booking with queue-based processing
- Interactive seat map with real-time availability
- Auto-generated **PNR** (Passenger Name Record) for each booking

### Passenger Management
- Input validation for Phone, Email, and Passport ID (Regex-based)
- Choose Travel Class: **Economy / Business / First**
- Choose Meal Preference: **Veg / Non-Veg / Jain**
- View booking details by PNR

### Cancellation & Refund
- Cancel booking using PNR
- **80% refund** automatically calculated
- Seat freed and marked available after cancellation

###  Admin Panel *(Password Protected)*
- View seat map for any flight
- View full passenger list
- Add new flights dynamically
- View booking statistics (Revenue & Total Passengers)
- Update flight status: `On-Time / Delayed / Cancelled`

###  Data Export
- Passenger list auto-saved to `.txt` file per flight

---

## Data Structures Used

| Data Structure | Usage |
|----------------|-------|
| **Linked List** | Storing passenger records per flight |
| **Queue (STL)** | Processing multiple passengers during booking |
| **Stack (STL)** | Undo last seat booking operation |
| **2D Array** | Seat map (27 rows × 10 columns) |
| **Vector** | Managing multiple flights and cities |

---

##  Flight Classes & Pricing

| Class | Rows | Price Multiplier |
|-------|------|-----------------|
| First | 1–9 | Base × 1.75 |
| Business | 10–18 | Base × 1.40 |
| Economy | 19–27 | Base × 1.00 |

> Base Price: ₹5,000

---

## Seat Map Layout

```
    A  B  C     D  E  F  G     H  I  J
 1 [O][O][O]   [O][O][O][O]   [O][O][O]   ← First Class (Rows 1–9)
...
10 [O][O][O]   [O][O][O][O]   [O][O][O]   ← Business Class (Rows 10–18)
...
19 [O][O][O]   [O][O][O][O]   [O][O][O]   ← Economy Class (Rows 19–27)
```
- `[O]` = Available  
- `[X]` = Booked

---

##  How to Run

### Prerequisites
- C++ compiler (GCC / G++ recommended)
- Terminal / Command Prompt

### Compile & Run

```bash
# Compile
g++ -o airline main.cpp

# Run
./airline        # Linux/Mac
airline.exe      # Windows
```

---

## Sample Flights

| Flight ID | Route | Time |
|-----------|-------|------|
| EY0641 | Ahmedabad → Mumbai | 06:00 – 08:00 |
| AI2035 | Ahmedabad → Mumbai | 09:30 – 11:30 |
| AI3042 | Mumbai → Ahmedabad | 07:00 – 09:00 |
| 6E2020 | Mumbai → Ahmedabad | 12:00 – 14:00 |

---

## Admin Access

```
Password: Admin123
```
>  5 attempts allowed before returning to main menu.

---

##  Project Structure

```
Airline-Reservation-System/
│
├── main.cpp          # Full source code
└── README.md         # Project documentation
```

> On booking, a `<FlightID>_passenger_list.txt` file is generated automatically.

---

## Author

**Dev Kotadiya**  
B.Tech Computer Engineering  
GitHub: [@devkotadiya](https://github.com/devkotadiya)

---

## License

This project is for educational purposes only.
