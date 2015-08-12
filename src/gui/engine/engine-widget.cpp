#include "gui/engine/engine-widget.hpp"
#include "gui/engine/engine-selection-dialog.hpp"
#include "util/html-move-tree-builder.hpp"
#include "settings/settings-factory.hpp"
#include "game/tree.hpp"
#include "ui_engine-widget.h"
#include <QDebug>
#include <QThread>

EngineWidget::EngineWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::EngineWidget)
    , m_engine(nullptr)
{
    ui->setupUi(this);

    QObject::connect(ui->analyzeButton, &QPushButton::clicked, this, &EngineWidget::onAnalyzeClicked);
    QObject::connect(ui->stopButton, &QPushButton::clicked, this, &EngineWidget::onStopClicked);
    QObject::connect(ui->select, &QPushButton::clicked, this, &EngineWidget::onSelectClicked);

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
    static const QString statusFmt = "<b>Depth: %1 (%2 kN/s)</b>";
    QString lines;


    for (const VariantInfo& info : m_variants) {
        double kiloNodes = info.nps() / 1000.0;
        QString status = statusFmt.arg(QString::number(info.depth()), QString::number(kiloNodes));
        HtmlMoveTreeBuilder builder;
        Board board = m_currentBoard;
        QString score;

        ui->status->setText(status);

        if (info.mate()) {
            score = evalFmt.arg("#", QString::number(info.mate()));
        } else {
            int whiteCp = board.currentPlayer().isBlack() ? -info.score() : info.score();
            score = evalFmt.arg(whiteCp > 0 ? "+" : "", QString::number(whiteCp / 100.0, 'f', 2));
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

            if (!board.makeMove(move)) {
                qDebug() << "BUG: " << board.toFen() << "move: " << moveString;
                Q_ASSERT(!"Invalid move");
            }
        }
        lines.append(outputFmt.arg(lineFmt.arg(score, movesFmt.arg(builder.htmlWithStyle()))));
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
    // No engine and no engine was selected by the user.
    if (!m_engine && !onSelectClicked())
        return;

    m_variants.clear();
    m_engine->startAnalysis(m_currentBoard);
}

void EngineWidget::onStopClicked()
{
    if (!m_engine)
        return;
    m_engine->stopAnalysis();
}

bool EngineWidget::onSelectClicked()
{
    EngineSelectionDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted)
        return false;

    setEngine(dialog.engineName());

    return true;
}

void EngineWidget::setEngine(QString name)
{
    if (m_engine)
        delete m_engine;

    m_engine = new Engine(SettingsFactory::engines().config(name));
    QObject::connect(m_engine, &Engine::variantParsed, this, &EngineWidget::onVariantParsed);

    // Update name of the selected engine;
    ui->name->setText(name);
}
