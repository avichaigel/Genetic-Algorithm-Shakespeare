//
// Created by avichai on 12/05/2020.
//

#ifndef EVO1B_CHROM_H
#define EVO1B_CHROM_H

#include <vector>

using namespace std;

class Chrom {
private:
    int fitnessScore;
    vector<char> chars;

public:
    int getFitnessScore() const;
    void setFitnessScore(int fitnessScore);
    vector<char> &getChars();
    void setChars(const vector<char> &chrom);
    bool operator < (const Chrom& other){
        return fitnessScore < other.fitnessScore;
    }
};


#endif //EVO1B_CHROM_H
