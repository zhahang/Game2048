//
//  main.cpp
//  2048
//
//  Created by zhahang on 2022/6/28.
//

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <ctime>
#include <cstdlib>

#include "scene.hpp"
#include "hardware_input.hpp"


using namespace std;


static zh::Scene scene;

void onKeyboardDown(int type) {
    if (zh::Action(type) == zh::Action::kQuit) {
        exit(0);
        return;
    } else if (zh::Action(type) == zh::Action::kReset) {
        scene.clear();
    } else {
        scene.control(zh::Action(type));
    }
    scene.draw();
}

int main(int argc, const char * argv[]) {
    std::cout << "welcom to 2048!\n";
    scene.draw();
    auto result_callback = [=](zh::Result result) {
        switch (result) {
            case zh::Result::kVictory:
                cout << "--- VICTORY! ---" << endl;
                cout << endl;
                break;
            case zh::Result::kDefeat:
                cout << "--- DEFEAT! ---" << endl;
                cout << endl;
                break;
            case zh::Result::kError:
                cout << "--- ERROR! ---" << endl;
                break;
            default:
                break;
        }
    };
    scene.setResultCallback(result_callback);
    scanKeyboard(onKeyboardDown);
    return 0;
}
