#include "client.h"

int main() {
    const char *serv_ip = "127.0.0.1";
    const int serv_port = 8080;

    char buf[1024];

    while (1) {
        int cli_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (cli_sock < 0) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(serv_port);
        inet_pton(AF_INET, serv_ip, &serv_addr.sin_addr);

        if (connect(cli_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            perror("connection failed");
            close(cli_sock);
            exit(EXIT_FAILURE);
        }

        printf("press the info cmd:\n[1]sysinfo\n[2]recent proc info\n[3]proc info\n[4]backup\n[5]kill\n");
        int cmd, cmd_detail;
        scanf("%d", &cmd);
        if (cmd == 0) {
            break;
        }
        if (cmd == 5) {
            printf("press the killed pid:\n");
            scanf("%d", &cmd_detail);
        }

        struct InfoPackage package;
        memset(&package, 0, sizeof(package));
        package.cmd_type = cmd;
        package.cmd_detail = cmd_detail;
        strcpy(package.content_buf, cmd_typename[cmd]);

        send(cli_sock, &package, sizeof(package), 0);
        printf("send success: %s\n", package.content_buf);

        memset(&package, 0, sizeof(package));

        recv(cli_sock, &package, sizeof(package), 0);
        printf("recv from ip[%s]:\n %s\n", serv_ip, package.content_buf);

        FILE *json_file = fopen("/home/carry/BoardBridge/client/file/output.json", "w");
        if (json_file == NULL) {
            perror("Failed to open file");
            return 1;
        }
        fputs(package.content_buf, json_file);
        fclose(json_file);

        close(cli_sock);
    }

    return 0;
}