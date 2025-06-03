#include "Tasks.hpp"
#include "Task.hpp"
#include "json.hpp"

#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <optional>
#include <exception>
#include <stdexcept>
#include <algorithm>

const std::string DATA_FILE_NAME = "data.json";

constexpr int MINIMUM_ID = 0;
constexpr int MAXIMUM_ID = 999;

void Tasks::addTask(Task& task)
{
    int id = generateTaskIDs(task);
    task.setID(id);
    m_tasks.emplace_back(task);
}

const std::vector<Task>& Tasks::getAllTasks() const { return m_tasks; }

void Tasks::saveTasks() const
{
    std::ofstream write;
    write.open(DATA_FILE_NAME);

    nlohmann::json jTasks = nlohmann::json::array();
    for (const auto& task : m_tasks) {
        jTasks.push_back({
            {"id", task.getID()},
            {"name", task.getName()},
            {"status", task.getStatus()}
            });
    }

    write << jTasks.dump(4); // Pretty print with 4 spaces indentation
    write.close();
}

void Tasks::loadTasks() 
{
    std::ifstream read;
    read.open(DATA_FILE_NAME);
    if (!read.is_open()) {
        throw std::runtime_error("File could not be opened.");
    }

    nlohmann::json jTasks;
    read >> jTasks;

    m_tasks.reserve(jTasks.size());

    for (const auto& jTask : jTasks) {
        Task task(
            jTask["name"].get<std::string>(),
            jTask["status"].get<int>()
        );
        task.setID(jTask["id"].get<int>());
        m_tasks.push_back(task);
    }

    m_tasks.shrink_to_fit();
    read.close();
}

Task* Tasks::findTaskbyID(int id)
{
    auto it = std::find_if(m_tasks.begin(), m_tasks.end(), [id](const Task& task) {
        return task.getID() == id;
        });

    if (it == m_tasks.end()) {
        return nullptr;
    }

    return &(*it);
}

void Tasks::removeTask(const Task& task)
{
    auto it = std::find(m_tasks.begin(), m_tasks.end(), task);
    if (it == m_tasks.end()) {
        throw std::runtime_error("Cannot remove the task");
    }

    m_tasks.erase(it);
}

std::vector<Task> Tasks::searchTasksByName(const std::string& taskName) const
{
    std::vector<Task> vec;
    vec.reserve(m_tasks.size());

    std::copy_if(m_tasks.begin(), m_tasks.end(), std::back_inserter(vec), [&taskName](const Task& task) {
        return task.getName() == taskName;
    });

    vec.shrink_to_fit();
    return vec;
}

bool Tasks::isUniqueID(int id) const
{
    auto it = std::find_if(m_tasks.begin(), m_tasks.end(), [id](const Task& task) {
        return task.getID() == id;
        });

    return (it == m_tasks.end());
}

int Tasks::generateTaskIDs(Task& task) const
{
    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }
    
    int id;
    do {
        id = rand() % (MAXIMUM_ID - MINIMUM_ID + 1) + MINIMUM_ID;
    } while (!isUniqueID(id));

    task.setID(id);
    return id;
}
