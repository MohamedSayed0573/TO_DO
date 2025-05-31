#include "Tasks.hpp"
#include "Task.hpp"

#include <fstream>

void Tasks::addTask(const Task& task)
{
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
    write.open("data.txt");
    write << m_tasks.size() << std::endl;

    for (auto task : m_tasks)
    {
        write << task.getName() << "\t" << task.getStatus() << std::endl;
    }
}

void Tasks::loadTasks()
{
    std::ifstream read;
    read.open("data.txt");

    int numTasks;
    read >> numTasks;

    for (int i = 0; i < numTasks; i++)
    {
        std::string taskName;
        read >> taskName;

        int taskStatus;
        read >> taskStatus;

        m_tasks.push_back(Task(taskName, taskStatus));
    }
}
