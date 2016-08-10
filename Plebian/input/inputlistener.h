#ifndef IINPUTLISTENER_H_
#define IINPUTLISTENER_H_

class IInputListener {
public:
    virtual bool KeyUp(int key) = 0;
    virtual bool KeyDown(int key) = 0;
    virtual bool KeyRepeat(int key) = 0;
    virtual bool MouseMoved(double xpos, double ypos) = 0;
    virtual bool MouseUp(int button) = 0;
    virtual bool MouseDown(int button) = 0;
    virtual bool MouseEnter() = 0;
    virtual bool MouseLeave() = 0;
};

#endif //IINPUTLISTENER_H_
