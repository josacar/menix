#ifndef NAGIOSSMS_H
#define NAGIOSSMS_H

#include <QMenu>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QtGui/QWidget>
#include <QCloseEvent>
#include <QMainWindow>
#include <QFile>
#include "smsat.h"
#include "opcionesglobales.h"

namespace Ui {
class menix;
}

class menix : public QMainWindow {
    Q_OBJECT

public:
    menix(QWidget *parent = 0);
    ~menix();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::menix *m_ui;
    void readSettings();
    void crearBandeja();
    void crearAcciones();
    void closeEvent ( QCloseEvent *event );
    void testPuerto( smsat *mensaje );
    QMenu *trayIconMenu;
    QSystemTrayIcon *trayIcon;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QIcon *icon;
    smsat *mensaje;
    opcionesGlobales *opciones;
    QStringList *ips;
    QString numero;
    QString puerto;
    QString centro;
    QString pin;
    QTextStream *flog;
    QFile *file;
    bool fixok;

private slots:
//    void setNumero ( );
//    void setPuerto ( );
    void on_pushButton_clicked();
    void sendMessage( const QString& );
    void sendedMessage( const QString& );
    void showPopup( const QString& );
    void accionesBandeja( QSystemTrayIcon::ActivationReason reason );
    void restore();
    void abrirOpciones();
    void abrirDebug();
};

#endif // NAGIOSSMS_H
