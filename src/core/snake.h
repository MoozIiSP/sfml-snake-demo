//
// Created by Diana-07 on 2017/5/16.
//

#ifndef SFML_SNAKE_H
#define SFML_SNAKE_H

#include <SFML/Config.hpp>

#include "Point.h"
#include "Map.h"
#include <deque>

class Snake {
public:
    enum Direction {Left, Up, Right, Down};
    enum Grid {None, Apple, Obstacle};
    // enum Turn {tLeft, tFront, tRight};
private:
    sf::Uint16 dir;
    std::deque<Point> body;
    Map* dMap;
    bool is_alive;
public:
    Snake(Map* map, Point p, sf::Uint16 dir = Down);
    ~Snake();

    bool isAlived();
    // bool hasAte();
    sf::Uint16 move();
    void setDir(sf::Uint16 dir);
    sf::Uint16 getDir() const;
    sf::Uint16 getLength();
    std::deque<Point> getBody();

};


#endif //SFML_SNAKE_H
