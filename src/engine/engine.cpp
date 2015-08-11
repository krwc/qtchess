#include "engine.hpp"
#include "game/board.hpp"
#include <QDebug>
#include <stdexcept>
#include "settings/settings-factory.hpp"

Engine::Engine(EngineSettings& settings, const int timeoutMs)
    : m_timeoutMs(timeoutMs)
    , m_process(new QProcess())
    , m_state(Engine::Initializing)
    , m_settings(settings)
{
    QObject::connect(m_process, &QProcess::started, this, &Engine::onStarted);
    QObject::connect(m_process, &QProcess::readyRead, this, &Engine::onReadyRead);
    m_process->setProgram(m_settings.get("stringPathExec").toString());
}

Engine::~Engine()
{
}

void Engine::start()
{
    // Start the engine.
    m_process->start();
}

void Engine::startAnalysis(const Board& current)
{
    if (m_process->state() != QProcess::Running)
        m_process->start();
    // FIXME: This is stupid and plain wrong. And doesn't even do what it is supposed to.
    while (m_state != Engine::Idling) {
        if (!m_process->waitForReadyRead(m_timeoutMs))
            throw new std::runtime_error("Engine did not stop in time.");
    }

    setState(Engine::Working);
    send("ucinewgame");
    for (const QString& key : m_settings.keys())
        setOption(key, m_settings.get(key).toString());
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

void Engine::setOption(const QString& name, const QString& value)
{
    send(QString("setoption name %1 value %2").arg(name, value));
}

bool Engine::isAnalysing() const
{
    return m_state == Engine::Working;
}

void Engine::onStarted()
{
    send("uci");
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
    static QStringList keywords = {
        "option", "name", "type", "check",
        "spin", "combo", "button", "string",
        "default", "min", "max", "var"
    };

    QStringList tokens = line.split(" ");
    QString name;

    for (int i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "name") {
            while (!keywords.contains(tokens[++i])) {
                name.append(tokens[i]);
                name.append(' ');
            }
            name.chop(1);
            --i;
        } else if (tokens[i] == "type") {
            QString type = tokens[++i];
            EngineOption option;

            if (type == "check") {
                ++i;
                option = EngineOption::checkbox(name, tokens[++i] == "true");
            } else if (type == "button") {
                option = EngineOption::button(name);
            } else if (type == "string") {
                ++i;
                QString value;
                while (++i < tokens.size()) {
                    value.append(tokens[i]);
                    value.append(' ');
                }
                value.chop(1);
                option = EngineOption::string(name, value);
            } else if (type == "spin") {
                int defaultValue;
                int minValue;
                int maxValue;
                while (++i < tokens.size()) {
                    if (tokens[i] == "default")
                        defaultValue = tokens[++i].toInt();
                    else if (tokens[i] == "min")
                        minValue = tokens[++i].toInt();
                    else if (tokens[i] == "max")
                        maxValue = tokens[++i].toInt();
                }
                option = EngineOption::spinbox(name, minValue, maxValue, defaultValue);
            } else {
                qDebug() << "Warning: ignored option: " << line;
                break;
            }
            m_parsedOptions.push_back(option);
        }
    }
}

void Engine::parseInfo(const QString& line)
{
    // It is variant info
    if (line.contains("score")) {
        QStringList tokens = line.split(" ");
        QStringList moves;
        VariantInfo info;

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
            } else if (token == "nps") {
                info.setNps(tokens[++i].toInt());
            }
        }

        emit variantParsed(info);
    } else {
        // It is engine info.
    }
}

Engine::State Engine::parseLine(const QString& line)
{
    Q_ASSERT(m_state != Engine::Idling && "Engine talks while it is idle.");

    switch (m_state) {
    case Engine::Initializing:
        // End of the initialization, engine is now idle
        if (line.startsWith("uciok")) {
            emit optionsParsed(m_parsedOptions);
            return Engine::Idling;
        } else if (line.startsWith("option"))
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

