#ifndef ENGINE_DESCRIPTION_HPP
#define ENGINE_DESCRIPTION_HPP
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>

class EngineDescription
{
private:
    static constexpr const char* FIELD_NAME = "name";
    static constexpr const char* FIELD_WORK_DIR = "work_dir";
    static constexpr const char* FIELD_EXEC_PATH = "exec_path";
    static constexpr const char* FIELD_ARGS = "args";
public:
    const QString& name() const {
        return m_name;
    }
    const QString& workDir() const {
        return m_workDir;
    }
    const QString& execPath() const {
        return m_execPath;
    }
    const QString& arguments() const {
        return m_arguments;
    }

    void setName(const QString& name) {
        m_name = name;
    }
    void setWorkDir(const QString& workDir) {
        m_workDir = workDir;
    }
    void setExecPath(const QString& execPath) {
        m_execPath = execPath;
    }
    void setArguments(const QString& arguments) {
        m_arguments = arguments;
    }

    QVariantMap toVariantMap() const
    {
        QVariantMap engine;
        engine[FIELD_NAME] = name();
        engine[FIELD_ARGS] = arguments();
        engine[FIELD_WORK_DIR] = workDir();
        engine[FIELD_EXEC_PATH] = execPath();
        return engine;
    }

    static EngineDescription fromVariantMap(const QVariantMap& map)
    {
        EngineDescription desc;
        desc.setName(map[FIELD_NAME].toString());
        desc.setArguments(map[FIELD_ARGS].toString());
        desc.setWorkDir(map[FIELD_WORK_DIR].toString());
        desc.setExecPath(map[FIELD_EXEC_PATH].toString());
        return desc;
    }

private:
    QString m_name;
    QString m_workDir;
    QString m_execPath;
    QString m_arguments;
};

#endif // ENGINE_DESCRIPTION_HPP
