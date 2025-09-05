#include <QtWidgets/QApplication>
#include "OknoGlowne.hpp"
int main(int argc, char** argv) {
    QApplication app(argc, argv);
    OknoGlowne w; w.resize(1200,700); w.show();
    return app.exec();
}
