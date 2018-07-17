#ifndef _INPUTSTATE_H_
#define _INPUTSTATE_H_

struct InputState {    
    InputState(): upPressed(false),
    downPressed(false),
    leftPressed(false),
    rightPressed(false)
    {}

    // Is the arrow key currently being held down?
    bool upPressed;
    bool downPressed;
    bool leftPressed;
    bool rightPressed;
};

#endif
