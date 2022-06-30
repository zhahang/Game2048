//
//  scene.hpp
//  2048
//
//  Created by zhahang on 2022/6/29.
//

#ifndef _ZH_TEST_GAME_2048_SCENE_H
#define _ZH_TEST_GAME_2048_SCENE_H

#include <functional>


namespace zh {

enum class Action {
    kUp = 0,
    kDown,
    kLeft,
    kRight,
    kQuit,
    kReset
};

enum class Result {
    kError = -1,
    kOk,
    kVictory,
    kDefeat,
};

using ResultCallback = std::function<void(Result result)>;

class Scene {
public:
    Scene();
    ~Scene();
    
    void setResultCallback(ResultCallback callback);
    
    void control(Action action);
    
    void clear();
    
    virtual void draw();

private:
    Result insertOneNumberRandom();
    
    static int getNumLength(int num);
    
    bool isLiving();
    
    void swap(int i1, int j1, int i2, int j2, bool is_swap);
    
    inline int get(int i, int j, bool is_swap) {
        return is_swap ? chess_board_[j][i] : chess_board_[i][j];
    }
    
    inline void set(int i, int j, bool is_swap, int value) {
        if (is_swap) chess_board_[j][i] = value;
        else chess_board_[i][j] = value;
    }
    
private:
    int chess_board_[4][4] = {0};
    int score_;
    int max_num_;
    bool living_;
    ResultCallback result_callback_;
};

} // namespace zh

#endif // _ZH_TEST_GAME_2048_SCENE_H
