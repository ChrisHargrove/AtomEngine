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

/*!
    * \brief Maps a number from one range to another.
    * \param number The number you wish to map.
    * \param inRangeStart The starting point of the input range of numbers.
    * \param inRangeEnd The end point of the input range of numbers.
    * \param outRangeStart The starting point of the output range of numbers.
    * \param outRangeEnd The end point of the output range of numbers.
    * \return Returns a floating point number within the newly mapped range.
    *
    * Will take a number and mapp it from an original range of numbers to a new range of numbers
    * and will return the newly mapped number.
*/
ATOM_API float MapRange(float number, float inRangeStart, float inRangeEnd, float outRangeStart, float outRangeEnd);