//DATA ACCESS OBJECT
// serializacja/deserialzacja
/*
manages database // CSVDbID
it works with any type that implements BasicData
*/
#ifndef BASEDAO_H_
#define BASEDAO_H_
#include "CSVDb.h"
#include <memory>
#include <map>

namespace cheshire {
	
template<class T>
class BaseDao {
	typedef unsigned int uint;

	const char* m_file_name;
	std::unique_ptr<cheshire::CSVDbID> m_database;

public:
	explicit BaseDao(const char* filename, const std::vector<std::string>& fieldnames) : m_file_name(filename), m_database(std::make_unique<cheshire::CSVDbID>(filename, fieldnames)) {}
	// redirects to CSVDb using BasicData interface
	bool add_student(const T& student) const
	{
		return m_database->add_row(student.to_string_vector());
	}
	// redirects to CSVDb using BasicData interface
	bool remove_student(uint id) const
	{
		return m_database->remove_row(id);
	}
	// redirects to CSVDb using BasicData interface
	bool update_student(const T& student) const
	{
		return m_database->update_row(student.get_id(), student.to_string_vector());
	}
	// converts from map returned by CSVDb to Map of T
	// that pointer management though!
	std::unique_ptr<std::map<uint, std::unique_ptr<T>>> get_students() const
	{
		auto out_ptr = std::make_unique<std::map<uint, std::unique_ptr<T>>>();
		auto rows_ptr = m_database->get_rows();
		for (auto &map_ptr : *rows_ptr)
		{
			auto sinfo_ptr = std::make_unique<T>();
			sinfo_ptr->from_map(map_ptr.first, std::move(map_ptr.second));			
			out_ptr->emplace(map_ptr.first, std::move(sinfo_ptr));
		}

		return out_ptr;
	}

	std::unique_ptr<T> get_student(uint id) const
	{
		auto pair = m_database->get_row(id);
		auto ptr = std::make_unique<T>();
		if (!pair.second->empty()) ptr->from_map(id, std::move(pair.second));
		return ptr;
	}
};

}

#endif
