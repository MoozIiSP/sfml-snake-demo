//
// Created by Diana-07 on 2017/5/16.
//

#include "snake.h"

Snake::Snake(Map* map, Point p, sf::Uint16 dir) {
    this->dMap = map;
    this->is_alive = true;
    this->dir = dir;
    this->body.push_front(p);
    this->dMap->setPosition(p, Obstacle);
}

Snake::~Snake() {}

bool Snake::isAlived() {
    return this->is_alive;
}

// bool Snake::hasAte() {
//     return this->has_ate;
// }

sf::Uint16 Snake::move() {
    Point head = this->body.front();
    switch (this->dir) {
        case Up:
            head.y -= 1;
            break;
        case Down:
            head.y += 1;
            break;
        case Left:
            head.x -= 1;
            break;
        case Right:
            head.x += 1;
            break;
        default:    // Hard Fixed
            return -1;
    }

    if ((*this->dMap).at(head) == Apple) {
        this->body.push_front(head);
        this->dMap->setPosition(head, Obstacle);
        //this->has_ate = true;
        return Apple;
    } else if ((*this->dMap).at(head) == None) {
        this->dMap->setPosition(head, Obstacle);
        this->body.push_front(head);
        this->dMap->setPosition(this->body.back(), None);
        this->body.pop_back();
        return None;
    } else {
        this->is_alive = false;
        return Obstacle;
    }
}

void Snake::setDir(sf::Uint16 dir) {
    Snake::dir = dir;
}

sf::Uint16 Snake::getDir() const {
    return dir;
}

sf::Uint16 Snake::getLength() {
    return (sf::Uint16) this->body.size();
}

std::deque<Point> Snake::getBody() {
    return this->body;
}
