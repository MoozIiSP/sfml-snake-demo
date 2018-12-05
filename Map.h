//
// Created by Diana-07 on 2017/5/16.
//

#ifndef SFML_MAP_H
#define SFML_MAP_H

#include <SFML/Config.hpp>
#include "Point.h"
#include <vector>


class Map {
    enum Grid {None, Apple, Obstacle};
private:
    std::vector<std::vector<sf::Uint16> > *board;
    sf::Uint16 _width;
    sf::Uint16 _height;
public:
    Map(sf::Uint16 width = 5, sf::Uint16 height = 5)
            : _width(width), _height(height) {
        this->board = new std::vector<std::vector<sf::Uint16> >(this->_height,
                                                              std::vector<sf::Uint16>(this->_width));
    }
    ~Map() {
        delete this->board;
    }

    sf::Int16 at(const Point& p);
    sf::Uint32 getSize() const;
    bool isInsideOf(Point p);
    void setPosition(Point p, int type);
    void newMap(int i = 5, int j = 5);
    sf::Uint16 getWidth() const;
    void setWidth(sf::Uint16 _width);
    sf::Uint16 getHeight() const;
    void setHeight(sf::Uint16 _height);
};


#endif //SFML_MAP_H
