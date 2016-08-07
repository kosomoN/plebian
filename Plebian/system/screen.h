#ifndef ISCREEN_H_
#define ISCREEN_H_

class IScreen {
public:
    virtual ~IScreen() = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void Show() = 0;
    virtual void Hide() = 0;
    virtual void Update(double delta) = 0;
};

#endif // ISCREEN_H_
