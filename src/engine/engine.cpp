#include "engine.hpp"
#include "game/board.hpp"
#include <QDebug>
#include <stdexcept>


Engine::Engine(const EngineConfig& config, const int timeoutMs)
    : m_timeoutMs(timeoutMs)
    , m_process(new QProcess())
    , m_state(Engine::Initializing)
    , m_config(config)
{
    QObject::connect(m_process, &QProcess::started, this, &Engine::onStarted);
    QObject::connect(m_process, &QProcess::readyRead, this, &Engine::onReadyRead);
    m_process->setProgram(m_config.command());
}

Engine::~Engine()
{
}

void Engine::start()
{
    // Start the engine.
    m_process->start();

    if (!m_process->waitForStarted(m_timeoutMs))
        throw std::runtime_error("Cannot start engine.");

    waitForStateOrThrow(Engine::Idling);
}

void Engine::startAnalysis(const Board& current)
{
    if (!started())
        start();

    waitForStateOrThrow(Engine::Idling);

    setState(Engine::Working);
    // Set all options.
    for (const QString& key : m_config.options())
        setOption(key, m_config.option(key));

    send("position fen " + current.toFen());
    send("go infinite");
}

void Engine::stopAnalysis()
{
    if (!isAnalysing())
        return;
    setState(Engine::Stopping);
    send("stop");

    waitForStateOrThrow(Engine::Idling);
}

void Engine::setOption(const QString& name, const QString& value)
{
    send(QString("setoption name %1 value %2").arg(name, value));
}

bool Engine::isAnalysing() const
{
    return m_state == Engine::Working;
}

bool Engine::started() const
{
    return m_process->state() == QProcess::Running;
}

EngineConfig Engine::config()
{
    return m_config;
}

const QList<EngineOption>& Engine::options()
{
    return m_parsedOptions;
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

void Engine::waitForStateOrThrow(State expectedState)
{
    while (m_state != expectedState) {
        if (!m_process->waitForReadyRead(m_timeoutMs))
            throw std::runtime_error("Engine stopped responding.");
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

