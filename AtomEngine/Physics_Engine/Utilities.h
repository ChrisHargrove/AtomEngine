#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <string>
#include <vector>
#include <future>

/*!
    * \brief Splits a string at the delimeter.
    * \param string The string being split.
    * \param delimeter The character you wish to split the string at.
    * \param include Wether the delimeter character should be included in return string.
    * \return Returns a string containing everything after the delimeter.
    *
    * Will split a string given at the delimeter and return everything after the delimeter
    * as a new string.
*/
ATOM_API std::string SplitEnd(const std::string& string, const std::string& delimeter, bool include = false);

/*!
    * \brief Splits a string at the delimeter.
    * \param string The string being split.
    * \param delimeter The character you wish to split the string at.
    * \param include Wether the delimeter character should be included in return string.
    * \return Returns a string containing everything before the delimeter.
    *
    * Will split a string given at the delimeter and return everything before the delimeter
    * as a new string.
*/
ATOM_API std::string SplitFront(const std::string& string, const std::string& delimeter, bool include = false);

