#include "settings/engine-settings.hpp"

EngineSettings::EngineSettings(const QString& engineName)
    : AbstractSettings("engine", engineName)
{
    set("stringName", engineName);
    set("stringPathExec", "");
    set("stringPathWork", ".");
    set("stringArguments", "");
    reset();
}

QStringList EngineSettings::engines()
{
    AbstractSettings settings("engine");
    return settings.groups();
}

void EngineSettings::remove(const QString& engine)
{
    AbstractSettings settings("engine");
    settings.unset(engine);
    settings.save();
}
