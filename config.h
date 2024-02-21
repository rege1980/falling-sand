#pragma once

#include <fstream>
#include <map>
#include <string>

inline bool load_cfg(const char* path, std::map<std::string, int*>* values)
{
	// load file
	std::ifstream config_file(path);


	// read file line by line
	if (config_file.is_open())
	{
		std::string line;
		int val_int;

		while (config_file)
		{
			std::getline(config_file, line);
			if (line[0] != '#') // skip comments
			{
				// subtract variable name
				std::string var = line.substr(0, line.find('='));
				// search variable name in values, exit if not found
				if (values->find(var) == values->end()) return false;
				// subtract value, exit if exception occured
				std::string val_str = line.substr(line.find('=') + 1, line.find(';') - line.find('=') - 1);
				try { val_int = std::stoi(val_str); }
				catch (std::invalid_argument& ex)
				{
					return false;
				}
				catch (const std::out_of_range& ex)
				{
					return false;
				}
				// set value
				*values->at(var) = val_int;
			}
		}
		return true;
	}
	return false;
}
