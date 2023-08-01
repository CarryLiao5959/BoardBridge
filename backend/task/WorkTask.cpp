#include "WorkTask.h"
#include "Logger.h"
#include "Singleton.h"
using namespace yazi::util;
using namespace yazi::task;
using namespace yazi::socket;
#include <sstream>
using std::ostringstream;
#include <fstream>

void WorkTask::run() {
    debug("WorkTask running");
    SocketHandler *handler = Singleton<SocketHandler>::instance();
    Socket *socket = static_cast<Socket *>(m_data);

    MsgHead msg_head;
    memset(&msg_head, 0, sizeof(msg_head));
    int len = socket->recv((char *)(&msg_head), sizeof(msg_head));
    debug("recv len: %d", len);
    debug("recv MsgHead.flag: %s", msg_head.flag);
    debug("recv MsgHead.cmd: %d", msg_head.cmd);
    debug("recv MsgHead.len: %d", msg_head.len);

    if (len == 0) {
        error("socket closed by peer");
        handler->remove(socket);
        return;
    }
    if (len == -1) {
        switch (errno) {
        case EWOULDBLOCK:
            error("socket recv len: %d, error msg: EWOULDBLOCK errno: %d", len, errno);
            handler->attach(socket);
            return;
        case EINTR:
            error("socket recv len: %d, error msg: EINTR errno: %d", len, errno);
            handler->attach(socket);
            return;
        }
    }
    if (len != sizeof(msg_head)) {
        error("recv msg head error length: %d, errno: %d", len, errno);
        handler->remove(socket);
        return;
    }

    switch (msg_head.cmd) {
    case 1:
        echo((int)(msg_head.len));
        break;
    case 2:
        text();
        break;
    case 3:
        img();
        break;
    case 4:
        gif();
        break;
    case 5:
        mp4();
        break;
    default:
        error("Unknown command");
        break;
    }

    handler->attach(socket);
}

void WorkTask::mp4() {
    debug("mp4");
    SocketHandler *handler = Singleton<SocketHandler>::instance();
    Socket *socket = static_cast<Socket *>(m_data);

    string filename = "file/mp4/android.mp4";
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        error("could not open %s", filename.c_str());
        handler->remove(socket);
        return;
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char buf[buf_size[5]];
    memset(buf, 0, buf_size[5]);

    int cnt = 1;
    while (!file.eof()) {
        file.read(buf, buf_size[5]);
        std::streamsize count = file.gcount();
        socket->send(buf, count);
        usleep(100);
        if(cnt%100==0){
            debug("send package %d", cnt);
        }
        cnt++;
        memset(buf, 0, buf_size[5]);
    }
    debug("send package %d", cnt);
    debug("img sent success");

    file.close();
}

void WorkTask::echo(int msg_head_len) {
    debug("echo");
    SocketHandler *handler = Singleton<SocketHandler>::instance();
    Socket *socket = static_cast<Socket *>(m_data);

    char buf[buf_size[1]];
    // memset(buf, 0, recv_buff_size);
    // memcpy(buf, "OK", 2);
    // socket->send(buf, 2);
    // usleep(1);
    memset(buf, 0, buf_size[1]);
    int len = socket->recv(buf, msg_head_len);

    if (len == -1) {
        switch (errno) {
        case EWOULDBLOCK:
            error("socket recv len: %d, error msg: EWOULDBLOCK errno: %d", len, errno);
            handler->attach(socket);
            return;
        case EINTR:
            error("socket recv len: %d, error msg: EINTR errno: %d", len, errno);
            handler->attach(socket);
            return;
        }
    }
    if (len != msg_head_len) {
        error("recv msg body error length: %d, body: %s, errno: %d", len, buf, errno);
        handler->remove(socket);
        return;
    }

    info("recv len: %d, msg data: %s", len, buf);

    memset(buf, 0, buf_size[1]);
    memcpy(buf, "hello client", 13);
    socket->send(buf, 13);
}

void WorkTask::text() {
    debug("text");
    SocketHandler *handler = Singleton<SocketHandler>::instance();
    Socket *socket = static_cast<Socket *>(m_data);

    string filename = "file/txt/file.txt";
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        error("could not open %s",filename.c_str());
        handler->remove(socket);
        return;
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char buf[buf_size[2]];
    memset(buf, 0, buf_size[2]);

    int cnt = 1;
    while (!file.eof()) {
        file.read(buf, buf_size[2]);
        std::streamsize count = file.gcount();
        socket->send(buf, count);
        // usleep(100);
        debug("send package %d", cnt++);
        memset(buf, 0, buf_size[2]);
    }
    memset(buf, 0, buf_size[2]);
    socket->send(buf, 0);
    debug("send package %d", cnt++);
    debug("text sent success");

    file.close();
}

void WorkTask::img() {
    debug("img");
    SocketHandler *handler = Singleton<SocketHandler>::instance();
    Socket *socket = static_cast<Socket *>(m_data);

    string filename = "file/img/road.jpg";
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        error("could not open %s", filename.c_str());
        handler->remove(socket);
        return;
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char buf[buf_size[3]];
    memset(buf, 0, buf_size[3]);

    int cnt = 1;
    while (!file.eof()) {
        file.read(buf, buf_size[3]);
        std::streamsize count = file.gcount();
        socket->send(buf, count);
        usleep(100);
        debug("send package %d", cnt++);
        memset(buf, 0, buf_size[3]);
    }
    memset(buf, 0, buf_size[3]);
    socket->send(buf, 0);
    debug("send package %d", cnt++);
    debug("img sent success");

    file.close();
}

void WorkTask::gif() {
    debug("gif");
    SocketHandler *handler = Singleton<SocketHandler>::instance();
    Socket *socket = static_cast<Socket *>(m_data);

    string filename = "file/gif/cheer.gif";
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        error("could not open %s", filename.c_str());
        handler->remove(socket);
        return;
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char buf[buf_size[4]];
    memset(buf, 0, buf_size[4]);

    int cnt = 1;
    while (!file.eof()) {
        file.read(buf, buf_size[4]);
        std::streamsize count = file.gcount();
        socket->send(buf, count);
        usleep(100);
        debug("send package %d", cnt++);
        memset(buf, 0, buf_size[4]);
    }
    memset(buf, 0, buf_size[4]);
    socket->send(buf, 0);
    debug("send package %d", cnt++);
    debug("img sent success");

    file.close();
}