#ifndef IINPUTLISTENER_H_
#define IINPUTLISTENER_H_

class IInputListener {
public:
    virtual bool KeyUp(int key) { return false; }
    virtual bool KeyDown(int key) { return false; }
    virtual bool KeyRepeat(int key) { return false; }
    virtual bool MouseMoved(double xpos, double ypos) { return false; }
    virtual bool MouseUp(int button) { return false; }
    virtual bool MouseDown(int button) { return false; }
    virtual bool MouseEnter() { return false; }
    virtual bool MouseLeave() { return false; }
};

#endif //IINPUTLISTENER_H_
