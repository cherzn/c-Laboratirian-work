#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Создаем сокет
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Ошибка создания сокета" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Преобразуем IPv4 и IPv6 адреса из текстового формата в бинарный
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cout << "Неверный адрес" << std::endl;
        return -1;
    }

    // Подключаемся к серверу
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Ошибка подключения" << std::endl;
        return -1;
    }

    while (true) {
        std::cout << "Введите сообщение: ";
        std::cin.getline(buffer, BUFFER_SIZE);

        send(sock, buffer, strlen(buffer), 0); // Отправка сообщения на сервер

        memset(buffer, 0, BUFFER_SIZE); // Очистка буфера
        read(sock, buffer, BUFFER_SIZE); // Чтение ответа от сервера
        std::cout << "Ответ от сервера: " << buffer << std::endl;
    }

    close(sock);
    return 0;
}