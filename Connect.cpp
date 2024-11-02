/**
 * @authentication_file Connect.cpp
 */
#include "Connect.h"
#include "md5.h"

// Заливка текста при выводе в консоль: https://stackoverflow.com/a/21786287/21356333 
string CSI = "\x1B[";

/**
 * @brief Получение из файла логина и пароля.
 */

void Connect::GetLoginPassword(){
    // Открытие файла для аутентификации
    
    ifstream authentication_file(name_auto_file);
    if(!authentication_file.is_open()){
        authentication_file.close();
        throw error_proj(std::string("fun:GetLoginPassword, param:name_auto_file.\n" + 
                                    CSI+"31;40m" + "Ошибка отрытия файла для аутентификации! Содержание \'name_auto_file\': " + CSI+"0m" + name_auto_file));
    }

    string line;
    if(authentication_file.peek() == EOF){
        authentication_file.close();
        throw error_proj(std::string("fun:GetLoginPassword, param:name_auto_file.\n"+
                                    CSI+"31;40m"+"Ошибка отрытия файла для аутентификации! Файл пуст!"+CSI+"0m"));
    }

    getline(authentication_file, line);              // Считывание строки из файла
    int spaceIndex = line.find(" ");  // Поиск индекса первого пробела
    string usernamePart = line.substr(0, spaceIndex);  // Извлечение имени пользователя до пробела
    string passwordPart = line.erase(0, spaceIndex + 1);  // Извлечение пароля после пробела
    username = usernamePart;          // Присвоение имяени пользователя
    password = passwordPart;          // Присвоение пароля


}

/**
* @brief Взаимодействие с сервером.
* @param ip_addr адрес сервера.
* @param port порт сервера.
* @throw error_proj при возникновении ошибки.
*/


int Connect::Connect_to_server(const char *ip_addr, int port) {
    GetLoginPassword();

    // Открытие бинарного файла для чтения векторов
    ifstream input_file(name_original_file, ios::binary);
    if (!input_file.is_open()) {
        throw error_proj("Ошибка открытия файла с векторами!");
    }

    // Открытие текстового файла для записи результатов
    ofstream output_file(name_result_file);
    if (!output_file.is_open()) {
        throw error_proj("Ошибка открытия файла для записи результатов!");
    }

    // Проверка соединения
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        throw error_proj("Ошибка создания сокета!");
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip_addr);

    if (connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        throw error_proj("Ошибка подключения к серверу!");
    }

    // Отправка логина клиента
    char buffer[1024];
    strcpy(buffer, username.c_str());
    send(socket_fd, buffer, username.length(), 0);

    // Получение SALT или ERR
    int bytes_read = recv(socket_fd, buffer, sizeof(buffer), 0);
    string salt(buffer, bytes_read);
    if (salt == "ERR") {
        close(socket_fd);
        throw error_proj("Ошибка идентификации!");
    }

    // Вычисление и отправка хеша от SALT+PASSWORD
    string hashedPassword = MD5_hash(salt + password);
    strcpy(buffer, hashedPassword.c_str());
    send(socket_fd, buffer, hashedPassword.length(), 0);

    // Получение ответа об аутентификации
    bytes_read = recv(socket_fd, buffer, sizeof(buffer), 0);
    string isAuthenticated(buffer, bytes_read);
    if (isAuthenticated == "ERR") {
        close(socket_fd);
        throw error_proj("Ошибка аутентификации!");
    }

    // Чтение количества векторов
    int len;
    input_file.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (input_file.fail() || len <= 0) {
        throw error_proj("Не удалось прочитать количество векторов или некорректное значение!");
    }

    cout << "Количество векторов: " << len << endl;
    send(socket_fd, &len, sizeof(len), 0);

    // Чтение и отправка каждого вектора
    for (int l = 0; l < len; l++) {
        int vectorLength;
        input_file.read(reinterpret_cast<char*>(&vectorLength), sizeof(vectorLength));
        if (input_file.fail() || vectorLength <= 0) {
            throw error_proj("Не удалось прочитать размер вектора или некорректное значение!");
        }

        vector<uint32_t> vectorArray(vectorLength);
        input_file.read(reinterpret_cast<char*>(vectorArray.data()), vectorLength * sizeof(uint32_t));
        if (input_file.fail()) {
            throw error_proj("Ошибка чтения вектора из файла!");
        }

        // Отправка длины и вектора на сервер
        send(socket_fd, &vectorLength, sizeof(vectorLength), 0);
        send(socket_fd, vectorArray.data(), vectorLength * sizeof(uint32_t), 0);

        uint32_t sum = 0;
        recv(socket_fd, &sum, sizeof(sum), 0); // Получение суммы от сервера
        cout << "Sum(" << l + 1 << "): " << sum << endl;

        // Запись суммы в текстовый файл
        output_file << "Sum(" << l + 1 << "): " << sum << endl;
    }

    // Закрытие файлов и соединения
    input_file.close();
    output_file.close();
    cout << "Результат сохранен в файле: " << name_result_file << endl;
    close(socket_fd);

    return 0;
}
