/*!
    * \class AlignedAllocation "AlignedAllocation.h"
    * \brief Class to ensure memory allocation is 8 or 16 byte aligned.
    *
    * This is a templated class that is designed to ensure that classes are aligned to 8 or 16 byte alignment in memory
    * as it can lead to problems with shaders if data is not aligned correctly. To ensure classes are aligned
    * correctly they must inherit from AlignedAllocation and specify the alignment.
*/

#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <malloc.h>

const  size_t BYTE8 = 8; /*!< Constant for aligning memory to 8 Bytes. */
const  size_t BYTE16 = 16; /*!< Constant for aligning memory to 16 Bytes. */

template<size_t Alignment>
class ATOM_API AlignedAllocation
{
public:
    virtual ~AlignedAllocation() = default;

    /*!
        * \brief Overloaded new operator to allow for alignment to that specified.
        *
        * Allows this class to use the aligned malloc function to assign memory in the alignment specified.
    */
    inline static void* operator new(size_t size) {
        return _aligned_malloc(size, Alignment);
    }

    /*!
        * \brief Overloaded delete operator to delete aligned memory.
        *
        * Allows this class to delete using the aligned free function to ensure that all data in the current 
        * aligned block of memory.
    */
    inline static void operator delete(void* memory) {
        _aligned_free(memory);
    }
};