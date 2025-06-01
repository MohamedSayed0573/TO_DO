#include "Tasks.hpp"
#include "Task.hpp"

#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <optional>

const std::string DATA_FILE_NAME = "data.txt";
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
    write << m_tasks.size() << std::endl;

    for (auto task : m_tasks)
    {
        write << task.getID() << "|" << task.getName() << "|" << task.getStatus() << std::endl; // Delimiter is '|'
    }

    write.close();
}

void Tasks::loadTasks()
{
    std::ifstream read;
    read.open(DATA_FILE_NAME);

    int numTasks;
    read >> numTasks;
    read.ignore(); // Ignore the new line \n

    for (int i = 0; i < numTasks; i++)
    {
        int taskID;
        read >> taskID;

        read.ignore(); // Ignore the first '|'

        std::string taskName;
        std::getline(read, taskName, '|'); // Read until you see the delimiter '|'

        int taskStatus;
        read >> taskStatus;

        Task task(taskName, taskStatus);
        task.setID(taskID);
        m_tasks.push_back(task);
    }

    read.close();
}

Task* Tasks::findTaskbyID(int id)
{
    for (Task& task : m_tasks) {
        if (task.getID() == id) {
            return &task;
        }
    }
    return nullptr;
}


bool Tasks::isUniqueID(int id)
{
    for (const auto& task : m_tasks)
    {
        if (task.getID() == id) {
            return false;
        }
    }
    return true;
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
