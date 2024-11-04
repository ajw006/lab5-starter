#include "http-server.h"
#include <string.h>

char const HTTP_404_NOT_FOUND[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
char const HTTP_200_OK[] = "HTTP/1.1 200 OK \r\nContent-Tpe: text/plain\r\n\n";
// "\r" character code for windows write new line in "inspect"
// Sometimes will need to do "\r\n" to do http request

void handle_404(int client_sock, char *path)  {
    printf("SERVER LOG: Got request for unrecognized path \"%s\"\n", path);

    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "Error 404:\r\nUnrecognized path \"%s\"\r\n", path);
    // snprintf includes a null-terminator

    // TODO: send response back to client?
    write(client_sock, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND));
    write(client_sock, response_buff, strlen(response_buff));
}

void handle_root(int client_sock)
{
    char message[] = "Get http 200 ok response, current number to come\n";
    char response_buff(BUFFER_SIZE);
    snprintf(message, BUFFER_SIZE, "Current number: %d\n", num);
    write(client_sock, HTTP_200_OK, strlen(HTTP_200_OK));
    write(client_sock, message, strlen(message));
}

void handle_response(char *request, int client_sock) {
    char path[256];

    printf("\nSERVER LOG: Got request: \"%s\"\n", request);

    // Parse the path out of the request line (limit buffer size; sscanf null-terminates)
    if (sscanf(request, "GET %255s", path) != 1) 
    //IN-CLASS NOTE: Read up to 255 string charactr and put into 'path'
    {
        printf("Invalid request line\n");
        return;
    }

    /*
    "/" - shows * current number: ____"
    "/increment" - adds 1 to number and shows "Incremented to: "
    
    //How to write the if statements to detect which path we have
    //IDEAS:
    strcmp(path, things like "/")
    */
    if (strcmp(path, "/") == 0)
    //Set equal 0 because strcmp compares elements in 2 arrays and if the same, returns 0
    //Therefore the if statement contains '== 0'
    {
        handle_root(client_sock, path);
        return;
    }
    else 
    {
        handle_404(client_sock, path);
    }

    /*
    strstr if there might be shared prefixes, like looking for "/post" in the "path"?
    SAVE STRSTR FOR LATER
   */

    handle_404(client_sock, path);
}

int main(int argc, char *argv[]) {
    int port = 0;
    if(argc >= 2) { // if called with a port number, use that
        port = atoi(argv[1]);
    }

    start_server(&handle_response, port);
}
