#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

class Date {
private:
    int day, month, year;
    static int existingCount;
    static int totalCreated;

    // Перевірка на високосний рік
    bool isLeapYear(int y) const {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }

    // Кількість днів у місяці для конкретного року
    int daysInMonth(int m, int y) const {
        switch (m) {
        case 2: return isLeapYear(y) ? 29 : 28;
        case 4: case 6: case 9: case 11: return 30;
        default: return 31;
        }
    }

    // Перевірка коректності дати
    void validate() {
        if (year < 1 || month < 1 || month > 12 || day < 1 || day > daysInMonth(month, year)) {
            std::cerr << "Error: Invalid date input (" << day << "." << month << "." << year << ")\n";
            throw std::invalid_argument("Invalid date");
        }
    }

public:
    // Конструктор за замовчуванням і з параметрами
    Date(int d = 1, int m = 1, int y = 2000) : day(d), month(m), year(y) {
        validate();
        ++existingCount;
        ++totalCreated;
    }

    // Конструктор копіювання
    Date(const Date& other) : day(other.day), month(other.month), year(other.year) {
        ++existingCount;
        ++totalCreated;
    }

    // Деструктор
    ~Date() {
        --existingCount;
    }

    // Чи є рік високосним
    bool isLeap() const {
        return isLeapYear(year);
    }

    // Додавання днів до дати
    void addDays(int days) {
        day += days;
        while (day > daysInMonth(month, year)) {
            day -= daysInMonth(month, year);
            month++;
            if (month > 12) {
                month = 1;
                year++;
            }
        }
    }

    // Віднімання днів від дати
    void subtractDays(int days) {
        day -= days;
        while (day < 1) {
            month--;
            if (month < 1) {
                month = 12;
                year--;
            }
            day += daysInMonth(month, year);
        }
    }

    // Оператор різниці між датами
    int operator-(const Date& other) const {
        auto countDays = [this](int d, int m, int y) {
            int days = d;
            for (int i = 1; i < m; ++i) {
                days += daysInMonth(i, y);
            }
            for (int i = 1; i < y; ++i) {
                days += isLeapYear(i) ? 366 : 365;
            }
            return days;
            };
        return countDays(day, month, year) - countDays(other.day, other.month, other.year);
    }

    // Оператори порівняння
    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }

    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    bool operator>(const Date& other) const {
        return other < *this;
    }

    // Префіксний інкремент
    Date& operator++() {
        addDays(1);
        return *this;
    }

    // Постфіксний інкремент
    Date operator++(int) {
        Date temp = *this;
        addDays(1);
        return temp;
    }

    // Префіксний декремент
    Date& operator--() {
        subtractDays(1);
        return *this;
    }

    // Постфіксний декремент
    Date operator--(int) {
        Date temp = *this;
        subtractDays(1);
        return temp;
    }

    // Визначення дня тижня (0 = понеділок)
    int dayOfWeek() const {
        int d = day, m = month, y = year;
        if (m < 3) {
            m += 12;
            y--;
        }
        int w = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
        return (w + 6) % 7; // 0 = понеділок
    }

    // Порядковий номер тижня в році
    int weekNumber() const {
        Date firstJan(1, 1, year);
        int daysPassed = *this - firstJan;
        return daysPassed / 7 + 1;
    }

    // Перетворення дати в рядок
    std::string toString() const {
        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << day << "."
            << std::setw(2) << std::setfill('0') << month << "."
            << year;
        return oss.str();
    }

    // Зчитування дати з рядка
    void fromString(const std::string& str) {
        std::istringstream iss(str);
        char dot1, dot2;
        int d, m, y;
        if (iss >> d >> dot1 >> m >> dot2 >> y && dot1 == '.' && dot2 == '.') {
            day = d; month = m; year = y;
            validate();
        }
        else {
            throw std::invalid_argument("Invalid string format");
        }
    }

    // Ввід дати з консолі
    void input() {
        std::cout << "Enter date (dd mm yyyy): ";
        std::cin >> day >> month >> year;
        validate();
    }

    // Вивід дати на екран
    void print() const {
        std::cout << (day < 10 ? "0" : "") << day << "."
            << (month < 10 ? "0" : "") << month << "."
            << year << "\n";
    }

    // Геттери
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }

    // Сеттери з перевіркою коректності
    void setDay(int d) {
        int oldDay = day;
        day = d;
        try { validate(); }
        catch (...) { day = oldDay; throw; }
    }

    void setMonth(int m) {
        int oldMonth = month;
        month = m;
        try { validate(); }
        catch (...) { month = oldMonth; throw; }
    }

    void setYear(int y) {
        int oldYear = year;
        year = y;
        try { validate(); }
        catch (...) { year = oldYear; throw; }
    }

    // Статичні методи для підрахунку об'єктів
    static int getExistingCount() { return existingCount; }
    static int getTotalCreated() { return totalCreated; }
};

// Ініціалізація статичних змінних
int Date::existingCount = 0;
int Date::totalCreated = 0;

int main() {
    Date date1;
    try {
        date1.input();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    std::cout << "Entered date: ";
    date1.print();

    std::cout << "Leap year? " << (date1.isLeap() ? "Yes" : "No") << "\n";

    date1.addDays(40);
    std::cout << "Date after adding 40 days: ";
    date1.print();

    date1.subtractDays(60);
    std::cout << "Date after subtracting 60 days: ";
    date1.print();

    Date date2(1, 1, 2023);
    std::cout << "Second date: ";
    date2.print();

    std::cout << "Comparison:\n";
    if (date1 == date2) std::cout << "Dates are equal.\n";
    else if (date1 < date2) std::cout << "First date is earlier.\n";
    else std::cout << "First date is later.\n";

    std::cout << "Difference in days: " << (date1 - date2) << "\n";

    std::cout << "Day of week: " << date1.dayOfWeek() << " (0=Monday)\n";
    std::cout << "Week number: " << date1.weekNumber() << "\n";

    std::cout << "Date to string: " << date1.toString() << "\n";
    std::cout << "Total created: " << Date::getTotalCreated() << ", Existing: " << Date::getExistingCount() << "\n";

    return 0;
}
