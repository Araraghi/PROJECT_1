#ifndef PROFESSOR_VIEW_H_
#define PROFESSOR_VIEW_H_
#include "DatabaseModel.h"
#include <iostream>

using namespace project;

class ProfessorView
{
private:
    ProfessorInfo m_pinfo;
	DatabaseModel& m_db;
	bool m_stop = false;
	std::string m_response;
	unsigned m_response2;
    uint id;

public:
	ProfessorView(DatabaseModel& db, unsigned id): m_db(db), id(id) {}
	bool is_exit_requested() { return m_stop; }
    void menu();
    void manage_marks();
    void students_view();
    void class_view();
    void personal_information();
private:
	void get_response()
	{
		while (true)
		{
			std::cin >> m_response;
			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(32767, '\n');
			}
			else
			{
				if (m_response == "q" || m_response == "exit" || m_response == "Q" || m_response == "quit") { m_stop = true; break; }
				break;
			}
		}
	}
	void get_response2()
	{
		while (true)
		{
			std::cin >> m_response2;
			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(32767, '\n');
			}
			else break;
		}
	}
};

#endif //
