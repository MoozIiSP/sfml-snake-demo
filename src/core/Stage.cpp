//
// Created by Diana-07 on 2017/5/16.
//

#include <sstream>
#include "Stage.h"

Stage::Stage(sf::Uint32 seed, int x, int y) {
    if (this->font.loadFromFile("UbuntuMono.ttf")){
    }

    this->r.seed(seed);
    this->_map = new Map(x, y);
    this->_snake = new Snake(this->_map, Point(this->_map->getWidth() / 2,
                                               this->_map->getHeight() / 2));
    this->apple = Point(0, 0);
    this->_map->setPosition(apple, 1);
    this->_score = 0;
    this->_speed = 1; // not implemented
    this->moved = 0;

    this->reward = 0;

    this->scale = this->_map->getWidth() > this->_map->getHeight()?
                  this->wndHeight / this->_map->getHeight() : this->wndWidth / this->_map->getWidth();

    this->head.setSize(sf::Vector2f(this->scale, this->scale));
    this->head.setFillColor(sf::Color(255, 97, 0));
    this->body.setSize(sf::Vector2f(this->scale, this->scale));
    this->body.setFillColor(sf::Color::Yellow);
    this->rApple.setRadius((float) this->scale / 2.0f);
    this->rApple.setFillColor(sf::Color::Green);

    // Text
    this->system_status.setFont(font);
    this->system_status.setPosition(0.0f, 0.0f);
    this->system_status.setFillColor(sf::Color::White);
    this->system_status.setCharacterSize(16);
    this->system_status.setStyle(sf::Text::Regular);
}

Stage::~Stage() {
    delete this->_map;
    delete this->_snake;
}

Snake& Stage::getSnake() const {
    return *this->_snake;
}

Point Stage::getApple() const {
    return apple;
}

void Stage::waitKeyboard(sf::Event &evt) {
    if (evt.type == sf::Event::KeyPressed) {
        switch (evt.key.code) {
            case sf::Keyboard::Up:
                if (_snake->getDir() != _snake->Down)
                    (this->_snake)->setDir(_snake->Up);
                break;
            case sf::Keyboard::Down:
                if (_snake->getDir() != _snake->Up)
                    (this->_snake)->setDir(_snake->Down);
                break;
            case sf::Keyboard::Left:
                if (_snake->getDir() != _snake->Right)
                    (this->_snake)->setDir(_snake->Left);
                break;
            case sf::Keyboard::Right:
                if (_snake->getDir() != _snake->Left)
                    (this->_snake)->setDir(_snake->Right);
                break;
            default:
                break;
        }
    }
}

void Stage::drawTo(sf::RenderWindow& wind) {
    std::ostringstream msg;
    msg << "Resolution: ";
    msg << wind.getSize().x << "x" << wind.getSize().y << "\n";
    msg << "Map: " << (*this->_map).getHeight() << "x" << (*this->_map).getWidth();
    msg << "\n";
    msg << "Snake: " << (*this->_snake).getBody().front().x << "," << (*this->_snake).getBody().front().y << " (Position)"
        << " " << (*this->_snake).getLength();
    msg << "\n";
    msg << "Apple: " << (apple).x << "," << (apple).y << " "
        << this->_map->at(apple);
    msg << "\n";
    msg << "Score: " << this->_score << " " << this->reward << " (reward)";
    msg << "\n";
    system_status.setString(msg.str());

    std::deque<Point> b = (this->_snake)->getBody();
    for (std::deque<Point>::iterator it = b.begin()+1;
         it < b.end();
         it++) {
        body.setPosition(sf::Vector2f(it->x * scale, it->y * scale));

        wind.draw(body);
    }

    head.setPosition(sf::Vector2f(b.front().x * scale, b.front().y * scale));
    wind.draw(head);

    rApple.setPosition(sf::Vector2f(apple.x * this->scale, apple.y * this->scale));
    wind.draw(rApple);

    wind.draw(system_status);
}

bool Stage::iteration() {

    if (this->_snake->isAlived()) {
        this->moved++;
        sf::Uint8 flag = this->_snake->move();
        Point head = this->_snake->getBody().front();
        switch (flag) {
            case 0:
                // no action, but case 2 will handle border overflow.
                this->reward = -1.0 * sqrt(pow(apple.x - head.x, 2.0f) + pow(apple.y - head.y, 2.0f)) / (sqrt(2) * (float) this->_map->getSize());
                break;
            case 1:
                this->_score++;
                this->reward = 1.0;
                this->genRandApple();
                break;
            case 2:
                // new game
                this->reward = -1;
                reset();
                return false;
            default:
                break;
        }
    }
    return true;
}

Map& Stage::getMap() const {
    return *(this->_map);
}

void Stage::reset() {
    // delete old
    delete this->_map;
    delete this->_snake;

    // new
    this->isRuning = true;

    this->_map = new Map(this->_map->getWidth(), this->_map->getHeight());
    this->_snake = new Snake(this->_map, Point(this->_map->getWidth() / 2,
                                               this->_map->getHeight() / 2));
    this->genRandApple();
    this->_score = 0;
    this->_speed = 1;
    this->moved = 0;

    this->scale = this->_map->getWidth() > this->_map->getHeight()?
                  this->wndHeight / this->_map->getHeight() : this->wndWidth / this->_map->getWidth();
    this->body.setSize(sf::Vector2f(this->scale, this->scale));
    this->body.setFillColor(sf::Color::Yellow);
    this->rApple.setRadius((float) this->scale / 2.0f);
    this->rApple.setFillColor(sf::Color::Green);
}

void Stage::FixedApple() {
    // check apple exception - dirty fixed
    if (this->_map->at(this->getApple()) == 0 || this->_map->at(this->getApple()) == 2)
        this->genRandApple();
}

float Stage::getReward() const {
    return reward;
}

float Stage::getMoved() const {
    return this->moved;
}

void Stage::genRandApple() {
    this->_map->setPosition(this->apple, 0);
    while (true) {
        this->apple = Point(r() % this->_map->getHeight(),
                            r() % this->_map->getWidth());
        if (this->_map->at(this->apple) == 0) {
            this->_map->setPosition(this->apple, 1);
            break;
        }
    }
}
