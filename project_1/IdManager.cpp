#include "IdManager.h"
#include <fstream>
#include <sstream>

using namespace cheshire;

IdManager::IdManager(const std::string& filename)
{
	sfilename = filename;
	auto file = std::ifstream(sfilename, std::ios::in);

	std::string str;
	std::getline(file, str);
	std::istringstream stream{str};
	while (stream >> str)
	{	uint i = std::stoul(str);
		id_vector.push_back(i);
	}
	if (id_vector.empty()) id_vector.push_back(1);
}

unsigned int IdManager::get_id()
{
	if (id_vector.size() == 1)
	{
		uint id = id_vector[0]++;
		save();
		return id;
	}
	else
	{
		uint id = id_vector[1];
		id_vector.erase(id_vector.begin() + 1);
		save();
		return id;
	}
}

bool IdManager::save()
{
	auto file = std::ofstream(sfilename, std::ios::trunc | std::ios::out);
	for (unsigned int i = 0; i < id_vector.size(); ++i)
	{
		file << id_vector[i] << ((i < id_vector.size() - 1) ? " " : "");
	}
	return true;
}


bool IdManager::release_id(unsigned int id)
{
	// checks if id was used
	if (id < id_vector[0])
	{
		// checks if id wasn't already released
		for (auto i : id_vector) if (i == id) return false;
		id_vector.push_back(id);
		save();
		return true;
	}
	else
		return false;
}