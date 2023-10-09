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

#include "CppVer.hpp"




class json {


	/*-----Define variable(protected)-----*/
protected:
	std::string Encorder;


	/*-----Define functions(protected)-----*/
protected:
	std::string GetData(std::string* Encorded_Data, const char* key)
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

	//Pass the Object
	std::string Json_Encorder_GetObject
	(
		std::string* a_text,
		unsigned long long& a_first,
		unsigned long long  a_end
	)
	{
		std::string obj = "";
		bool        is_start = false;
		bool        is_TFN = false;

		auto        is_num = [](char a)
			{
				char buf = a - '0';
				return 0 <= buf && buf <= 9;
			};
		auto        is_TorForN = [a_text](unsigned long long _pos)
			{
				//true
				if ((*a_text)[_pos] == 't') {
					const char* b_true = "true";
					for (unsigned long long i = _pos; (i - _pos) < 4; i++) {
						if ((*a_text)[i] != b_true[i - _pos])
							return false;
					}
					return true;
				}
				//false
				else if ((*a_text)[_pos] == 'f') {
					const char* b_true = "false";
					for (unsigned long long i = _pos; (i - _pos) < 5; i++) {
						if ((*a_text)[i] != b_true[i - _pos])
							return false;
					}
					return true;
				}
				//null
				else if ((*a_text)[_pos] == 'n') {
					const char* b_true = "null";
					for (unsigned long long i = _pos; (i - _pos) < 4; i++) {
						if ((*a_text)[i] != b_true[i - _pos])
							return false;
					}
					return true;
				}

				//none
				return false;
			};

		for (unsigned long long& _pos = a_first; _pos <= a_end; _pos++)
		{
			char buf_char = (*a_text)[_pos];

			//Start String
			if (buf_char == '\"')
			{
				//obj += buf_char;

				if (!is_start)
					is_start = true;
				else
				{
					_pos++;
					return obj;
				}
			}
			//String
			else if (is_start || is_num(buf_char))
				obj += buf_char;

			//StopCode
			else if (buf_char == ',' || buf_char == '{' || buf_char == '}' || buf_char == '[' || buf_char == ']')
				return obj;

			else if (is_TorForN(_pos) || is_TFN)
			{
				is_TFN = true;
				obj += buf_char;
			}
			else
				obj += buf_char;
		}

		return obj;
	}

	void Json_Encorder
	(
		std::string* a_text,
		std::string* a_store,

		std::string  a_namespace = "",
		unsigned long long a_first = 0,
		unsigned long long a_end = 0
	)
	{
		auto does_clear = [](char buf)
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
			std::string objname = Json_Encorder_GetObject(a_text, _pos, a_end);
			auto        char_live = [&_pos, a_text]() {return (*a_text)[_pos]; };

			//':' should appear except for the first time.
			if (char_live() != ':' && _pos != 0)
				throw std::runtime_error("Format is different.");
			//Initial value
			else if (_pos == 0) {/*Do nothing.*/ }
			else
				_pos++;

			//'{' or '[' should come.
			char b_char;
			if ((b_char = char_live()) == '{')
			{
				signed   long long forward = 1, backward = -1;

				unsigned long long b_first = _pos;
				unsigned long long b_end = a_text->find_last_of('}', a_end);

				if (b_end == std::string::npos)
					throw std::runtime_error("End point not found.");

				std::string new_namespace = (a_namespace == "") ? objname : (a_namespace + "::" + objname);
				Json_Encorder(a_text, a_store, new_namespace, b_first + forward, b_end + backward);

				_pos = b_end + forward;  //Move _pos to the next part of '}'
			}
			//Array
			else if (b_char == '[')
			{
				std::string b_pas = a_namespace == "" ? "" : a_namespace + "::";


				_pos++;  //'[' Exit.

				unsigned long long count = 0;
				while (true) {
					std::string buf = b_pas + objname + "[" + std::to_string(count) + "]=" + Json_Encorder_GetObject(a_text, _pos, a_end) + ",";
					(*a_store) += buf;
					count++;

					if (char_live() == ',')
						_pos++;
					else
						break;
				}

				_pos++;  //']' Exit.
			}
			//Node confirmed
			else
			{
				std::string b_pas = a_namespace == "" ? "" : a_namespace + "::";
				b_pas += objname + "=" + Json_Encorder_GetObject(a_text, _pos, a_end) + ",";

				(*a_store) += b_pas;
			}

			if (b_char = char_live() == '}')
				break;

			else if (b_char == ',')
				_pos++;
		}
	}


	/*-----Define functions(public)-----*/
public:
	json(const char* a_filename) {
		std::string test;

		std::ifstream ifs("test.json");
		while (true)
		{
			char buf;
			ifs.read(&buf, sizeof(char));

			if (ifs.eof())
				break;

			test += buf;
		}

		Json_Encorder(&test, &this->Encorder);
	}

	json(const json& in) 
	{
		this->Encorder = in.Encorder;
	}

	//Access
	std::string operator [](const char* a_key) 
	{
		return this->GetData(&this->Encorder,a_key);
	}
};




#endif