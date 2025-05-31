#pragma once
#include <vector>
#include <fstream>

class Task;

class Tasks
{
private:
    std::vector<Task> m_tasks;

public:
    void addTask(const Task& task);
    std::vector<Task*> giveAllTasks();
    void saveTasks();
    void loadTasks();
};
