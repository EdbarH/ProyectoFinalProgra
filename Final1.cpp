#include <mysql.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Database {
private:
    MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;

    string intToString(int value) {
        ostringstream oss;
        oss << value;
        return oss.str();
    }

public:
    Database() {
        conn = mysql_init(nullptr);
        if (conn == nullptr) {
            cerr << "\033[31m" << "mysql_init() failed\n" << "\033[0m";
            exit(EXIT_FAILURE);
        }

        cout << "\033[33m" << "Connecting to database..." << "\033[0m" << endl;

        if (mysql_real_connect(conn, "127.0.0.1", "root", "Hhoil_132", "Basevuelos", 3306, nullptr, 0) == nullptr) {
            cerr << "\033[31m" << "mysql_real_connect() failed\n" << "Error: " << mysql_error(conn) << "\033[0m" << endl;
            mysql_close(conn);
            exit(EXIT_FAILURE);
        }
        else {
            cout << "\033[32m" << "Successfully connected to database!" << "\033[0m" << endl;
        }
    }

    ~Database() {
        if (res != nullptr) {
            mysql_free_result(res);
        }
        mysql_close(conn);
    }

    void createFlight(const string& flight_number, const string& departure, const string& arrival, const string& departure_time, const string& arrival_time) {
        string query = "INSERT INTO flights (flight_number, departure, arrival, departure_time, arrival_time) VALUES ('" + flight_number + "', '" + departure + "', '" + arrival + "', '" + departure_time + "', '" + arrival_time + "')";
        if (mysql_query(conn, query.c_str())) {
            cerr << "\033[31m" << "INSERT flight failed. Error: " << mysql_error(conn) << "\033[0m" << endl;
        }
        else {
            cout << "\033[32m" << "Flight created successfully!" << "\033[0m" << endl;
        }
    }

    void readFlights() {
        if (mysql_query(conn, "SELECT * FROM flights")) {
            cerr << "\033[31m" << "SELECT * failed. Error: " << mysql_error(conn) << "\033[0m" << endl;
            return;
        }

        res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "\033[31m" << "mysql_store_result() failed. Error: " << mysql_error(conn) << "\033[0m" << endl;
            return;
        }

        int num_fields = mysql_num_fields(res);
        cout << "\033[34m" << "Flights in the database:" << "\033[0m" << endl;
        while ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < num_fields; i++) {
                cout << (row[i] ? row[i] : "NULL") << " ";
            }
            cout << endl;
        }
    }

    void createPassenger(const string& name, const string& ticket, const string& seat, int flight_id) {
        string query = "INSERT INTO passengers (name, ticket_number, seat_number, flight_id) VALUES ('" + name + "', '" + ticket + "', '" + seat + "', " + intToString(flight_id) + ")";
        if (mysql_query(conn, query.c_str())) {
            cerr << "\033[31m" << "INSERT passenger failed. Error: " << mysql_error(conn) << "\033[0m" << endl;
        }
        else {
            cout << "\033[32m" << "Passenger created successfully!" << "\033[0m" << endl;
        }
    }

    void readPassengers() {
        if (mysql_query(conn, "SELECT * FROM passengers")) {
            cerr << "\033[31m" << "SELECT * failed. Error: " << mysql_error(conn) << "\033[0m" << endl;
            return;
        }

        res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "\033[31m" << "mysql_store_result() failed. Error: " << mysql_error(conn) << "\033[0m" << endl;
            return;
        }

        int num_fields = mysql_num_fields(res);
        cout << "\033[34m" << "Passengers in the database:" << "\033[0m" << endl;
        while ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < num_fields; i++) {
                cout << (row[i] ? row[i] : "NULL") << " ";
            }
            cout << endl;
        }
    }

    void updatePassenger(int id, const string& name, const string& ticket, const string& seat) {
        string query = "UPDATE passengers SET name='" + name + "', ticket_number='" + ticket + "', seat_number='" + seat + "' WHERE id=" + intToString(id);
        if (mysql_query(conn, query.c_str())) {
            cerr << "\033[31m" << "UPDATE failed. Error: " << mysql_error(conn) << "\033[0m" << endl;
        }
        else {
            cout << "\033[32m" << "Passenger updated successfully!" << "\033[0m" << endl;
        }
    }

    void deletePassenger(int id) {
        string query = "DELETE FROM passengers WHERE id=" + intToString(id);
        if (mysql_query(conn, query.c_str())) {
            cerr << "\033[31m" << "DELETE failed. Error: " << mysql_error(conn) << "\033[0m" << endl;
        }
        else {
            cout << "\033[32m" << "Passenger deleted successfully!" << "\033[0m" << endl;
        }
    }

    void readPassengersByFlight(int flight_id) {
        string query = "SELECT * FROM passengers WHERE flight_id=" + intToString(flight_id);
        if (mysql_query(conn, query.c_str())) {
            cerr << "\033[31m" << "SELECT * failed. Error: " << mysql_error(conn) << "\033[0m" << endl;
            return;
        }

        res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "\033[31m" << "mysql_store_result() failed. Error: " << mysql_error(conn) << "\033[0m" << endl;
            return;
        }

        int num_fields = mysql_num_fields(res);
        cout << "\033[34m" << "Passengers for flight ID " << flight_id << ":" << "\033[0m" << endl;
        while ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < num_fields; i++) {
                cout << (row[i] ? row[i] : "NULL") << " ";
            }
            cout << endl;
        }
    }
};

void showMenu() {
    cout << "\033[35m" << "Menu:" << "\033[0m" << endl;
    cout << "1. Crear vuelo" << endl;
    cout << "2. Crear nueva reservacion" << endl;
    cout << "3. Eliminar reservacion" << endl;
    cout << "4. Ver vuelos" << endl;
    cout << "5. Ver reservaciones" << endl;
    cout << "6. Salir" << endl;
}

int main() {
    try {
        Database db;
        int choice;

        while (true) {
            showMenu();
            cout << "Seleccione una opcion: ";
            cin >> choice;

            if (choice == 1) {
                string flight_number, departure, arrival, departure_time, arrival_time;
                cout << "Ingrese numero de vuelo: ";
                cin.ignore();
                getline(cin, flight_number);
                cout << "Ingrese ciudad de salida: ";
                getline(cin, departure);
                cout << "Ingrese ciudad de llegada: ";
                getline(cin, arrival);
                cout << "Ingrese hora de salida (HH:MM:SS): ";
                getline(cin, departure_time);
                cout << "Ingrese hora de llegada (HH:MM:SS): ";
                getline(cin, arrival_time);
                db.createFlight(flight_number, departure, arrival, departure_time, arrival_time);
            }
            else if (choice == 2) {
                string name, ticket, seat;
                int flight_id;
                cout << "Ingrese nombre del pasajero: ";
                cin.ignore();
                getline(cin, name);
                cout << "Ingrese numero de ticket: ";
                getline(cin, ticket);
                cout << "Ingrese numero de asiento: ";
                getline(cin, seat);
                cout << "Ingrese ID del vuelo: ";
                cin >> flight_id;
                db.createPassenger(name, ticket, seat, flight_id);
            }
            else if (choice == 3) {
                int id;
                cout << "Ingrese ID de la reservacion a eliminar: ";
                cin >> id;
                db.deletePassenger(id);
            }
            else if (choice == 4) {
                db.readFlights();
            }
            else if (choice == 5) {
                db.readPassengers();
            }
            else if (choice == 6) {
                break;
            }
            else {
                cout << "\033[31m" << "Opcion invalida, por favor intente de nuevo." << "\033[0m" << endl;
            }
        }

    }
    catch (const exception& e) {
        cerr << "\033[31m" << "Exception: " << e.what() << "\033[0m" << endl;
    }

    return 0;
}
