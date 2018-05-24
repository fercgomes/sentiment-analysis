#include "WordClassification.h"

WordClassification::WordClassification()
{
    averageScore = 0.0f;
    ocurrences = 0;
}

WordClassification::~WordClassification()
{}

void WordClassification::AddOcurrence(float score)
{
    averageScore = (ocurrences * averageScore + score) / (ocurrences + 1);
    ocurrences++;
}