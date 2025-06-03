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
    const std::vector<Task>& getAllTasks() const;
    void saveTasks() const;
    void loadTasks();
    void removeTask(const Task& task);
    std::vector<Task> searchTasksByName(const std::string& taskName) const;

    Task* findTaskbyID(int id);

    bool isUniqueID(int id) const;
    int generateTaskIDs(Task& task) const;
};
