#include <iostream>
#include <string>


using namespace std;

class Exception : public exception {
    string _message;
    string _file;
    string _date;
    string _time;
    int _line;

public:
    Exception(const string& message, const string& file, const string& date, const string& time, int line)
        : exception(message.c_str()), _message(message), _file(file), _date(date), _time(time), _line(line)
    {
    }

    const string& getMessage() const {
        return _message;
    }

    const string& getFile() const {
        return _file;
    }

    const string& getDate() const {
        return _date;
    }

    const string& getTime() const {
        return _time;
    }

    int getLine() const {
        return _line;
    }

    const char* what() const noexcept override {
        return _message.c_str();
    }
};

class DatabaseException : public Exception {
public:
    DatabaseException(const string& message, const string& file, const string& date, const string& time, int line)
        : Exception(message, file, date, time, line)
    {
    }
};

class InvalidArgumentException : public Exception {
public:
    InvalidArgumentException(const string& message, const string& file, const string& date, const string& time, int line)
        : Exception(message, file, date, time, line)
    {
    }
};

class User {
    int id;
    string username;
    string password;
    string name;
    string surname;

public:
    User() = default;
    User(const string& _username, const string& _password, const string& _name, const string& _surname)
        : username(_username), password(_password), name(_name), surname(_surname)
    {
    }

    void setName(const string& _name) {
        if (_name.length() < 3) {
            throw InvalidArgumentException("Name minimum 3 simvol olmalidir", __FILE__, __DATE__, __TIME__, __LINE__);
        }

        name = _name;
    }

    void setSurname(const string& _surname) {
        if (_surname.length() < 4) {
            throw InvalidArgumentException("Surname minimum 4 simvol olmalidir.", __FILE__, __DATE__, __TIME__, __LINE__);
        }

        surname = _surname;
    }

    void setUsername(const string& _username) {
        username = _username;
    }

    void setPassword(const string& _password) {
        if (_password.length() < 6) {
            throw InvalidArgumentException("Password minimum 6 simvol olmalidir", __FILE__, __DATE__, __TIME__, __LINE__);
        }

        password = _password;
    }

    int getId() const {
        return id;
    }

    const string& getName() const {
        return name;
    }

    const string& getSurname() const {
        return surname;
    }

    const string& getUsername() const {
        return username;
    }

    const string& getPassword() const {
        return password;
    }

    void show() const {
        cout << "-------------------------" << endl;
        cout << "Id: " << id << endl;
        cout << "Username: " << username << endl;
        cout << "Password: " << password << endl;
        cout << "Name: " << name << endl;
        cout << "Surname: " << surname << endl;
        cout << "-------------------------" << endl;
    }
};

class Database {
    User** users;
    int user_count = 0;

public:
    void addUser(const User& user) {
        User** temp_users = new User * [user_count + 1];

        for (int i = 0; i < user_count; i++) {
            temp_users[i] = users[i];
        }

        temp_users[user_count] = new User(user);
        user_count++;

        delete[] users;
        users = temp_users;
    }

    User& getUserByUsername(const string& username) {
        for (int i = 0; i < user_count; i++) {
            if (users[i]->getUsername() == username) {
                return *users[i];
            }
        }

        throw DatabaseException("User not found.", __FILE__, __DATE__, __TIME__, __LINE__);
    }

    void updateUser(User& olduser, const User& newuser) {
        for (int i = 0; i < user_count; i++) {
            if (users[i] == &olduser) {
                *users[i] = newuser;
                return;
            }
        }

        throw DatabaseException("User not found.", __FILE__, __DATE__, __TIME__, __LINE__);
    }

    void deleteUserById(const int& id) {
        for (int i = 0; i < user_count; i++) {
            if (users[i]->getId() == id) {
                delete users[i];
                users[i] = nullptr;

               
                for (int j = i; j < user_count - 1; j++) {
                    users[j] = users[j + 1];
                }

                user_count--;
                return;
            }
        }

        throw DatabaseException("User not found.", __FILE__, __DATE__, __TIME__, __LINE__);
    }


};

class Registration {
    Database _database;

public:
    Registration(const Database& database)
        : _database(database)
    {
    }

    void signIn(const string& username, const string& password) {
        try {
            User& user = _database.getUserByUsername(username);

            if (password == user.getPassword()) {
                cout << "Sign in successful. Welcome, " << user.getUsername() << "!" << endl;
            }
            else {
                throw InvalidArgumentException("Invalid password.", __FILE__, __DATE__, __TIME__, __LINE__);
            }
        }
        catch (const DatabaseException& ex) {
            cout << "Sign in failed. " << ex.getMessage() << endl;
        }
        catch (const InvalidArgumentException& ex) {
            cout << "Sign in failed. " << ex.getMessage() << endl;
        }
    }

    void signUp(const string& username, const string& password, const string& name, const string& surname) {
        if (_database.getUserByUsername(username).getUsername() == username) {
            throw DatabaseException("User movcuddur", __FILE__, __DATE__, __TIME__, __LINE__);
        }

        if (username.length() < 6) {
            throw InvalidArgumentException("username minimum 6 simvol olmalidir", __FILE__, __DATE__, __TIME__, __LINE__);
        }

        if (islower(username[0])) {
            throw InvalidArgumentException("username boyukle baslamalidir", __FILE__, __DATE__, __TIME__, __LINE__);
        }

        if (password.length() < 6) {
            throw InvalidArgumentException("Password minimum 6 simvol olmalidir", __FILE__, __DATE__, __TIME__, __LINE__);
        }

        if (name.length() < 3) {
            throw InvalidArgumentException("ad minimum 4 simvol olmalidir", __FILE__, __DATE__, __TIME__, __LINE__);
        }

        if (surname.length() < 4) {
            throw InvalidArgumentException("soyad minimum 4 simvol olmalidir.", __FILE__, __DATE__, __TIME__, __LINE__);
        }

        User user(username, password, name, surname);
        _database.addUser(user);

        cout << "Sign up edildi" << endl;
    }
};

class StartUp {
public:
    static void Start() {
        Database db;
        Registration twitter(db);

   
    }
};

int main() {
    StartUp::Start();

    system("pause");
    return 0;
}
