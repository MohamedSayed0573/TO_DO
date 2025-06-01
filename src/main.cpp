#include "Tasks.hpp"
#include "Task.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <exception>
#include <stdexcept>

constexpr double VERSION = 1.1;

enum Status
{
    ToDo,
    InProgress,
    Completed
};

std::string_view statusToStr(int s) {
    switch (s) {
        case ToDo: return "To-Do";
        case InProgress: return "In Progress";
        case Completed: return "Completed";
        default: return "Unknown";
    }
}

int handleStatusInput(char* argStatus)
{
    int intStatus;

    try {
        intStatus = std::stoi(argStatus);
        if (intStatus < 0 || intStatus > 2) {
            throw std::out_of_range("Status must be 0, 1, or 2.");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Invalid status: " << e.what() << "\n";
        exit(1);
    }
    return intStatus;
}

int handleIDInput(char* argID)
{
    int intID;

    try {
        intID = std::stoi(argID);
    }
    catch (const std::exception& e) {
        std::cerr << "Invalid ID: " << e.what() << "\n";
        exit(1);
    }
    return intID;
}

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
        if (argc != 3 && argc != 4) {
            std::cerr << "Invalid format. Use --help for info\n";
            return 1;
        }

        // The user entered only a task name
        if (argc == 3)
        {
            const std::string& taskName = (std::string)argv[2];
            TO_DO.addTask(Task(taskName));
        }
        // The user entered a task name and a status
        else if (argc == 4)
        {
            const std::string& taskName = (std::string)argv[2];
            int taskStatus = handleStatusInput(argv[3]);
            TO_DO.addTask(Task(taskName, taskStatus));
        }
        TO_DO.saveTasks();

        return 0;
    }
    else if (command == "show")
    {
        if (argc != 2) {
            std::cerr << "Invalid format. Use --help for info\n";
            return 1;
        }

        std::vector<Task*> Tasks = TO_DO.giveAllTasks();
        for (auto& task : Tasks)
        {
            std::cout << "Task: " << task->getID() << ", " << task->getName() << ", [" << statusToStr(task->getStatus())  << "]\n";
        }

        return 0;
    }
    else if (command == "update")
    {
        if (argc != 4 && argc != 5) {
            std::cerr << "Invalid format. Use --help for info\n";
            return 1;
        }
        
        // Find the task by ID
        int taskID = handleIDInput(argv[2]);
        Task* task = TO_DO.findTaskbyID(taskID);
        if (!task) {
            std::cerr << "ID was not found." << std::endl;
            return 1;
        }

        // If the user entered both a new name and a new status
        if (argc == 5)
        {
            std::string newName = std::string(argv[3]);

            int newStatus = handleStatusInput(argv[4]);

            task->setName(newName);
            task->setStatus(newStatus);
        }
        // If the user entered either a new name or a new status
        else if (argc == 4)
        {
            std::string arg = std::string(argv[3]);
            if (arg == "0" || arg == "1" || arg == "2")  // If the input was a new status
            {
                int newStatus = handleStatusInput(argv[3]);
                task->setStatus(newStatus);
            }
            else // If the input was a new name
            {
                std::string newName = arg;
                task->setName(newName);
            }
        }
        TO_DO.saveTasks();
        return 0;
    }
    else if (command == "--help" || command == "-h")
    {
        if (argc != 2) {
            std::cerr << "Invalid format. Use --help for info\n";
            return 1;
        }

        std::cout << "Usage: " << argv[0] << " <command> [arguments]\n";
        std::cout << "Commands:\n";
        std::cout << "  add <task_name> <status>             - Add a new task (0 -> To-Do | 1 -> In Progress | 2 -> Completed)\n";
        std::cout << "  show                                 - Show all tasks\n";
        std::cout << "  update <task_ID> <Name> <Status>     - Update a task\n";
        std::cout << "  --version (-v)                       - Show current version\n";
        std::cout << "  --help (-h)                          - Show help menu\n";
        return 0;
    }
    else if (command == "--version" || command == "-v")
    {
        if (argc != 2) {
            std::cerr << "Invalid format. Use --help for info\n";
            return 1;
        }

        std::cout << VERSION << "\n";
        return 0;
    }
    else
    {
        std::cout << "Unknown command: " << argv[1] << "\n";
        return 1;
    }

    return 0;
}
