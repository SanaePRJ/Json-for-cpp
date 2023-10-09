/*=============================================================
* NAME      : json.hpp
* AUTHOR    : SanaeProject
* VERSION   : 1.0.0
* COPYRIGHGT: Copyright 2023 SanaeProject.
=============================================================*/




/*-----INCLUDE GUARD-----*/
#ifndef SANAE_JSON_HPP
#define SANAE_JSON_HPP




/*-----Include-----*/
#include <iostream>

#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>




/*----Define Class-----*/
namespace Sanae {
	class json;
};




/*-------------------------------------------------------------
* Define Class
*
* Author:Sanae
-------------------------------------------------------------*/


class Sanae::json
{


	/*-----Define variable(protected)-----*/
protected:


	/*----------------------------------------------
	* Store encoded data.
	* エンコードしたデータを格納する。
	----------------------------------------------*/
	std::string Encorder;


	/*-----Define Functions(protected)*/
protected:


	/*----------------------------------------------
	* Get data from key.
	* キーからデータを入手する。
	----------------------------------------------*/
	std::string GetData
	(
		std::string*, const char*
	);


	/*----------------------------------------------
	* Get object data.
	* オブジェクトデータを入手する。
	----------------------------------------------*/
	std::string Json_Encorder_GetObject
	(
		std::string*t,
		unsigned long long&,
		unsigned long long
	);


	/*----------------------------------------------
	* Rewrite the json format data into your own format.
	* json形式のデータを独自形式に書き換えます。
	----------------------------------------------*/
	void Json_Encorder
	(
		std::string*,
		std::string*,

		std::string,
		unsigned long long,
		unsigned long long
	);


	/*-----Define Functions(public)-----*/
public:


	/*----------------------------------------------
	* Read files and convert them to proprietary formats.
	* ファイルを読み取って独自形式に変換する。
	----------------------------------------------*/
	json(const char*);


	/*----------------------------------------------
	* Copy constructor.
	----------------------------------------------*/
	json(const json&);
	

	/*----------------------------------------------
	* Get data from key.
	  Throws runtime_error if it fails.
	* キーからデータを入手します。
	  失敗した場合runtime_errorをthrowします。
	----------------------------------------------*/
	std::string operator [](const char*);
};




/*-------------------------------------------------------------
* Implementation.
*
* Author:Sanae
-------------------------------------------------------------*/




/*-----Functions(protected)-----*/
/*----------------------------------------------
* Get data from key.
* キーからデータを入手する。
----------------------------------------------*/
std::string Sanae::json::GetData(std::string* Encorded_Data, const char* key)
{
	unsigned long long find = Encorded_Data->find(key);

	if (find == std::string::npos || (find != 0 && (*Encorded_Data)[find - 1] != ','))
		throw std::runtime_error("Not found key.");

	find = find + strlen(key)+1;
	std::string buf = "";

	for (; (*Encorded_Data)[find] != ','; find++)
		buf += (*Encorded_Data)[find];

	return buf;
}

/*----------------------------------------------
* Get object data.
* オブジェクトデータを入手する。
----------------------------------------------*/
std::string Sanae::json::Json_Encorder_GetObject
(
	std::string* a_text,
	unsigned long long& a_first,
	unsigned long long  a_end
)
{
	std::string obj      = "";
	bool        is_start = false;
	bool        is_TFN   = false;

	
	auto        is_num = [](char a)
		{
			char buf = a - '0';                            //To num
			return 0 <= buf && buf <= 9;                   //the number should 0~9
		};
	auto        check  = [a_text](unsigned long long b_pos, const char* b_str)
		{
			for (unsigned long long i = b_pos; (i - b_pos) < strlen(b_str); i++) {
				if ((*a_text)[i] != b_str[i - b_pos])      //is matching.
					return false;
			}
			return true;
		};
	auto        is_TorForN = [a_text,check](unsigned long long _pos)
		{
			if ((*a_text)[_pos] == 't')                    //check true.
				return check(_pos, "true");

			else if ((*a_text)[_pos] == 'f')               //check false.
				return check(_pos, "false");

			else if ((*a_text)[_pos] == 'n')               //check null.
				return check(_pos, "null");

			return false;                                  //not match.
		};


	for (unsigned long long& _pos = a_first; _pos <= a_end; _pos++)
	{
		char buf_char = (*a_text)[_pos];

		
		if (buf_char == '\"')                              //Start or End String
		{
			if (!is_start)
				is_start = true;
			else
			{
				_pos++;
				return obj;
			}
		}
		
		else if (is_start || is_num(buf_char))             //String or number
			obj += buf_char;

		//StopCode
		else if (buf_char == ',' || buf_char == '{' || buf_char == '}' || buf_char == '[' || buf_char == ']')
			return obj;

		else if (is_TorForN(_pos) || is_TFN)               //true or false or null
		{
			is_TFN = true;
			obj += buf_char;
		}
		else                                               //Unknown code.
			return obj;
	}

	return obj;
}

/*----------------------------------------------
* Rewrite the json format data into your own format.
* json形式のデータを独自形式に書き換えます。
----------------------------------------------*/
void Sanae::json::Json_Encorder
(
	std::string* a_text,
	std::string* a_store,

	std::string  a_namespace   = "",
	unsigned long long a_first =  0,
	unsigned long long a_end   =  0
)
{
	auto does_clear = [](char buf)                         //clear code.
		{
			return (buf == ' ' || buf == '\t' || buf == '\n' || buf == '\r' || buf == '\f' || buf == '\v');
		};
	//Delete unnecessary characters such as spaces.
	a_text->erase(std::remove_if(a_text->begin(), a_text->end(), does_clear), a_text->end());

	//Initial value
	if (a_end == 0) a_end = a_text->size() - 1;

	for (unsigned long long _pos = a_first; _pos <= a_end; _pos++)
	{
		//A key or namespace comes.
		std::string objname   = Json_Encorder_GetObject(a_text, _pos, a_end);  //Get key.
		auto        char_live = [&_pos, a_text]() {return (*a_text)[_pos]; };  //Get the values of various places.

		//':' should appear except for the first time.
		if (char_live() != ':' && _pos != 0)
			throw std::runtime_error("Format is different.");
		//Initial value
		else if (_pos == 0) {/*Do nothing.*/ }
		else
			_pos++;

		//'{' or '[' should come.
		char b_char;
		if ((b_char = char_live()) == '{')                 //recursive
		{
			signed   long long forward = 1, backward = -1;

			unsigned long long b_first = _pos;             //startpoint
			unsigned long long b_end = a_text->find_last_of('}', a_end);  //end point

			if (b_end == std::string::npos)                //is not found.
				throw std::runtime_error("End point not found.");

			std::string new_namespace = (a_namespace == "") ? objname : (a_namespace + "::" + objname);
			Json_Encorder(a_text, a_store, new_namespace, b_first + forward, b_end + backward);    //call own.

			_pos = b_end + forward;  //Move _pos to the next part of '}'
		}
		else if (b_char == '[')                            //Array
		{
			std::string b_pas = a_namespace == "" ? "" : a_namespace + "::";

			_pos++;                                        //'[' Exit.

			unsigned long long count = 0;                  //Store array number.
			while (true) {
				std::string buf = b_pas + objname + "[" + std::to_string(count) + "]=" + Json_Encorder_GetObject(a_text, _pos, a_end) + ",";
				(*a_store) += buf;
				count++;

				if (char_live() == ',')
					_pos++;
				else
					break;
			}

			_pos++;                                         //']' Exit.
		}
		else                                               //Node confirmed
		{
			std::string b_pas = a_namespace == "" ? "" : a_namespace + "::";
			b_pas            += objname + "=" + Json_Encorder_GetObject(a_text, _pos, a_end) + ",";

			(*a_store) += b_pas;
		}

		if (b_char = char_live() == '}')                   //Object End
			break;

		else if (b_char == ',')                            //Next key and data.
			_pos++;
	}
}


/*-----Functions(public)-----*/
/*----------------------------------------------
* Rewrite the json format data into unique format.
* json形式のデータを独自形式に書き換えます。
----------------------------------------------*/
Sanae::json::json(const char* a_filename) {
	std::string before_encoding;

	std::ifstream ifs(a_filename);                         //Read json file
	while (true)
	{
		char buf;                                          //Store char
		ifs.read(&buf, sizeof(char));                      //Read  char

		if (ifs.eof())                                     //Is ending of file?
			break;

		before_encoding += buf;                            //Add read char.
	}

	Json_Encorder(&before_encoding, &this->Encorder);      //Encode json format into unique format.
}

/*----------------------------------------------
* Copy constructor.
----------------------------------------------*/
Sanae::json::json(const json& in) 
{
	this->Encorder = in.Encorder;                          //Get encoded data.
}

/*----------------------------------------------
* Get data from key.
  Throws runtime_error if it fails.
* キーからデータを入手します。
  失敗した場合runtime_errorをthrowします。
----------------------------------------------*/
std::string Sanae::json::operator [](const char* a_key) 
{
	return this->GetData(&this->Encorder,a_key);           //Get data from key.
}




#endif