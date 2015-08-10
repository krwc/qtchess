#include "engine.hpp"
#include "game/board.hpp"
#include <QDebug>
#include <QRegExp>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <QThread>
#include <QMutex>

Engine::Engine(const QString& path, const int timeoutMs)
    : m_timeoutMs(timeoutMs)
    , m_process(new QProcess())
    , m_state(Engine::Initializing)
{
    QObject::connect(m_process, &QProcess::readyRead, this, &Engine::onReadyRead);
    // Start the engine.
    m_process->start(path);
    // Say hello
    send("uci");
}

void Engine::startAnalysis(const Board& current)
{
    // FIXME: This is stupid and plain wrong.
    while (m_state != Engine::Idling) {
        if (!m_process->waitForReadyRead(m_timeoutMs))
            throw new std::runtime_error("Engine did not stop in time.");
    }

    setState(Engine::Working);
    send("ucinewgame");
    send("position fen " + current.toFen());
    send("go infinite");
}

void Engine::stopAnalysis()
{
    if (!isAnalysing())
        return;
    setState(Engine::Stopping);
    send("stop");
}

bool Engine::isAnalysing() const
{
    return m_state == Engine::Working;
}

void Engine::onReadyRead()
{
    while (m_process->canReadLine()) {
        QString line = m_process->readLine();
        // Make Windows users happy.
        if (line.contains("\r\n"))
            line.chop(2);
        else
            line.chop(1);

        setState(parseLine(line));
    }
}

void Engine::parseOption(const QString& line)
{
    QStringList tokens = line.split(" ");
    QString name;

    for (int i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "name") {
            while (tokens[i+1] != "type")
                name += tokens[i++] + " ";
            name.chop(1);
        } else if (tokens[i] == "type") {

        }
    }
}

void Engine::parseInfo(const QString& line)
{
    // Currently we don't care about non-eval lines
    if (!line.contains("score"))
        return;

    QStringList tokens = line.split(" ");
    QStringList moves;
    LineInfo info;

    for (int i = 0; i < tokens.size(); i++) {
        const QString& token = tokens[i];

        if (token == "score") {
            const QString& type = tokens[++i];
            if (type == "cp")
                info.setScore(tokens[++i].toInt());
            else if (type == "mate")
                info.setMate(tokens[++i].toInt());
        } else if (token == "depth") {
            info.setDepth(tokens[++i].toInt());
        } else if (token == "multipv") {
            info.setId(tokens[++i].toInt());
        } else if (token == "pv") {
            ++i;
            while (i < tokens.size())
                moves.append(tokens[i++]);

            info.setMoveList(moves);
        }
    }

    emit lineInfo(info);
}

Engine::State Engine::parseLine(const QString& line)
{
    Q_ASSERT(m_state != Engine::Idling && "Engine talks while it is idle.");

    switch (m_state) {
    case Engine::Initializing:
        // End of the initialization, engine is now idle
        if (line.startsWith("uciok"))
            return Engine::Idling;
        else if (line.startsWith("option"))
            parseOption(line);
        break;
    case Engine::Working:
        if (line.startsWith("info"))
            parseInfo(line);
        break;
    case Engine::Stopping:
        if (line.startsWith("bestmove"))
            return Engine::Idling;
        break;
    default:
        break;
    }
    // No state change
    return m_state;
}

void Engine::setState(State state)
{
    m_state = state;
}

void Engine::send(const QString& command)
{
    m_process->write(command.toStdString().c_str());
    m_process->write("\n");
}

QString Engine::readLine() const
{
    return QString(m_process->readLine());
}

