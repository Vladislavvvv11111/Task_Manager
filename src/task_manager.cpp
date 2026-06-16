#include "task_manager.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {

void validatePriority(int priority) {
    if (priority < 1 || priority > 3) {
        throw std::invalid_argument("Важность должна быть от 1 до 3.");
    }
}

} // namespace

std::vector<Task> loadTasks(const std::string& filename) {
    std::vector<Task> tasks;
    std::ifstream file(filename);

    // Если файл ещё не создан, считаем, что список задач пуст.
    if (!file.is_open()) {
        return tasks;
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string idStr, description, deadline, priorityStr, doneStr;

        if (!std::getline(ss, idStr, ';') ||
            !std::getline(ss, description, ';') ||
            !std::getline(ss, deadline, ';') ||
            !std::getline(ss, priorityStr, ';') ||
            !std::getline(ss, doneStr, ';')) {
            throw std::runtime_error("Некорректный формат файла задач в строке " +
                                     std::to_string(lineNumber) + ".");
        }

        if (doneStr != "0" && doneStr != "1") {
            throw std::runtime_error("Некорректный статус задачи в строке " +
                                     std::to_string(lineNumber) + ".");
        }

        Task task;
        try {
            task.id = std::stoi(idStr);
            task.priority = std::stoi(priorityStr);
        } catch (const std::exception&) {
            throw std::runtime_error("Некорректные числовые данные в строке " +
                                     std::to_string(lineNumber) + ".");
        }

        try {
            validatePriority(task.priority);
        } catch (const std::invalid_argument&) {
            throw std::runtime_error("Некорректная важность задачи в строке " +
                                     std::to_string(lineNumber) + ".");
        }
        task.description = description;
        task.deadline = deadline;
        task.done = (doneStr == "1");

        tasks.push_back(task);
    }

    return tasks;
}

void saveTasks(const std::string& filename, const std::vector<Task>& tasks) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
    }

    for (const auto& task : tasks) {
        file << task.id << ';'
             << task.description << ';'
             << task.deadline << ';'
             << task.priority << ';'
             << (task.done ? "1" : "0") << '\n';
    }
}

void addTask(std::vector<Task>& tasks, const std::string& description,
             const std::string& deadline, int priority) {
    validatePriority(priority);

    int newId = 1;

    // Находим максимальный существующий id и берём следующий за ним.
    for (const auto& task : tasks) {
        if (task.id >= newId) {
            newId = task.id + 1;
        }
    }

    Task task;
    task.id = newId;
    task.description = description;
    task.deadline = deadline;
    task.priority = priority;
    task.done = false;

    tasks.push_back(task);
}

bool removeTask(std::vector<Task>& tasks, int id) {
    for (size_t i = 0; i < tasks.size(); ++i) {
        if (tasks[i].id == id) {
            tasks.erase(tasks.begin() + i);
            return true;
        }
    }
    return false;
}

bool markTaskDone(std::vector<Task>& tasks, int id) {
    for (auto& task : tasks) {
        if (task.id == id) {
            task.done = true;
            return true;
        }
    }
    return false;
}

bool editTask(std::vector<Task>& tasks, int id, const std::string& newDescription,
              const std::string& newDeadline, int newPriority) {
    validatePriority(newPriority);

    for (auto& task : tasks) {
        if (task.id == id) {
            task.description = newDescription;
            task.deadline = newDeadline;
            task.priority = newPriority;
            return true;
        }
    }
    return false;
}

std::vector<Task> filterByKeyword(const std::vector<Task>& tasks, const std::string& keyword) {
    std::vector<Task> result;

    for (const auto& task : tasks) {
        // find возвращает npos, если подстрока не найдена.
        if (task.description.find(keyword) != std::string::npos) {
            result.push_back(task);
        }
    }

    return result;
}

std::vector<Task> filterByPriority(const std::vector<Task>& tasks, int priority) {
    validatePriority(priority);

    std::vector<Task> result;

    for (const auto& task : tasks) {
        if (task.priority == priority) {
            result.push_back(task);
        }
    }

    return result;
}

std::vector<Task> filterByStatus(const std::vector<Task>& tasks, bool done) {
    std::vector<Task> result;

    for (const auto& task : tasks) {
        if (task.done == done) {
            result.push_back(task);
        }
    }

    return result;
}

void sortByDeadline(std::vector<Task>& tasks) {
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.deadline < b.deadline;
    });
}

void printTask(const Task& task) {
    std::cout << "ID: " << task.id
              << " | Описание: " << task.description
              << " | Дедлайн: " << task.deadline
              << " | Важность: " << task.priority
              << " | Статус: " << (task.done ? "выполнена" : "не выполнена")
              << '\n';
}

void printTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "Список задач пуст.\n";
        return;
    }

    for (const auto& task : tasks) {
        printTask(task);
    }
}
