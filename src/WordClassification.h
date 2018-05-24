class WordClassification
{
public:
    float score;
    unsigned int ocurrences;
    /* TODO: standard deviation */

    WordClassification();
    ~WordClassification();

    void AddOcurrence(float score);

private:

};