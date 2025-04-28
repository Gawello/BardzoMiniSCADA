#include "core/NetworkManager.h"
#include <QTimer>
#include <QDataStream>

NetworkManager::NetworkManager(QObject* parent)
    : QObject(parent), socket(new QTcpSocket(this)) {
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::readData);
}

void NetworkManager::connectToServer(const QString& host, quint16 port) {
    socket->connectToHost(host, port);
}

void NetworkManager::startSimulation() {
    simulationMode = true;
    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &NetworkManager::simulateData);
    simulationTimer->start(100); // np. co 100 ms nowe dane
}

void NetworkManager::stop() {
    if (simulationTimer) simulationTimer->stop();
    socket->close();
}

void NetworkManager::readData() {
    while (socket->bytesAvailable() >= sizeof(double)) {
        double value;
        socket->read(reinterpret_cast<char*>(&value), sizeof(double));
        emit newSampleReceived(value);
    }
}

void NetworkManager::simulateData() {
    double simulatedValue = static_cast<double>(qrand()) / RAND_MAX * 100.0;
    emit newSampleReceived(simulatedValue);
}
