#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "task_manager.h"

#include <cstdio>
#include <fstream>
#include <stdexcept>

TEST_CASE("addTask добавляет задачу с правильным id") {
    std::vector<Task> tasks;

    addTask(tasks, "Сделать домашку", "2026-06-20", 2);

    CHECK(tasks.size() == 1);
    CHECK(tasks[0].id == 1);
    CHECK(tasks[0].description == "Сделать домашку");
    CHECK(tasks[0].deadline == "2026-06-20");
    CHECK(tasks[0].priority == 2);
    CHECK(tasks[0].done == false);
}

TEST_CASE("addTask увеличивает id для следующей задачи") {
    std::vector<Task> tasks;

    addTask(tasks, "Первая задача", "2026-06-20", 1);
    addTask(tasks, "Вторая задача", "2026-06-21", 2);

    CHECK(tasks[0].id == 1);
    CHECK(tasks[1].id == 2);
}

TEST_CASE("removeTask удаляет существующую задачу") {
    std::vector<Task> tasks;
    addTask(tasks, "Задача", "2026-06-20", 1);

    bool result = removeTask(tasks, 1);

    CHECK(result == true);
    CHECK(tasks.empty());
}

TEST_CASE("removeTask возвращает false, если задача не найдена") {
    std::vector<Task> tasks;
    addTask(tasks, "Задача", "2026-06-20", 1);

    bool result = removeTask(tasks, 99);

    CHECK(result == false);
    CHECK(tasks.size() == 1);
}

TEST_CASE("markTaskDone отмечает существующую задачу выполненной") {
    std::vector<Task> tasks;
    addTask(tasks, "Задача", "2026-06-20", 1);

    bool result = markTaskDone(tasks, 1);

    CHECK(result == true);
    CHECK(tasks[0].done == true);
}

TEST_CASE("markTaskDone возвращает false для несуществующего id") {
    std::vector<Task> tasks;
    addTask(tasks, "Задача", "2026-06-20", 1);

    bool result = markTaskDone(tasks, 42);

    CHECK(result == false);
    CHECK(tasks[0].done == false);
}

TEST_CASE("editTask изменяет данные существующей задачи") {
    std::vector<Task> tasks;
    addTask(tasks, "Старое описание", "2026-06-20", 1);

    bool result = editTask(tasks, 1, "Новое описание", "2026-07-01", 3);

    CHECK(result == true);
    CHECK(tasks[0].description == "Новое описание");
    CHECK(tasks[0].deadline == "2026-07-01");
    CHECK(tasks[0].priority == 3);
}

TEST_CASE("editTask возвращает false для несуществующего id") {
    std::vector<Task> tasks;
    addTask(tasks, "Описание", "2026-06-20", 1);

    bool result = editTask(tasks, 100, "Новое описание", "2026-07-01", 3);

    CHECK(result == false);
    CHECK(tasks[0].description == "Описание");
}

TEST_CASE("filterByKeyword находит задачи по подстроке") {
    std::vector<Task> tasks;
    addTask(tasks, "Сделать отчёт", "2026-06-20", 1);
    addTask(tasks, "Купить продукты", "2026-06-21", 2);

    std::vector<Task> result = filterByKeyword(tasks, "отчёт");

    CHECK(result.size() == 1);
    CHECK(result[0].description == "Сделать отчёт");
}

TEST_CASE("filterByKeyword возвращает пустой список, если совпадений нет") {
    std::vector<Task> tasks;
    addTask(tasks, "Сделать отчёт", "2026-06-20", 1);

    std::vector<Task> result = filterByKeyword(tasks, "несуществующее слово");

    CHECK(result.empty());
}

TEST_CASE("filterByPriority находит задачи с заданной важностью") {
    std::vector<Task> tasks;
    addTask(tasks, "Задача 1", "2026-06-20", 1);
    addTask(tasks, "Задача 2", "2026-06-21", 3);

    std::vector<Task> result = filterByPriority(tasks, 3);

    CHECK(result.size() == 1);
    CHECK(result[0].priority == 3);
}

TEST_CASE("filterByPriority возвращает пустой список для несуществующей важности") {
    std::vector<Task> tasks;
    addTask(tasks, "Задача 1", "2026-06-20", 1);

    std::vector<Task> result = filterByPriority(tasks, 3);

    CHECK(result.empty());
}

TEST_CASE("filterByStatus находит выполненные и невыполненные задачи") {
    std::vector<Task> tasks;
    addTask(tasks, "Задача 1", "2026-06-20", 1);
    addTask(tasks, "Задача 2", "2026-06-21", 2);
    markTaskDone(tasks, 1);

    std::vector<Task> done = filterByStatus(tasks, true);
    std::vector<Task> notDone = filterByStatus(tasks, false);

    CHECK(done.size() == 1);
    CHECK(done[0].id == 1);
    CHECK(notDone.size() == 1);
    CHECK(notDone[0].id == 2);
}

TEST_CASE("sortByDeadline сортирует задачи по возрастанию даты") {
    std::vector<Task> tasks;
    addTask(tasks, "Поздняя задача", "2026-12-01", 1);
    addTask(tasks, "Ранняя задача", "2026-01-01", 1);

    sortByDeadline(tasks);

    CHECK(tasks[0].description == "Ранняя задача");
    CHECK(tasks[1].description == "Поздняя задача");
}

TEST_CASE("saveTasks и loadTasks сохраняют и читают задачи без изменений") {
    const std::string filename = "test_tasks.txt";

    std::vector<Task> tasks;
    addTask(tasks, "Задача для сохранения", "2026-06-20", 2);
    markTaskDone(tasks, 1);

    saveTasks(filename, tasks);
    std::vector<Task> loaded = loadTasks(filename);

    CHECK(loaded.size() == 1);
    CHECK(loaded[0].id == 1);
    CHECK(loaded[0].description == "Задача для сохранения");
    CHECK(loaded[0].deadline == "2026-06-20");
    CHECK(loaded[0].priority == 2);
    CHECK(loaded[0].done == true);

    std::remove(filename.c_str());
}

TEST_CASE("loadTasks возвращает пустой список для несуществующего файла") {
    std::vector<Task> tasks = loadTasks("файл_которого_нет.txt");
    CHECK(tasks.empty());
}

TEST_CASE("loadTasks выбрасывает исключение для строки с неполным набором полей") {
    const std::string filename = "bad_format_tasks.txt";
    std::ofstream file(filename);
    file << "1;Описание;2026-06-20;2\n";
    file.close();

    CHECK_THROWS_AS(loadTasks(filename), std::runtime_error);

    std::remove(filename.c_str());
}

TEST_CASE("loadTasks выбрасывает исключение для нечисловых данных") {
    const std::string filename = "bad_number_tasks.txt";
    std::ofstream file(filename);
    file << "один;Описание;2026-06-20;2;0\n";
    file.close();

    CHECK_THROWS_AS(loadTasks(filename), std::runtime_error);

    std::remove(filename.c_str());
}

TEST_CASE("loadTasks выбрасывает исключение для некорректного статуса") {
    const std::string filename = "bad_status_tasks.txt";
    std::ofstream file(filename);
    file << "1;Описание;2026-06-20;2;выполнено\n";
    file.close();

    CHECK_THROWS_AS(loadTasks(filename), std::runtime_error);

    std::remove(filename.c_str());
}

TEST_CASE("loadTasks выбрасывает исключение для некорректной важности") {
    const std::string filename = "bad_priority_tasks.txt";
    std::ofstream file(filename);
    file << "1;Описание;2026-06-20;4;0\n";
    file.close();

    CHECK_THROWS_AS(loadTasks(filename), std::runtime_error);

    std::remove(filename.c_str());
}

TEST_CASE("addTask выбрасывает исключение для некорректной важности") {
    std::vector<Task> tasks;

    CHECK_THROWS_AS(addTask(tasks, "Описание", "2026-06-20", 4), std::invalid_argument);
}

TEST_CASE("editTask выбрасывает исключение для некорректной важности") {
    std::vector<Task> tasks;
    addTask(tasks, "Описание", "2026-06-20", 2);

    CHECK_THROWS_AS(editTask(tasks, 1, "Новое описание", "2026-06-21", 0),
                    std::invalid_argument);
}

TEST_CASE("filterByPriority выбрасывает исключение для некорректной важности") {
    std::vector<Task> tasks;

    CHECK_THROWS_AS(filterByPriority(tasks, 10), std::invalid_argument);
}
