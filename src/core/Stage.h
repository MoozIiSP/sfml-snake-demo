//
// Created by Diana-07 on 2017/5/16.
//

#ifndef SFML_STAGE_H
#define SFML_STAGE_H

#include <random>
#include <deque>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics/Text.hpp>

#include <fann.h>

#include "snake.h"
#include "Map.h"

class Stage {
private:
    const int wndHeight = 500;
    const int wndWidth = 500;
    //// Act
    Map *_map;
    Snake *_snake;
    Point apple;
    //// System
    sf::Uint32 scale;
    float reward;
    float moved;
    //// AI
    struct fann* ann;
private:
    sf::Uint64 _score; // TODO 将_score改写为reward，反正要和ann的输出对应起来
    sf::Uint8 _speed;
    sf::Text system_status; // 每次iteration之后更新信息
    sf::Text ann_status; // TODO 一些调试信息
    sf::Font font;
    //
    bool isRuning;
    //// Render
    sf::RectangleShape head;
    sf::RectangleShape body;
    sf::CircleShape rApple;
public:
    std::minstd_rand r;

    Stage(sf::Uint32 seed, int x, int y);
    ~Stage();
    Snake& getSnake() const;
    Point getApple() const;
    Map& getMap() const;
    float getReward() const;
    float getMoved() const;
    void reset();
    void waitKeyboard(sf::Event& evt);
    void genRandApple();
    void drawTo(sf::RenderWindow& wind);
    void FixedApple();
    bool iteration(); // 同时计分
    void showSysInfo();
    //void updateWall();
};


#endif //SFML_STAGE_H
