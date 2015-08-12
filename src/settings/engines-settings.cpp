#include "settings/engines-settings.hpp"
#include "engine/engine-config.hpp"

EnginesSettings::EnginesSettings()
    : AbstractSettings("engines")
{
    set("configs", QList<QVariant>());
    reset();
}

QStringList EnginesSettings::names() const
{
    QStringList list;

    for (const QVariant& current : get("configs").toList())
        list.append(EngineConfig(current.toMap()).name());

    return list;
}

void EnginesSettings::saveConfig(const EngineConfig& config)
{
    QList<QVariant> configs = get("configs").toList();
    bool added = false;

    for (int i = 0; i < configs.size(); i++) {
        EngineConfig current(configs.at(i).toMap());

        // Engine of given name already exists -> replace.
        if (current.name() == config.name()) {
            configs[i] = config.toVariantMap();
            added = true;
            break;
        }
    }
    if (!added)
        configs.append(config.toVariantMap());

    set("configs", configs);
    save();
}

void EnginesSettings::remove(const QString& name)
{
    QList<QVariant> configs = get("configs").toList();

    for (int i = 0; i < configs.size(); i++) {
        EngineConfig current(configs[i].toMap());

        if (current.name() == name) {
            configs.removeAt(i);
            break;
        }
    }
    set("configs", configs);
    save();
}

EngineConfig EnginesSettings::config(const QString& name) const
{
    for (const QVariant& variant : get("configs").toList()) {
        EngineConfig current(variant.toMap());

        if (current.name() == name)
            return current;
    }
    Q_ASSERT(!"No such engine exists.");
}
