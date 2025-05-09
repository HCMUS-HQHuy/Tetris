#ifndef BOTBRAIN_HPP
#define BOTBRAIN_HPP

#include <cstdint>
#include <fstream>
#include <vector>

using std::vector;

class BotBrain {
private:
    int numWeight, numSpecLimmit;
    int64_t **weight;
    int64_t *shapeWeight;
    uint8_t *lineLimit;

    static int8_t countSet;
    static int64_t sumScore;
    
    uint64_t* map; // share pointer!

    uint8_t getLineLimit(uint8_t curY = 25); // curY = default means doesnt need it

    int sizeOfPupulation;
    uint8_t mutationRateInver; // mutationRate = 1/mutationRateInver;

    int random(int L, int R);
    vector<vector<int>> population;
    vector<int> individual;
    vector<int> score;

    vector<int> generateIndividual();
    void generatePopulation();
    vector<int> crossover(vector<int> &a, vector<int> &b);
    void mutate(vector<int> &a);

public:
    BotBrain(uint64_t* map);
    ~BotBrain();

    void updateWeight();
    void load();
    void writeDown(vector<vector<int>> &a);

    static void setScore(int val);
    int getScore();
    int getHeuristicScore(uint16_t shape, uint8_t shapeID, int X, int Y);
};

#endif