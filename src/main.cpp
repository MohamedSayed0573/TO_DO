#include "Tasks.hpp"
#include "Task.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <ranges>

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
void checkargc(int argc, int min, int max);
void checkargc(int argc, int num);

void saveTasks(Tasks& TO_DO);
void loadTasks(Tasks& TO_DO);
std::vector<Task> filterTasksByStatus(const std::vector<Task>& allTasks, Status filterStatus);
void printTask(const Task& task);
void printTasks(const std::vector<Task>& task);

void handleAddTask(Tasks& TO_DO, int argc, char* argv[]);
void handleShowTasks(Tasks& TO_DO, int argc, char* argv[]);
void handleUpdateTasks(Tasks& TO_DO, int argc, char* argv[]);
void handleRemoveTasks(Tasks& TO_DO, int argc, char* argv[]);
void handleSearchTasks(Tasks& TO_DO, int argc, char* argv[]);
void handleHelpCommand(Tasks& TO_DO, int argc, char* argv[]);
void handleVersionCommand(Tasks& TO_DO, int argc, char* argv[]);

int main(int argc, char* argv[])
{
    Tasks TO_DO;
    loadTasks(TO_DO);

    std::unordered_map <std::string, std::function<void(Tasks&, int, char* [])>> commandsMap;
    
    commandsMap["add"] = handleAddTask;
    commandsMap["show"] = handleShowTasks;
    commandsMap["update"] = handleUpdateTasks;
    commandsMap["remove"] = handleRemoveTasks;
    commandsMap["search"] = handleSearchTasks;
    commandsMap["--help"] = handleHelpCommand;
    commandsMap["-h"] = handleHelpCommand;
    commandsMap["--version"] = handleVersionCommand;
    commandsMap["-v"] = handleVersionCommand;

    if (argc < 2) {
        std::cerr << "No command specified. Type --help for info" << std::endl;
        return 1;
    }

if (const std::string& command = argv[1]; commandsMap.contains(command)) {
        commandsMap[command](TO_DO, argc, argv);
    }
    else {
        std::cerr << "Invalid command. Use --help for info" << "\n";
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

int handleStatusInput(const char* argStatus)
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

int handleIDInput(const char* argID)
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

void checkargc(int argc, int min, int max)
{
    if (!(argc >= min && argc <= max)) {
        std::cerr << "Invalid format. Use --help for info\n";
        exit(1);
    }
}
void checkargc(int argc, int num)
{
    if (argc != num) {
        std::cerr << "Invalid format. User --help for info\n";
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

std::vector<Task> filterTasksByStatus(const std::vector<Task>& allTasks, Status filterStatus)
{
    std::vector<Task> vec;
    vec.reserve(allTasks.size());
    std::copy_if(allTasks.begin(), allTasks.end(), std::back_inserter(vec), [&filterStatus](const Task& task) {
        return task.getStatus() == filterStatus;
        });
    
    vec.shrink_to_fit();
    return vec;
}

void printTask(const Task& task)
{
    std::cout << "Task: " << task.getID() << ", " << task.getName() << ", [" << statusToStr(task.getStatus())  << "]\n";
}

void printTasks(const std::vector<Task>& tasksList)
{
    for (const auto& task : tasksList) {
        std::cout << "Task: " << task.getID() << ", " << task.getName() << ", [" << statusToStr(task.getStatus())  << "]\n";
    }
}


void handleAddTask(Tasks& TO_DO, int argc, char* argv[])
{
    checkargc(argc, 3, 4); // We expect 3 to 4 arguments

    const std::string& taskName = argv[2];
    if (taskName.empty() || taskName.length() > 100) {
        std::cerr << "Invalid Input. Task name length should be from 1 to 100 characters" << "\n";
        exit(1);
    }
    // The user entered only a task name
    // Status is 0 (To-Do) by default
    if (argc == 3)
    {
        Task newTask(taskName);
        TO_DO.addTask(newTask);
    }
    // The user entered a task name and a status
    else if (argc == 4)
    {
        int taskStatus = handleStatusInput(argv[3]);
        Task newTask(taskName, taskStatus);
        TO_DO.addTask(newTask);
    }
    saveTasks(TO_DO);
    std::cout << "The task was added successfully" << "\n";
}

void handleShowTasks(Tasks& TO_DO, int argc, char* argv[])
{
    checkargc(argc, 2, 3); // We expect 2 arguments
    
    const std::vector<Task>& allTasks = TO_DO.getAllTasks();

    if (allTasks.empty()) {
        std::cerr << "No Tasks were found" << "\n";
        exit(1);
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
            exit(1);
        }

        const std::vector<Task>& filteredTasks = filterTasksByStatus(allTasks, statusFilter);
        if (filteredTasks.empty()) {
            std::cout << "No [" << statusToStr(statusFilter) << "] Tasks were found..." << "\n";
            exit(1);
        }

        printTasks(filteredTasks);
    }
}

void handleUpdateTasks(Tasks& TO_DO, int argc, char* argv[])
{
    checkargc(argc, 4, 5); // We expect 4 to 5 arguments

    // Find the task by ID
    int taskID = handleIDInput(argv[2]);
    Task* task = TO_DO.findTaskbyID(taskID);
    if (!task) {
        std::cerr << "ID [" << taskID << "] was not found." << std::endl;
        exit(1);
    }

    // If the user entered both a new name and a new status
    if (argc == 5)
    {
        const std::string& newName = argv[3];
        int newStatus = handleStatusInput(argv[4]);

        task->setName(newName);
        task->setStatus(newStatus);
    }
    // If the user entered either a new name or a new status
    else if (argc == 4)
    {
        const std::string& arg = argv[3];
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
}

void handleRemoveTasks(Tasks& TO_DO, int argc, char* argv[])
{
    checkargc(argc, 3);

    int taskID = handleIDInput(argv[2]);

    Task* task = TO_DO.findTaskbyID(taskID);
    if (!task) {
        std::cerr << "No task with ID [" << taskID << "] was found" << "\n";
        exit(1);
    }

    // Store task info for confirmation message before removal
    int removedID = task->getID();
    std::string removedName = task->getName();

    try {
        TO_DO.removeTask(*task);
    }
    catch (const std::exception& ex) {
        std::cerr << "Error occurred while removing task: " << ex.what() << "\n";
        exit(1);
    }

    TO_DO.saveTasks();
    std::cout << "Task [" << removedID << "] \"" << removedName << "\" was removed successfully" << "\n";
}

void handleSearchTasks(Tasks& TO_DO, int argc, char* argv[])
{
    checkargc(argc, 3);

    const std::string& taskName = argv[2];
    std::vector<Task> vec = TO_DO.searchTasksByName(taskName);
    if (vec.empty()) {
        std::cout << "No tasks were found with the name \"" << taskName << "\".\n";
        exit(1);
    }

    printTasks(vec);
}

void handleHelpCommand(Tasks& TO_DO __attribute__((unused)), int argc, char* argv[])
{
    checkargc(argc, 2); // We expect 2 arguments

    std::cout << "Usage: " << argv[0] << " <command> [arguments]\n";
    std::cout << "commandsMap:\n";
    std::cout << "  add <task_name> <status>             - Add a new task (0 -> To-Do | 1 -> In Progress | 2 -> Completed)\n";
    std::cout << "  show <filter_flag>                   - Show all tasks (-t -> To-Do | -i -> In Progress | -c -> Completed)\n";
    std::cout << "  update <task_ID> <Name> <Status>     - Update a task\n";
    std::cout << "  remove <task_ID>                     - Removes a task\n";
    std::cout << "  search <task_name>                   - Search tasks by name\n";
    std::cout << "  --version (-v)                       - Show current version\n";
    std::cout << "  --help (-h)                          - Show help menu\n";
}

void handleVersionCommand(Tasks& TO_DO __attribute__((unused)), int argc, char* argv[] __attribute__((unused)))
{
    checkargc(argc, 2); // We expect 2 arguments

    std::cout << "Version: " << VERSION << "\n";
}
