#pragma once

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QString>

class ConfigManager {
public:
    static void save(const QString& filename, const QList<QWidget*>& widgets);
    static QList<QJsonObject> load(const QString& filename);
};
