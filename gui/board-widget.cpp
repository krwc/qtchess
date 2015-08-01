#include "gui/board-widget.hpp"
#include "gui/board-widget-state.hpp"
#include "settings-manager.hpp"
#include "game/game-model.hpp"
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
    , mManager(nullptr)
    , mModel(nullptr)
    , mFlipped(false)
    , mDraggedField(-1, -1)
    , mSelectedField(-1, -1)
{
    setMinimumSize(MinSize, MinSize);
    setMouseTracking(true);
}

void BoardWidget::setModel(GameModel* Model) {
    mModel = Model;
    redraw();
}

void BoardWidget::setManager(SettingsManager* Manager) {
    mManager = Manager;
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
    mWidth  = width();
    mHeight = height();
    int Margin = 0;
    int BorderSize = 0;
    int TotalMargin = 0;

    if (mManager->getShouldDrawCoords())
        BorderSize = mManager->getBorderSize();

    Margin = mManager->getMarginSize();
    TotalMargin = BorderSize + Margin;

    mFieldSize = (std::min(mWidth, mHeight) - 2. * TotalMargin) / 8.0;
    double CenteringX = 0.5 * (mWidth - 2 * TotalMargin - 8 * mFieldSize);
    double CenteringY = 0.5 * (mHeight - 2 * TotalMargin - 8 * mFieldSize);
    mFirstFieldX = TotalMargin + CenteringX;
    mFirstFieldY = TotalMargin + CenteringY;
}

bool BoardWidget::isFieldAt(double x, double y, int* file, int* rank) {
    const int Size = 8 * mFieldSize;
    x -= mFirstFieldX;
    y -= mFirstFieldY;
    // Clicked inside field
    if (x >= 0 && x <= Size && y >= 0 && y <= Size) {
        if (mFlipped) {
            *file = 7 - int(x / mFieldSize);
            *rank = 7 - int(y / mFieldSize);
        } else {
            *file = int(x / mFieldSize);
            *rank = int(y / mFieldSize);
        }
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

void BoardWidget::draw(QPainter& ctx) {
    drawBorder(ctx);
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            drawField(ctx, rank, file);
            drawPiece(ctx, rank, file);
        }
    }
    drawSelection(ctx);
    drawDraggedPiece(ctx);
}

void BoardWidget::drawBorder(QPainter& ctx) {
    static const char* Files[] = {
        "A", "B", "C", "D", "E", "F", "G", "H"
    };
    static const char* Ranks[] = {
        "1", "2", "3", "4", "5", "6", "7", "8"
    };

    if (!mManager->getShouldDrawCoords())
        return;

    int size = mManager->getBorderSize();
    QPen Pen;
    // FIXME: Color should be controllable by the user
    Pen.setColor(QColor(48, 48, 48));
    Pen.setWidth(size);
    Pen.setJoinStyle(Qt::MiterJoin);
    ctx.setPen(Pen);
    QPointF TopLeft(getFileOffset(0) - 0.5*size, getRankOffset(0) - 0.5*size);
    QPointF TopRight(getFileOffset(8) + 0.5*size, getRankOffset(0) - 0.5*size);
    QPointF BottomRight(getFileOffset(8) + 0.5*size, getRankOffset(8) + 0.5*size);
    QPointF BottomLeft(getFileOffset(0) - 0.5*size, getRankOffset(8) + 0.5*size);
    // Fill border with color
    ctx.drawLine(TopLeft, TopRight);
    ctx.drawLine(TopRight, BottomRight);
    ctx.drawLine(BottomRight, BottomLeft);
    ctx.drawLine(BottomLeft, TopLeft);

    QBrush Brush = QBrush(Qt::GlobalColor::white);
    QFont Font("Monospace", 10, QFont::Bold);

    ctx.setBrush(Brush);
    // FIXME: Should also be configurable
    ctx.setPen(Qt::GlobalColor::white);
    ctx.setFont(Font);

    for (int i = 0; i < 8; i++) {
        int j = mFlipped ? 7 - i : i;
        QRectF FileRect(getFileOffset(i), getRankOffset(8), mFieldSize, size);
        QRectF RankRect(getFileOffset(0) - size, getRankOffset(i), size, mFieldSize);
        ctx.drawText(FileRect, Qt::AlignCenter, Files[j]);
        ctx.drawText(RankRect, Qt::AlignCenter, Ranks[7-j]);
    }
}

void BoardWidget::drawDraggedPiece(QPainter& ctx) {
    if (mDraggedField == Coord2D<int>(-1, -1))
        return;

    int FieldX = mFlipped ? 7 - mDraggedField.x : mDraggedField.x;
    int FieldY = mFlipped ? 7 - mDraggedField.y : mDraggedField.y;
    QRect Dest(FieldX * mFieldSize + mFirstFieldX + mDragOffset.x,
               FieldY * mFieldSize + mFirstFieldY + mDragOffset.y,
               mFieldSize, mFieldSize);

    Piece Piece  = mModel->piece(mDraggedField);
    Player Owner = mModel->owner(mDraggedField);

    if (Piece == PIECE_NONE)
        return;

    drawPiece(ctx, Dest, Piece, Owner);
}

void BoardWidget::drawField(QPainter& ctx, int rank, int file) {
    QBrush Brush;

    if ((rank + file) % 2 == 0)
        Brush = QBrush(mManager->getLSColor());
    else
        Brush = QBrush(mManager->getDSColor());

    ctx.fillRect(getFileOffset(file), getRankOffset(rank),
                 mFieldSize, mFieldSize, Brush);
}

void BoardWidget::drawPiece(QPainter& ctx, QRectF Dst, Piece Piece, Player Owner) {
    QRectF Src(0,0,mFieldSize,mFieldSize);
    ctx.drawPixmap(Dst, mManager->getPieceSet().getPiecePixmap(Piece, Owner, mFieldSize), Src);
}

void BoardWidget::drawPiece(QPainter& ctx, int rank, int file) {
    int x = mFlipped ? 7 - file : file;
    int y = mFlipped ? 7 - rank : rank;
    Piece Piece = mModel->piece(x, y);
    Player Owner = mModel->owner(x, y);

    if (Piece == PIECE_NONE || mDraggedField == Coord2D<int>(x, y))
        return;

    QRectF Dest(getFileOffset(file), getRankOffset(rank), mFieldSize, mFieldSize);
    drawPiece(ctx, Dest, Piece, Owner);
}

void BoardWidget::drawSelection(QPainter& ctx) {
    if (mSelectedField == Coord2D<int>(-1, -1))
        return;
    int file = mFlipped ? 7 - mSelectedField.x : mSelectedField.x;
    int rank = mFlipped ? 7 - mSelectedField.y : mSelectedField.y;
    int size = 2*int(double(std::min(mWidth,mHeight)) / MinSize);
    QBrush Brush = QBrush(QColor(0, 0, 0, 0));
    QPen Pen;
    Pen.setColor(mManager->getSelectionColor());
    Pen.setWidth(size);
    Pen.setJoinStyle(Qt::MiterJoin);

    ctx.setPen(Pen);
    ctx.setBrush(Brush);
    ctx.drawRect(getFileOffset(file), getRankOffset(rank),
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
