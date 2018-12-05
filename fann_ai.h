//
// Created by Diana-07 on 2017/5/22.
//

#ifndef SFML_FANN_AI_H
#define SFML_FANN_AI_H

#include <fann.h>
#include <string>
#include <iostream>
#include "Stage.h"

// TODO 将main中的有关fann的代码单独写成模块
class snake_ai_interface {
private:
    struct fann *ann;
public:
    enum Direction {Left, Up, Right, Down};
    enum Grid {None, Apple, Obstacle};
    enum Turn {tLeft, tFront, tRight};
    const int INF = 123456;
    int curiosity;

    // attribute
    bool train;

    // method
    snake_ai_interface(int num_input, int num_output, int num_layers, int num_neurons_hidden);
    ~snake_ai_interface();

    short nextDir(const short base, const short dir);
    std::string detectedBlockType(fann_type input) const;
    void createInput(const Stage& game, fann_type* input, short nDir = -1);
    void think(Stage& game, bool debug = false);
    void saveANN(std::string filename);
    bool loadANN(std::string filename);
    void showANNStatus(fann_type input[], fann_type output[]);
};

#endif //SFML_FANN_AI_H
