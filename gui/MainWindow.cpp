#include "MainWindow.h"
#include "LineDiagram.h"
#include "PointDiagram.h"
#include "BarDiagram.h"
#include "ElementConfigDialog.h"
#include "TextDisplay.h"
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

void MainWindow::addDiagram(const QString& type) {
    Diagram* diagram = nullptr;

    if (type == "LineDiagram") {
        diagram = new LineDiagram();
    } else if (type == "PointDiagram") {
        diagram = new PointDiagram();
    } else if (type == "BarDiagram") {
        diagram = new BarDiagram();
    }

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


void MainWindow::configureElement(int index) {
    if (index < 0 || index >= dynamicWidgets.size())
        return;

    QWidget* widget = dynamicWidgets.at(index);

    ElementConfigDialog dialog(this);

    // Wykresy (Diagram)
    Diagram* diagram = qobject_cast<Diagram*>(widget);
    if (diagram) {
        dialog.setTitle(diagram->getChart()->title());
        dialog.enableWarningControls(false);
        if (dialog.exec() == QDialog::Accepted) {
            diagram->getChart()->setTitle(dialog.getTitle());
        }
        return;
    }

    // Wyświetlacze (TextDisplay lub BarDisplay)
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

