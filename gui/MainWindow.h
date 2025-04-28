#pragma once

#include <QMainWindow>
#include "NetworkManager.h"
#include "DataBuffer.h"
#include "DataLogger.h"
#include "DataProcessor.h"
#include "Diagram.h"
#include "TextDisplay.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleNewSample(double sample);

private:
    NetworkManager* network;
    DataBuffer* buffer;
    DataLogger* logger;
    DataProcessor* processor;
    Diagram* diagram;
    TextDisplay* display;
};
