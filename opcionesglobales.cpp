#include "opcionesglobales.h"
#include <QtDebug>
#include <QStringList>
#include <QFileInfo>

opcionesGlobales::opcionesGlobales() {
    ips = new QStringList();
    readSettings();
    setFileLog("menix.log");
    
}

opcionesGlobales::~opcionesGlobales(){

}

QString opcionesGlobales::CentroMensajes() {
    return centroMensajes;
}

QString opcionesGlobales::NumeroDestino() {
    return numeroDestino;
}

QString opcionesGlobales::Puerto() {
    return puerto;
}

QString opcionesGlobales::Pin() {
    return pin;
}

QStringList opcionesGlobales::Ips() {
    return *ips;
}

bool opcionesGlobales::Fixok() {
    return fixok;
}

QString opcionesGlobales::fileLog() {
	QSettings settings;
	QFileInfo fi ( settings.fileName() );
	return fi.canonicalPath() + QLatin1String ( "/" ) + flog;
}

void opcionesGlobales::setCentroMensajes ( QString centro ) {
    centroMensajes = centro;
}

void opcionesGlobales::setNumeroDestino ( QString numero ) {
    numeroDestino = numero;
}

void opcionesGlobales::setPuerto ( QString _puerto ) {
    puerto = _puerto;
}

void opcionesGlobales::setPin ( QString _pin ) {
    pin = _pin;
}

void opcionesGlobales::setIps ( QStringList &lista ) {
    delete ips;
    ips = new QStringList ( lista );
}

void opcionesGlobales::setFixok ( bool valor ) {
    fixok = valor;
}

void opcionesGlobales::setFileLog ( QString fichero) {
    flog = fichero;
}

void opcionesGlobales::readSettings() {
#ifdef Q_OS_WIN32
    QSettings::setDefaultFormat ( QSettings::IniFormat );
#endif
    settings = new QSettings();
#ifdef QT_DEBUG
    qDebug() << "Leyendo : " << settings->fileName();
#endif
    settings->beginGroup ( QLatin1String ( "whitelist" ) );

    int size = settings->beginReadArray ( QLatin1String ( "IP" ) );
#ifdef QT_DEBUG
    qDebug() << "Num. IP :" << size;
#endif
    for ( int i = 0; i < size; ++i ) {
        settings->setArrayIndex ( i );
        ips->append ( settings->value ( QLatin1String ( "IPv4" ) ).toString() );
#ifdef QT_DEBUG
        qDebug() << "IP: " << settings->value ( QLatin1String ( "IPv4" ) ).toString();
#endif
    }

    settings->endArray();
    settings->endGroup();

    // Opciones globales
    settings->beginGroup ( QLatin1String ( "menix" ) );

    numeroDestino = settings->value ( QLatin1String ( "numero" ) ).toString();
    puerto = settings->value ( QLatin1String ( "puerto" ) ).toString();
    pin = settings->value ( QLatin1String ( "pin" ) ).toString();
    centroMensajes = settings->value ( QLatin1String ( "centro" ),"+34609090909" ).toString(); // Centro de Vomistar
    fixok = settings->value ( QLatin1String ( "fixok" ),false ).toBool();
#ifdef QT_DEBUG
    qDebug() << "Numero: " << numeroDestino;
    qDebug() << "Puerto: " << puerto;
#endif
    settings->endGroup();

    delete settings;
}

void opcionesGlobales::writeSettings() {
#ifdef Q_OS_WIN32
    QSettings::setDefaultFormat ( QSettings::IniFormat );
#endif
    settings = new QSettings();
#ifdef QT_DEBUG
    qDebug() << "Escribiendo : " << settings->fileName();
#endif
    settings->beginGroup ( QLatin1String ( "whitelist" ) );

    settings->beginWriteArray ( QLatin1String ( "IP" ) );
    for ( int i = 0; i < ips->size(); ++i ) {
        settings->setArrayIndex ( i );
        settings->setValue ( "IPv4",ips->at ( i ) );
#ifdef QT_DEBUG
        qDebug() << "IP: " << settings->value ( QLatin1String ( "IPv4" ) ).toString();
#endif
    }

    settings->endArray();
    settings->endGroup();

    // Opciones globales
    settings->beginGroup ( QLatin1String ( "menix" ) );

    settings->setValue ( QLatin1String ( "numero" ),numeroDestino );
    settings->setValue ( QLatin1String ( "puerto" ),puerto );
    settings->setValue ( QLatin1String ( "pin" ),pin );
    settings->setValue ( QLatin1String ( "centro" ),centroMensajes );
    settings->setValue ( QLatin1String ( "fixok" ),fixok );
#ifdef QT_DEBUG
    qDebug() << "Numero: " << numeroDestino;
    qDebug() << "Puerto: " << puerto;
#endif
    settings->endGroup();

    delete settings;
}
