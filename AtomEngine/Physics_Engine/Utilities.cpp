#include "Utilities.h"

namespace StringUtils {
    std::string SplitEnd(std::string string, const std::string & delimeter, bool include)
    {
        //TODO: Handle errors for when delimeter is not found!!

        unsigned int position = 0;
        //Get the position of the first character after the delimeter.
        position = string.find(delimeter);
        if (!include) position++;
        //Split the string at the position found, and return everything from there
        //to the end of the string.
        return string.substr(position, string.length() - position);
    }

    std::string SplitFront(std::string string, const std::string & delimeter, bool include)
    {
        unsigned int position = 0;
        //Get the position of the delimeter.
        position = string.find(delimeter);
        if (include) position++;
        //Split the string from the beginning of the string to the position found.
        return string.substr(0, position);
    }
}

namespace MathUtils {
    float MapRange(float number, float inRangeStart, float inRangeEnd, float outRangeStart, float outRangeEnd)
    {
        return ((number - inRangeStart) * ((outRangeEnd - outRangeStart) / (inRangeEnd - inRangeStart))) + outRangeEnd;
    }
}
