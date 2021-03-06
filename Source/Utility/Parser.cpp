#include "Utility/Parser.hpp"

Parser::Parser(const std::string& path)
: file{ path }, cursor{ 0u }
{
	// Try to open the file
	if(!file.good())
		throw std::runtime_error{ "[PARSER] Could not load file " + path };
}

bool Parser::skipToNextLine()
{
	// Keep reading lines
    do
	{
		if(file.eof())
			return false;

		// Get the next line
		std::getline(file, currentLine);
	}
	// Check if line is empty or a comment ("//")
	while(currentLine.empty() || (currentLine[0] == '/' && currentLine[1] == '/'));

	// Reset the cursor
	cursor = 0u;
	return true;
}

void Parser::getNextString(std::string& str, bool cvt)
{
	str = currentLine;

	if(cvt)
	{
		uint64_t pos = 0u, first = pos;
		while((pos = str.find("\\n", first)) != std::string::npos)
		{
			str.replace(pos, 2, "\n");
			first = pos - 2;
		}
	}
}

float Parser::getNextFloat()
{
    std::string num;

    while(!(currentLine[cursor] == '-' || std::isdigit(currentLine[cursor]) || currentLine[cursor] == '.') && cursor < currentLine.size())
		++cursor;

    while((currentLine[cursor] == '-' || std::isdigit(currentLine[cursor]) || currentLine[cursor] == '.') && cursor < currentLine.size())
		num += currentLine[cursor++];

    return std::stof(num);
}

int32_t Parser::getNextInt()
{
	std::string num;

    while(!(currentLine[cursor] == '-' || std::isdigit(currentLine[cursor])) && cursor < currentLine.size())
		++cursor;

    while((currentLine[cursor] == '-' || std::isdigit(currentLine[cursor])) && cursor < currentLine.size())
		num += currentLine[cursor++];

    return std::stol(num);
}

void Parser::getNextKeyValuePair(std::string& key, std::string& value, const char& separator)
{
	key.clear();

	// Build the key word
	while(currentLine[cursor] != separator)
		key += currentLine[cursor++];

	// Skip the separator
	++cursor;

	value.clear();

	// Build the value word
	while(cursor < currentLine.size())
		value += currentLine[cursor++];
}
