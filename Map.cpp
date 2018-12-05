//
// Created by Diana-07 on 2017/5/16.
//

#include "Map.h"

sf::Int16 Map::at(const Point& p) {
    // border checking
    if (0 <= p.x && p.x < this->_width &&
            0 <= p.y && p.y < this->_height) {
        return (*this->board).at(p.y).at(p.x);
    }
    else
        return (sf::Int16) -1;
}

sf::Uint32 Map::getSize() const {
    return this->_width * this->_height;
}

void Map::setPosition(Point p, int type) {
    (*this->board)[p.y][p.x] = (sf::Uint8) type;
}

void Map::newMap(int i, int j) {
    delete this->board;
    this->board = new std::vector<std::vector<sf::Uint16>>(i,
                                                          std::vector<sf::Uint16>(j));
}

sf::Uint16 Map::getWidth() const {
    return _width;
}

void Map::setWidth(sf::Uint16 _width) {
    Map::_width = _width;
}

sf::Uint16 Map::getHeight() const {
    return _height;
}

void Map::setHeight(sf::Uint16 _height) {
    Map::_height = _height;
}

bool Map::isInsideOf(Point p) {
    if (0 <= p.x && p.x < this->_width &&
            0 <= p.y && p.y < this->_height)
        return true;
    return false;
}

