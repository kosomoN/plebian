#ifndef IINPUT_LISTENER_H_
#define IINPUT_LISTENER_H_

class IInputListener {
public:
    virtual bool KeyUp(int key) = 0;
    virtual bool KeyDown(int key) = 0;
    virtual bool KeyRepeat(int key) = 0;
};

#endif //IINPUT_LISTENER_H_
