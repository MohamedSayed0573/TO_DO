#include "Task.hpp"

Task::Task(const std::string& name, Status status, Priority priority)
    : m_name(name), m_id(0), m_status(status), m_priority(priority)
{
}

const std::string& Task::getName() const { return m_name; }

int Task::getStatus() const { return m_status; }

int Task::getID() const { return m_id; }

Task::Priority Task::getPriority() const { return m_priority; }

void Task::setName(const std::string& name) { m_name = name; }

void Task::setID(int id) { m_id = id; }

void Task::setStatus(Status status) { m_status = status; }

void Task::setPriority(Priority priority) { m_priority = priority; }

bool Task::operator==(const Task& other) const
{
    return m_id == other.m_id;
}
