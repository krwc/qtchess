#include "gui/board/board-widget-state.hpp"
#include "gui/board/board-widget.hpp"
#include "game/board.hpp"


BoardWidgetState*
BoardWidgetStateDragging::onMouseRelease(BoardWidget* Board, QMouseEvent* Event) {
    Coord2D<int> Field;

    if (Event->button() != Qt::LeftButton)
        return nullptr;

    if (Board->isFieldAt(Event->x(), Event->y(), &Field.x, &Field.y)) {
        Move move(Board->m_draggedField, Field);

        if (Board->m_board.isLegal(move, NULL, NULL))
            Board->m_selectedField = Field;

        Board->emitMove(move);
    }

    Board->m_draggedField = Coord2D<int>(-1, -1);
    Board->redraw();

    return new BoardWidgetStateNormal();
}

BoardWidgetState*
BoardWidgetStateDragging::onMouseMove(BoardWidget* Board, QMouseEvent* Event) {
    Board->m_dragOffset.x = Event->x() - Board->m_dragStart.x;
    Board->m_dragOffset.y = Event->y() - Board->m_dragStart.y;
    Board->redraw();

    return nullptr;
}

BoardWidgetState*
BoardWidgetStateNormal::onMousePress(BoardWidget* Board, QMouseEvent* Event) {
    Coord2D<int> Field;

    if (Board->isFieldAt(Event->x(), Event->y(), &Field.x, &Field.y)) {
        if (Board->m_selectedField == Coord2D<int>(-1, -1))
            Board->m_selectedField = Field;
        else if (Board->m_selectedField != Field) {
            Move move(Board->m_selectedField, Field);

            if (Board->m_board.isLegal(move, NULL, NULL))
                Board->emitMove(move);

            Board->m_selectedField = Field;
        } else
            Board->m_selectedField = Coord2D<int>(-1,-1);

        Board->m_draggedField = Field;
        Board->m_dragOffset = Coord2D<int>(0, 0);
        Board->m_dragStart = Coord2D<int>(Event->x(), Event->y());
        Board->redraw();
        return new BoardWidgetStateDragging();
    }
    return nullptr;
}
