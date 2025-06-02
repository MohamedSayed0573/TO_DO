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

void Tasks::addTask(Task task)
{
    generateTaskIDs(task);
    m_tasks.emplace_back(task);
}

std::vector<Task*> Tasks::giveAllTasks()
{
    std::vector<Task*> TasksVec;

    for (Task& task : m_tasks)
    {
        TasksVec.push_back(&task);
    }
    return TasksVec;
}

void Tasks::saveTasks()
{
    std::ofstream write;
    write.open(DATA_FILE_NAME);

    nlohmann::json jTasks = nlohmann::json::array();
    for (auto task : m_tasks) {
        nlohmann::json jTask;
        jTask = {
            {"id", task.getID()},
            {"name", task.getName()},
            {"status", task.getStatus()}
        };
        jTasks.push_back(jTask);
    }

    write << jTasks.dump(4); // Pretty print with 4 spaces indentation
    write.close();
}

void Tasks::loadTasks()
{
    std::ifstream read;
    read.open(DATA_FILE_NAME);
    if (!read.is_open()) {
        return;
    }

    nlohmann::json jTasks;
    read >> jTasks;

    for (const auto& jTask : jTasks) {
        Task task(
            jTask["name"].get<std::string>(),
            jTask["status"].get<int>()
        );
        task.setID(jTask["id"].get<int>());
        m_tasks.push_back(task);
    }

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
    auto it = std::find_if(m_tasks.begin(), m_tasks.end(), [&task](const Task& t) {
        return t == task;
        });

    if (it == m_tasks.end()) {
        throw std::runtime_error("Cannot remove the task");
    }

    m_tasks.erase(it);
}

bool Tasks::isUniqueID(int id)
{
    auto it = std::find_if(m_tasks.begin(), m_tasks.end(), [id](const Task& task) {
        return task.getID() == id;
        });

    return (it == m_tasks.end());
}

void Tasks::generateTaskIDs(Task& task)
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
}
