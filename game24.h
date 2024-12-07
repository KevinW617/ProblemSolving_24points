#ifndef GAME24_H
#define GAME24_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <string>
#include <vector>
#include <random>
#include <FL/fl_ask.H>

class Game24Window : public Fl_Window {
public:
    Game24Window(int w, int h, const char* title);
    ~Game24Window();

private:
    static void deal_cb(Fl_Widget*, void* v);
    static void calculate_cb(Fl_Widget*, void* v);
    static void load_file_cb(Fl_Widget*, void* v);
    
    void deal_cards();
    bool calculate_24(std::vector<double>& numbers);
    bool try_operations(std::vector<double>& nums);

    Fl_Button* deal_button;
    Fl_Button* calc_button;
    Fl_Button* file_button;
    Fl_Box* card_displays[4];
    Fl_Input* number_inputs[4];
    Fl_Output* result_output;
    
    std::random_device rd;
    std::mt19937 gen;
};

#endif