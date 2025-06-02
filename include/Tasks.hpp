#pragma once
#include <vector>
#include <fstream>
#include <optional>

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
    void removeTask(const Task& task);

    Task* findTaskbyID(int id);

    bool isUniqueID(int id);
    void generateTaskIDs(Task& task);
};
