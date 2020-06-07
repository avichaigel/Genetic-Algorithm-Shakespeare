//
// Created by avichai on 12/05/2020.
//

#include "Chrom.h"

int Chrom::getFitnessScore() const {
    return fitnessScore;
}

void Chrom::setFitnessScore(int score) {
    Chrom::fitnessScore = score;
}

vector<char> &Chrom::getChars() {
    return chars;
}

void Chrom::setChars(const vector<char> &chromos) {
    Chrom::chars = chromos;
}
