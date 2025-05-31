#pragma once
#include <string>

class Task
{
private:
    std::string m_name;
    int m_status;
public:
    Task(const std::string& name, int status);

    const std::string& getName() const;
    int getStatus() const;
};
