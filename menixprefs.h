#ifndef MENIXPREFS_H
#define MENIXPREFS_H

#include <QtGui/QDialog>
#include <QtGui/QListWidgetItem>
#include "opcionesglobales.h"

namespace Ui {
    class menixPrefs;
}

class menixPrefs : public QDialog {
    Q_OBJECT
public:
    menixPrefs(opcionesGlobales *opciones,QWidget *parent = 0);
    ~menixPrefs();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::menixPrefs *m_ui;
    opcionesGlobales *_opciones;
protected slots:
    void updateValues();
    void addIp(bool);
    void delIp(bool);
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
};

#endif // MENIXPREFS_H
