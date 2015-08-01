#ifndef BOARDWIDGET_HPP
#define BOARDWIDGET_HPP

#include "game/game-model.hpp"
#include "settings.hpp"
#include <QWidget>

class SettingsManager;
class BoardWidgetState;
class BoardWidgetStateDragging;
class BoardWidgetStateNormal;

class BoardWidget : public QWidget
{
    friend class BoardWidgetStateDragging;
    friend class BoardWidgetStateNormal;

    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = 0);

    void setModel(GameModel*);
    void setManager(Settings *);
    /* Reverses board view */
    void flip();
    /* Redraws entire board */
    void redraw();
    /* Emits move signal */
    void emitMove(Move move);
    /* Handles settings change */
    void update();
    /* Checks if given (x, y) is in some square of the board, if so,
       it initializes rank, file variables */
    bool isFieldAt(double x, double y, int* file, int* rank);
protected:
    virtual void resizeEvent(QResizeEvent*) override;
    virtual void paintEvent(QPaintEvent *) override;
    virtual void mousePressEvent(QMouseEvent *) override;
    virtual void mouseReleaseEvent(QMouseEvent *) override;
    virtual void mouseMoveEvent(QMouseEvent* ) override;
signals:
    void moveMade(Move);
public slots:
private:
    /* Draws board contents */
    void draw(QPainter& ctx);
    /* Draws piece at x,y in the canvas using SVG renderer */
    void drawPiece(QPainter& ctx, QRectF Dest, Piece Piece, Player Owner);
    /* Draws piece at given (rank, file) */
    void drawPiece(QPainter& ctx, int rank, int file);
    /* Draws floating / dragging piece if any */
    void drawDraggedPiece(QPainter& ctx);
    /* Draws single square at (rank, file) of the chessboard */
    void drawField(QPainter& ctx, int rank, int file);
    /* Draws selection border */
    void drawSelection(QPainter& ctx);
    /* Draws border with files and ranks */
    void drawBorder(QPainter& ctx);
    /* Helper function to determine given rank offset */
    int getRankOffset(int rank) const;
    /* Helper function to determine given file offset */
    int getFileOffset(int file) const;
    /* Sets state if passed State is not null */
    void setState(BoardWidgetState* State);
    /* Returns absolute coordinate depending on board rotation. */
    int absolute(int coord) const;
private:
    BoardWidgetState* mState;
    Settings* mSettings;
    GameModel* mModel;
    /* Geometry properties */
    bool mFlipped;
    int mWidth;  /* width of the drawing area */
    int mHeight; /* height of the drawing area */
    int mFieldSize;
    /* Beginning of the chessboard squares */
    int mFirstFieldX;
    int mFirstFieldY;

    Coord2D<int> mDraggedField;
    Coord2D<int> mDragOffset;
    Coord2D<int> mDragStart;
    Coord2D<int> mSelectedField;
};

#endif // BOARDWIDGET_HPP
