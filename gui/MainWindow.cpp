#include "MainWindow.h"
#include "LineDiagram.h"
#include "PointDiagram.h"
#include "BarDiagram.h"
#include "ElementConfigDialog.h"
#include "TextDisplay.h"
#include "BarDisplay.h"
#include <QtCharts/QChartView>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    network(new NetworkManager(this)),
    buffer(new DataBuffer(1000)),
    logger(new DataLogger("log.txt")),
    processor(new DataProcessor()) {

    QWidget* centralWidget = new QWidget(this);
    layout = new QVBoxLayout(centralWidget);
    centralWidget->setBaseSize(600, 800);

    setCentralWidget(centralWidget);

    configPanel = new ConfigurationPanel(this);
    configDock = new QDockWidget("Konfiguracja", this);
    configDock->setWidget(configPanel);
    addDockWidget(Qt::LeftDockWidgetArea, configDock);

    connect(configPanel, &ConfigurationPanel::addDiagramRequested, this, &MainWindow::addDiagram);
    connect(configPanel, &ConfigurationPanel::addDisplayRequested, this, &MainWindow::addDisplay);
    connect(configPanel, &ConfigurationPanel::removeSelectedRequested, this, &MainWindow::removeElement);
    connect(configPanel, &ConfigurationPanel::elementConfigureRequested, this, &MainWindow::configureElement);
    connect(configPanel, &ConfigurationPanel::saveConfigRequested, this, &MainWindow::saveConfig);
    connect(configPanel, &ConfigurationPanel::loadConfigRequested, this, &MainWindow::loadConfig);

    loadConfiguration("config.json");
    network->startSimulation();
}

MainWindow::~MainWindow() {
    delete buffer;
    delete logger;
    delete processor;
}

void MainWindow::handleNewSample(double sample) {
    double processed = processor->process(sample);

    buffer->addSample(processed);
    logger->logSample(processed);

    auto samples = buffer->getSamples();

    for (auto* diag : diagrams)
        diag->updateData(samples);

    for (auto* disp : displays)
        disp->setValue(processed);
}

void MainWindow::addDiagram(const QString& type) {
    Diagram* diagram = nullptr;

    if (type == "LineDiagram")
        diagram = new LineDiagram();
    else if (type == "PointDiagram")
        diagram = new PointDiagram();
    else if (type == "BarDiagram")
        diagram = new BarDiagram();

    if (!diagram)
        return;

    QChartView* chartView = new QChartView(diagram->getChart());
    chartView->setMinimumHeight(200);

    layout->addWidget(chartView);
    dynamicWidgets.append(chartView);
    diagrams.append(diagram);
}

void MainWindow::addDisplay() {
    auto display = new TextDisplay(this);
    display->setMinimumHeight(50);
    layout->addWidget(display);

    dynamicWidgets.append(display);
    displays.append(display);
}

void MainWindow::removeElement(int index) {
    if (index >= 0 && index < dynamicWidgets.size()) {
        QWidget* widget = dynamicWidgets.at(index);
        layout->removeWidget(widget);

        Diagram* d = qobject_cast<Diagram*>(widget);
        if (d)
            diagrams.removeOne(d);

        TextDisplay* textDisp = qobject_cast<TextDisplay*>(widget);
        if (textDisp)
            displays.removeOne(textDisp);

        BarDisplay* barDisp = qobject_cast<BarDisplay*>(widget);
        if (barDisp)
            displays.removeOne(barDisp);

        widget->deleteLater();
        dynamicWidgets.removeAt(index);
    }
}

void MainWindow::configureElement(int index) {
    if (index < 0 || index >= dynamicWidgets.size())
        return;

    QWidget* widget = dynamicWidgets.at(index);
    ElementConfigDialog dialog(this);

    Diagram* diagram = qobject_cast<Diagram*>(widget);
    if (diagram) {
        dialog.setTitle(diagram->getChart()->title());
        dialog.enableWarningControls(false);

        LineDiagram* line = qobject_cast<LineDiagram*>(widget);
        if (line) {
            auto lineSeries = qobject_cast<QLineSeries*>(line->getChart()->series().first());
            if (lineSeries) {
                dialog.setColor(lineSeries->pen().color());
            }
            dialog.enableColorControl(true);
        }

        if (dialog.exec() == QDialog::Accepted) {
            diagram->getChart()->setTitle(dialog.getTitle());

            if (line)
                line->setColor(dialog.getColor());
        }
        return;
    }

    TextDisplay* textDisp = qobject_cast<TextDisplay*>(widget);
    BarDisplay* barDisp = qobject_cast<BarDisplay*>(widget);

    if (textDisp || barDisp) {
        if (textDisp)
            dialog.setWarningLimits(textDisp->getWarningMin(), textDisp->getWarningMax());
        if (barDisp)
            dialog.setWarningLimits(barDisp->getWarningMin(), barDisp->getWarningMax());

        dialog.setTitle(textDisp ? textDisp->text() : barDisp->text());
        dialog.enableWarningControls(true);

        if (dialog.exec() == QDialog::Accepted) {
            if (textDisp)
                textDisp->setText(dialog.getTitle());
            if (barDisp)
                barDisp->setFormat(dialog.getTitle() + " %v");

            if (textDisp)
                textDisp->setWarningLimits(dialog.getWarningMin(), dialog.getWarningMax());
            if (barDisp)
                barDisp->setWarningLimits(dialog.getWarningMin(), dialog.getWarningMax());
        }
    }
}
void MainWindow::loadConfiguration(const QString& filename) {
    auto elements = ConfigManager::load(filename);

    for (const auto& obj : elements) {
        QString type = obj["type"].toString();

        if (type.contains("Diagram")) {
            addDiagram(type);

            Diagram* diagram = diagrams.last();
            diagram->getChart()->setTitle(obj["title"].toString());

            if (type == "LineDiagram") {
                auto* line = dynamic_cast<LineDiagram*>(diagram);
                if (line)
                    line->setColor(QColor(obj["color"].toString()));
            }
        } else if (type == "TextDisplay") {
            auto* disp = new TextDisplay(this);
            disp->setText(obj["title"].toString());
            disp->setWarningLimits(obj["minWarning"].toDouble(), obj["maxWarning"].toDouble());
            layout->addWidget(disp);
            dynamicWidgets.append(disp);
            displays.append(disp);
        } else if (type == "BarDisplay") {
            auto* disp = new BarDisplay(this);
            disp->setFormat(obj["title"].toString());
            disp->setWarningLimits(obj["minWarning"].toDouble(), obj["maxWarning"].toDouble());
            layout->addWidget(disp);
            dynamicWidgets.append(disp);
            displays.append(disp);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    ConfigManager::save("config.json", dynamicWidgets);
    event->accept();
}

void MainWindow::saveConfig() {
    ConfigManager::save("config.json", dynamicWidgets);
}

void MainWindow::loadConfig() {
    // Usuń wszystkie aktualne elementy
    for (auto* widget : dynamicWidgets) {
        layout->removeWidget(widget);
        widget->deleteLater();
    }

    dynamicWidgets.clear();
    diagrams.clear();
    displays.clear();

    // Wczytaj z config.json
    loadConfiguration("config.json");
}
