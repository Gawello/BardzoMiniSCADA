#include "MainWindow.h"
#include "LineDiagram.h"
#include <QtCharts/QChartView>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), network(new NetworkManager(this)), buffer(new DataBuffer(1000)),
    logger(new DataLogger("log.txt")), processor(new DataProcessor()), display(new TextDisplay(this)) {

    diagram = new LineDiagram();

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    auto chartView = new QChartView(diagram->getChart());
    layout->addWidget(chartView);
    layout->addWidget(display);

    setCentralWidget(centralWidget);

    connect(network, &NetworkManager::newSampleReceived, this, &MainWindow::handleNewSample);

    network->startSimulation(); // startujemy symulację
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
    diagram->updateData(samples);

    display->setValue(processed);
}
