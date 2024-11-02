#include <iostream>
#include <string>
#include <getopt.h>
#include <stdlib.h> // Для функции strtol()
#include <unistd.h>
#include <limits.h>
#include "Connect.h"

using namespace std;
/** 
* @file main.cpp
* @brief Главный модуль программы для получения параметров от пользователя
* @param rez переменная для работы с параметрами командной строки
* @param optarg переменная для получения парметров командной строки
*/

// Заливка текста при выводе в консоль: https://stackoverflow.com/a/21786287/21356333 
const string CSI = "\x1B[";

int main (int argc, char *argv[]){

	const char* short_options = "hi:p::o:r:c::"; // ":" - принимает аргумент, "::" - принимает аргумент, но является необязательным   
                                                 // -c<File_name> предаётся без пробела. Подробнее: https://stackoverflow.com/questions/26431682/why-cant-i-have-a-space-between-option-and-optional-argument-using-getopt

	const struct option long_options[] = {
		{ "help", no_argument, nullptr, 'h' },
		{ "ip", required_argument, nullptr, 'i' },
        { "port", optional_argument, nullptr, 'p' },
		{ "fileoriginal", required_argument, nullptr, 'o' },
        { "fileresult", required_argument, nullptr, 'r' },
        { "config", optional_argument, nullptr, 'c' },
		{ nullptr, 0, nullptr, 0 }
	};

	int rez;
	int option_index;

    string helpMessage = "1 параметр -h или --help для справки";
    string ipAddressParam = "2 параметр -i или --ip сетевой адрес сервера ОБЯЗАТЕЛЬНЫЙ";
    string portParam = "3 параметр -p или --port порт сервера необязательный";
    string inputFileParam = "4 параметр -o или --fileoriginal файл с векторами ОБЯЗАТЕЛЬНЫЙ";
    string resultFileParam = "5 параметр -r или --fileresult файл-результат ОБЯЗАТЕЛЬНЫЙ";
    string configFileParam = "6 параметр -c или --config файл-аутентификации необязательный";


    // Класс для взаимодействия с клиент-сервером
    Connect MyConnect;

    if(argc == 1){
        cout << "Данная программа клиент-сервер" << endl;
        cout << helpMessage << endl;
        cout << ipAddressParam << endl;
        cout << portParam << endl;
        cout << inputFileParam << endl;
        cout << resultFileParam << endl;
        cout << configFileParam << endl;
        return 1;
    }

    // Определения деректории в которой выполняется программа
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    // Обработка переданных аргументов
    char *ip_addr;
    int port;
	while ((rez=getopt_long(argc, argv, short_options, long_options, &option_index))!=-1){

		switch(rez){
			case 'h': {
				cout << "Данная программа клиент-сервер" << endl;
                cout << helpMessage << endl;
                cout << ipAddressParam << endl;
                cout << portParam << endl;
                cout << inputFileParam << endl;
                cout << resultFileParam << endl;
                cout << configFileParam << endl;
				return 1;
			};
			case 'i': {
				if (optarg==nullptr){
                    cout << ipAddressParam << endl;
                    return 2;
                }
				else{
                    ip_addr = optarg;
                    cout << "Переданно IP: " << optarg << " \n";
                }
				break;
			};
	
			case 'p': {
				if (optarg!=nullptr){
                    port = atoi(optarg);
                    cout << "Переданно PORT: " << optarg << " \n";
                }
				else{
                    port = 33333;
                    cout << CSI+"33;40m" + "PORT Поумолчанию: " << port << CSI+"0m" + " \n"; 
                }
				break;
			};
			case 'o': {
				if (optarg==nullptr){
                    cout << inputFileParam << endl;
                    return 3;
                }
				else{
                    MyConnect.name_original_file = string(optarg);
                    cout << "Переданно name_original_file: " << optarg << " \n";
                }
				break;
			};

            case 'r': {
				if (optarg==nullptr){
                    cout << resultFileParam << endl;
                    return 4;
                }
				else{
                    MyConnect.name_result_file = string(optarg);
                    cout << "Переданно name_result_file: " << optarg << " \n";
                }
				break;
			};

            case 'c': {
				if (optarg!=nullptr){
                    MyConnect.name_auto_file = string(optarg);
                    cout << "Переданно name_auto_file: " << optarg << " \n";
                }
				else{
                    MyConnect.name_auto_file = string(cwd) + "/config/vclient.conf";
                    cout << CSI+"33;40m" + "name_auto_file Поумолчанию: " << MyConnect.name_auto_file << CSI+"0m" + " \n"; 
                    // cout << CSI+"30;43m" + "name_auto_file optarg received: " << (optarg != nullptr ? optarg : "nullptr") << CSI+"0m" << endl;
    
                }
				break;
			};

            case '?': default: {
				cout << "Таких опций нет. Запустите программу с параметром h" << endl;
				return 5;
			};
		};
	};

    // Запуск
    // ./server -k uint32_t
    // ./client -i 127.0.0.1 -p33333 -o my.bin -r resultat.txt -c./config/vclient.conf
    MyConnect.Connect_to_server(ip_addr, port);


	return 0;
};
