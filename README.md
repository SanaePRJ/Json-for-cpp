# Json for CPP
Reads data in json format and returns the value!

Website: http://sanae.starfree.jp  
Author: SanaeProject

# Coding Convention

    //Comment
    ReturnClass Func
    (
        Argument1,
        Argument2,
        ...
    )
    {
        //Process
    }

# Declearation
    
    namespace Sanae{
        class json;
    }

# Sample code
## test.json

    {
      "SanaeProject": {
        "Version": 1,
        "Author": [ "SanaeProject", "Huku" ],
        "copyright": null
      }
    }

## test.cpp

    #include <iostream>
    #include "json.hpp"


    int main() {
	    Sanae::json hello("test.json");
	
	    printf("Version:%d\n",std::stoi(hello["SanaeProject::Version"])); //1
	    printf("Author:%s\n", hello["SanaeProject::Author[1]"].c_str());  //Huku
	    printf("Author:%s\n", hello["SanaeProject::copyright"].c_str());  //null

	    return 0;
    }