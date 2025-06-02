#include "Tasks.hpp"
#include "Task.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <exception>
#include <stdexcept>
#include <algorithm>

constexpr double VERSION = 1.1;

enum Status
{
    ToDo,
    InProgress,
    Completed
};

std::string_view statusToStr(int s);
int handleStatusInput(char* argStatus);
int handleIDInput(char* argID);
void checkargc(int argc, int num1, int num2 = -1);
void saveTasks(Tasks& TO_DO);
void loadTasks(Tasks& TO_DO);
std::vector<Task> filterTasksByStatus(std::vector<Task> allTasks, Status filterStatus);
void printTask(const Task& task);
void printTasks(const std::vector<Task>& task);

int main(int argc, char* argv[])
{
    Tasks TO_DO;
    loadTasks(TO_DO);

    if (argc < 2) {
        std::cerr << "No command specified. Type --help for info" << std::endl;
        return 1;
    }

    std::string_view command = argv[1];
    if (command == "add")
    {
        checkargc(argc, 3, 4); // We expect 3 to 4 arguments

        const std::string& taskName = argv[2];
        if (taskName.empty() || taskName.length() > 100) {
            std::cerr << "Invalid Input. Task name length should be from 1 to 100 characters" << "\n";
            return 1;
        }
        // The user entered only a task name
        // Status is 0 (To-Do) by default
        if (argc == 3)
        {
            TO_DO.addTask(Task(taskName));
        }
        // The user entered a task name and a status
        else if (argc == 4)
        {
            int taskStatus = handleStatusInput(argv[3]);
            TO_DO.addTask(Task(taskName, taskStatus));
        }
        saveTasks(TO_DO);
        std::cout << "The task was added successfully" << "\n";
        return 0;
    }
    else if (command == "show")
    {
        checkargc(argc, 2, 3); // We expect 2 arguments
        
        std::vector<Task> allTasks = TO_DO.giveAllTasks();

        if (allTasks.empty()) {
            std::cerr << "No Tasks were found" << "\n";
            return 1;
        }

        if (argc == 2) // No Filtering
        {
            printTasks(allTasks);
        }
        else if (argc == 3) // Filtering
        {
            std::string_view filter = argv[2];
            Status statusFilter;

            if (filter == "-t") { statusFilter = Status::ToDo; }
            else if (filter == "-i") { statusFilter = Status::InProgress; }
            else if (filter == "-c") { statusFilter = Status::Completed; }
            else {
                std::cerr << "Invalid Format. Check --help for info." << "\n";
                return 1;
            }

            const std::vector<Task>& filteredTasks = filterTasksByStatus(allTasks, statusFilter);
            if (filteredTasks.empty()) {
                std::cout << "No [" << statusToStr(statusFilter) << "] Tasks were found..." << "\n";
                return 1;
            }

            printTasks(filteredTasks);
        }
        return 0;
    }
    else if (command == "update")
    {
        checkargc(argc, 4, 5); // We expect 4 to 5 arguments

        // Find the task by ID
        int taskID = handleIDInput(argv[2]);
        Task* task = TO_DO.findTaskbyID(taskID);
        if (!task) {
            std::cerr << "ID [" << taskID << "] was not found." << std::endl;
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
        saveTasks(TO_DO);
        return 0;
    }
    else if (command == "remove")
    {
        checkargc(argc, 3);

        int taskID = handleIDInput(argv[2]);

        Task* task = TO_DO.findTaskbyID(taskID);
        if (!task) {
            std::cerr << "No task with ID [" << taskID << "] was found" << "\n";
            return 1;
        }

        // Store task info for confirmation message before removal
        int removedID = task->getID();
        std::string removedName = task->getName();

        try {
            TO_DO.removeTask(*task);
        }
        catch (const std::exception& ex) {
            std::cerr << "Error occurred while removing task: " << ex.what() << "\n";
            return 1;
        }

        TO_DO.saveTasks();
        std::cout << "Task [" << removedID << "] \"" << removedName << "\" was removed successfully" << "\n";
        return 0;
    }
    else if (command == "search")
    {
        checkargc(argc, 3);

        std::string taskName = argv[2];
        std::vector<Task> vec = TO_DO.searchTasks(taskName);
        if (vec.empty()) {
            std::cout << "No tasks were found with the name \"" << taskName << "\".\n";
            return 1;
        }

        printTasks(vec);
    }
    else if (command == "--help" || command == "-h")
    {
        checkargc(argc, 2); // We expect 2 arguments

        std::cout << "Usage: " << argv[0] << " <command> [arguments]\n";
        std::cout << "Commands:\n";
        std::cout << "  add <task_name> <status>             - Add a new task (0 -> To-Do | 1 -> In Progress | 2 -> Completed)\n";
        std::cout << "  show <filter_flag>                   - Show all tasks (-t -> To-Do | -i -> In Progress | -c -> Completed)\n";
        std::cout << "  update <task_ID> <Name> <Status>     - Update a task\n";
        std::cout << "  remove <task_ID>                     - Removes a task\n";
        std::cout << "  --version (-v)                       - Show current version\n";
        std::cout << "  --help (-h)                          - Show help menu\n";
        return 0;
    }
    else if (command == "--version" || command == "-v")
    {
        checkargc(argc, 2); // We expect 2 arguments

        std::cout << "Version: " << VERSION << "\n";
        return 0;
    }
    else
    {
        std::cout << "Unknown command: " << argv[1] << "\n";
        return 1;
    }

    return 0;
}

std::string_view statusToStr(int s) 
{
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

void checkargc(int argc, int num1, int num2)
{
    if (num2 == -1) { // Didn't get num2 argument
        num2 = num1;
    }

    if (!(argc >= num1 && argc <= num2)) {
        std::cerr << "Invalid format. Use --help for info\n";
        exit(1);
    }
}

void saveTasks(Tasks& TO_DO)
{
    try {
        TO_DO.saveTasks();
    }
    catch (const std::exception& ex) {
        std::cerr << "An Error occured while saving " << "\n" << ex.what() << "\n";
        exit(1);
    }
}

void loadTasks(Tasks& TO_DO)
{
    try {
        TO_DO.loadTasks();
    }
    catch (const std::exception& ex) {
        std::cerr << "An Error occured while loading " << "\n" << ex.what() << "\n";
        exit(1);
    }
}

std::vector<Task> filterTasksByStatus(std::vector<Task> allTasks, Status filterStatus)
{
    std::vector<Task> vec;
    std::copy_if(allTasks.begin(), allTasks.end(), std::back_inserter(vec), [&filterStatus](const Task& task) {
        return task.getStatus() == filterStatus;
        });

    return vec;
}

void printTask(const Task& task)
{
    std::cout << "Task: " << task.getID() << ", " << task.getName() << ", [" << statusToStr(task.getStatus())  << "]\n";
}

void printTasks(const std::vector<Task>& tasksList)
{
    for (auto task : tasksList) {
        std::cout << "Task: " << task.getID() << ", " << task.getName() << ", [" << statusToStr(task.getStatus())  << "]\n";
    }
}
