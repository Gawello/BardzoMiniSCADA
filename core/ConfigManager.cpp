#include "ConfigManager.h"
#include "LineDiagram.h"
#include "PointDiagram.h"
#include "BarDiagram.h"
#include "TextDisplay.h"
#include "BarDisplay.h"
#include <QChartView>

void ConfigManager::save(const QString& filename, const QList<QWidget*>& widgets) {
    QJsonArray elements;

    for (auto* widget : widgets) {
        QJsonObject obj;

        if (auto* view = qobject_cast<QChartView*>(widget)) {
            auto* diagram = qobject_cast<Diagram*>(view);
            if (auto* line = dynamic_cast<LineDiagram*>(diagram)) {
                obj["type"] = "LineDiagram";
                obj["title"] = line->getChart()->title();
                auto lineSeries = qobject_cast<QLineSeries*>(line->getChart()->series().first());
                if (lineSeries)
                    obj["color"] = lineSeries->pen().color().name();

            } else if (dynamic_cast<PointDiagram*>(diagram)) {
                obj["type"] = "PointDiagram";
                obj["title"] = diagram->getChart()->title();
            } else if (dynamic_cast<BarDiagram*>(diagram)) {
                obj["type"] = "BarDiagram";
                obj["title"] = diagram->getChart()->title();
            }
        }

        if (auto* text = qobject_cast<TextDisplay*>(widget)) {
            obj["type"] = "TextDisplay";
            obj["title"] = text->text();
            obj["minWarning"] = text->getWarningMin();
            obj["maxWarning"] = text->getWarningMax();
        }

        if (auto* bar = qobject_cast<BarDisplay*>(widget)) {
            obj["type"] = "BarDisplay";
            obj["title"] = bar->format();
            obj["minWarning"] = bar->getWarningMin();
            obj["maxWarning"] = bar->getWarningMax();
        }

        elements.append(obj);
    }

    QJsonObject root;
    root["elements"] = elements;

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson());
        file.close();
    }
}

QList<QJsonObject> ConfigManager::load(const QString& filename) {
    QFile file(filename);
    QList<QJsonObject> list;

    if (!file.open(QIODevice::ReadOnly))
        return list;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    auto elements = doc.object()["elements"].toArray();
    for (const auto& el : elements)
        list.append(el.toObject());

    return list;
}
