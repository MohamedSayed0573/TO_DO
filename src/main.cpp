#include "Tasks.hpp"
#include "Task.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <string_view>

int main(int argc, char* argv[])
{
    Tasks TO_DO;
    TO_DO.loadTasks();

    if (argc < 2)
    {
        std::cerr << "No command specified. Type --help for info" << std::endl;
        return 1;
    }

    std::string_view command = argv[1];

    if (command == "add")
    {
        const std::string& taskName = (std::string)argv[2];
        int taskStatus;
        if (argc > 3)
            taskStatus = std::stoi(argv[3]);

        TO_DO.addTask(Task(taskName, taskStatus));
        TO_DO.saveTasks();

        return 0;
    }
    else if (command == "show")
    {
        std::vector<Task*> Tasks = TO_DO.giveAllTasks();
        for (auto& task : Tasks)
        {
            std::cout << "Task: " << task->getName() << ", " << task->getStatus() << "\n";
        }

        return 0;
    }
    else if (command == "--help")
    {
        std::cout << "Usage: " << argv[0] << " <command> [arguments]\n";
        std::cout << "Commands:\n";
        std::cout << "  add <task_name> <status>  - Add a new task\n";
        std::cout << "  show                      - Show all tasks\n";
        std::cout << "  --help                      - Show help menu\n";
        return 0;
    }
    else
    {
        std::cout << "Unknown command: " << argv[1] << "\n";
        return 1;
    }

    return 0;
}
