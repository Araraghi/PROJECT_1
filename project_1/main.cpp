#include "DatabaseModel.h"
#include <iostream>
#include <iomanip>
#include <string_view>
#include "AdminView.h"
#include "Generator.h"


bool stop = false;
using namespace project;
//enum used to tell what privileges should use have
enum class USERTYPE
{
	ADMIN, PROFESSOR, STUDENT
};
bool is_exit_requested(std::string_view str)
{
	if (str == "q" || str == "exit" || str == "Q" || str == "quit") { stop = true; return true; }
	return false;
}

//helper for geting userinput
void get_response(std::string& buffer);
void clean_db()
{
	std::remove("std_db.txt");
	std::remove("id_std_db.txt");
	std::remove("mrk_db.txt");
	std::remove("id_mrk_db.txt");
	std::remove("prs_db.txt");
	std::remove("id_prs_db.txt");
	std::remove("cls_db.txt");
	std::remove("id_cls_db.txt");
	std::remove("passwords.txt");
}

//view where user can login returns info about privileges
USERTYPE login_view(const project::DatabaseModel& db);
int main()
{
	clean_db();
	project::DatabaseModel db("cls_db.txt", "std_db.txt", "prs_db.txt", "mrk_db.txt");
	 //generator if you want for testing
	const unsigned students = 200;
	const unsigned classes = 10;
	const unsigned professors = 20;
	const unsigned marks = 1000;
	Generator gen(students, classes, professors, marks);
	gen.populate(db);

	login:
	USERTYPE type = login_view(db);
	if (stop) return 1;
	switch (type)
	{
	case USERTYPE::ADMIN:
		AdminView admin_view(db);
		admin_view.menu();
		if (admin_view.is_exit_requested()) return 2;
		else goto login;
		break;
	}
	// ... USERTYPE::STUDENT ...

	
	return 0;
}

USERTYPE login_view(const project::DatabaseModel& db)
{
	std::string response;
	std::string password;
	USERTYPE type;
	std::cout << "q|Q|exit|quit for exit\n";
	username:
	std::cout << "Username: Admin|Professor|Student\n";
	while (true)
	{
		get_response(response);
		if (is_exit_requested(response)) { stop = true; return USERTYPE::STUDENT; }
		if (response == "Admin") { password = db.get_admin_password(); type = USERTYPE::ADMIN;	break; }
		else if (response == "Professor") { password = db.get_professor_password();	type = USERTYPE::PROFESSOR; break; }
		else if (response == "Student") { password = db.get_student_password(); type = USERTYPE::STUDENT;	break; }
		std::cout << "Try again\n";
	}
	if (password.empty())
	{
		std::cout << "Password not set!\n";
		goto username;
	}
	std::cout << "Password:\n";
	while (true)
	{
		get_response(response);
		if (is_exit_requested(response)) { stop = true; return USERTYPE::STUDENT; }
		if (response == password) { std::cout << "access granted!\n"; return type; }
		std::cout << "Wrong password!\n";
	}
}
void get_response(std::string& buffer)
{
	while (true) 
	{
		std::cin >> buffer;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			continue;
		}
		break;
	}
}