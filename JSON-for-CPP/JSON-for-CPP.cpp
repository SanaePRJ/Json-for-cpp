#include <iostream>
#include <map>
#include "json.hpp"


int main() {
	json hello("test.json");
	
	printf("Version:%d\n",std::stoi(hello["SanaeProject::JsonReader::Version"]));
	printf("Author:%s\n",hello["SanaeProject::JsonReader::Author[1]"].c_str());

	return 0;
}