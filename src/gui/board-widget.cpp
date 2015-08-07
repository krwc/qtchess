#include "gui/board-widget.hpp"
#include "gui/board-widget-state.hpp"
#include "settings.hpp"
#include "game/board.hpp"
#include "piece-set.hpp"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <QPainter>

static const int MinSize = 256;

BoardWidget::BoardWidget(QWidget *parent)
    : QWidget(parent)
    , mState(new BoardWidgetStateNormal())
    , mModel(nullptr)
    , mPieceSet(nullptr)
    , mFlipped(false)
    , mDraggedField(-1, -1)
    , mSelectedField(-1, -1)
{
    setMinimumSize(MinSize, MinSize);
    setMouseTracking(true);

    QObject::connect(&Settings::instance(), &Settings::changed, this, &BoardWidget::update);
}

void BoardWidget::setModel(const Board* model) {
    mModel = model;
    redraw();
}

void BoardWidget::flip() {
    mFlipped ^= true;
    redraw();
}

void BoardWidget::redraw() {
    repaint();
}

void BoardWidget::emitMove(Move move) {
    emit moveMade(move);
}

void BoardWidget::update() {
    mWidth = width();
    mHeight = height();

    int borderSize = 0;

    if (Settings::instance().get(Settings::ShouldDrawCoords).toBool())
        borderSize = Settings::instance().get(Settings::BorderSize).toInt();

    int margin = Settings::instance().get(Settings::MarginSize).toInt();
    int totalMargin = borderSize + margin;
    mFieldSize = (std::min(mWidth, mHeight) - 2. * totalMargin) / 8.0;

    double centeringX = 0.5 * (mWidth - 2 * totalMargin - 8 * mFieldSize);
    double centeringY = 0.5 * (mHeight - 2 * totalMargin - 8 * mFieldSize);
    mFirstFieldX = totalMargin + centeringX;
    mFirstFieldY = totalMargin + centeringY;
    redraw();
}

bool BoardWidget::isFieldAt(double x, double y, int* file, int* rank) {
    const int Size = 8 * mFieldSize;
    x -= mFirstFieldX;
    y -= mFirstFieldY;
    // Clicked inside field
    if (x >= 0 && x <= Size && y >= 0 && y <= Size) {
        *file = absolute(int(x / mFieldSize));
        *rank = absolute(int(y / mFieldSize));
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
    setState(mState->onMousePress(this, Event));
}

void BoardWidget::mouseReleaseEvent(QMouseEvent* Event) {
    setState(mState->onMouseRelease(this, Event));
}

void BoardWidget::mouseMoveEvent(QMouseEvent* Event) {
    setState(mState->onMouseMove(this, Event));
}

void BoardWidget::ensureValidPieceSet() {
    QString currentName = Settings::instance().get(Settings::PieceStyleName).toString();

    if (mPieceSet == nullptr || mPieceSet->styleName() != currentName) {
        delete mPieceSet;
        mPieceSet = new PieceSet(currentName);
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

    if (!Settings::instance().get(Settings::ShouldDrawCoords).toBool())
        return;

    int size = Settings::instance().get(Settings::BorderSize).toInt();

    QPen Pen;

    Pen.setColor(Settings::instance().get(Settings::CoordsBorderColor).value<QColor>());
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
    context.setPen(Settings::instance().get(Settings::CoordsTextColor).value<QColor>());
    context.setFont(Font);

    for (int i = 0; i < 8; i++) {
        int j = absolute(i);
        QRectF FileRect(getFileOffset(i), getRankOffset(8), mFieldSize, size);
        QRectF RankRect(getFileOffset(0) - size, getRankOffset(i), size, mFieldSize);
        context.drawText(FileRect, Qt::AlignCenter, Files[j]);
        context.drawText(RankRect, Qt::AlignCenter, Ranks[7-j]);
    }
}

void BoardWidget::drawDraggedPiece(QPainter& context) {
    if (mDraggedField == Coord2D<int>(-1, -1))
        return;

    int FieldX = absolute(mDraggedField.x);
    int FieldY = absolute(mDraggedField.y);
    QRect Dest(FieldX * mFieldSize + mFirstFieldX + mDragOffset.x,
               FieldY * mFieldSize + mFirstFieldY + mDragOffset.y,
               mFieldSize, mFieldSize);

    Piece piece = mModel->pieceAt(mDraggedField);

    if (piece.isNone())
        return;

    drawPiece(context, Dest, piece);
}

void BoardWidget::drawField(QPainter& context, int rank, int file) {
    QBrush Brush;

    if ((rank + file) % 2 == 0)
        Brush = QBrush(Settings::instance().get(Settings::LightSquareColor).value<QColor>());
    else
        Brush = QBrush(Settings::instance().get(Settings::DarkSquareColor).value<QColor>());

    context.fillRect(getFileOffset(file), getRankOffset(rank),
                 mFieldSize, mFieldSize, Brush);
}

void BoardWidget::drawPiece(QPainter& context, QRectF dest, Piece piece) {
    ensureValidPieceSet();

    const QRectF source(0, 0, mFieldSize, mFieldSize);
    context.drawPixmap(dest, mPieceSet->getPiecePixmap(piece, mFieldSize), source);
}

void BoardWidget::drawPiece(QPainter& context, int rank, int file) {
    int x = absolute(file);
    int y = absolute(rank);
    Piece Piece = mModel->pieceAt(x, y);

    if (Piece.isNone() || mDraggedField == Coord2D<int>(x, y))
        return;

    QRectF Dest(getFileOffset(file), getRankOffset(rank), mFieldSize, mFieldSize);
    drawPiece(context, Dest, Piece);
}

void BoardWidget::drawSelection(QPainter& context) {
    if (mSelectedField == Coord2D<int>(-1, -1))
        return;
    int file = absolute(mSelectedField.x);
    int rank = absolute(mSelectedField.y);
    int size = 2*int(double(std::min(mWidth,mHeight)) / MinSize);
    QBrush Brush = QBrush(QColor(0, 0, 0, 0));
    QPen Pen;
    Pen.setColor(Settings::instance().get(Settings::SelectionColor).value<QColor>());
    Pen.setWidth(size);
    Pen.setJoinStyle(Qt::MiterJoin);

    context.setPen(Pen);
    context.setBrush(Brush);
    context.drawRect(getFileOffset(file), getRankOffset(rank),
                 mFieldSize, mFieldSize);
}

int BoardWidget::getRankOffset(int rank) const {
    return mFirstFieldY + rank * mFieldSize;
}

int BoardWidget::getFileOffset(int file) const {
    return mFirstFieldX + file * mFieldSize;
}

void BoardWidget::setState(BoardWidgetState *State) {
    if (State == nullptr)
        return;
    delete mState;
    mState = State;
}

int BoardWidget::absolute(int coord) const {
    if (mFlipped)
        return 7 - coord;
    return coord;
}
