#include <iostream>
#include <windows.h> // для доступу до вікна / to access the window

#include <thread> // для затримки часу / for time delaying

#include <random>

#include <fstream> // для ofstream та ifstream / for ofstream and ifstream
#include <sstream> // для stringstream / for stringstream
#include <string>
#include <map>
#include <conio.h> // для _getch() / for _getch()

#include <iomanip> // для std::setw() / for std::setw()

struct User 
{
    std::string login;
    std::string password;
    int balance;
};

std::string currentUserLogin;

void gameOption(std::map<std::string, User>& users); // прототип функції gameOption() / gameOption() function prototype

// Функція для виведення поля 
// Function for field output
void NumField()
{
    for (int i = 1; i <= 30; i++)
    {
        std::cout << std::setw(2) << i << " ";

        if (i % 10 == 0)
            std::cout << "|" << std::endl;
    }
}

// Функція для зміни кольору поля з обраним числом
// Function for changing the color of the field with the selected number
void PaintNum(int &number)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position{};
    position.X = 0;
    position.Y = 0;

    if (number < 10)
        position.X = 3 * number - 2;

    else if (number == 10)
        position.X = 27;

    else if (number > 10 && number <= 20)
    {
        position.X = 3 * number - 33;
        position.Y = 1;
    }

    else if (number > 20 && number <= 30)
    {
        position.X = 3 * number - 63;
        position.Y = 2;
    }

    SetConsoleCursorPosition(hConsole, position);
    SetConsoleTextAttribute(hConsole, BACKGROUND_RED | FOREGROUND_INTENSITY);
    std::cout << number;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

// Функція для виведення введеного Вами числа
// A function for outputting the number you entered
void PositionAfterInputNumber(int &number)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position{};
    position.X = 0;
    position.Y = 3;
    SetConsoleCursorPosition(hConsole, position);

    if (number >= 1 && number <= 30)
        std::cout << "Your number: " << number << "                  " << std::endl;

    else
        std::cout << "It is not a number from 1 to 30. Your number: " << number << "   " << std::endl;
}

// Функція для збереження логіна та пароля
// Function for saving login and password
void saveToFile(const std::map<std::string, User>& users, const std::string& filename)
{
    std::ofstream file(filename);

    if (file.is_open()) 
    {
        for (const auto& pair : users)
            file << pair.second.login << "," << pair.second.password << "," << pair.second.balance << std::endl;
 
        file.close();
    }
    else 
        std::cerr << "Error of opening." << std::endl;
}

// Функція для реєстрації гравця
// Player registration function
void registerUser(std::map<std::string, User>& users)
{
    User newUser;

    std::cout << "Enter new login: ";
    std::cin >> newUser.login;

    if (users.find(newUser.login) != users.end())
    {
        std::cerr << "There is a user with this login. Choose another login." << std::endl;
        return;
    }

    std::cout << "Enter password: ";

    char ch;
    while ((ch = _getch()) != '\r') // '\r' - Enter
    {
        if (ch == '\b') // '\b' - Backspace
        {
            if (!newUser.password.empty())
            {
                newUser.password.pop_back();
                std::cout << "\b \b";
            }
        }
        else
        {
            newUser.password.push_back(ch);
            std::cout << '*';
        }
    }

    newUser.balance = 1000; // Початковий баланс для гри / Starting balance for the game

    users[newUser.login] = newUser;
    std::cout << "\nNew user registered!" << std::endl;
}

// Функція для входу користувача
// Function for user login
void loginUser(const std::map<std::string, User>& users)
{
    std::string login, password;

    bool validLogin = false;

    do {
        std::cout << "Enter login: ";
        std::cin >> login;

        if (users.find(login) == users.end())
        {
            std::cerr << "User with this login is not found. Try again" << std::endl;
            std::cout << "Wait 2 seconds";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            system("cls");
        }
        else
        {
            validLogin = true;  // Логін знайдено, можемо вийти з циклу / The login is found, we can exit the loop
        }
    } while (!validLogin);

    // Тепер введений правильний логін, можемо приступити до вводу паролю
    // The correct login has been entered, we can start entering the password

    currentUserLogin = login;

    bool ifCorrectPassword = false;

    std::cout << "Enter password: ";

    do {
        password.clear();

        char ch;
        while ((ch = _getch()) != '\r') // '\r' - Enter
        {
            if (ch == '\b') // '\b' - Backspace
            {
                if (!password.empty())
                {
                    password.pop_back();
                    std::cout << "\b \b";
                }
            }
            else
            {
                password.push_back(ch);
                std::cout << '*';
            }
        }

        if (users.find(login)->second.password == password)
        {
            std::cout << "\nYou successfully logged in. Your balance: " << users.find(login)->second.balance << " UAH" << std::endl;
            ifCorrectPassword = true;
        }
        else
        {
            std::cerr << "\nWrong password. Try again" << std::endl;
            std::cout << "Wait 2 seconds";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            system("cls");
            std::cout << "Enter login: " << login << std::endl;
            std::cout << "Enter password: ";
        }
    } while (!ifCorrectPassword);
}

// Функція для завантаження даних з файлу
// Function for downloading data from a file
auto loadFromFile(std::map<std::string, User>& users, const std::string& filename)
{
    std::ifstream file(filename);

    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            std::stringstream ss(line);
            User user;
            getline(ss, user.login, ',');
            getline(ss, user.password, ',');
            ss >> user.balance;
            users[user.login] = user;
        }
        file.close();
    }
    else
    {
        std::cerr << "Error opening file for reading." << std::endl;
        return -1;
    }
}

// Функція для зміни балансу гравця (для адміна)
// Function for changing player balance (for admin)
void changeBalance(std::map<std::string, User>& users)
{
    std::string login;
    int newBalance;
    std::string inputPassword;

    do {
        std::cout << "Enter login of the user whose balance you want to change: ";
        std::cin >> login;

        if (users.find(login) == users.end())
        {
            std::cerr << "User with this login is not found." << std::endl;
            std::cout << "Wait 2 seconds";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            system("cls");
        }

        else
        {
            break;
        }
    } while (true);

    bool correctPassword = false;

    do {
        std::cout << "Enter the password for verification: ";

        char ch;
        while ((ch = _getch()) != '\r') // '\r' - Enter
        {
            if (ch == '\b') // '\b' - Backspace
            {
                if (!inputPassword.empty()) 
                {
                    inputPassword.pop_back();
                    std::cout << "\b \b";
                }
            }
            else 
            {
                inputPassword.push_back(ch);
                std::cout << '*';
            }
        }

        // Перевірка пароля / Password verification
        if (inputPassword == "15924866")
        {
            correctPassword = true;
        }
        else
        {
            std::cerr << "\nIncorrect password. Try again." << std::endl;
            std::cout << "Wait 2 seconds";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            system("cls");
            inputPassword.clear();
            std::cout << "Enter login of the user whose balance you want to change: " << login << std::endl;
        }
    } while (!correctPassword);

    std::cout << "\nEnter the new balance: ";
    std::cin >> newBalance;

    users.find(login)->second.balance = newBalance;
    std::cout << "Balance changed successfully for user " << login << ". New balance: " << newBalance << " UAH\n" << std::endl;
    std::cout << "Wait 5 seconds";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    system("cls");
}

// Функція для гри
// Game function
void Game1(std::map<std::string, User>& users)
{
    auto it = users.find(currentUserLogin);

    while (it != users.end() && it->second.balance > 0)
    {
        system("cls");
        NumField();
        int number;
        std::cout << "Guess number (1-30): ";
        std::cin >> number;

        PositionAfterInputNumber(number);
        PaintNum(number);
        PositionAfterInputNumber(number);

        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> range(1, 30);

        int random_number = range(gen);
        std::cout << "Random number: " << random_number;

        if (number == random_number)
        {
            it->second.balance += 500;
            std::cout << "\nCongratulations! You guessed the number. You earned 500 UAH. Your new balance: " << it->second.balance << " UAH" << std::endl;
        }
        else
        {
            it->second.balance -= 100;
            std::cout << "\nSorry, you didn't guess the number. You lost 100 UAH. Your new balance: " << it->second.balance << " UAH" << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Press 1 to continue the game or any other key to stop the game: ";
        char key = _getch();

        if (key == '1')
        {
            saveToFile(users, "users.txt");
            continue;
        }

        else
        {
            saveToFile(users, "users.txt");
            gameOption(users);
            break;
        } 
    }
    saveToFile(users, "users.txt");

    if (it->second.balance == 0)
    {
        system("cls");
        std::cout << "Sorry! But you lost all your money. Thank you for the game! \nIf you want to start again, ask Admin to change your balance.\nBest wishes!\n\n";
    }
}

// Функція для виведення режимів гри
// Function for outputing game modes
void gameOption(std::map<std::string, User>& users)
{
    std::cout << "\nWait 2 seconds";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    system("cls");

    std::cout << "Choose option: ";
    std::cout << "\n1. Play game";
    std::cout << "\n2. Main menu";
    std::cout << "\n3. Exit\n";

    char newoper = _getch();

    switch (newoper)
    {
    case '1':
        Game1(users);
        break;
    case '2':
        system("cls");
        break;
    case '3':
        exit(EXIT_FAILURE);
        break;
    default: 
        std::cerr << "Invalid option." << std::endl;
        gameOption(users);
        break;
    }
}

// Функція для виведення режимів програми
// Function for outputting program modes
void displayMenu()
{
    std::cout << "Welcome to our Roulette! Choose option:\n" << std::endl;
    std::cout << "1. Registration" << std::endl;
    std::cout << "2. Entrance" << std::endl;
    std::cout << "3. Change balance (for admin)" << std::endl;
    std::cout << "4. Exit" << std::endl;  // Додаємо опцію для виходу
}

int main()
{
    std::map<std::string, User> users;
    loadFromFile(users, "users.txt");

    bool running = true;

    while (running) 
    {
        displayMenu();

        char oper = _getch();

        switch (oper)
        {
        case '1':
            system("cls");
            registerUser(users);
            saveToFile(users, "users.txt");
            std::cout << "Wait 2 seconds";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            system("cls");
            break;
        case '2':
            system("cls");
            loginUser(users);
            gameOption(users);
            break;
        case '3':
            system("cls");
            changeBalance(users);
            saveToFile(users, "users.txt");
            break;
        case '4':
            running = false;
            break;
        default:
            std::cerr << "Invalid option." << std::endl;
            std::cout << "Wait 2 seconds";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            system("cls");
            break;
        }
    }

    return 0;
}