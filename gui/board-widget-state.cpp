#include "gui/board-widget-state.hpp"
#include "gui/board-widget.hpp"
#include "game/game-model.hpp"
#include <iostream>

BoardWidgetState*
BoardWidgetStateDragging::onMouseRelease(BoardWidget* Board, QMouseEvent* Event) {
    Coord2D<int> Field;

    if (Event->button() != Qt::LeftButton)
        return nullptr;

    if (Board->isFieldAt(Event->x(), Event->y(), &Field.x, &Field.y)) {
        Move move(Board->mDraggedField, Field);

        if (Board->mModel->isLegal(move, NULL, NULL))
            Board->mSelectedField = Field;

        Board->emitMove(move);
    }

    Board->mDraggedField = Coord2D<int>(-1, -1);
    Board->redraw();

    return new BoardWidgetStateNormal();
}

BoardWidgetState*
BoardWidgetStateDragging::onMouseMove(BoardWidget* Board, QMouseEvent* Event) {
    Board->mDragOffset.x = Event->x() - Board->mDragStart.x;
    Board->mDragOffset.y = Event->y() - Board->mDragStart.y;
    Board->redraw();

    return nullptr;
}

BoardWidgetState*
BoardWidgetStateNormal::onMousePress(BoardWidget* Board, QMouseEvent* Event) {
    Coord2D<int> Field;

    if (Board->isFieldAt(Event->x(), Event->y(), &Field.x, &Field.y)) {
        if (Board->mSelectedField == Coord2D<int>(-1,-1) || Board->mSelectedField != Field) {
            Move move(Board->mSelectedField, Field);

            if (Board->mModel->isLegal(move, NULL, NULL))
                Board->emitMove(move);

            Board->mSelectedField = Field;
        } else
            Board->mSelectedField = Coord2D<int>(-1,-1);

        Board->mDraggedField = Field;
        Board->mDragOffset = Coord2D<int>(0, 0);
        Board->mDragStart = Coord2D<int>(Event->x(), Event->y());
        Board->redraw();
        return new BoardWidgetStateDragging();
    }
    return nullptr;
}
