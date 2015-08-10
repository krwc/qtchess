#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <QProcess>
#include <string>
#include "engine/line-info.hpp"

class Board;
class Engine : public QObject
{
    Q_OBJECT
public:
    enum State { Working, Stopping, Idling, Initializing };

    explicit Engine(const QString& path, const int timeoutMs = 2000);

    /*! \brief Puts an engine into an ifinite analysis mode */
    void startAnalysis(const Board& current);

    /*! \brief Stops analysis and blocks until an engine shuts up. */
    void stopAnalysis();

    /*! \brief Returns true if engine is currently analysing the game */
    bool isAnalysing() const;

signals:
    void lineInfo(LineInfo);
private slots:
    void onReadyRead();
private:
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
};

#endif // ENGINE_HPP
