#include "Task.hpp"

Task::Task(const std::string& name, int status)
    : m_name(name), m_status(status)
{
}

const std::string& Task::getName() const
{
    return m_name;
}

int Task::getStatus() const
{
    return m_status;
}
