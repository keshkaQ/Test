#include <iostream>
#include <cmath>     // Для математических функций
#include <stack>     // Для работы со стеком
#include <windows.h> // Для Windows специфичных функций (например, system)
#include <iomanip>   // Для работы с выводом (setprecision)
using namespace std; // Для использования пространств имен std

const double Pi = acos(-1); // Константа для числа Пи

// Функция для округления числа до заданной точности
double roundToPrecision(double value, int precision)
{
    return round(value * pow(10, precision)) / pow(10, precision); // Округление числа
}

// Тригонометрические функции (синус, косинус, котангенс)
double Sin(double x)
{
    return roundToPrecision(sin(x), 7);// Округляем результат синуса
}

double Cos(double x)
{
    return roundToPrecision(cos(x), 7);// Округляем результат косинуса
}

double Ctg(double x)
{
    if (Sin(x) == 0)
    {
        cout << "\nНеверный аргумент для котангенса!\n"; // Проверка на деление на ноль
        return NAN;                                      // Возвращаем нечисловое значение при ошибке
    }
    return roundToPrecision(Cos(x) / Sin(x), 7);         // Округляем результат котангенса
}

// Структура для представления лексем (чисел и операторов)
struct Leksema
{
    char type; // '0' для чисел, другие символы для операций
    double value; // Значение, только для чисел
};

// Функция для выполнения выбранной математической операции
bool Maths(stack<Leksema>& Stack_numbers, stack<Leksema>& Stack_operations, Leksema& item)
{
    if (Stack_numbers.size() < 2) // Проверка на достаточное количество элементов в стеке с числами
    {
        cout << "\nНедостаточно операторов в стеке!\n";
        return false;
    }

    // Извлечение значений из стека
    double a = Stack_numbers.top().value; // Верхний элемент стека
    Stack_numbers.pop();                  // Удаляем верхний элемент

    double b = Stack_numbers.top().value; // Следующий элемент стека
    Stack_numbers.pop();                  // Удаляем следующий элемент

    // Выполнение операции в зависимости от типа оператора
    switch (Stack_operations.top().type)
    {
    case '+':
        item.value = roundToPrecision(b + a, 7);// Сложение
        break;
    case '-':
        item.value = roundToPrecision(b - a, 7);// Вычитание
        break;
    case '*':
        item.value = roundToPrecision(b * a, 7);// Умножение
        break;
    case '/':
        if (a == 0) // Проверка деления на ноль
        {
            cout << "\nНа 0 делить нельзя!\n";
            return false;
        }
        item.value = roundToPrecision(b / a, 7);// Деление
        break;
    case '^':
        item.value = roundToPrecision(pow(b, a), 7);// Возведение в степень
        break;
    case 's':
        item.value = Sin(a); // Синус
        break;
    case 'c':
        item.value = Cos(a); // Косинус
        break;
    case 't':
        if (Cos(a) == 0) // Проверка для тангенса
        {
            cout << "\nНеверный аргумент для тангенса!\n";
            return false;
        }
        item.value = roundToPrecision(tan(a), 7); // Тангенс
        break;
    case 'g':
        item.value = Ctg(a); // Котангенс
        break;
    case 'e':
        item.value = roundToPrecision(exp(a), 7); // Экспонента
        break;
    default:
        cerr << "\nОшибка!\n";  // Ошибка, если оператор неизвестен
        return false;
    }

    item.type = '0';          // Устанавливаем тип результата
    Stack_numbers.push(item); // Помещаем результат в стек значений
    Stack_operations.pop();   // Удаляем оператор из стека
    return true;              // Операция выполнена успешно
}

// Функция для определения приоритета операторов
int getRang(char Ch)
{
    if (Ch == 's' || Ch == 'c' || Ch == 't' || Ch == 'g' || Ch == 'e') return 4; // Тригонометрические функции имеют 4-й приоритет
    if (Ch == '+' || Ch == '-') return 1; // Сложение и вычитание имеют 1-й приоритет
    if (Ch == '*' || Ch == '/') return 2; // Умножение и деление имеют 2-й приоритет
    if (Ch == '^') return 3;              // Возведение в степень имеет 3-й приоритет
    return 0;                             // Оператор неизвестен
}

int main()
{
    setlocale(LC_ALL, "Rus"); 
    SetConsoleOutputCP(1251); 
    SetConsoleCP(1251); 

    while (true) // Основной цикл программы
    {
        system("cls"); // Очистка консоли
        cout << "Привет! Это программа - калькулятор!\n"; // Приветствие
        cout << "Для использования числа Пи введите 'p', для использования числа Е введите 'exp(1)'\n"; // Инструкция

        bool validInput; // Флаг для отслеживания правильности ввода

        do
        {
            validInput = true;               // Сброс правильности ввода
            cout << "Введите выражение: ";   // Запрос на ввод
            char Ch;                         // Переменная для считывания выражения посимвольно
            double value;                    // Переменная для числовых значений
            stack<Leksema> Stack_numbers;    // Стек для чисел 
            stack<Leksema> Stack_operations; // Стек для операторов
            Leksema item;                    // Лексема для хранения результата

            while (true) // Цикл для считывания ввода
            {
                Ch = cin.get();              // Считываем символ
                if (Ch == '\n') break;       // Если нажат Enter, прерываем ввод
                if (Ch == ' ') continue;     // Игнорируем пробелы

                if (Ch == '(') // Если встречаем '('
                {
                    item.type = Ch;              // Устанавливаем тип лексемы как открывающая скобка
                    item.value = 0;              // Значение не имеет значения для скобки
                    Stack_operations.push(item); // Добавляем скобку в стек операторов
                    continue;
                }

                if (Ch == ')') // Если встречаем ')'
                {
                    while (!Stack_operations.empty() && Stack_operations.top().type != '(') // Выполняем операции до открытия скобки и до тех пор пока стек с операциями не пуст
                    {
                        if (!Maths(Stack_numbers, Stack_operations, item)) // Выполняем математическую функцию
                        {
                            cout << "Ошибка при выполнении операций!\n";   // Сообщение об ошибке
                            validInput = false;                            // Устанавливаем флаг ошибки
                            break;                                         // Выход при ошибке
                        }
                    }
                    if (!Stack_operations.empty() && Stack_operations.top().type == '(') // Если встретили новую открывающую скобку и стек с операторами не пуст
                    {
                        Stack_operations.pop();                                          // Удаляем открытую скобку
                    }
                    continue;
                }

                if (Ch == 'p') // Если введено число Пи
                {
                    item.type = '0';          // Указываем, что это число
                    item.value = Pi;          // Устанавливаем значение Пи
                    Stack_numbers.push(item); // Добавляем Пи в стек
                    continue;
                }

                // Обработка числа
                if (isdigit(Ch) || (Ch == '-' && (Stack_numbers.empty() || Stack_numbers.top().type == '(')))
                {
                    cin.putback(Ch);          // Возвращаем символ в поток
                    cin >> value;             // Считываем число
                    item.type = '0';          // Указываем, что это число
                    item.value = value;       // Сохраняем значение
                    Stack_numbers.push(item); // Добавляем число в стек
                    continue;
                }

                // Обработка операторов
                if (Ch == '+' || Ch == '-' || Ch == '*' || Ch == '/' || Ch == '^')
                {
                    // Выполнение операций, если текущий оператор имеет меньший приоритет
                    while (!Stack_operations.empty() && getRang(Ch) <= getRang(Stack_operations.top().type))
                    {
                        if (!Maths(Stack_numbers, Stack_operations, item))
                        {
                            cout << "Ошибка при выполнении операций!\n"; // Сообщение об ошибке
                            validInput = false;                          // Устанавливаем флаг ошибки
                            break;                                       // Выход при ошибке
                        }
                    }
                    item.type = Ch;              // Устанавливаем тип операции
                    item.value = 0;              // Значение для операторов не важно
                    Stack_operations.push(item); // Добавляем оператор в стек
                    continue;
                }

                // Обработка функций
                if (Ch == 's' || Ch == 'c' || Ch == 't' || Ch == 'e')
                {
                    cin.putback(Ch); // Возвращаем символ в поток
                    string func;     // Переменная для хранения имени функции
                    cin >> func;     // Считываем функцию
                    // Устанавливаем соответствующий тип для функции
                    if (func == "sin") { item.type = 's'; }
                    else if (func == "cos") { item.type = 'c'; }
                    else if (func == "tan") { item.type = 't'; }
                    else if (func == "ctg") { item.type = 'g'; }
                    else if (func == "exp") { item.type = 'e'; }
                    else
                    {
                        cout << "Ошибка: Неверная функция.\n"; // Сообщаем об ошибке
                        validInput = false;                    // Устанавливаем флаг ошибки
                        break;                                 // Выход при ошибке
                    }

                    Stack_operations.push(item); // Добавляем функцию в стек
                    continue;
                }

                cout << "\nНеверно введено выражение!\n"; // Сообщение об ошибке
                validInput = false;                       // Устанавливаем флаг ошибки
                break;                                    // Выход при ошибке
            }

            // Выполняем оставшиеся операции из стека, если ввод валиден
            if (validInput)
            {
                while (!Stack_operations.empty())
                {
                    if (!Maths(Stack_numbers, Stack_operations, item))
                    {
                        cout << "Ошибка при выполнении операций!\n"; // Сообщение об ошибке
                        validInput = false;                          // Устанавливаем флаг ошибки
                        break;                                       // Выход при ошибке
                    }
                }
            }

            if (validInput) // Если ввод валиден
            {
                if (!Stack_numbers.empty()) // Если в стеке чисел остался результат
                {
                    cout << "   Ответ: " << setprecision(7) << Stack_numbers.top().value << endl; // Выводим ответ
                }
                else
                {
                    cout << "\nОшибка: Стек чисел пуст!\n"; // Сообщение об ошибке, если стек пуст
                }
            }

        } while (!validInput); // Продолжаем, если ввод невалиден

        system("pause"); // Пауза в конце перед следующим вводом
    }
    return 0; // Завершение программы
}




