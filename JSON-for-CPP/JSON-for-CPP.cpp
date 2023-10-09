#include <iostream>
#include "json.hpp"


int main() {
	Sanae::json hello("test.json");
	
	printf("Version:%d\n",std::stoi(hello["SanaeProject::Version"])); //1
	printf("Author:%s\n", hello["SanaeProject::Author[1]"].c_str());  //Huku
	printf("Author:%s\n", hello["SanaeProject::copyright"].c_str());  //null

	return 0;
}