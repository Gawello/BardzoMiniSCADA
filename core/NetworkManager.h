#pragma once

#include <QTcpSocket>
#include <QObject>

class NetworkManager : public QObject {
    Q_OBJECT

public:
    explicit NetworkManager(QObject* parent = nullptr);

    void connectToServer(const QString& host, quint16 port);
    void startSimulation(); // alternatywny tryb
    void stop();

signals:
    void newSampleReceived(double sample);

private slots:
    void readData();

private:
    QTcpSocket* socket;
    bool simulationMode = false;
    QTimer* simulationTimer = nullptr;

    void simulateData();
};
