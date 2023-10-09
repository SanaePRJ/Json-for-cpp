/*=============================================================
* NAME      : json.hpp
* AUTHOR    : SanaeProject1.0.0
* COPYRIGHGT: Copyright 2023 SanaeProject.
=============================================================*/




/*-----INCLUDE GUARD-----*/
#ifndef SANAE_CPPVER_HPP

#ifdef _MSVC_LANG
	#define SANAE_CPPVER_HPP  _MSVC_LANG
#else
	#define SANAE_CPPVER_HPP __cplusplus
#endif




/*-----Version Check.C++11 or more.-----*/
#if SANAE_CPPVER_HPP >= 201103L


enum class Version : long
{
	CPP98 = 199711L,
	CPP03 = 199711L,
	CPP11 = 201103L,
	CPP14 = 201402L,
	CPP17 = 201703L,
	CPP20 = 202002L,

	CPP_UnKnown = 000000L
};


/*-----Less than C++11-----*/
#else


typedef enum
{
	CPP98 = 199711L,
	CPP03 = 199711L,
	CPP11 = 201103L,
	CPP14 = 201402L,
	CPP17 = 201703L,
	CPP20 = 202002L,

	CPP_UnKnown = 000000L
}Version;


#endif




/*-----Compare-----*/
#if SANAE_CPPVER_HPP >= 201103L


	constexpr bool is_same_ver
	(
		Version a_version
	)
	{
		return (long)a_version == SANAE_CPPVER_HPP;
	}


	constexpr bool is_same_or_more_ver
	(
		Version a_version
	)
	{
		return (long)a_version <= SANAE_CPPVER_HPP;
	}


#else


	#define is_same_ver(a_version)         ((long)a_version == SANAE_CPPVER_HPP)
	#define is_same_or_more_ver(a_version) ((long)a_version <= SANAE_CPPVER_HPP)


#endif




#endif