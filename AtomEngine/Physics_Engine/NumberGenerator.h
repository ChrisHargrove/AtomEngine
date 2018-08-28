/*!
    * \class NumberGenerator "NumberGenerator.h"
    * \brief A better random number generator.
    *
    * A random number generator using a Mersenne Twister to generate numbers. Allows for different
    * ranges of numbers that can be changed whenever required, and can also be re-seeded as
    * many times as required.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <random>

class ATOM_API NumberGenerator
{
public:
    /*!
        * \brief Creates a number generator.
        * \param seed Sets the seed value for the generator. Defaults to 0.
        * \param minimum The minimum number that can generate. Defaults to 0.
        * \param maximum The maximum number that can generate. Defaults to 1.
        *
        * Creates the number generator and sets the intial seed and range values. By default the 
        * generator works like rand().
    */
    NumberGenerator(unsigned int seed = 0, int minimum = 0, int maximum = 1);
    ~NumberGenerator();

    /*!
        * \brief Gets the next random number as an integer.
        * \return Returns the next random integer.
    */
    int GetNumber();

    /*!
        * \brief Gets the next random number as a floating point number.
        * \return Returns the next random floating point number.
    */
    float GetNumberF();

    /*!
        * \brief Gets the next random number as a double.
        * \return Returns the next random double precision number.
    */
    double GetNumberD();

    /*!
        * \brief Sets the seed of the number generator.
        * \param seed The new seed for the number generator.
    */
    void SetSeed(unsigned int seed);

    /*!
        * \brief Gets the seed of the number generator.
        * \return Returns the seed of the number generator.
    */
    unsigned int GetSeed();

    /*!
        * \brief Sets the range of generated numbers.
        * \param minimum The minimum number that can be generated.
        * \param maximum The maximum number that can be generated.
    */
    void SetRange(int minimum, int maximum);

private:
    int m_rangeMinimum;     /*!< The minimum number able to generate. */
    int m_rangeMaximum;     /*!< The maximum number able to generate. */
    unsigned int m_seed;    /*!< The seed for the number generator. */

    std::mt19937 m_mersenneTwister; /*!< The actual generator. */
    std::uniform_real_distribution<> m_numberDistribution;  /*!< The number distribution for generating numbers. */

};

