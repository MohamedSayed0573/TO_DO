#include "Task.hpp"


Task::Task(const std::string& name, int status)
    : m_name(name), m_status(status), m_id(0)
{
}

const std::string& Task::getName() const { return m_name; }

int Task::getStatus() const { return m_status; }

int Task::getID() const { return m_id; }

void Task::setName(const std::string& name) { m_name = name; }

void Task::setStatus(int status) { m_status = status; }

void Task::setID(int id) { m_id = id; }

bool Task::operator==(const Task& other) const
{
    return m_id == other.m_id;
}
