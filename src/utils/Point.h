//
// Created by Diana-07 on 2017/5/16.
//

#ifndef SFML_POINT_H
#define SFML_POINT_H

//sf::Vector2i

struct Point {
    int x, y;
    Point() {x=0, y=0;}
    Point(int _x, int _y) {x=_x, y=_y;}
    Point(const Point& p) {x=p.x, y=p.y;}
};


#endif //SFML_POINT_H
