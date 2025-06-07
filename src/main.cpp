#include "Tasks.hpp"
#include "Task.hpp"
//#include "utilis.hpp"

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

constexpr double VERSION = 1.5;

std::string_view statusToStr(int s);
std::string_view priorityToStr(Task::Priority priority);

int handleInputID(const char* argID);
Task::Status handleInputStatus(const char* argStatus);
Task::Priority handleInputPriority(const char* argPriority);

void checkargc(int argc, int min, int max);
void checkargc(int argc, int num);

void handleSaveTasks(Tasks& TO_DO);
void handleLoadTasks(Tasks& TO_DO);

std::vector<Task> filterTasks(const std::vector<Task>& allTasks, int filterStatus, int filterPriority);
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
    handleLoadTasks(TO_DO);

    std::unordered_map <std::string, std::function<void(Tasks&, int, char* [])>> commandsMap;

    commandsMap["add"] = handleAddTask;
    commandsMap["show"] = handleShowTasks;
    commandsMap["update"] = handleUpdateTasks;
    commandsMap["remove"] = handleRemoveTasks;
    commandsMap["rm"] = handleRemoveTasks;
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
    case Task::Status::To_Do: return "To-Do";
    case Task::Status::In_Progress: return "In Progress";
    case Task::Status::Completed: return "Completed";
    default: return "Unknown";
    }
}

std::string_view priorityToStr(Task::Priority priority)
{
    switch (priority) {
    case Task::Priority::High: return "High";
    case Task::Priority::Medium: return "Medium";
    case Task::Priority::Low: return "Low";
    default: return "Unknown";
    }
}

int handleInputID(const char* argID)
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

Task::Status handleInputStatus(const char* argStatus)
{
    Task::Status intStatus;

    try {
        intStatus = static_cast<Task::Status> (std::stoi(argStatus));
        if (intStatus < 1 || intStatus > 3) {
            throw std::out_of_range("Status must be 1, 2, or 3.");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Invalid status: " << e.what() << "\n";
        exit(1);
    }
    return intStatus;
}

Task::Priority handleInputPriority(const char* argPriority)
{
    Task::Priority priority;

    try {
        priority = static_cast<Task::Priority>(std::stoi(argPriority));

        if (priority < 1 || priority > 3) {
            throw std::out_of_range("Priority must be 1, 2, or 3.");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Invalid Priority: " << e.what() << "\n";
        exit(1);
    }
    return priority;
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
        std::cerr << "Invalid format. Use --help for info\n";
        exit(1);
    }
}

void handleSaveTasks(Tasks& TO_DO)
{
    try {
        TO_DO.saveTasks();
    }
    catch (const std::exception& ex) {
        std::cerr << "An Error occurred while saving " << "\n" << ex.what() << "\n";
        exit(1);
    }
}

void handleLoadTasks(Tasks& TO_DO)
{
    try {
        TO_DO.loadTasks();
    }
    catch (const std::runtime_error& ex) {
    }
    catch (const std::exception& ex) {
        std::cerr << "An Error occurred while loading " << "\n" << ex.what() << "\n";
        exit(1);
    }
}

std::vector<Task> filterTasks(const std::vector<Task>& allTasks, int filterStatus = -1, int filterPriority = -1)
{
    std::vector<Task> vec;
    vec.reserve(allTasks.size());

    if (filterPriority == -1) {
        std::copy_if(allTasks.begin(), allTasks.end(), std::back_inserter(vec), [&filterStatus, &filterPriority](const Task& task) {
            return (task.getStatus() == filterStatus);
            });
    }
    else if (filterStatus == -1) {
        std::copy_if(allTasks.begin(), allTasks.end(), std::back_inserter(vec), [&filterStatus, &filterPriority](const Task& task) {
            return (task.getPriority() == filterPriority);
            });
    }
    else if (filterStatus != -1 && filterPriority != -1) {
        std::copy_if(allTasks.begin(), allTasks.end(), std::back_inserter(vec), [&filterStatus, &filterPriority](const Task& task) {
            return (task.getStatus() == filterStatus && task.getPriority() == filterPriority);
            });
    }

    vec.shrink_to_fit();
    return vec;
}

void printTask(const Task& task)
{
    std::cout << "Task: " << task.getID() << ", " << task.getName() << ", [" << statusToStr(task.getStatus()) << "]\n";
}

void printTasks(const std::vector<Task>& tasksList)
{
    for (const auto& task : tasksList) {
        std::cout << "Task: "
            << task.getID() << ", "
            << task.getName() << ", ["
            << statusToStr(task.getStatus()) << "], ["
            << priorityToStr(task.getPriority()) << "]\n";
    }
}

void handleAddTask(Tasks& TO_DO, int argc, char* argv[])
{
    checkargc(argc, 3, 5); // We expect 3 to 4 arguments

    const std::string& taskName = argv[2];
    if (taskName.empty() || taskName.length() > 100) {
        std::cerr << "Invalid Input. Task name length should be from 1 to 100 characters" << "\n";
        exit(1);
    }

    int taskID;
    // The Default (Only task Name)
    if (argc == 3) {
        Task newTask(taskName);
        try {
            TO_DO.addTask(newTask);
            taskID = newTask.getID();
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what();
        }
    }
    // The user entered a task name and a status
    else if (argc == 4) {
        Task::Status taskStatus = handleInputStatus(argv[3]);
        Task newTask(taskName, taskStatus);
        try {
            TO_DO.addTask(newTask);
            taskID = newTask.getID();
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what();
        }
    }
    // The user entered task name, status and priority
    else if (argc == 5) {
        Task::Status taskStatus = handleInputStatus(argv[3]);
        Task::Priority taskPriority = handleInputPriority(argv[4]);
        Task newTask(taskName, taskStatus, taskPriority);
        try {
            TO_DO.addTask(newTask);
            taskID = newTask.getID();
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what();
        }
    }
    handleSaveTasks(TO_DO);
    std::cout << "Task " << taskID << " was added successfully" << "\n";
}

void handleShowTasks(Tasks& TO_DO, int argc, char* argv[])
{
    checkargc(argc, 2, 3); // We expect 2 or 3 arguments

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
        int statusFilter = -1;
        int priorityFilter = -1;

        if (filter == "-t") { statusFilter = Task::Status::To_Do; }
        else if (filter == "-i") { statusFilter = Task::Status::In_Progress; }
        else if (filter == "-c") { statusFilter = Task::Status::Completed; }
        else if (filter == "-l") { priorityFilter = Task::Priority::Low; }
        else if (filter == "-m") { priorityFilter = Task::Priority::Medium; }
        else if (filter == "-h") { priorityFilter = Task::Priority::High; }
        else {
            std::cerr << "Invalid Format. Check --help for info." << "\n";
            exit(1);
        }

        const std::vector<Task>& filteredTasks = filterTasks(allTasks, statusFilter, priorityFilter);
        if (filteredTasks.empty()) {
            std::cout << "No [" << statusToStr(statusFilter) << "] Tasks were found..." << "\n";
            exit(1);
        }
        printTasks(filteredTasks);
    }
}

void handleUpdateTasks(Tasks& TO_DO, int argc, char* argv[])
{
    checkargc(argc, 6); // We expect 4 to 5 arguments

    // Find the task by ID
    int taskID = handleInputID(argv[2]);
    Task* task = TO_DO.findTaskbyID(taskID);
    if (!task) {
        std::cerr << "ID [" << taskID << "] was not found." << std::endl;
        exit(1);
    }

    // If the user entered new name, new status and new priority
    if (argc == 6)
    {
        const std::string& newName = argv[3];
        Task::Status newStatus = handleInputStatus(argv[4]);
        Task::Priority newPriority = handleInputPriority(argv[5]);

        task->setName(newName);
        task->setStatus(newStatus);
        task->setPriority(newPriority);
    }
    // The following is under maintenance
    // // If the user entered either a new name or a new status
    // else if (argc == 4)
    // {
    //     const std::string& arg = argv[3];
    //     if (arg == "1" || arg == "2" || arg == "3")  // If the input was a new status
    //     {
    //         Task::Status newStatus = handleInputStatus(argv[3]);
    //         task->setStatus(newStatus);
    //     }
    //     else // If the input was a new name
    //     {
    //         std::string newName = arg;
    //         task->setName(newName);
    //     }
    //    }
    handleSaveTasks(TO_DO);
    std::cout << "Task [" << taskID << "] was updated successfully" << "\n";
}

void handleRemoveTasks(Tasks& TO_DO, int argc, char* argv[])
{
    checkargc(argc, 3);

    int taskID = handleInputID(argv[2]);

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
    std::cout << "Commands:\n";
    std::cout << "  add <task_name> [status] [priority]  - Add a new task (1 -> To-Do         | 2 -> In Progress         | 3 -> Completed)\n";
    std::cout << "                                                        (1 -> Low Priority  | 2 -> Medium Priority     | 3 -> High Priority)\n";
    std::cout << "  show [filter_flag]                   - Show all tasks (-t -> To-Do Status | -i -> In Progress Status | -c -> Completed Status)\n";
    std::cout << "                                                        (-l -> Low Priority | -m -> Medium Priority    | -h -> High Priority)\n";
    std::cout << "  update <task_ID> <Name> <Status> <Priority> - Update a task\n";
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
