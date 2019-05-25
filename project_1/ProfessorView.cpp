#include "ProfessorView.h"
#include "DatabaseModel.h"
#include <iomanip>

#include <iostream>

using namespace project;

void ProfessorView::menu()
{
	while(true)
	{
		std::cout << "1 - Manage Marks\n2 - Students View\n3 - Class View\n4 - Personal Information \n5 - Back to Login\n";
        get_response();
		if (m_stop) return;
		if (m_response.size() == 1)
		{
			switch (m_response[0])
			{
			case '1':
				manage_marks();
				break;
			case '2':
				students_view();
				break;
			case '3':
				class_view();
				break;
			case '4':
				personal_information();
				break;
		    case '5':
				return;
			default:
				std::cout << "Wrong Input!\n";
			}
		}
		if (m_stop) return;
	}

}

void ProfessorView::manage_marks()
{   unsigned id;
        std::cin.ignore(32767, '\n');
    while(true)
    {
        std::string phrase;
        //unsigned id;
        std::cout << "Search for a student: \n";
        std::getline(std::cin,phrase);
        if (phrase == "q" || phrase == "exit" || phrase == "Q" || phrase == "quit") return;
        std::list<StudentInfo> list_of_students = m_db.find_students(phrase);           // Wyszukiwanie studenta na podstawie wpisanej frazy
        if(list_of_students.size()==0) std::cout << "Not found!\n";                      // Nie znaleziono studenta
        else if(list_of_students.size()>100) std::cout << "Too many results!\n";        // Znaleziono zbyt duzo studentow
        else
        {
            std::cout << "\nList of founded students:\n";        // Lista znalezionych studentow
            for (auto &stud : list_of_students)
            {
                std::cout << stud << "\n";
            }
            std::cout << "Enter the id: \n";
            get_response2();
            id = m_response2;                                // Wprowadzenie id studenta
            bool exist = false;
            for(auto& student : list_of_students)
            {
                if(id==student.get_id()) exist = true;      // Sprawdzanie poprawnosci id
            }
            if(exist) break;
            else std::cout << "This id not exist!\n";
        }
    }
    while(true)
    {
        std::cout << "1 - Show marks\n2 - Add marks\n3 - Remove marks\n4 - Update marks\n5 - Back to Menu\n";
        get_response();
        if (m_stop) return;
        if (m_response.size() == 1)
        {
            switch (m_response[0])
            {
            case '1':           // Przeglad ocen
                for(auto& mark : m_db.get_marks_by_student_id(id))
                {
                    std::cout << mark << "\n";
                }
                break;
            case '2':           // Dodawanie oceny
                {std::string date;
                std::string value;
                std::string subj;
                MARK mark;
                SUBJECT subject;
                while (true)
                {
					std::cout << "Enter value\n";                        // Nalezy podac ocene np: 3.5
					get_response();
					if(m_stop) return;
					if(m_response=="2") {mark=MARK::_2; break;}
					else if(m_response=="2.5") {mark=MARK::_2_5; break;}
					else if(m_response=="3") {mark=MARK::_3; break;}
					else if(m_response=="3.5") {mark=MARK::_3_5; break;}
					else if(m_response=="4") {mark=MARK::_4; break;}
					else if(m_response=="4.5") {mark=MARK::_4_5; break;}
					else if(m_response=="5") {mark=MARK::_5; break;}
					else std::cout << "wrong mark value!\n";
                }
                while (true)
                {
					std::cout << "Enter subject MD/PE/PP\n";                     // Nalezy podac nazwe przedmiotu np: PP
					get_response();
					if(m_stop) return;
					if(m_response=="MD") { subject=SUBJECT::MD; break;}
					else if(m_response=="PE") {subject=SUBJECT::PE; break;}
					else if(m_response=="PP") {subject=SUBJECT::PP; break;}
					else std::cout << "Wrong subject name!\n";
                }
                std::cout << "Enter date\n";                       // Nalezy podac date np: 25-05-2019
                get_response();
                if(m_stop) return;
                date = m_response;
                MarkInfo mark1{id, mark, date, subject, m_pinfo.get_id()};
                m_db.add(mark1);                                    // Dodanie oceny
                std::cout << "It is done\n";
                break;}
            case '3':               //Usuwanie oceny
                {unsigned m_id;
                bool success;
                while(true)
                {
                    for(auto& mark : m_db.get_marks_by_student_id(id))          //Wyszukiwanie ocen danego studenta
                    {
                        std::cout << mark;
                        std::cout << "\t mark id:" << mark.get_id() << "\n";
                    }
                    std::cout << "Enter the id: \n";                            // Wprowadzenie id oceny do usuniecia
                    get_response2();
                    m_id = m_response2;
                    bool m_exist = false;
                    for(auto& mark0 : m_db.get_marks_by_student_id(id))
                    {
                        if(m_id==mark0.get_id()) m_exist = true;                // Sprawdzenie poprawnosci id
                    }
                    if(m_exist)
                    {
                        success = m_db.remove_mark_by_id(m_id);
                        if(success)
                        {
                            std::cout << "It is done\n";                        // Usunieto ocene
                            break;
                        }
                        else std::cout << "Try again\n";
                    }
                    else std::cout << "This id not exist!\n";
                }
                    break;}
            case '4':           // poprawa oceny
                {unsigned mark_id;
                while(true)
                {
                    for(auto& mark : m_db.get_marks_by_student_id(id))
                    {
                        std::cout << mark;
                        std::cout << "\t mark id:" << mark.get_id() << "\n";            // Lista ocen
                    }
                    std::cout << "Enter id of mark:\n";                                // Wprowadzenie id oceny
                    get_response2();
                    mark_id = m_response2;
                    bool mark_exist = false;
                    MarkInfo mar;
                    for(auto& mrk : m_db.get_marks_by_student_id(id))
                    {
                        if(mark_id==mrk.get_id())
                        {
                            mark_exist = true;                // Sprawdzenie poprawnosci id
                            mar = mrk;
                        }
                    }
                    if(mark_exist)
                    {
						while (true)
						{
							std::cout << "Enter value\n";                        // Nalezy podac ocene np: 3.5
							get_response();
							if(m_stop) return;
							if(m_response=="2") {mar.m_value=MARK::_2; break;}
							else if(m_response=="2.5") {mar.m_value=MARK::_2_5; break;}
							else if(m_response=="3") {mar.m_value=MARK::_3; break;}
							else if(m_response=="3.5") {mar.m_value=MARK::_3_5; break;}
							else if(m_response=="4") {mar.m_value=MARK::_4; break;}
							else if(m_response=="4.5") {mar.m_value=MARK::_4_5; break;}
							else if(m_response=="5") {mar.m_value=MARK::_5; break;}
							else std::cout << "wrong mar.m_valuek value!\n";
						}
                                                m_db.update(mar);
                        std::cout << "It is done\n";
                        break;
                    }
                    else std::cout << "This id not exist!\n";
                }
                    break;}
            case '5':           // Back to login
                return;
            default:
                std::cout << "Wrong Input!\n";
            }
        }
    }
}

void ProfessorView::students_view()
{
    while(true)
	{
		std::cout << "1 - Show all students\n2 - Show students in the selected class\n3 - Back to Menu\n";
        get_response();
		if (m_stop) return;
		if (m_response.size() == 1)
		{
			switch (m_response[0])
			{
			case '1':               //lista wszystkich studentow
			    std::cout << "\nStudents:\n";
                for (auto& cls : m_db.get_all<StudentInfo>())
                {
                    std::cout << *cls.second << "\n";
                }
			    break;
            case '2':               // lista studentow w danej klasie
                { unsigned c_id;
                std::cout << "\nClasses:\n";
			    for (auto& cls : m_db.get_all<ClassInfo>())
                {
                    std::cout << std::setw(7) << "id: " << cls.first << " " << *cls.second << "\n";       // Lista klas
                }
                std::cout << "Enter id of a class:\n";                                                    // Wprowadzenie id klasy
                get_response2();
                c_id = m_response2;
                bool c_exist = false;
                for(auto& cls : m_db.get_all<ClassInfo>())
                {
                    if(c_id==cls.first) c_exist = true;                                               // Sprawdzenie poprawnosci id
                }
                if(c_exist)
                {
                    std::cout << "\nStudents in the selected class:\n";
                    for (auto &stud : m_db.get_students_by_class(c_id))
                    {
                        std::cout << stud << "\n";                                                       // Wyswietlenie listy studentow w danej klasie
                    }
                }
                else std::cout << "This id not exist!\n";
                break;}
            case '3': //back to login
                {
                    return;}
            default:
                std::cout << "Wrong Input!\n";
			}
		}
	}
}

void ProfessorView::class_view()
{
    std::cout << "\nClasses:\n";
    for (auto& cls : m_db.get_all<ClassInfo>())             // Przeglad wszystkich klas
		{
			std::cout << std::setw(7) << "id: " << cls.first << " " << *cls.second << "\n";
		}
}

void ProfessorView::personal_information()                  // Dane zalogowanego profesora
{
	ProfessorInfo prof_info = m_db.get_by_id<ProfessorInfo>(id);
    std::cout<<"\nPersonal information: "<<"\n";
    std::cout<<"Title: "<<prof_info.m_title<<"\n";
    std::cout<<"Name: "<<prof_info.m_first_name<<" ";
    std::cout<<prof_info.m_last_name<<"\n";
    std::cout<<"PESEL: "<<prof_info.m_PESEL<<"\n";
    std::cout<<"Subjects: ";
    for (auto &i : prof_info.m_subjects) std::cout << i << ", ";
    std::cout << "\n";
}

