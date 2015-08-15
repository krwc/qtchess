#include "gui/board/board-widget.hpp"
#include "gui/board/board-widget-state.hpp"
#include "game/board.hpp"
#include "piece-set.hpp"
#include <algorithm>
#include <QPainter>

static const int MinSize = 256;

BoardWidget::BoardWidget(QWidget* parent, BoardSettings& settings)
    : QWidget(parent)
    , m_settings(settings)
    , m_state(new BoardWidgetStateNormal())
    , m_pieceSet(nullptr)
    , m_flipped(false)
    , m_draggedField(-1, -1)
    , m_selectedField(-1, -1)
{
    setMinimumSize(MinSize, MinSize);
    setMouseTracking(true);

    QObject::connect(&settings, &AbstractSettings::changed, this, &BoardWidget::update);
}

void BoardWidget::setBoard(Board board)
{
    m_board = board;
    redraw();
}

void BoardWidget::flip() {
    m_flipped ^= true;
    redraw();
}

void BoardWidget::redraw() {
    repaint();
}

void BoardWidget::emitMove(Move move) {
    emit moveMade(move);
}

void BoardWidget::update() {
    m_width = width();
    m_height = height();

    int borderSize = 0;

    if (m_settings.get("boolBorder").toBool())
        borderSize = m_settings.get("intBorderSize").toInt();

    int margin = m_settings.get("intMarginSize").toInt();
    int totalMargin = borderSize + margin;
    m_fieldSize = (std::min(m_width, m_height) - 2. * totalMargin) / 8.0;

    double centeringX = 0.5 * (m_width - 2 * totalMargin - 8 * m_fieldSize);
    double centeringY = 0.5 * (m_height - 2 * totalMargin - 8 * m_fieldSize);
    m_firstFieldX = totalMargin + centeringX;
    m_firstFieldY = totalMargin + centeringY;
    redraw();
}

bool BoardWidget::isFieldAt(double x, double y, int* file, int* rank) {
    const int Size = 8 * m_fieldSize;
    x -= m_firstFieldX;
    y -= m_firstFieldY;
    // Clicked inside field
    if (x >= 0 && x <= Size && y >= 0 && y <= Size) {
        *file = absolute(int(x / m_fieldSize));
        *rank = absolute(int(y / m_fieldSize));
        return true;
    }
    return false;
}

void BoardWidget::resizeEvent(QResizeEvent *) {
    update();
}

void BoardWidget::paintEvent(QPaintEvent *) {
    QPainter Painter(this);
    draw(Painter);
}

void BoardWidget::mousePressEvent(QMouseEvent* Event) {
    setState(m_state->onMousePress(this, Event));
}

void BoardWidget::mouseReleaseEvent(QMouseEvent* Event) {
    setState(m_state->onMouseRelease(this, Event));
}

void BoardWidget::mouseMoveEvent(QMouseEvent* Event) {
    setState(m_state->onMouseMove(this, Event));
}

void BoardWidget::ensureValidPieceSet() {
    QString currentName = m_settings.get("stringPieceStyle").toString();

    if (m_pieceSet == nullptr || m_pieceSet->styleName() != currentName) {
        delete m_pieceSet;
        m_pieceSet = new PieceSet(currentName);
    }
}

void BoardWidget::draw(QPainter& context) {
    drawBorder(context);
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            drawField(context, rank, file);
            drawPiece(context, rank, file);
        }
    }
    drawSelection(context);
    drawDraggedPiece(context);
}

void BoardWidget::drawBorder(QPainter& context) {
    static const char* Files[] = {
        "A", "B", "C", "D", "E", "F", "G", "H"
    };
    static const char* Ranks[] = {
        "1", "2", "3", "4", "5", "6", "7", "8"
    };

    if (!m_settings.get("boolBorder").toBool())
        return;

    int size = m_settings.get("intBorderSize").toInt();

    QPen Pen;

    Pen.setColor(m_settings.get("colorBorder").value<QColor>());
    Pen.setWidth(size);
    Pen.setJoinStyle(Qt::MiterJoin);
    context.setPen(Pen);
    QPointF TopLeft(getFileOffset(0) - 0.5*size, getRankOffset(0) - 0.5*size);
    QPointF TopRight(getFileOffset(8) + 0.5*size, getRankOffset(0) - 0.5*size);
    QPointF BottomRight(getFileOffset(8) + 0.5*size, getRankOffset(8) + 0.5*size);
    QPointF BottomLeft(getFileOffset(0) - 0.5*size, getRankOffset(8) + 0.5*size);
    // Fill border with color
    context.drawLine(TopLeft, TopRight);
    context.drawLine(TopRight, BottomRight);
    context.drawLine(BottomRight, BottomLeft);
    context.drawLine(BottomLeft, TopLeft);

    QBrush Brush = QBrush(Qt::white);
    QFont Font("Monospace", 10, QFont::Bold);

    context.setBrush(Brush);
    context.setPen(m_settings.get("colorBorderText").value<QColor>());
    context.setFont(Font);

    for (int i = 0; i < 8; i++) {
        int j = absolute(i);
        QRectF FileRect(getFileOffset(i), getRankOffset(8), m_fieldSize, size);
        QRectF RankRect(getFileOffset(0) - size, getRankOffset(i), size, m_fieldSize);
        context.drawText(FileRect, Qt::AlignCenter, Files[j]);
        context.drawText(RankRect, Qt::AlignCenter, Ranks[7-j]);
    }
}

void BoardWidget::drawDraggedPiece(QPainter& context) {
    if (m_draggedField == Coord2D<int>(-1, -1))
        return;

    int FieldX = absolute(m_draggedField.x);
    int FieldY = absolute(m_draggedField.y);
    QRect Dest(FieldX * m_fieldSize + m_firstFieldX + m_dragOffset.x,
               FieldY * m_fieldSize + m_firstFieldY + m_dragOffset.y,
               m_fieldSize, m_fieldSize);

    Piece piece = m_board.pieceAt(m_draggedField);

    if (piece.isNone())
        return;

    drawPiece(context, Dest, piece);
}

void BoardWidget::drawField(QPainter& context, int rank, int file) {
    QBrush Brush;

    if ((rank + file) % 2 == 0)
        Brush = QBrush(m_settings.get("colorSquareLight").value<QColor>());
    else
        Brush = QBrush(m_settings.get("colorSquareDark").value<QColor>());

    context.fillRect(getFileOffset(file), getRankOffset(rank),
                     m_fieldSize, m_fieldSize, Brush);
}

void BoardWidget::drawPiece(QPainter& context, QRectF dest, Piece piece) {
    ensureValidPieceSet();

    const QRectF source(0, 0, m_fieldSize, m_fieldSize);
    context.drawPixmap(dest, m_pieceSet->getPiecePixmap(piece, m_fieldSize), source);
}

void BoardWidget::drawPiece(QPainter& context, int rank, int file) {
    int x = absolute(file);
    int y = absolute(rank);
    Piece Piece = m_board.pieceAt(x, y);

    if (Piece.isNone() || m_draggedField == Coord2D<int>(x, y))
        return;

    QRectF Dest(getFileOffset(file), getRankOffset(rank), m_fieldSize, m_fieldSize);
    drawPiece(context, Dest, Piece);
}

void BoardWidget::drawSelection(QPainter& context) {
    if (m_selectedField == Coord2D<int>(-1, -1))
        return;
    int file = absolute(m_selectedField.x);
    int rank = absolute(m_selectedField.y);
    int size = 2*int(double(std::min(m_width, m_height)) / MinSize);
    QBrush Brush = QBrush(QColor(0, 0, 0, 0));
    QPen Pen;
    Pen.setColor(m_settings.get("colorPicking").value<QColor>());
    Pen.setWidth(size);
    Pen.setJoinStyle(Qt::MiterJoin);

    context.setPen(Pen);
    context.setBrush(Brush);
    context.drawRect(getFileOffset(file), getRankOffset(rank),
                     m_fieldSize, m_fieldSize);
}

int BoardWidget::getRankOffset(int rank) const {
    return m_firstFieldY + rank * m_fieldSize;
}

int BoardWidget::getFileOffset(int file) const {
    return m_firstFieldX + file * m_fieldSize;
}

void BoardWidget::setState(BoardWidgetState *State) {
    if (State == nullptr)
        return;
    delete m_state;
    m_state = State;
}

int BoardWidget::absolute(int coord) const {
    if (m_flipped)
        return 7 - coord;
    return coord;
}
