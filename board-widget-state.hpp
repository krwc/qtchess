#ifndef BOARDWIDGETSTATE_HPP
#define BOARDWIDGETSTATE_HPP
#include <QMouseEvent>
#include "board-widget.hpp"

class BoardWidgetState
{
public:
    virtual ~BoardWidgetState() = default;
    /* Every virtual method returns nullptr by default indicating that
       no state change occurs. */
    virtual BoardWidgetState* onMousePress(BoardWidget*, QMouseEvent*) {
        return nullptr;
    }
    virtual BoardWidgetState* onMouseMove(BoardWidget*, QMouseEvent*) {
        return nullptr;
    }
    virtual BoardWidgetState* onMouseRelease(BoardWidget*, QMouseEvent*) {
        return nullptr;
    }
};

/* Concrete subclasses defining behaviour */
class BoardWidgetStateDragging : public BoardWidgetState {
public:
    virtual BoardWidgetState* onMouseRelease(BoardWidget*, QMouseEvent*) override;
    virtual BoardWidgetState* onMouseMove(BoardWidget*, QMouseEvent*) override;
};

class BoardWidgetStateNormal : public BoardWidgetState {
public:
    virtual BoardWidgetState* onMousePress(BoardWidget*, QMouseEvent*) override;
};

#endif // BOARDWIDGETSTATE_HPP
