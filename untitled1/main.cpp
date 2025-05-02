#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket1, new_socket2;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Создаем сокет
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Привязываем сокет к порту
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Слушаем входящие соединения
    if (listen(server_fd, 2) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Ожидание клиентов..." << std::endl;

    // Принимаем два клиента
    new_socket1 = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket1 < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "Клиент 1 подключен." << std::endl;

    new_socket2 = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket2 < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "Клиент 2 подключен." << std::endl;

    while (true) {
        // Чтение сообщения от клиента 1
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(new_socket1, buffer, BUFFER_SIZE);
        if (valread > 0) {
            std::cout << "Клиент 1: " << buffer << std::endl;
            send(new_socket2, buffer, valread, 0); // Отправка клиенту 2
        }

        // Чтение сообщения от клиента 2
        memset(buffer, 0, BUFFER_SIZE);
        valread = read(new_socket2, buffer, BUFFER_SIZE);
        if (valread > 0) {
            std::cout << "Клиент 2: " << buffer << std::endl;
            send(new_socket1, buffer, valread, 0); // Отправка клиенту 1
        }
    }

    close(new_socket1);
    close(new_socket2);
    close(server_fd);
    return 0;
}