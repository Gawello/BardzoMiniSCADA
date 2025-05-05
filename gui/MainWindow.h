#pragma once

#include <QMainWindow>
#include "NetworkManager.h"
#include "DataBuffer.h"
#include "DataLogger.h"
#include "DataProcessor.h"
#include "Diagram.h"
#include "TextDisplay.h"
#include "ConfigurationPanel.h"
#include "BarDisplay.h"
#include <QDockWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    ConfigurationPanel* configPanel;
    QDockWidget* configDock;

    QVBoxLayout* layout;
    QList<QWidget*> dynamicWidgets;
    QList<Diagram*> diagrams; // Dynamiczna lista wykresów
    QList<DataDisplay*> displays; // Dynamiczna lista wyświetlaczy (np. TextDisplay)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleNewSample(double sample);
    void addDiagram();
    void addDisplay();
    void removeElement(int index);

private:
    NetworkManager* network;
    DataBuffer* buffer;
    DataLogger* logger;
    DataProcessor* processor;
    Diagram* diagram;
    TextDisplay* display;
};
