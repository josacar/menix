#ifndef MENIXDEBUG_H
#define MENIXDEBUG_H

#include <QtGui/QDialog>
#include <QtGui/QTextEdit>

namespace Ui {
    class menixDebug;
}

class menixDebug : public QDialog {
    Q_OBJECT
public:
    menixDebug(QString log,QWidget *parent = 0);
    ~menixDebug();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::menixDebug *m_ui;
    QString fichero;
    void verLog(QTextEdit *qte);
};

#endif // MENIXDEBUG_H
