#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include <string>
#include <vector>

/// Структура, описывающая одну задачу в списке дел.
struct Task {
    int id;                 ///< Уникальный номер задачи.
    std::string description; ///< Текстовое описание задачи.
    std::string deadline;    ///< Дедлайн в формате ГГГГ-ММ-ДД.
    int priority;             ///< Уровень важности (1 - низкий, 2 - средний, 3 - высокий).
    bool done;                ///< Флаг выполнения задачи (true - выполнена).
};

/**
 * @brief Загружает список задач из текстового файла.
 *
 * Если файл не существует, возвращается пустой список задач.
 * Каждая строка файла соответствует одной задаче и имеет вид:
 * id;description;deadline;priority;done
 *
 * @param filename Путь к файлу с задачами.
 * @return Вектор задач, прочитанных из файла.
 */
std::vector<Task> loadTasks(const std::string& filename);

/**
 * @brief Сохраняет список задач в текстовый файл.
 *
 * Перезаписывает файл целиком, записывая каждую задачу в отдельной строке
 * в формате: id;description;deadline;priority;done
 *
 * @param filename Путь к файлу для сохранения.
 * @param tasks Вектор задач для сохранения.
 */
void saveTasks(const std::string& filename, const std::vector<Task>& tasks);

/**
 * @brief Добавляет новую задачу в список.
 *
 * Идентификатор новой задачи вычисляется автоматически
 * как максимальный существующий id плюс один.
 *
 * @param tasks Вектор задач, в который добавляется новая задача.
 * @param description Описание задачи.
 * @param deadline Дедлайн задачи в формате ГГГГ-ММ-ДД.
 * @param priority Уровень важности задачи (1-3).
 */
void addTask(std::vector<Task>& tasks, const std::string& description,
             const std::string& deadline, int priority);

/**
 * @brief Удаляет задачу с указанным идентификатором.
 *
 * Если задача с таким id не найдена, вектор остаётся без изменений.
 *
 * @param tasks Вектор задач.
 * @param id Идентификатор задачи, которую нужно удалить.
 * @return true, если задача была найдена и удалена, иначе false.
 */
bool removeTask(std::vector<Task>& tasks, int id);

/**
 * @brief Отмечает задачу с указанным идентификатором как выполненную.
 *
 * @param tasks Вектор задач.
 * @param id Идентификатор задачи.
 * @return true, если задача найдена и отмечена, иначе false.
 */
bool markTaskDone(std::vector<Task>& tasks, int id);

/**
 * @brief Изменяет описание, дедлайн и приоритет задачи с указанным id.
 *
 * @param tasks Вектор задач.
 * @param id Идентификатор задачи, которую нужно изменить.
 * @param newDescription Новое описание задачи.
 * @param newDeadline Новый дедлайн задачи.
 * @param newPriority Новый уровень важности задачи.
 * @return true, если задача найдена и изменена, иначе false.
 */
bool editTask(std::vector<Task>& tasks, int id, const std::string& newDescription,
              const std::string& newDeadline, int newPriority);

/**
 * @brief Возвращает список задач, описание которых содержит заданное слово.
 *
 * Поиск регистрозависимый и ищет ключевое слово как подстроку описания.
 *
 * @param tasks Вектор задач.
 * @param keyword Ключевое слово для поиска.
 * @return Вектор задач, прошедших фильтр.
 */
std::vector<Task> filterByKeyword(const std::vector<Task>& tasks, const std::string& keyword);

/**
 * @brief Возвращает список задач с заданным уровнем важности.
 *
 * @param tasks Вектор задач.
 * @param priority Требуемый уровень важности (1-3).
 * @return Вектор задач, прошедших фильтр.
 */
std::vector<Task> filterByPriority(const std::vector<Task>& tasks, int priority);

/**
 * @brief Возвращает список задач с заданным статусом выполнения.
 *
 * @param tasks Вектор задач.
 * @param done Искомый статус выполнения (true - выполненные, false - невыполненные).
 * @return Вектор задач, прошедших фильтр.
 */
std::vector<Task> filterByStatus(const std::vector<Task>& tasks, bool done);

/**
 * @brief Сортирует задачи по дедлайну в порядке возрастания.
 *
 * Сортировка происходит по строковому представлению даты в формате
 * ГГГГ-ММ-ДД, что соответствует хронологическому порядку.
 *
 * @param tasks Вектор задач для сортировки.
 */
void sortByDeadline(std::vector<Task>& tasks);

/**
 * @brief Печатает информацию об одной задаче в консоль.
 *
 * @param task Задача, которую нужно вывести.
 */
void printTask(const Task& task);

/**
 * @brief Печатает список задач в консоль.
 *
 * Если список пуст, выводится соответствующее сообщение.
 *
 * @param tasks Вектор задач для печати.
 */
void printTasks(const std::vector<Task>& tasks);

#endif // TASK_MANAGER_HPP
