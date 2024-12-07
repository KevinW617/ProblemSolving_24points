#include "game24.h"
#include <cmath>
#include <algorithm>
#include <sstream>

#define TARGET 24
#define PRECISION 1e-6
#define ADD 0
#define MULTIPLY 1
#define SUBTRACT 2
#define DIVIDE 3

std::vector<std::string> expr;

std::string convert_expr_to_readable(const std::vector<std::string>& expr) {
    if (expr.empty()) return "";
    
    std::string result;
    for (size_t i = 0; i < expr.size(); i++) {
        if (i == 0) {
            result = "(" + expr[i] + ")";
        } else {
            result += "----(" + expr[i] + ")";
        }
    }
    return result + "=24";
}

Game24Window::Game24Window(int w, int h, const char* title)
    : Fl_Window(w, h, title), gen(rd()) {
    
    // 创建发牌按钮
    deal_button = new Fl_Button(20, 20, 70, 25, "发牌");
    deal_button->callback(deal_cb, this);

    // 创建计算按钮
    calc_button = new Fl_Button(100, 20, 70, 25, "计算24");
    calc_button->callback(calculate_cb, this);

    // 创建文件按钮
    file_button = new Fl_Button(180, 20, 70, 25, "加载文件");
    file_button->callback(load_file_cb, this);

    // 创建4个数字输入框和显示框
    for (int i = 0; i < 4; i++) {
        number_inputs[i] = new Fl_Input(20 + i*80, 60, 70, 25);
        card_displays[i] = new Fl_Box(20 + i*80, 100, 70, 100);
        card_displays[i]->box(FL_UP_BOX);
    }

    // 创建结果显示框
    result_output = new Fl_Output(20, 220, 300, 25);
}

Game24Window::~Game24Window() {
    delete deal_button;
    delete calc_button;
    delete file_button;
    for (int i = 0; i < 4; i++) {
        delete number_inputs[i];
        delete card_displays[i];
    }
    delete result_output;
}

void Game24Window::deal_cb(Fl_Widget*, void* v) {
    ((Game24Window*)v)->deal_cards();
}

void Game24Window::deal_cards() {
    // 创建1-13的均匀分布
    std::uniform_int_distribution<> dis(1, 13);
    
    // 生成4个随机数
    for(int i = 0; i < 4; i++) {
        int number = dis(gen);
        std::string str = std::to_string(number);
        number_inputs[i]->value(str.c_str());
        card_displays[i]->copy_label(str.c_str());
    }
}

void Game24Window::calculate_cb(Fl_Widget*, void* v) {
    Game24Window* win = (Game24Window*)v;
    std::vector<double> numbers;
    
    // 获取输入的数字
    for (int i = 0; i < 4; i++) {
        const char* val = win->number_inputs[i]->value();
        if (!val || !*val) {
            fl_alert("请输入所有数字！");
            return;
        }
        numbers.push_back(std::stod(val));
    }

    // 清空上一次的表达式
    expr.clear();
    
    if (win->calculate_24(numbers)) {
        // 转换表达式并显示
        std::string readable = convert_expr_to_readable(expr);
        win->result_output->value(readable.c_str());
    } else {
        win->result_output->value("无解");
    }
}

void Game24Window::load_file_cb(Fl_Widget*, void* v) {
    fl_alert("文件加载功能待实现");
}

bool Game24Window::calculate_24(std::vector<double>& numbers) {
    return try_operations(numbers);
}

bool Game24Window::try_operations(std::vector<double> &nums) {
    if (nums.size() == 0) return false;
    if (nums.size() == 1) {
        return std::abs(nums[0] - TARGET) < PRECISION;
    }

    for (int i = 0; i < nums.size(); i++) {
        for (int j = 0; j < nums.size(); j++) {
            if (i == j) continue;

            std::vector<double> next_nums;
            for (int k = 0; k < nums.size(); k++) {
                if (k == i || k == j) continue;
                next_nums.push_back(nums[k]);
            }

            for (int op = 0; op < 4; op++) {
                // minor optimization
                if ((op == ADD || op == MULTIPLY) && i > j) continue;

                if (op == ADD) {
                    next_nums.push_back(nums[i] + nums[j]);
                    expr.push_back(std::to_string(int(nums[i])) + "+" + std::to_string(int(nums[j])));
                } else if (op == MULTIPLY) {
                    next_nums.push_back(nums[i] * nums[j]);
                    expr.push_back(std::to_string(int(nums[i])) + "*" + std::to_string(int(nums[j])));
                } else if (op == SUBTRACT) {
                    next_nums.push_back(nums[i] - nums[j]);
                    expr.push_back(std::to_string(int(nums[i])) + "-" + std::to_string(int(nums[j])));
                } else if (op == DIVIDE) {
                    if (std::abs(nums[j]) < PRECISION) continue;
                    next_nums.push_back(nums[i] / nums[j]);
                    expr.push_back(std::to_string(int(nums[i])) + "/" + std::to_string(int(nums[j])));
                }

                if (try_operations(next_nums)) return true;
                expr.pop_back();
                next_nums.pop_back();
            }
        }
    }
    return false;
}