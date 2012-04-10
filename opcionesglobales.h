#ifndef OPCIONESGLOBALES_H
#define OPCIONESGLOBALES_H

#include <QObject>
#include <QSettings>

class opcionesGlobales : public QObject {
    public:
        opcionesGlobales();
	~opcionesGlobales();
        QString CentroMensajes();
        QString NumeroDestino();
        QString Puerto();
	QString Pin();
        QStringList Ips();
        bool Fixok();
        void setCentroMensajes ( QString );
        void setNumeroDestino ( QString );
        void setPuerto ( QString );
	void setPin ( QString );
        void setIps ( QStringList &lista );
        void setFixok ( bool );
        QString fileLog();
        void setFileLog ( QString fichero );
    private:
        QString centroMensajes;
        QString numeroDestino;
        QString puerto;
	QString pin;
        QSettings *settings;
        QStringList *ips;
        QString flog;

        bool fixok;
        void readSettings();
    public slots:
        void writeSettings();
};

#endif // OPCIONESGLOBALES_H
