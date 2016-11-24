#pragma once

#include <cJSON.h>
#include  <vector>

class JsonUtils
{
public:

	static void printJSON(cJSON *json);
	static void itemParse(const char *text, std::vector<std::string> &output);
	static void parseFile(const char *filename, std::vector<std::string> &output);
	static std::string removeQuote(const char* src);
private:
	JsonUtils() {};
	~JsonUtils() {};
};

