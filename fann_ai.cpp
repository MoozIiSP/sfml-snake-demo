#include "fann_ai.h"

snake_ai_interface::snake_ai_interface(int num_input, int num_output, int num_layers, int num_neurons_hidden) {
    // fann
    // 输入层：苹果距离，蛇身长度，左边环境，前方环境，右边环境
    this->ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);

    // 设置网络学习率和时刻(？)
    fann_set_learning_rate(this->ann, 0.8);
    fann_set_learning_momentum(this->ann, 0.2);

    // 设置Layers的激励函数
    fann_set_activation_function_hidden(this->ann, FANN_GAUSSIAN);
    fann_set_activation_function_output(this->ann, FANN_SIGMOID_SYMMETRIC);
}

snake_ai_interface::~snake_ai_interface() {
    delete this->ann;
}

void snake_ai_interface::saveANN(std::string filename) {
    if (fann_save(this->ann, filename.c_str())) {
        std::cout << "Successful!" << std::endl;
    } else {
        std::cout << "Failed!" << std::endl;
    }
}

bool snake_ai_interface::loadANN(std::string filename) {
    this->ann = fann_create_from_file(filename.c_str());
    if (this->ann != NULL)
        return true;
    return false;
}

short snake_ai_interface::nextDir(const short base, const short dir) {
    switch (dir) {
        case tLeft:
            return (4 + base - 1) % 4;
        case tFront:
            return base;
        case tRight:
            return (4 + base + 1) % 4;
        default:
            break;
    }
}

//// Return val not label
std::string snake_ai_interface::detectedBlockType(fann_type input) const {
    int val = (int) input;
    switch (val) {
        case -1:
            return "Obstacle";
        case 0:
            return "None";
        case 1:
            return "Apple";
        default:
            break;
    }
    return "empty_string";
}

// C Style
void snake_ai_interface::createInput(const Stage& game, fann_type input[], short nDir) {
    // env
    Map& map = game.getMap();   // fixed, origin: Map map = game.getMap();
    Point head = game.getSnake().getBody().front();
    sf::Uint16 headDir = game.getSnake().getDir();
    // 蛇头转向并移动
    if (-1 != nDir) {
        headDir = (sf::Uint8) nextDir(headDir, nDir);
        switch (headDir) {
            case Up:
                head.y--;
                break;
            case Down:
                head.y++;
                break;
            case Left:
                head.x--;
                break;
            case Right:
                head.x++;
                break;
            default:    // Hard Fixed
                break;
        }
    }
    // 输入层0 -> 蛇头到食物的距离
    Point apple = game.getApple();
    //input[0] = 1.0 * sqrt(pow(apple.x - head.x, 2.0f) + pow(apple.y - head.y, 2.0f)) / (sqrt(2) * (float) map.getSize());
    input[0] = 1.0 * (pow(apple.x - head.x, 2.0f) + pow(apple.y - head.y, 2.0f)) / (pow(map.getWidth(), 2.0f) + pow(map.getHeight(), 2.0f));
    input[4] = 1.0 - 1.0 * ((float) game.getSnake().getLength() / (float) map.getSize());
    // 输入层2,3,4 -> 蛇头周围是什么
    Point atLeft(head);
    Point atFront(head);
    Point atRight(head);
    switch (headDir) {
        case Up:
            atLeft.x--;
            atFront.y--;
            atRight.x++;
            break;
        case Down:
            atLeft.x++;
            atFront.y++;
            atRight.x--;
            break;
        case Left:
            atLeft.y++;
            atFront.x--;
            atRight.y--;
            break;
        case Right:
            atLeft.y--;
            atFront.x++;
            atRight.y++;
            break;
        default:
            break;
    }
    // Left
    if (map.isInsideOf(atLeft)) {
        switch (map.at(atLeft)) {
            case None:
                input[1] = 0;
                break;
            case Apple:
                input[1] = 1;
                break;
            default:
                input[1] = -1;
                break;
        }
    } else {
        input[1] = -1;
    }
    // Front
    if (map.isInsideOf(atFront)) {
        switch (map.at(atFront)) {
            case None:
                input[2] = 0;
                break;
            case Apple:
                input[2] = 1;
                break;
            default:
                input[2] = -1;
                break;
        }
    } else {
        input[2] = -1;
    }
    // Right
    if (map.isInsideOf(atRight)) {
        switch (map.at(atRight)) {
            case None:
                input[3] = 0;
                break;
            case Apple:
                input[3] = 1;
                break;
            default:
                input[3] = -1;
                break;
        }
    } else {
        input[3] = -1;
    }
}

void snake_ai_interface::think(Stage& game, bool debug) {
    fann_type *t_output; // 临时输出
    int tDir = tFront; // noTurn 维持方向不变
    float max = -INF; // output 最大值
    float dismax = INF;

    // Left, Front, Right 三种移动决策的Rating
    // 并且决定蛇的方向
    fann_type input_front[5]; // 输入？
    createInput(game, input_front, tFront);
    t_output = fann_run(this->ann, input_front); // 将输入参数传入网络中来得到输出
    if (t_output[0] >= max /*&& dismax >= input_front[0]*/){
        max = t_output[0];
        dismax = input_front[0];
        tDir = tFront;
    }
    // Ann Log
    if (debug) showANNStatus(input_front, t_output);

    fann_type input_left[5];
    createInput(game, input_left, tLeft);
    t_output = fann_run(this->ann, input_left); // 将输入参数传入网络中来得到输出
    if (t_output[0] >= max /*&& dismax >= input_left[0]*/){
        max = t_output[0];
        dismax = input_left[0];
        tDir = tLeft;
    }
    // Ann Log
    if (debug) showANNStatus(input_left, t_output);

    fann_type input_right[5];
    createInput(game, input_right, tRight);
    t_output = fann_run(this->ann, input_right); // 将输入参数传入网络中来得到输出
    if (t_output[0] >= max /*&& dismax >= input_right[0]*/){
        max = t_output[0];
        dismax = input_right[0];
        tDir = tRight;
    }
    // Ann Log
    if (debug) showANNStatus(input_right, t_output);

    // first run - fixed
    // 随机上设置一个方向，然后curiosity减1，好奇度与决定新方向，主要解决死循环
    bool tmp = false;
    if (!debug && ((game.r)() % (100 - 0 + 1) + 0) < curiosity) {
        tmp = true;
        tDir = ((game.r)() % (2 - 0 + 1) + 0);
        curiosity--;
    }

    // Final Decision
    fann_type input[5];
    createInput(game, input);

    int head_dir = game.getSnake().getDir();
    game.getSnake().setDir(this->nextDir(head_dir, tDir));

    game.iteration(); // 游戏进程
    fann_type output[1];
    output[0] = game.getReward();

    if (debug) showANNStatus(input, output);
    if (debug) std::cout << "D: " << tDir << " <-> " << this->nextDir(head_dir, tDir) << std::endl;

    // real and predict

    // train net
    fann_train(ann, input, output);
    
    if (debug) t_output = fann_run(ann, input);
    if (debug) std::cout << "OUTPUT: " << t_output[0] << "<->" << output[0] << std::endl;
}

void snake_ai_interface::showANNStatus(fann_type input[], fann_type output[]) {
    std::cout << "Input Layers: " << std::endl;
    std::cout << "0~Distance:\t" << input[0] << std::endl;
    std::cout << "1~Left:\t\t" << this->detectedBlockType(input[1]) << std::endl;
    std::cout << "2~Front:\t" << this->detectedBlockType(input[2]) << std::endl;
    std::cout << "3~Right:\t" << this->detectedBlockType(input[3]) << std::endl;
    std::cout << "4~Length:\t\t" << input[4] << std::endl;
    std::cout << "Output Layers:" << std::endl;
    std::cout << "0~:\t" << output[0] << std::endl;
}

