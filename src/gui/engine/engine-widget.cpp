#include "gui/engine/engine-widget.hpp"
#include "util/html-move-tree-builder.hpp"
#include "settings/settings-factory.hpp"
#include "game/tree.hpp"
#include "ui_engine-widget.h"
#include <QDebug>
#include <QThread>

EngineWidget::EngineWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::EngineWidget)
{
    ui->setupUi(this);

    QObject::connect(ui->analyzeButton, &QPushButton::clicked, this, &EngineWidget::onAnalyzeClicked);
    QObject::connect(ui->stopButton, &QPushButton::clicked, this, &EngineWidget::onStopClicked);
    ui->engineOutput->setHtml("");
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
    if (!m_engine) {
        m_currentBoard = board;
        return;
    }

    bool isAnalysing = m_engine->isAnalysing();

    if (isAnalysing)
        m_engine->stopAnalysis();
    m_variants.clear();
    m_currentBoard = board;

    if (isAnalysing) {
        m_variants.clear();
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

    for (const VariantInfo& info : m_variants) {
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

void EngineWidget::onVariantParsed(VariantInfo info)
{
    m_variants.resize(info.id());
    m_variants[info.id()-1] = info;
    redraw();
}

void EngineWidget::onAnalyzeClicked()
{
    if (!m_engine) {
        if (EngineSettings::engines().size()) {
            QString name = EngineSettings::engines()[0];
            m_engine = std::unique_ptr<Engine>(new Engine(SettingsFactory::engineSettings(name)));
            QObject::connect(m_engine.get(), &Engine::variantParsed, this, &EngineWidget::onVariantParsed);
        } else
            return;
    }
    m_variants.clear();
    m_engine->startAnalysis(m_currentBoard);
}

void EngineWidget::onStopClicked()
{
    if (!m_engine)
        return;
    m_engine->stopAnalysis();
}
