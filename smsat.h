#ifndef SMSAT_H
#define SMSAT_H

#include <QThread>
#include <QSettings>
#include <QStringList>
#include <QList>
#include <QTextStream>

#ifdef WIN32
#include <windows.h>
#define SLEEP(n) Sleep(n * 1000);
#else
#include <unistd.h>
#define SLEEP(n) sleep(n);
#endif

class QextSerialPort;

class envioMensaje {
private:
    QString _numero;
    QString _mensaje;
public:
    envioMensaje(const QString& numero,const QString& mensaje);
    QString numero();
    QString mensaje();
    void setNumero(const QString& numero);
    void setMensaje(const QString& mensaje);
};

class smsat : public QThread {
    Q_OBJECT
  #define ERROR_PIN_REQUIRED 2
public:
    smsat(const QString &numero,const QString &puerto,QTextStream* log, QString pin = "", bool fixok = false);
    ~smsat();
    bool testPort();
public slots:
    bool encolaSms(const QString &numero,const QString &mensaje);
private:
    int enviarSms(const QString &numero,const QString &mensaje);
signals:
    void sendAt(const QString&);
    void displayMsg(const QString&);
private:
    QString     m_lastError;
    QString     m_lastCmd;
    QString     m_lastResponse;
    QString     m_lastMailData;
    QSettings *settings;
    QString     centro;
    QString	pin;
    int pinEnviado;
    QextSerialPort *port;
    bool fixok;
    bool read(const QString &waitfor);
    bool sendCommand(const QString &cmd, const QString &waitfor);
    bool sendMessage(const QString &cmd, const QString &waitfor);
    QTextStream *log;
//    QStringList *cola;
    QList<envioMensaje*> *cola;
protected:
    void run();
};

#endif // SMSAT_H
