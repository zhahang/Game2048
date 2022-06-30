//
//  scene.cpp
//  2048
//
//  Created by zhahang on 2022/6/29.
//

#include <iostream>
#include <vector>
#include <string>

#include "scene.hpp"


using namespace std;

namespace zh {

Scene::Scene() :
    score_(0),
    max_num_(0),
    living_(true) {
    srand((unsigned)time(nullptr));
    insertOneNumberRandom();
    insertOneNumberRandom();
};

Scene::~Scene() {
    clear();
}

void Scene::setResultCallback(ResultCallback callback) {
    result_callback_ = callback;
}

void Scene::control(Action action) {
    int n = 0; // start idx, 0 or 3
    bool r = false; // is swap i j, effect process direction
    
    if (action == Action::kUp) {
        n = 0;
        r = true;
    } else if (action == Action::kDown) {
        n = 3;
        r = true;
    } else if (action == Action::kLeft) {
        n = 0;
        r = false;
    } else if (action == Action::kRight) {
        n = 3;
        r = false;
    } else {
        cout << "not support action:" << int(action) << endl;
        return;
    }
    
    bool is_change = false;
    // move number
    for (int i = 0; i < 4; ++i) {
        int s = n; // slow pointer
        int f = n; // fast pointer
        if (n == 3) { // move to right or move to down
            while (s >= 0 && f >= 0) {
                if (get(i, f, r) != 0) {
                    if (s != f) {
                        swap(i, s, i, f, r);
                        is_change = true;
                    }
                    s--;
                }
                f--;
            }
        } else { // move to left or move to up
            while (s < 4 && f < 4) {
                if (get(i, f, r) != 0) {
                    if (s != f) {
                        swap(i, s, i, f, r);
                        is_change = true;
                    }
                    s++;
                }
                f++;
            }
        }
        
        // merge number
        s = abs(n - s);
        int j0 = abs(n - 0);
        int j1 = abs(n - 1);
        int j2 = abs(n - 2);
        int j3 = abs(n - 3);
        if (s == 2) {
            if (get(i, j0, r) == get(i, j1, r)) { // 11
                set(i, j0, r, get(i, j0, r) * 2);
                set(i, j1, r, 0);
                score_ += get(i, j0, r);
                max_num_ = max(max_num_, get(i, j0, r));
                is_change = true;
            }
        } else if (s == 3) {
            if (get(i, j0, r) == get(i, j1, r)) { // 112
                set(i, j0, r, get(i, j0, r) * 2);
                set(i, j1, r, get(i, j2, r));
                set(i, j2, r, 0);
                score_ += get(i, j0, r);
                max_num_ = max(max_num_, get(i, j0, r));
                is_change = true;
            } else if (get(i, j1, r) == get(i, j2, r)) { // 211
                set(i, j1, r, get(i, j1, r) * 2);
                set(i, j2, r, 0);
                score_ += get(i, j1, r);
                max_num_ = max(max_num_, get(i, j1, r));
                is_change = true;
            }
        } else if (s == 4) {
            if (get(i, j0, r) == get(i, j1, r) &&
                get(i, j2, r) == get(i, j3, r)) { // 1122
                set(i, j0, r, get(i, j0, r) * 2);
                set(i, j1, r, get(i, j2, r) * 2);
                set(i, j2, r, 0);
                set(i, j3, r, 0);
                score_ += get(i, j0, r);
                max_num_ = max(max_num_, get(i, j0, r));
                score_ += get(i, j1, r);
                max_num_ = max(max_num_, get(i, j1, r));
                is_change = true;
            } else if (get(i, j0, r) == get(i, j1, r) &&
                       get(i, j2, r) != get(i, j3, r)) { // 1123
                set(i, j0, r, get(i, j0, r) * 2);
                set(i, j1, r, get(i, j2, r));
                set(i, j2, r, get(i, j3, r));
                set(i, j3, r, 0);
                score_ += get(i, j0, r);
                max_num_ = max(max_num_, get(i, j0, r));
                is_change = true;
            } else if (get(i, j0, r) != get(i, j1, r) &&
                       get(i, j2, r) == get(i, j3, r)) { // 2311
                set(i, j2, r, get(i, j2, r) * 2);
                set(i, j3, r, 0);
                score_ += get(i, j2, r);
                max_num_ = max(max_num_, get(i, j2, r));
                is_change = true;
            } else if (get(i, j1, r) == get(i, j2, r)) { // 2112, 2113
                set(i, j1, r, get(i, j1, r) * 2);
                set(i, j2, r, get(i, j3, r));
                set(i, j3, r, 0);
                score_ += get(i, j1, r);
                max_num_ = max(max_num_, get(i, j1, r));
                is_change = true;
            }
        }
    }
    
    if (!is_change) {
        living_ = isLiving();
        if (result_callback_ && !living_) result_callback_(Result::kDefeat);
        return;
    }
    
    Result result = insertOneNumberRandom();
    if (result != Result::kOk) living_ = isLiving();
    
    if (result_callback_) {
        if (max_num_ == 2048) {
            result_callback_(Result::kVictory);
            return;
        }
        if (!living_) {
            result_callback_(Result::kDefeat);
            return;
        }
    }
}

void Scene::clear() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            chess_board_[i][j] = 0;
        }
    }
    score_ = 0;
    max_num_ = 0;
    living_ = true;
}

void Scene::draw() {
    system("clear");
    int max_num_len = getNumLength(max_num_);
    vector<string> num_str_vec(16);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int len = getNumLength(chess_board_[i][j]);
            string num_str = to_string(chess_board_[i][j]);
            for (int i = 0; i < max_num_len - len; ++i)
                num_str.append(" ");
            num_str_vec[4 * i + j] = num_str;
        }
    }
    
    int line_len = max_num_len * 4 + 5; // has five '|' every line
    // draw tip
    cout << "quit: Q, reset: R, score: " << score_ << endl;
    // draw top frame
    for (int i = 0; i < line_len; ++i)
        cout << "-";
    cout << endl;
    // draw num and '|'
    for (int i = 0; i < num_str_vec.size(); ++i) {
        int row = i % 4;
        cout << "|" << num_str_vec[i];
        if (row == 3) cout << "|" << endl; // line over
    }
    // draw bottom frame
    for (int i = 0; i < line_len; ++i)
        cout << "-";
    cout << endl;
}

Result Scene::insertOneNumberRandom() {
    vector<int> empty_pos_vec;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (chess_board_[i][j] == 0) {
                empty_pos_vec.push_back(i);
                empty_pos_vec.push_back(j);
            }
        }
    }
    if (empty_pos_vec.empty()) return Result::kError;
    // generate random number
    int rand_num = (rand() % 10) < 1 ? 4 : 2;
    // generate random valid position
    int rand_idx = rand() % (empty_pos_vec.size() / 2);
    // insert random number
    chess_board_[empty_pos_vec[rand_idx * 2]][empty_pos_vec[rand_idx * 2 + 1]] = rand_num;
    return Result::kOk;
}

int Scene::getNumLength(int num) {
    if (num == 0) return 1;
    int len = 0;
    while (num > 0) {
        num /= 10;
        len++;
    }
    return len;
}

bool Scene::isLiving() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (get(i, j, false) == get(i, j + 1, false)) return true;
            if (get(i, j, true) == get(i, j + 1, true)) return true;
        }
    }
    return false;
}

void Scene::swap(int i1, int j1, int i2, int j2, bool is_swap) {
    int temp = get(i1, j1, is_swap);
    set(i1, j1, is_swap, get(i2, j2, is_swap));
    set(i2, j2, is_swap, temp);
}
    

} // namespace zh
