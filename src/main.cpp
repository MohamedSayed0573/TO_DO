#include "Tasks.hpp"
#include "Task.hpp"

#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[])
{
    Tasks TO_DO;

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <command> [arguments]\n";
        std::cout << "Commands:\n";
        std::cout << "  add <task_name> <status>  - Add a new task\n";
        std::cout << "  show                      - Show all tasks\n";
        return 1;
    }

    if (std::string(argv[1]) == "add")
    {
        if (argc < 4)
        {
            std::cout << "Usage: " << argv[0] << " add <task_name> <status>\n";
            return 1;
        }
        const std::string& taskName = (std::string)argv[2];
        int taskStatus = std::stoi(argv[3]);

        TO_DO.addTask(Task(taskName, taskStatus));
    }
    else if (std::string(argv[1]) == "show")
    {
        std::vector<Task*> Tasks = TO_DO.giveAllTasks();
        for (auto& task : Tasks)
        {
            std::cout << "Task: " << task->getName() << ", " << task->getStatus() << "\n";
        }
    }
    else
    {
        std::cout << "Unknown command: " << argv[1] << "\n";
        return 1;
    }

    return 0;
}
