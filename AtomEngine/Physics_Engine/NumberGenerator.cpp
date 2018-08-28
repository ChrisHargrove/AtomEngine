#include "NumberGenerator.h"

NumberGenerator::NumberGenerator(unsigned int seed, int minimum, int maximum)
{
    m_mersenneTwister.seed(seed);
    m_rangeMinimum = minimum;
    m_rangeMaximum = maximum;
    m_numberDistribution = std::uniform_real_distribution<>(m_rangeMinimum, m_rangeMaximum);
}

NumberGenerator::~NumberGenerator()
{
}

int NumberGenerator::GetNumber()
{
    return (int)m_numberDistribution(m_mersenneTwister);
}

float NumberGenerator::GetNumberF()
{
    return (float)m_numberDistribution(m_mersenneTwister);
}

double NumberGenerator::GetNumberD()
{
    return m_numberDistribution(m_mersenneTwister);
}

void NumberGenerator::SetSeed(unsigned int seed)
{
    m_seed = seed;
    m_mersenneTwister.seed(seed);
}

unsigned int NumberGenerator::GetSeed()
{
    return m_seed;
}

void NumberGenerator::SetRange(int minimum, int maximum)
{
    m_rangeMinimum = minimum;
    m_rangeMaximum = maximum;
    m_numberDistribution = std::uniform_real_distribution<>(m_rangeMinimum, m_rangeMaximum);
}
