#include "Task.h"
using namespace bb::task;
#include "Autolock.h"
using namespace bb::thread;

Task::Task() : m_data(nullptr) {
}

Task::Task(void *data) : m_data(data){
}

Task::~Task() {
}

void *Task::get_data() {
    Autolock lock(&m_mutex);
    return m_data;
}

void Task::set_data(void *data) {
    Autolock lock(&m_mutex);
    m_data = data;
}

void Task::set_cmd(int cmd){
    Autolock lock(&m_mutex);
    m_cmd = cmd;
}