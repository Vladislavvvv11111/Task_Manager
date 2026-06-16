#include "task_manager.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Печатает главное меню программы в консоль.
 */
void printMenu() {
    std::cout << "\n===== МЕНЕДЖЕР ЗАДАЧ =====\n";
    std::cout << "1. Показать все задачи\n";
    std::cout << "2. Добавить задачу\n";
    std::cout << "3. Удалить задачу\n";
    std::cout << "4. Изменить задачу\n";
    std::cout << "5. Отметить задачу как выполненную\n";
    std::cout << "6. Фильтр по ключевому слову\n";
    std::cout << "7. Фильтр по важности\n";
    std::cout << "8. Фильтр по статусу\n";
    std::cout << "9. Сортировать по дедлайну\n";
    std::cout << "0. Выход\n";
    std::cout << "Выберите пункт меню: ";
}

/**
 * @brief Считывает целое число от пользователя с проверкой ошибок.
 *
 * Если пользователь ввёл не число, выбрасывается исключение std::invalid_argument.
 *
 * @return Введённое целое число.
 */
int readInt() {
    int value;

    if (!(std::cin >> value)) {
        // Очищаем флаги ошибок и остаток строки, чтобы не сломать дальнейший ввод.
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Введено не число.");
    }

    // Убираем символ конца строки, оставшийся после чтения числа.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

/**
 * @brief Считывает целую строку текста от пользователя.
 *
 * @return Введённая строка.
 */
std::string readLine() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

int main(int argc, char* argv[]) {
    const std::string filename = (argc > 1) ? argv[1] : "tasks.txt";
    std::vector<Task> tasks;

    try {
        tasks = loadTasks(filename);
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << '\n';
        return 1;
    }

    int choice = -1;

    while (choice != 0) {
        printMenu();

        try {
            choice = readInt();

            switch (choice) {
                case 1: {
                    printTasks(tasks);
                    break;
                }
                case 2: {
                    std::cout << "Описание задачи: ";
                    std::string description = readLine();

                    std::cout << "Дедлайн (ГГГГ-ММ-ДД): ";
                    std::string deadline = readLine();

                    std::cout << "Важность (1 - низкая, 2 - средняя, 3 - высокая): ";
                    int priority = readInt();

                    if (priority < 1 || priority > 3) {
                        throw std::invalid_argument("Важность должна быть от 1 до 3.");
                    }

                    addTask(tasks, description, deadline, priority);
                    saveTasks(filename, tasks);
                    std::cout << "Задача добавлена.\n";
                    break;
                }
                case 3: {
                    std::cout << "Введите ID задачи для удаления: ";
                    int id = readInt();

                    if (removeTask(tasks, id)) {
                        saveTasks(filename, tasks);
                        std::cout << "Задача удалена.\n";
                    } else {
                        throw std::invalid_argument("Задача с таким ID не найдена.");
                    }
                    break;
                }
                case 4: {
                    std::cout << "Введите ID задачи для изменения: ";
                    int id = readInt();

                    std::cout << "Новое описание: ";
                    std::string description = readLine();

                    std::cout << "Новый дедлайн (ГГГГ-ММ-ДД): ";
                    std::string deadline = readLine();

                    std::cout << "Новая важность (1-3): ";
                    int priority = readInt();

                    if (priority < 1 || priority > 3) {
                        throw std::invalid_argument("Важность должна быть от 1 до 3.");
                    }

                    if (editTask(tasks, id, description, deadline, priority)) {
                        saveTasks(filename, tasks);
                        std::cout << "Задача изменена.\n";
                    } else {
                        throw std::invalid_argument("Задача с таким ID не найдена.");
                    }
                    break;
                }
                case 5: {
                    std::cout << "Введите ID задачи для отметки выполненной: ";
                    int id = readInt();

                    if (markTaskDone(tasks, id)) {
                        saveTasks(filename, tasks);
                        std::cout << "Задача отмечена как выполненная.\n";
                    } else {
                        throw std::invalid_argument("Задача с таким ID не найдена.");
                    }
                    break;
                }
                case 6: {
                    std::cout << "Введите ключевое слово: ";
                    std::string keyword = readLine();

                    std::vector<Task> filtered = filterByKeyword(tasks, keyword);
                    printTasks(filtered);
                    break;
                }
                case 7: {
                    std::cout << "Введите важность (1-3): ";
                    int priority = readInt();

                    if (priority < 1 || priority > 3) {
                        throw std::invalid_argument("Важность должна быть от 1 до 3.");
                    }

                    std::vector<Task> filtered = filterByPriority(tasks, priority);
                    printTasks(filtered);
                    break;
                }
                case 8: {
                    std::cout << "Показать выполненные задачи? (1 - да, 0 - нет): ";
                    int statusChoice = readInt();

                    if (statusChoice != 0 && statusChoice != 1) {
                        throw std::invalid_argument("Введите 0 или 1.");
                    }

                    bool done = (statusChoice == 1);
                    std::vector<Task> filtered = filterByStatus(tasks, done);
                    printTasks(filtered);
                    break;
                }
                case 9: {
                    sortByDeadline(tasks);
                    saveTasks(filename, tasks);
                    std::cout << "Задачи отсортированы по дедлайну.\n";
                    printTasks(tasks);
                    break;
                }
                case 0: {
                    std::cout << "Выход из программы.\n";
                    break;
                }
                default: {
                    throw std::invalid_argument("Неизвестный пункт меню. Попробуйте снова.");
                    break;
                }
            }
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << '\n';
        }
    }

    return 0;
}
