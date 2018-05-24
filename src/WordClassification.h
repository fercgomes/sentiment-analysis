#pragma once

class WordClassification
{
public:
    float averageScore;
    unsigned int ocurrences;
    /* TODO: standard deviation */

    WordClassification();
    ~WordClassification();

    void AddOcurrence(float score);

private:

};