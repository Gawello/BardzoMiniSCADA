#include "MainWindow.h"
#include "LineDiagram.h"
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

    setCentralWidget(centralWidget);

    configPanel = new ConfigurationPanel(this);
    configDock = new QDockWidget("Konfiguracja", this);
    configDock->setWidget(configPanel);
    addDockWidget(Qt::LeftDockWidgetArea, configDock);

    connect(configPanel, &ConfigurationPanel::addDiagramRequested, this, &MainWindow::addDiagram);
    connect(configPanel, &ConfigurationPanel::addDisplayRequested, this, &MainWindow::addDisplay);
    connect(configPanel, &ConfigurationPanel::removeSelectedRequested, this, &MainWindow::removeElement);

    network->startSimulation();
}

MainWindow::~MainWindow() {
    delete buffer;
    delete logger;
    delete processor;
    delete diagram;
}

void MainWindow::handleNewSample(double sample) {
    double processed = processor->process(sample);

    buffer->addSample(processed);
    logger->logSample(processed);

    auto samples = buffer->getSamples();

    // Aktualizujemy WSZYSTKIE wykresy
    for (auto* diag : diagrams) {
        diag->updateData(samples);
    }

    // Aktualizujemy WSZYSTKIE wyświetlacze
    for (auto* disp : displays) {
        disp->setValue(processed);
    }
}

void MainWindow::addDiagram() {
    auto diagram = new LineDiagram();
    auto chartView = new QChartView(diagram->getChart());
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
        if (d) {
            diagrams.removeOne(d);
        }

        TextDisplay* textDisp = qobject_cast<TextDisplay*>(widget);
        if (textDisp) {
            displays.removeOne(textDisp);
        }

        BarDisplay* barDisp = qobject_cast<BarDisplay*>(widget);
        if (barDisp) {
            displays.removeOne(barDisp);
        }


        widget->deleteLater();
        dynamicWidgets.removeAt(index);
    }
}


