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
    void addTask(Task& task);
    const std::vector<Task>& giveAllTasks();
    void saveTasks();
    void loadTasks();
    void removeTask(const Task& task);
    const std::vector<Task> searchTasksByName(const std::string& taskName);

    Task* findTaskbyID(int id);

    bool isUniqueID(int id);
    void generateTaskIDs(Task& task);
};
