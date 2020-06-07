#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include "Chrom.h"

#define POPSIZE 1000
using namespace std;
bool solutionFound = false;

vector<char> randomize(const vector<char>& dict){
    vector<char> chars;
    for (int i=0;i<298;i++){
        char c = dict[rand() % 28];
        chars.push_back(c);
    }
    return chars;
}

vector<Chrom> initializePopulation(const vector<char>& dict){
    vector<Chrom> population;
    for(int i=0;i<POPSIZE;i++){
        auto chrom = Chrom();
        chrom.getChars() = randomize(dict);
        population.push_back(chrom);
    }
    return population;
}

//fitness function will be such that each correct char will add 1 point
//to the chromosome, in a span of 0-298
int fitnessScore(const vector<char>& chromosome, const string& compare){
    int score = 0, i=0;
    for (char c : chromosome){
        if (c == compare[i]){
            score++;
        }
        i++;
    }
    return score;
}

vector<Chrom> fitnessFunction(vector<Chrom>* population, const string& compare){
    for (auto &chrom : *population) {
        if (chrom.getFitnessScore()==0) {
            chrom.setFitnessScore(fitnessScore(chrom.getChars(), compare));
        }
        if (chrom.getFitnessScore() == 298) {
            solutionFound = true;
            break;
        }
    }
    return *population;
}

Chrom selection(const vector<Chrom>& ranked){
    int random = rand()%ranked.size();
    return ranked[random];
}

vector<Chrom> crossover(Chrom parent1, Chrom parent2, vector<Chrom> children){
    Chrom child1 = Chrom();
    Chrom child2 = Chrom();
    int coRate = rand()%10;
    if (coRate <= 7) { //crossover rate is 80%
        for(int i=0;i<parent1.getChars().size();i++){
            int chance = rand()%2; //uniform crossover
            if(chance == 0){
                child1.getChars().push_back(parent1.getChars()[i]);
                child2.getChars().push_back(parent2.getChars()[i]);
            } else if (chance == 1){
                child1.getChars().push_back(parent2.getChars()[i]);
                child2.getChars().push_back(parent1.getChars()[i]);
            }
        }
    } else {
        child1 = parent1;
        child2 = parent2;
    }
    children.push_back(child1);
    children.push_back(child2);
    return children;
}

Chrom mutation(Chrom child, vector<char> dict){
    int mutationRate;
    for(int i=0; i<child.getChars().size();i++) {
        mutationRate = rand() % 1000;
        if (mutationRate == 1) {
            child.getChars()[i] = dict[rand() % 28];
        }
    }
    return child;
}

vector<Chrom> newGeneration(vector<Chrom> oldGen, const vector<char>& dict,
                            vector<double> *best, vector<double> *avg){
    vector<Chrom> newGen, ranked;
    sort(oldGen.begin(), oldGen.end());
    cout << "fittest chromosome's score: " << oldGen[POPSIZE-1].getFitnessScore() << endl;
    best->push_back(oldGen[POPSIZE-1].getFitnessScore());
    //elitism
    newGen.push_back(oldGen[POPSIZE-1]);
    newGen.push_back(oldGen[POPSIZE-2]);
    oldGen.erase(oldGen.begin(),oldGen.begin()+2);
    //create ranked vector
    for (int i=0;i<oldGen.size();i++){
        int j=i;
        while(j>=0){
            ranked.push_back(oldGen[i]);
            j--;
        }
    }
    //add 2 children until generation is full
    while(newGen.size()!=POPSIZE) {
        Chrom parent1 = selection(ranked);
        Chrom parent2 = selection(ranked);

        vector<Chrom> children;
        children = crossover(parent1, parent2, children);
        Chrom child1 = children[0];
        Chrom child2 = children[1];

        child1 = mutation(child1, dict);
        child2 = mutation(child2, dict);
        newGen.push_back(child1);
        newGen.push_back(child2);
    }
    return newGen;
}

void printVec(const vector<double>& vec, const string& name){
    cout << name << endl;
    for (auto v : vec){
        cout << v << endl;
    }
    cout << endl;
}

int main() {
    time_t begin, end;
    time (&begin);
    srand( (unsigned)time(nullptr) );
    vector<char> dict = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n',
                         'o','p','q','r','s','t','u','v','w','x','y','z',' ','.'};
    string compare = "to be or not to be that is the question. whether tis "
                     "nobler in the mind to suffer. the slings and arrows of "
                     "outrageous fortune. or to take arms against a sea of "
                     "troubles and by opposing end them. to die to sleep. no "
                     "more. and by a sleep to say we end. the heartache and "
                     "the thousand natural shocks.";
    vector<Chrom> population;
    vector<double> best, avg, gen;
    population = initializePopulation(dict);
    int gens = 1;
    while (true) {
        gen.push_back(gens);
        population = fitnessFunction(&population, compare);
        if (solutionFound) {
            cout << "fittest chromosome's score: 298" << endl;
            break;
        }
        //calculate average
        double average=0;
        for (const auto& n : population){
            average += n.getFitnessScore();
        }
        average /= POPSIZE;
        avg.push_back(average);
        population = newGeneration(population, dict, &best, &avg);
        gens++;
    }
    time (&end);
    int seconds = difftime(end,begin);
    int minutes = seconds/60;
    if (minutes == 0) {
        cout << "Solution was found within " << seconds << " seconds and " <<
             gens << " generations" << endl;
    } else {
        cout << "Solution was found within " << minutes <<
             " minutes and " << seconds % 60 << " seconds and " << gens << " generations" << endl;
    }
    return 0;
}