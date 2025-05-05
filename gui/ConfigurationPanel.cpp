#include "ConfigurationPanel.h"

ConfigurationPanel::ConfigurationPanel(QWidget* parent)
    : QWidget(parent) {

    itemList = new QListWidget(this);
    addDiagramButton = new QPushButton("Dodaj Wykres", this);
    addDisplayButton = new QPushButton("Dodaj Wyświetlacz", this);
    removeButton = new QPushButton("Usuń Wybrany", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(itemList);
    layout->addWidget(addDiagramButton);
    layout->addWidget(addDisplayButton);
    layout->addWidget(removeButton);

    setLayout(layout);

    connect(addDiagramButton, &QPushButton::clicked, this, &ConfigurationPanel::onAddDiagram);
    connect(addDisplayButton, &QPushButton::clicked, this, &ConfigurationPanel::onAddDisplay);
    connect(removeButton, &QPushButton::clicked, this, &ConfigurationPanel::onRemove);
}

void ConfigurationPanel::onAddDiagram() {
    emit addDiagramRequested();
}

void ConfigurationPanel::onAddDisplay() {
    emit addDisplayRequested();
}

void ConfigurationPanel::onRemove() {
    int currentRow = itemList->currentRow();
    if (currentRow >= 0) {
        emit removeSelectedRequested(currentRow);
        delete itemList->takeItem(currentRow);
    }
}
