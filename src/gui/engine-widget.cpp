#include "gui/engine-widget.hpp"
#include "game/tree.hpp"
#include "util/html-move-tree-builder.hpp"
#include "ui_engine-widget.h"
#include <QDebug>
#include <QThread>

EngineWidget::EngineWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::EngineWidget)
    , m_engine(new Engine("/usr/games/stockfish"))
{
    ui->setupUi(this);

    QObject::connect(ui->analyzeButton, &QPushButton::clicked, this, &EngineWidget::onAnalyzeClicked);
    QObject::connect(ui->stopButton, &QPushButton::clicked, this, &EngineWidget::onStopClicked);
    ui->engineOutput->setHtml("");

    QObject::connect(m_engine, &Engine::lineInfo, this, &EngineWidget::onLineInfo);
    // TODO: This should not be a constant
    m_lines.resize(2);
}

EngineWidget::~EngineWidget()
{
    delete ui;
}

QSize EngineWidget::sizeHint() const
{
    return QSize(10, 10);
}

void EngineWidget::setBoard(const Board& board)
{
    bool isAnalysing = m_engine->isAnalysing();

    if (isAnalysing)
        m_engine->stopAnalysis();
    m_lines.clear();
    m_currentBoard = board;
   // qDebug() << "setBoard to " << m_currentBoard.toFen();

    if (isAnalysing) {
        m_lines.clear();
        m_engine->startAnalysis(m_currentBoard);
    }
}

void EngineWidget::redraw()
{
    static const QString outputFmt = "<table><th></th><th></th>%1</table>";
    static const QString evalFmt = "<td><strong>(%1%2)</strong></td>";
    static const QString movesFmt = "<td>%1</td>";
    static const QString lineFmt = "<tr>%1%2</tr>";

    QString lines;

    for (const LineInfo& info : m_lines) {
        HtmlMoveTreeBuilder builder;
        Board board = m_currentBoard;
        QString score;

        if (info.mate()) {
            score = evalFmt.arg("#", QString::number(info.mate()));
        } else {
            int whiteCp = board.currentPlayer().isBlack() ? -info.score() : info.score();
            score = evalFmt.arg(whiteCp > 0 ? "+" : "", QString::number(whiteCp / 100.0, 'g', 2));
        }

        if (board.currentPlayer().isBlack())
            builder.addMoveNumber(QString::number(board.fullMoveCount()) + "... ");

        for (int i = 0; i < info.moveList().size(); i++) {
            const QString& moveString = info.moveList()[i];
            QString mate;
            Move move = board.longAlgebraicNotationToMove(moveString);

            if (board.currentPlayer().isWhite())
                builder.addMoveNumber(QString::number(board.fullMoveCount()) + ". ");

            QString algebraicMove = board.algebraicNotationString(move);
            if (i == info.moveList().size() - 1 && info.mate()) {
                algebraicMove.chop(1);
                algebraicMove.append('#');
            }
            builder.addMove(algebraicMove);

            Q_ASSERT(board.makeMove(move) && "Engine passed an invalid move.");
        }
        lines.append(outputFmt.arg(lineFmt.arg(score, movesFmt.arg(builder.htmlWithStyle()))));
        lines.append("<br/>");
    }

    ui->engineOutput->setHtml(lines);
}

void EngineWidget::onLineInfo(LineInfo info)
{
    m_lines.resize(info.id());
    m_lines[info.id()-1] = info;
    redraw();
}

void EngineWidget::onAnalyzeClicked()
{
    m_lines.clear();
    m_engine->startAnalysis(m_currentBoard);
}

void EngineWidget::onStopClicked()
{
    m_engine->stopAnalysis();
}
