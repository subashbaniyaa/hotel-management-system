#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <string>
#include <cctype>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>

using namespace std;

string getCurrentDateTime() {
	time_t now = time(0);
	tm * localTime = localtime( & now);
	char buffer[80];
	strftime(buffer, 80, "%Y-%m-%d", localTime);
	return string(buffer);
}

void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

class Person {
protected:
	string name;
	string address;
	string phone;
public:
	void setDetails(const string & n,
	                const string & a,
	                const string & p) {
		name = n;
		address = a;
		phone = p;
	}
	string getName() const {
		return name;
	}
	string getAddress() const {
		return address;
	}
	string getPhone() const {
		return phone;
	}
};

class Customer: public Person {
private:
	int roomNo;
	int days;
	string roomType;
	long cost;
	string bookingDateTime;
	bool checkedOut;

public:
	Customer(): roomNo(0),
		days(0),
		cost(0),
		roomType(""),
		checkedOut(false) {}

	void bookRoom(int r, int d,
	              const string & type) {
		roomNo = r;
		days = d;
		roomType = type;
		bookingDateTime = getCurrentDateTime();
		checkedOut = false;

		if (type == "Deluxe") cost = 5000 * d;
		else if (type == "Executive") cost = 7000 * d;
		else if (type == "Presidential") cost = 10000 * d;
	}

	int getRoomNo() const {
		return roomNo;
	}
	string getRoomType() const {
		return roomType;
	}
	long getCost() const {
		return cost;
	}
	int getDays() const {
		return days;
	}
	string getBookingDateTime() const {
		return bookingDateTime;
	}
	bool isCheckedOut() const {
		return checkedOut;
	}
	void setCheckedOut(bool status) {
		checkedOut = status;
	}
	void setBookingDateTime(const string & dt) {
		bookingDateTime = dt;
	}
	void setCost(long c) {
		cost = c;
	}

	void addCost(long extra) {
		cost += extra;
	}

	void displayInfo() const;
	void displayBill(const class Service & s) const;
};

class Service {
private:
	struct FoodOrder {
		string name;
		int quantity;
		long price;
	};
	vector < FoodOrder > orders;

public:
	Service() {}

	void orderFood(Customer & c) {
		int choice, people;
		string input;

		cout << "\033[1m\n#  RESTAURANT  MENU\033[0m\n";
		cout << "1. Breakfast\n";
		cout << "2. Lunch\n";
		cout << "3. Dinner\n";
		cout << "\nEnter your choice: ";
		cin >> input;
		if (input == "#") return;

		try {
			choice = stoi(input);
		} catch (...) {
			cout << "Invalid choice! Please enter a number.\n";
			cin.clear();
			cin.ignore(numeric_limits < streamsize > ::max(), '\n');
			return;
		}

		cout << "Enter number of people: ";
		cin >> input;
		if (input == "#") return;

		try {
			people = stoi(input);
		} catch (...) {
			cout << "Invalid number! Please enter a valid number.\n";
			cin.clear();
			cin.ignore(numeric_limits < streamsize > ::max(), '\n');
			return;
		}

		if (people <= 0) {
			cout << "Invalid number of people!\n";
			return;
		}

		string foodName;
		long amt = 0;

		switch (choice) {
		case 1:
			foodName = "Breakfast";
			amt = 500 * people;
			break;
		case 2:
			foodName = "Lunch";
			amt = 1000 * people;
			break;
		case 3:
			foodName = "Dinner";
			amt = 1200 * people;
			break;
		default:
			cout << "Invalid choice!\n";
			return;
		}

		orders.push_back({
			foodName,
			people,
			amt
		});
		c.addCost(amt);

		cout << "\nOrdered " << people << "x " << foodName;
		cout << ".\nAmount added to the bill: Rs. " << amt << "\n";
		cout << "Service is on the way, please wait...\n";
	}

	long getServiceCost() const {
		long total = 0;
		for (auto & o: orders) total += o.price;
		return total;
	}

	void displayOrders() const {
		if (orders.empty()) {
			cout << "\033[1m\n#  FOOD ORDERS\033[0m\n";
			cout << "No food orders.\n\n";
			return;
		}
		cout << "\033[1m\n#  FOOD  ORDERS\033[0m\n";
		for (auto & o: orders) {
			cout << o.quantity << "x " << o.name << " (Rs. " << o.price << ")\n";
		}
	}

	string serializeOrders() const {
		stringstream ss;
		for (auto & o: orders) {
			ss << o.name << "," << o.quantity << "," << o.price << ";";
		}
		return ss.str();
	}

	void deserializeOrders(const string & data) {
		orders.clear();
		if (data.empty()) return;

		stringstream ss(data);
		string orderStr;
		while (getline(ss, orderStr, ';')) {
			if (orderStr.empty()) continue;

			stringstream orderSs(orderStr);
			string name, qtyStr, priceStr;

			getline(orderSs, name, ',');
			getline(orderSs, qtyStr, ',');
			getline(orderSs, priceStr, ',');

			if (!name.empty()) {
				try {
					orders.push_back({
						name,
						stoi(qtyStr),
						stol(priceStr)
					});
				} catch (...) {}
			}
		}
	}
};

void Customer::displayInfo() const {
	cout << "\033[1m\n----- GUEST DETAILS -----\033[0m\n";
	cout << "Room Number : " << roomNo << "\n";
	cout << "Guest Name  : " << name << "\n";
	cout << "Address     : " << address << "\n";
	cout << "Contact     : " << phone << "\n";
	cout << "Staying     : " << days << " days\n";
	cout << "Room Type   : " << roomType << "\n";
	cout << "Booking     : " << bookingDateTime << "\n";
	cout << "Status      : " << (checkedOut ? "Checked Out" : "Checked In") << "\n";
}

void Customer::displayBill(const Service & s) const {
	cout << "\033[1m\n\n#  BILLING  &  CHECK-OUT\033[0m\n";
	cout << "Room Number  : " << roomNo << "\n";
	cout << "Guest Name   : " << name << "\n";
	cout << "Room Type    : " << roomType << "\n";
	cout << "Staying Days : " << days << "\n";
	cout << "Booking Date : " << bookingDateTime << "\n";
	cout << "Status       : " << (checkedOut ? "Checked Out" : "Checked In") << "\n";

	s.displayOrders();

	long roomCost = cost;
	long serviceCost = s.getServiceCost();
	long totalCost = roomCost + serviceCost;
	double tax = totalCost * 0.05;
	double finalAmount = totalCost + tax;

	cout << fixed << setprecision(2);
	cout << "\nRoom Charges    : Rs. " << roomCost << "\n";
	cout << "Service Charges : Rs. " << serviceCost << "\n";
	cout << "Subtotal        : Rs. " << totalCost << "\n";
	cout << "Tax (5%)        : Rs. " << tax << "\n";
	cout << "-------------------------------\n";
	cout << "\033[1mTOTAL AMOUNT    : Rs. " << finalAmount << "\033[0m\n\n";
}

class Booking {
private:
	Customer customer;
	Service service;

public:
	Booking() {}

	Customer & getCustomer() {
		return customer;
	}
	const Customer & getCustomer() const {
		return customer;
	}
	Service & getService() {
		return service;
	}
	const Service & getService() const {
		return service;
	}

	void book(Customer & c) {
		customer = c;
	}

	string serialize() const {
		stringstream ss;
		ss << customer.getRoomNo() << "|" <<
		   customer.getName() << "|" <<
		   customer.getAddress() << "|" <<
		   customer.getPhone() << "|" <<
		   customer.getDays() << "|" <<
		   customer.getRoomType() << "|" <<
		   customer.getCost() << "|" <<
		   customer.getBookingDateTime() << "|" <<
		   (customer.isCheckedOut() ? "1" : "0") << "|" <<
		   service.serializeOrders();
		return ss.str();
	}

	bool deserialize(const string & data) {
		try {
			stringstream ss(data);
			vector < string > tokens;
			string token;

			while (getline(ss, token, '|')) {
				tokens.push_back(token);
			}

			if (tokens.size() >= 10) {
				int roomNo = stoi(tokens[0]);
				string name = tokens[1];
				string address = tokens[2];
				string phone = tokens[3];
				int days = stoi(tokens[4]);
				string roomType = tokens[5];
				long cost = stol(tokens[6]);
				string bookingDateTime = tokens[7];
				bool checkedOut = (tokens[8] == "1");
				string ordersData = tokens[9];

				customer.setDetails(name, address, phone);
				customer.bookRoom(roomNo, days, roomType);
				customer.setBookingDateTime(bookingDateTime);
				customer.setCheckedOut(checkedOut);
				customer.setCost(cost);

				service.deserializeOrders(ordersData);

				return true;
			}
		} catch (const exception & e) {
			return false;
		}
		return false;
	}
};

class Hotel {
private:
	const int totalRooms = 20;
	const string filename = "data.txt";
	vector < Booking > allBookings;

	string roomType(int r) {
		if (r >= 1 && r <= 9) return "Deluxe";
		if (r >= 10 && r <= 15) return "Executive";
		if (r >= 16 && r <= 20) return "Presidential";
		return "";
	}

	bool loadBookingsFromFile() {
		allBookings.clear();
		ifstream file(filename);
		if (!file.is_open()) {
			return true;
		}

		string line;
		while (getline(file, line)) {
			if (!line.empty()) {
				Booking b;
				if (b.deserialize(line)) {
					allBookings.push_back(b);
				}
			}
		}
		file.close();
		return true;
	}

	bool saveBookingsToFile() {
		ofstream file(filename, ios::trunc);
		if (!file.is_open()) {
			return false;
		}

		for (const auto & booking: allBookings) {
			file << booking.serialize() << "\n";
		}
		file.close();
		return true;
	}

	Booking * findBooking(int r) {
		for (auto & booking: allBookings) {
			if (booking.getCustomer().getRoomNo() == r && !booking.getCustomer().isCheckedOut()) {
				return & booking;
			}
		}
		return nullptr;
	}

	bool isValidName(const string & n) {
		if (n.empty() || n.length() > 50) return false;
		for (char c: n)
			if (!isalpha(c) && c != ' ') return false;
		return true;
	}

	bool isValidPhone(const string & p) {
		if (p.empty() || p.length() < 7 || p.length() > 15) return false;
		for (char c: p)
			if (!isdigit(c)) return false;
		return true;
	}

	void displayRoomStatus() {
		vector < int > deluxe, executive, presidential;

		for (auto & b: allBookings) {
			if (!b.getCustomer().isCheckedOut()) {
				int room = b.getCustomer().getRoomNo();
				if (room >= 1 && room <= 9) deluxe.push_back(room);
				else if (room >= 10 && room <= 15) executive.push_back(room);
				else if (room >= 16 && room <= 20) presidential.push_back(room);
			}
		}

		sort(deluxe.begin(), deluxe.end());
		sort(executive.begin(), executive.end());
		sort(presidential.begin(), presidential.end());

		cout << "\n Room No.   | Room Type     | Booked Rooms\n";
		cout << "------------|---------------|-------------\n";

		cout << "  1  -  9   | Deluxe        | ";
		for (size_t i = 0; i < deluxe.size(); ++i) {
			cout << deluxe[i];
			if (i != deluxe.size() - 1) cout << ", ";
		}
		if (deluxe.empty()) cout << "None";
		cout << "\n";

		cout << " 10  -  15  | Executive     | ";
		for (size_t i = 0; i < executive.size(); ++i) {
			cout << executive[i];
			if (i != executive.size() - 1) cout << ", ";
		}
		if (executive.empty()) cout << "None";
		cout << "\n";

		cout << " 16  -  20  | Presidential  | ";
		for (size_t i = 0; i < presidential.size(); ++i) {
			cout << presidential[i];
			if (i != presidential.size() - 1) cout << ", ";
		}
		if (presidential.empty()) cout << "None";
		cout << "\n\n";
	}

	vector < Booking > getAllActiveBookings() {
		vector < Booking > activeBookings;
		for (auto & b: allBookings) {
			if (!b.getCustomer().isCheckedOut()) {
				activeBookings.push_back(b);
			}
		}
		return activeBookings;
	}

public:
	Hotel() {
		loadBookingsFromFile();
	}

	void showPricing();

	void mainMenu() {
		int choice;
		string input;

		do {
			clearScreen();
			cout << "\033[1;32m\n***WELCOME TO HOTEL KASTURI***\n\n\033[0m";
			cout << "\033[1m----- MAIN MENU -----\033[0m\n";
			cout << "1. Book A Room\n";
			cout << "2. Guest Details\n";
			cout << "3. Rooms Allotted\n";
			cout << "4. Order Food\n";
			cout << "5. Billing & CheckOut\n";
			cout << "6. Pricing\n";
			cout << "7. Exit\n";
			cout << "\nEnter Choice: ";

			cin >> input;
			if (input == "#") continue;

			try {
				choice = stoi(input);
			} catch (...) {
				cout << "\nInvalid choice! Please enter a number (1-6).\n";
				cout << "\nPress any key to continue...";
				cin.ignore();
				cin.get();
				continue;
			}

			switch (choice) {
			case 1:
				bookRoom();
				break;
			case 2:
				customerInfo();
				break;
			case 3:
				roomsAllotted();
				break;
			case 4:
				orderFood();
				break;
			case 5:
				billingAndCheckout();
				break;
			case 6:
				showPricing();
				break;

			case 7:
				saveBookingsToFile();
				cout << "\n\n\033[1mThank you for visiting Hotel Kasturi!\033[0m\n";
				break;

			default:
				cout << "\nInvalid choice! Please enter a number between 1-7.\n";
				cout << "\nPress any key to continue...";
				cin.ignore();
				cin.get();
				break;
			}
		} while (choice != 7);
	}

	void bookRoom() {
		clearScreen();
		displayRoomStatus();

		string input;
		int r, days;
		string n, a, p;

		while (true) {
			cout << "Enter room number you want to book: ";
			cin >> input;
			if (input == "#") return;

			try {
				r = stoi(input);
			} catch (...) {
				cout << "Invalid room number! Please enter a number.\n";
				continue;
			}

			if (r < 1 || r > 20) {
				cout << "\nSorry, room numbers are only from 1 to 20.\n\n";
				continue;
			}

			Booking * existing = findBooking(r);
			if (existing) {
				if (existing -> getCustomer().isCheckedOut()) {
					for (auto it = allBookings.begin(); it != allBookings.end(); ++it) {
						if (it -> getCustomer().getRoomNo() == r) {
							allBookings.erase(it);
							break;
						}
					}
					cout << "Room was previously occupied. Available for booking.\n";
					break;
				} else {
					cout << "Sorry, room is already booked! Please choose another one.\n";
					continue;
				}
			}
			break;
		}

		cin.ignore();
		cout << "\033[1m\n# PROVIDE YOUR DETAILS\033[0m\n";

		while (true) {
			cout << "Name: ";
			getline(cin, n);
			if (n == "#") return;
			if (isValidName(n)) break;
			cout << "Invalid name. Use only letters and spaces (max 50 characters).\n";
		}

		while (true) {
			cout << "Address: ";
			getline(cin, a);
			if (a == "#") return;
			if (!a.empty() && a.length() <= 100) break;
			cout << "Address cannot be empty or exceed 100 characters.\n";
		}

		while (true) {
			cout << "Contact: ";
			getline(cin, p);
			if (p == "#") return;
			if (isValidPhone(p)) break;
			cout << "Invalid phone. Use 7-15 digits only.\n";
		}

		while (true) {
			cout << "Staying days: ";
			getline(cin, input);
			if (input == "#") return;

			try {
				days = stoi(input);
				if (days > 0 && days <= 365) break;
				cout << "Please enter days between 1 and 365.\n";
			} catch (...) {
				cout << "Invalid number. Please enter a valid number.\n";
			}
		}

		Customer c;
		c.setDetails(n, a, p);
		c.bookRoom(r, days, roomType(r));

		Booking b;
		b.book(c);
		allBookings.push_back(b);

		if (saveBookingsToFile()) {
			cout << "\nRoom has been booked successfully!\n";
			cout << "Checked In: " << c.getBookingDateTime() << "\n";
		} else {
			cout << "\nError saving booking to file!\n";
		}

		cout << "\nPress any key to continue...";
		cin.get();
	}

	void customerInfo() {
		clearScreen();
		string input;
		cout << "\nEnter Room Number: ";
		cin >> input;
		if (input == "#") return;

		int r;
		try {
			r = stoi(input);
		} catch (...) {
			cout << "Invalid room number!\n";
			cout << "\nPress any key to continue...";
			cin.ignore();
			cin.get();
			return;
		}

		if (r < 1 || r > 20) {
			cout << "\nRoom numbers are only from 1 to 20.\n\n";
			cout << "\nPress any key to continue...";
			cin.ignore();
			cin.get();
			return;
		}

		Booking * b = findBooking(r);
		if (!b) {
			bool foundCheckedOut = false;
			for (auto & booking: allBookings) {
				if (booking.getCustomer().getRoomNo() == r) {
					foundCheckedOut = true;
					cout << "\nRoom is vacant and available for booking.\n";
					break;
				}
			}

			if (!foundCheckedOut) {
				cout << "\nRoom is vacant and available for booking.\n";
			}
		} else {
			b -> getCustomer().displayInfo();
			b -> getService().displayOrders();
		}

		cout << "\nPress any key to continue...";
		cin.ignore();
		cin.get();
	}

	void roomsAllotted() {
		clearScreen();
		vector < Booking > activeBookings = getAllActiveBookings();

		if (activeBookings.empty()) {
			cout << "\033[1m\n#  ALLOTTED  ROOMS\033[0m\n";
			cout << "No rooms currently allotted.\n";
		} else {
			cout << "\033[1m\n#  ALLOTTED  ROOMS\033[0m\n\n";
			cout << "---------------------------------------------------------------------------------------------------------\n";

			cout << left <<
			     setw(10) << "Room No." <<
			     setw(20) << "Guest Name" <<
			     setw(25) << "Address" <<
			     setw(15) << "Room Type" <<
			     setw(20) << "Checked In" <<
			     setw(15) << "Contact No." << "\n";

			cout << setfill('-') << setw(105) << "" << setfill(' ') << "\n";

			for (auto & b: activeBookings) {
				cout << left <<
				     setw(10) << b.getCustomer().getRoomNo() <<
				     setw(20) << b.getCustomer().getName() <<
				     setw(25) << b.getCustomer().getAddress() <<
				     setw(15) << b.getCustomer().getRoomType() <<
				     setw(20) << b.getCustomer().getBookingDateTime() <<
				     setw(15) << b.getCustomer().getPhone() << "\n";
			}

			cout << "\n\nTotal Rooms Allotted: " << activeBookings.size() << " out of 20\n";
		}

		cout << "\nPress any key to continue...";
		cin.ignore();
		cin.get();
	}

	void orderFood() {
		clearScreen();
		string input;
		cout << "\nEnter Room Number: ";
		cin >> input;
		if (input == "#") return;

		int r;
		try {
			r = stoi(input);
		} catch (...) {
			cout << "Invalid room number!\n";
			cout << "\nPress any key to continue...";
			cin.ignore();
			cin.get();
			return;
		}

		Booking * b = findBooking(r);
		if (!b) {
			cout << "\nRoom is vacant and available for booking.\n";
		} else {
			cout << "\nPlace your order for Room " << r << "\n";
			b -> getService().orderFood(b -> getCustomer());

			if (saveBookingsToFile()) {
				cout << "Order saved successfully!\n";
			} else {
				cout << "\nError saving order to file!\n";
			}
		}

		cout << "\nPress any key to continue...";
		cin.ignore();
		cin.get();
	}

	void billingAndCheckout() {
		clearScreen();
		string input;
		cout << "\nEnter Room Number for Billing & Check-Out: ";
		cin >> input;
		if (input == "#") return;

		int r;
		try {
			r = stoi(input);
		} catch (...) {
			cout << "Invalid room number!\n";
			cout << "\nPress any key to continue...";
			cin.ignore();
			cin.get();
			return;
		}

		Booking * b = findBooking(r);
		if (!b) {
			bool found = false;
			for (auto & booking: allBookings) {
				if (booking.getCustomer().getRoomNo() == r && booking.getCustomer().isCheckedOut()) {
					found = true;
					cout << "\nRoom is vacant and available for booking.\n";
					break;
				}
			}

			if (!found) {
				cout << "\nRoom is Vacant and available for booking.\n";
			}
		} else {
			b -> getCustomer().displayBill(b -> getService());

			cout << "Do you want to check out? (y/n): ";
			cin >> input;

			if (input == "y" || input == "Y") {
				b -> getCustomer().setCheckedOut(true);

				if (saveBookingsToFile()) {
					cout << "\nCheckout successful for Room No. " << r << "\n";
					cout << "Thank you for staying at Hotel Kasturi!\n";
				} else {
					cout << "\nError saving checkout to file!\n";
				}
			} else {
				cout << "\nCheckout cancelled.\n";
			}
		}

		cout << "\nPress any key to continue...";
		cin.ignore();
		cin.get();
	}
};

void Hotel::showPricing() {
	clearScreen();
	cout << "\n\033[1;32m***HOTEL KASTURI - PRICING***\033[0m\n\n";
	cout << "\033[1m#  ROOM TARIFF (PER DAY)\033[0m\n";
	cout << "1. Deluxe Room        : Rs. 5000\n";
	cout << "2. Executive Room     : Rs. 7000\n";
	cout << "3. Presidential Room  : Rs. 10000\n\n";
	cout << "\033[1m#  FOOD & SERVICES (PER PERSON)\033[0m\n";
	cout << "1. Breakfast  : Rs. 500\n";
	cout << "2. Lunch      : Rs. 1000\n";
	cout << "3. Dinner     : Rs. 1200\n\n";
	cout << "\033[1m#  TAX CHARGES\033[0m\n";
	cout << "-  Service Tax : 5%\n\n";
	cout << "Press any key to continue...";
	cin.ignore();
	cin.get();
}

int main() {
	Hotel h;
	h.mainMenu();
	return 0;
}
