#pragma once
#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>

using namespace std;

/** 
* @brief Класс для подключения к серверу
* @param ip_addr адрес сервера
* @param port порт сервера
* @param name_original_file имя файла для считывания векторов
* @param name_result_file имя файла для записи суммы векторов
* @param name_auto_file имя файла, в котором храняться логины и пароли клиентов
* @param username логин клиента
* @param password пароль клиента
*/

class Connect{
    public:
        int Connect_to_server(const char *ip_addr, int port);
        void GetLoginPassword();
        char *ip_addr;
        char *port;
        string name_original_file;
        string name_result_file;
        string name_auto_file;
        string username;
        string password;
};  


/** @brief Класс обработки ошибок error_proj
* @details Выводит сообщения об ошибках
*/
class error_proj: public std::invalid_argument
{
public:
	explicit error_proj (const std::string& what_arg):
		std::invalid_argument(what_arg) {}
	explicit error_proj (const char* what_arg):
		std::invalid_argument(what_arg) {}
};

