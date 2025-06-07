#pragma once
#include <string>
//#include "utilis.hpp"

class Task
{
public:
    enum Priority { Low = 1, Medium , High};
    enum Status { To_Do = 1, In_Progress, Completed };
private:
    std::string m_name;
    int m_id;
    Status m_status;
    Priority m_priority;
public:
    Task(const std::string& name, Status status = To_Do, Priority priority = Low);

    const std::string& getName() const;
    int getStatus() const;
    int getID() const;
    Priority getPriority() const;

    void setName(const std::string& name);
    void setID(int id);
    void setStatus(Status status);
    void setPriority(Priority priority);

    bool operator==(const Task& other) const;
};
