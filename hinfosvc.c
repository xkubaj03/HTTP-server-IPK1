#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define LEN 1024
#define LENS 20
#define DEBUG 0

int main (int argc, char* argv[])
{
    int Port = 0;
    if(argc > 1)
    {
        Port = atoi(argv[1]);
        if(Port == 0)
        {
            printf("Špatný port\n");
            exit(-1);
        }
    }else
    {
        printf("Zadej port\n");
        exit(-1);
    }
    if(DEBUG) printf("Port: %d\n", Port);
    char CPU[LEN];
    char Hostname[LEN];
    char CpuUsage[LEN];
    char pomUsage[LENS];
    char command[60];
    unsigned long long int CpuArr[2][8];
    unsigned long long int PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, totald, idled, CpuFinal;
    double CPU_Percentage;
    FILE *fp;
    int rc;
    char buffer[LEN];

    int socket_desc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (socket_desc == -1)
    {
        printf("Could not create socket");
        return -1;
    }


    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(Port);
    address.sin_addr.s_addr = INADDR_ANY;
    int addrlen = sizeof(address);


    int on = 1;
    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&on, sizeof(on));

    if ((rc = bind(socket_desc, (struct sockaddr*)&address, sizeof(address))) < 0)
    {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }
    if ((rc = listen(socket_desc, 1)) < 0)
    {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }


    if(DEBUG) printf("Pred while\n");
    while(1)
    {   sleep(1);
        if(DEBUG) printf("while\n");
        int comm_socket = accept(socket_desc, (struct sockaddr*)&address, (socklen_t *)(&addrlen));
        if(DEBUG) printf("comm_socket: %d\n", comm_socket);
        if (comm_socket > 0)
        {
            if(DEBUG) printf("Začátek pruchodu, comm_socket: %d\n", comm_socket);
            int res = recv(comm_socket, buffer, LEN, 0);
            if (res > 0)
            {
                buffer[res] = '\0';
                if(DEBUG) printf("%s",buffer);
                char *request = strtok(buffer, " ");
                request = strtok(NULL, " ");
                if(DEBUG) printf("%s",request);

                int flags = fcntl(comm_socket, F_GETFL, 0);
                rc = fcntl(comm_socket, F_SETFL, flags | O_NONBLOCK);
                if (rc < 0)
                {
                    perror("ERROR: fcntl");
                    exit(EXIT_FAILURE);
                }

                if(!strcmp(request, "/hostname"))
                {
                    if(DEBUG) printf("Hostname rozpoznáno");
                    //Hostname
                    fp = popen("cat /proc/sys/kernel/hostname" , "r");
                    if(fp == NULL) return -1;

                    fgets(Hostname, LEN, fp);

                    if(DEBUG) printf("Obsah Hostname: %s", Hostname);
                    pclose(fp);
                    char response[LEN] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n";
                    strcat(response, Hostname);
                    send(comm_socket, response, strlen(response), 0);
                    if(DEBUG) printf("Zpráva odeslána\n");
                }else if(!strcmp(request, "/cpu-name"))
                {
                    //CPU info
                    fp = popen("cat /proc/cpuinfo | grep \"model name\" | head -n 1 | awk -F \": \" '{print $2}'" , "r");
                    if(fp == NULL) return -1;

                    fgets(CPU, LEN, fp);

                    if(DEBUG) printf("Obsah CPU: %s", CPU);
                    pclose(fp);
                    char response[LEN] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n";
                    strcat(response, CPU);
                    send(comm_socket, response, strlen(response), 0);

                }else if(!strcmp(request, "/load"))
                {
                    //Cpu usage
                    fp = popen("cat /proc/stat | awk 'FNR == 1 {print}'", "r");
                    if(fp == NULL) return -1;
                    fgets(CpuUsage, LEN, fp);


                    for(int i = 0; i < 2; i++)
                    {
                        for(int j = 0; j < 8; j++)
                        {
                            sprintf(command, "cat /proc/stat | awk -v COL=%d 'FNR == 1 {print $COL}'", (j + 2));
                            fp = popen(command, "r");
                            if(fp == NULL) return -1;
                            fgets(pomUsage, LENS, fp);
                            if(DEBUG) printf("číslo: %s", pomUsage);
                            CpuArr[i][j] = atoll(pomUsage);
                            if(DEBUG) printf("Konvertované číslo: %lld\n", CpuArr[i][j]);
                        }
                        if(DEBUG) printf("Delay-----------------------------------\n");
                        if(!i) sleep(1.0);
                    }

                    PrevIdle = CpuArr[0][3] + CpuArr[0][4];
                    Idle = CpuArr[1][3] + CpuArr[1][4];

                    PrevNonIdle = CpuArr[0][0] + CpuArr[0][1] + CpuArr[0][2] + CpuArr[0][5] + CpuArr[0][6] + CpuArr[0][7];
                    NonIdle = CpuArr[1][0] + CpuArr[1][1] + CpuArr[1][2] + CpuArr[1][5] + CpuArr[1][6] + CpuArr[1][7];

                    PrevTotal = PrevIdle + PrevNonIdle;
                    Total = Idle + NonIdle;

                    totald = Total - PrevTotal;
                    idled = Idle - PrevIdle;

                    CPU_Percentage = (double)(totald - idled) / totald;
                    CpuFinal = (int)(CPU_Percentage * 100.0);
                    if(DEBUG) printf("Obsah CpuUSage: %s", CpuUsage);
                    pclose(fp);
                    if(DEBUG) printf("Cpu percentage: %f%% \n", CPU_Percentage);
                    if(DEBUG) printf("Cpu percentage (int): %lld%%", CpuFinal);
                    char response[LEN];
                    sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n%lld%%\n", CpuFinal);
                    send(comm_socket, response, strlen(response), 0);
                }else
                {
                    char response[LEN] = "HTTP/1.1 400 OK\r\nContent-Type: text/plain;\r\n\r\n400: Bad request\n";
                    send(comm_socket, response, strlen(response), 0);
                }
                close(comm_socket);
            }

        }
        if(DEBUG) printf("Konec pruchodu\n");
    }

    return 0;
}
