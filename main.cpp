#include "menix.h"
#include <QApplication>

int main(int argc, char **argv) {
    QApplication app( argc, argv );
    app.setOrganizationName(QLatin1String("NIXVAL"));
    app.setApplicationName(QLatin1String("Menix"));
    menix *Ventana = new menix();
    Ventana->show();
    return app.exec();

}
