#pragma once
#include <vector>
#include <fstream>

class Task;

class Tasks
{
private:
    std::vector<Task> m_tasks;

public:
    void addTask(Task task);
    std::vector<Task*> giveAllTasks();
    void saveTasks();
    void loadTasks();

    bool isUniqueID(int id);
    void generateTaskIDs(Task& task);
};
