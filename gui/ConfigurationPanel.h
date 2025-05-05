#pragma once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

class ConfigurationPanel : public QWidget {
    Q_OBJECT

public:
    explicit ConfigurationPanel(QWidget* parent = nullptr);

signals:
    void addDiagramRequested();
    void addDisplayRequested();
    void removeSelectedRequested(int index);

private slots:
    void onAddDiagram();
    void onAddDisplay();
    void onRemove();

private:
    QListWidget* itemList;
    QPushButton* addDiagramButton;
    QPushButton* addDisplayButton;
    QPushButton* removeButton;
};
