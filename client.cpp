#include "CSocket.h"
#include <iostream>

int main()
{
    const string serv_ip="127.0.0.1";
    const int serv_port=5005;

    CSocket* socket=new CSocket(serv_ip,serv_port);
    int cli_sock = socket->sock_init();
    socket->set_recv_timeout(10);
    socket->set_send_timeout(10);
    socket->sock_conn();

    char buf[1024];
    
    while(1){
        printf("press the info cmd: 1:sysinfo, 2:proc info\n");
        int cmd;
        cin>>cmd;
        strcpy(buf, to_string(cmd).c_str());
        send(cli_sock, buf, sizeof(buf), 0);
        printf("send success: %s\n", buf);
        
        memset(buf, 0, sizeof(buf));
        recv(cli_sock, buf, sizeof(buf), 0);
        printf("recv from ip[%s]:\n %s\n", serv_ip.c_str(), buf);
        memset(buf, 0, sizeof(buf));
    }

    close(cli_sock);
    return 0;
}