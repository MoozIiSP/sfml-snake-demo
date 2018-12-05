#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
//#include <SFML/Config.hpp>

#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <deque>
#include <string>
#include <sstream>

#include <ctime>
#include <exception>

#include "fann_ai.h"

//enum Direction {Left, Up, Right, Down};
//enum Grid {None, Apple, Obstacle};
//enum Turn {tLeft, tFront, tRight};

// void itoa(int n, char* buff) {
//    int i = 0;
//    while (n > 0) {
//        buff[i++] = n % 10;
//        n = n / 10;
//    }
//    buff[i] = '\0';
// }

int main(int argc, char* argv[]) {

    int train_iteration;
    char load = false;
    std::cout << "How many time do you want to tarin? ";
    std::cin >> train_iteration;
    std::cout << "Load already ann file? [Y/n]";
    std::cin >> load;

    sf::RenderWindow wind(sf::VideoMode(500,500), "Snack AI");
    Stage game(time(NULL), 10, 10);

    snake_ai_interface ai(5,1,3,8);
    if (load == 'n') {
        ai.curiosity = train_iteration;
    } else if (load == 'Y') {
        if (!ai.loadANN("snake.net")) {
            std::cout << "Error" << std::endl;
            exit(-1);
        }
    } else {
        std::cout << "Please Input \"Y\" or \"n\"" << std::endl;
        exit(-1);
    }

    bool gamePaused = false;
    bool playerControl = false;
    bool notTrained = true;

    if (train_iteration == 0)   playerControl = true;

    if (!playerControl && load == 'n') {
        for (int it = 0; it < train_iteration; ) {
            //        itoa(i, buff);
            // std::this_thread::sleep_for(std::chrono::milliseconds(100));
            //char s = i % 10 + '0';
            //wind.setTitle(s);

            //wind.clear();
            ai.think(game);

            game.FixedApple();
            if (game.iteration()) {
                if (game.getMoved() > 100) {
                    it++;
            if (it%100000 == 0)
                std::cout << it << std::endl;
                    continue;
                }
            } else {
            if (it%100000 == 0)
                std::cout << it << std::endl;
                it++;
            }
            //game.drawTo(wind);
            // 输出Log
            //wind.display();
        }
    }

    // TODO 多窗口内容如何编排
    while (wind.isOpen()) {
        sf::Event evt;

        // if (notTrained) {
        //     // 先训练ann，一个完整的游戏过程，没有时间片段
            
        // }

        while (wind.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed) {
                wind.close();
            }

            // Configure
            if (evt.type == sf::Event::KeyPressed) {
                switch (evt.key.code) {
                    case sf::Keyboard::P:
                        gamePaused = !gamePaused;
                        break;
                    case sf::Keyboard::A:
                        break;
                    case sf::Keyboard::D:
                        break;
                    case sf::Keyboard::S:
                        playerControl = !playerControl;
                        break;
                    case sf::Keyboard::N:
                        // reset()
                        break;
                    case sf::Keyboard::Z:
                        ai.saveANN("snake.net");
                        break;
                }
            }

            // dir controller
            if (playerControl)
                game.waitKeyboard(evt);
        }

        if (gamePaused) continue;

        // Content Draw
        wind.clear();

        game.FixedApple();
        if (playerControl) {
            game.iteration();
        }
        else {
            ai.think(game, true);
        }

        game.drawTo(wind);

        wind.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}