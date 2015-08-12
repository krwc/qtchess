#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <QProcess>
#include <QVector>
#include "engine/variant-info.hpp"
#include "engine/engine-option.hpp"

class EngineSettings;
class Board;
class Engine : public QObject
{
    Q_OBJECT
public:
    enum State { Working, Stopping, Idling, Initializing };

    explicit Engine(EngineSettings& settings, const int timeoutMs = 2000);
    ~Engine();

    /*! \brief Starts an engine */
    void start();

    /*! \brief Puts an engine into an ifinite analysis mode */
    void startAnalysis(const Board& current);

    /*! \brief Stops analysis and blocks until an engine shuts up. */
    void stopAnalysis();

    /*! \brief Sets engine option. */
    void setOption(const QString& name, const QString& value);

    /*! \brief Returns true if engine is currently analysing the game */
    bool isAnalysing() const;
signals:
    void variantParsed(VariantInfo);
    void optionsParsed(QList<EngineOption>);
private slots:
    void onStarted();
    void onReadyRead();
private:
    /*! \brief Waits for state change to a specific one or throws. */
    void waitForStateOrThrow(State expectedState);

    /*! \brief Parses option */
    void parseOption(const QString& line);

    /*! \brief Parses info */
    void parseInfo(const QString& line);

    /*! \brief Parses line from the engine */
    State parseLine(const QString& line);

    /*! \brief Sets current state */
    void setState(State state);

    /*! \brief Sends command string to the engine */
    void send(const QString& command);

    /*! \brief Reads line from the engine */
    QString readLine() const;

    /*!< \brief Default timeout for engine actions */
    const int m_timeoutMs;
    QProcess* m_process;
    State m_state;
    EngineSettings& m_settings;

    /*!< \brief List of parsed options declared by the engine. */
    QList<EngineOption> m_parsedOptions;
};

#endif // ENGINE_HPP
