#include <UnitTest++/UnitTest++.h>
#include "Connect.h"

using namespace std;

struct Cons_fix {
	Connect * p;
	Cons_fix() {
		p = new Connect();
	}
	~Cons_fix() {
		delete p;
	}
};

SUITE(Connect_to_server)
{
	TEST_FIXTURE(Cons_fix, NormalTestCase){
		// 1 Удачный сценарий
		p->name_original_file = "/home/stud/Desktop/client32_bin-txt/my.bin";
		p->name_result_file = "/home/stud/Desktop/client32_bin-txt/resultat.txt";
		p->name_auto_file = "/home/stud/Desktop/client32_bin-txt/config/vclient.conf";
		p->Connect_to_server("127.0.0.1", 33333);
		CHECK(true);	
	}

	TEST_FIXTURE(Cons_fix, InvalidServerAddress){
		// 2 Подключение к серверу. Введен не верный адрес.
		p->name_original_file = "/home/stud/Desktop/client32_bin-txt/my.bin";
		p->name_result_file = "/home/stud/Desktop/client32_bin-txt/resultat.txt";
		p->name_auto_file = "/home/stud/Desktop/client32_bin-txt/config/vclient.conf";
		CHECK_THROW(p->Connect_to_server("999.1.1.1", 33333), error_proj);
			
	}

	TEST_FIXTURE(Cons_fix, InvalidServerPort){
		// 3 Подключение к серверу. Введен не верный порт.
		p->name_original_file = "/home/stud/Desktop/client32_bin-txt/my.bin";
		p->name_result_file = "/home/stud/Desktop/client32_bin-txt/resultat.txt";
		p->name_auto_file = "/home/stud/Desktop/client32_bin-txt/config/vclient.conf";
		CHECK_THROW(p->Connect_to_server("127.0.0.1", 10), error_proj);
			
	}

	TEST_FIXTURE(Cons_fix, PasswordFileDoesNotExist){
		// 4 Ошибка идентификации. Не верный адрес к файлу с логинами и паролями
		p->name_original_file = "/home/stud/Desktop/client32_bin-txt/my.bin";
		p->name_result_file = "/home/stud/Desktop/client32_bin-txt/resultat.txt";
		p->name_auto_file = "/home/stud/Desktop/client32_bin-txt/config/qwe//vclient.conf";
		CHECK_THROW(p->Connect_to_server("127.0.0.1", 33333), error_proj);
			
	}

	TEST_FIXTURE(Cons_fix, InvalidLogin){
		// 5 Ошибка идентификации. Введен не правильный логин
		p->name_original_file = "/home/stud/Desktop/client32_bin-txt/my.bin";
		p->name_result_file = "/home/stud/Desktop/client32_bin-txt/resultat.txt";
		p->name_auto_file = "/home/stud/Desktop/client32_bin-txt/config/ErrorLogin.conf";
		CHECK_THROW(p->Connect_to_server("127.0.0.1", 33333), error_proj);
			
	}

	TEST_FIXTURE(Cons_fix, InvalidPassword){
		// 6 Ошибка аутентификации. Введен не правильный пароль
		p->name_original_file = "//home/stud/Desktop/client32_bin-txt/my.bin";
		p->name_result_file = "/home/stud/Desktop/client32_bin-txt/resultat.txt";
		p->name_auto_file = "/home/stud/Desktop/client32_bin-txt/config/ErrorPassword.conf";
		CHECK_THROW(p->Connect_to_server("127.0.0.1", 33333), error_proj);
			
	}

	TEST_FIXTURE(Cons_fix, VectorFileOpeningError){
		// 7 Ошибка открытия файла с векторами
		p->name_original_file = "/home/stud/Desktop/client32_bin-txt/qwe//123.bin";
		p->name_result_file = "/home/stud/Desktop/client32_bin-txt/resultat.txt";
		p->name_auto_file = "/home/stud/Desktop/client32_bin-txt/config/vclient.conf";
		CHECK_THROW(p->Connect_to_server("127.0.0.1", 33333), error_proj);
			
	}

	TEST_FIXTURE(Cons_fix, VectorFileReadError){
		// 8 Ошибка чтения из файла с векторами
		p->name_original_file = "/home/stud/Desktop/client32_bin-txt/None.bin";
		p->name_result_file = "/home/stud/Desktop/client32_bin-txt/resultat.txt";
		p->name_auto_file = "/home/stud/Desktop/client32_bin-txt/config/vclient.conf";
		CHECK_THROW(p->Connect_to_server("127.0.0.1", 33333), error_proj);
			
	}

	TEST_FIXTURE(Cons_fix, SumFileReadError){
		// 9 Ошибка открытия файла для записи суммы
		p->name_original_file = "/home/stud/Desktop/client32_bin-txt/my.bin";
		p->name_result_file = "/home/stud/Desktop/client32_bin-txt/@/@/resultat.txt";
		p->name_auto_file = "/home/stud/Desktop/client32_bin-txt/config/vclient.conf";
		CHECK_THROW(p->Connect_to_server("127.0.0.1", 33333), error_proj);
			
	}

	TEST_FIXTURE(Cons_fix, WrongIpAddressEntered){
		// 10 Неверно введен ip-адрес
		p->name_original_file = "/home/stud/Desktop/client32_bin-txt/my.bin";
		p->name_result_file = "/home/stud/Desktop/client32_bin-txt/resultat.txt";
		p->name_auto_file = "/home/stud/Desktop/client32_bin-txt/config/vclient.conf";
		CHECK_THROW(p->Connect_to_server("string", 33333), error_proj);
			
	}

	TEST_FIXTURE(Cons_fix, WrongPortEntered){
		// 11 Неверно введен port
		p->name_original_file = "/home/stud/Desktop/client32_bin-txt/my.bin";
		p->name_result_file = "/home/stud/Desktop/client32_bin-txt/resultat.txt";
		p->name_auto_file = "/home/stud/Desktop/client32_bin-txt/config/vclient.conf";
		CHECK_THROW(p->Connect_to_server("127.0.0.1", 'string'), error_proj);
			
	}
	
}

int main(int argc, char **argv)
{
	return UnitTest::RunAllTests();
}
