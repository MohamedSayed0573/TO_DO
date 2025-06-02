#pragma once
#include <string>

class Task
{
private:
    std::string m_name;
    int m_status;
    int m_id;
public:
    Task(const std::string& name, int status = 0);

    const std::string& getName() const;
    int getStatus() const;
    int getID() const;
    
    void setName(const std::string& name);
    void setStatus(int status);
    void setID(int id);

    bool operator==(const Task& other) const;
};
