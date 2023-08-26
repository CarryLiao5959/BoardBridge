#include "Select.h"
using namespace bb::select;

Select::Select() {
    clear_set();
}

Select::Select(int max_fd) : m_max_fd(max_fd) {
    clear_set();
}

Select::~Select() {}

int Select::select() {
    m_tmpset = m_rdset;
    int infds = ::select(m_max_fd + 1, &m_tmpset, NULL, NULL, NULL);
    if (infds == -1) {
        printf("select fail errno:%d errstr:%s\n", errno, strerror(errno));
        exit(1);
    }
    else if (infds == 0) {
        printf("select timeout\n");
    }
    return infds;
}

bool Select::isset(int fd, bool tmp = false) {
    int ret;
    if (tmp) {

        ret = FD_ISSET(fd, &m_tmpset);
    }
    else {
        ret = FD_ISSET(fd, &m_rdset);
    }
    if (ret == 0) {
        return false;
    }
    return true;
}

void Select::clear_set() {
    FD_ZERO(&m_rdset);
}

void Select::add_fd(int fd) {
    FD_SET(fd, &m_rdset);
    if (fd > m_max_fd) {
        m_max_fd = fd;
    }
}

void Select::add_fd(Socket *socket) {
    int fd = socket->get_fd();
    FD_SET(fd, &m_rdset);
    if (fd > m_max_fd) {
        m_max_fd = fd;
    }
}

void Select::rm_fd(int fd) {
    close(fd);
    FD_CLR(fd, &m_rdset);
    if (fd == m_max_fd) {
        for (int i = m_max_fd; i > 0; i--) {
            if (isset(fd)) {
                m_max_fd = i;
                break;
            }
        }
    }
    printf("max_fd=%d\n", m_max_fd);
}

void Select::rm_fd(Socket *socket) {
    int fd = socket->get_fd();
    close(fd);
    FD_CLR(fd, &m_rdset);
    if (fd == m_max_fd) {
        for (int i = m_max_fd; i > 0; i--) {
            if (isset(fd)) {
                m_max_fd = i;
                break;
            }
        }
    }
    printf("max_fd=%d\n", m_max_fd);
}

void Select::set_max_fd(int max_fd) {
    m_max_fd = max_fd;
}

int Select::get_max_fd() {
    return m_max_fd;
}
