#include "smsat.h"
#include <qextserialport.h>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>

envioMensaje::envioMensaje(const QString& numero,const QString& mensaje){
    _numero = numero;
    _mensaje = mensaje;
}

QString envioMensaje::numero(){
    return _numero;
}

QString envioMensaje::mensaje(){
    return _mensaje;
}

void envioMensaje::setNumero(const QString& numero){
    _numero = numero;
}

void envioMensaje::setMensaje(const QString& mensaje){
    _mensaje = mensaje;
}


smsat::smsat(const QString &centro,const QString &puerto,QTextStream* log ,QString pin, bool fix) {
    this->centro = centro;
    this->log = log;
    this->pin = pin;
    
    pinEnviado = 0;
    
    QFile *aux = (QFile*)log->device();
    qDebug() << aux->fileName();
    
    fixok = fix;
    PortSettings *cfg = new PortSettings();
    cfg->BaudRate = BAUD9600;
    cfg->FlowControl = FLOW_HARDWARE;
    cfg->Parity = PAR_NONE;
    cfg->DataBits = DATA_8;
    cfg->StopBits = STOP_1;

    port = new QextSerialPort( puerto, *cfg );

    cola = new QList<envioMensaje*>();

}

smsat::~smsat(){
    delete cola;
    delete port;
}

void smsat::run(){
  int estado = 0;
    while ( !cola->isEmpty() ){
#ifdef QT_DEBUG
        *log << "Quedan " << cola->length() << " mensajes en cola\n";
	log->flush();
#endif
        if ( ! ( estado = enviarSms( cola->first( )->numero() ,cola->first( )->mensaje() ) ) ) {
            delete cola->first( );
            cola->removeFirst();
        } else {
	    if ( estado == ERROR_PIN_REQUIRED ) {
	      emit displayMsg("La sim necesita PIN");
	      SLEEP(60);
	    } else {
	      SLEEP(5);
	    }
        }
    }
}

bool smsat::encolaSms(const QString &numero,const QString &mensaje) {
    cola->append( new envioMensaje(numero,mensaje) );
#ifdef QT_DEBUG
    *log << cola->length() << " mensajes en cola\n";
    log->flush();
#endif
    if ( !isRunning() )
        start();
    return true;
}

bool smsat::testPort(){
    if ( !port ) return false;

    if ( !port->open(QIODevice::ReadWrite | QIODevice::Unbuffered ) ) {
        *log << "No se puede abrir el puerto serie\n";
	log->flush();
        return false;
    }

    if( !port->isOpen() ) {
        *log << "El puerto serie no esta abierto\n";
	log->flush();
        return false;
    }

    if ( !sendCommand( "AT", "OK") ) {
        *log << "Error en el comando AT\n";
	log->flush();
        port->close();
        return false;
    }

    port->close();
    return true;
}

int smsat::enviarSms(const QString &numero,const QString &mensaje) {
    if ( !port ) {
        return 1;
    }

    if ( !port->open(QIODevice::ReadWrite ) ) {
        *log << "No se puede abrir el puerto serie\n";
	log->flush();
        return 1;
    }

    if( !port->isOpen() ) {
        *log << "El puerto serie no esta abierto\n";
	log->flush();
        return 1;
    }

    if ( !sendCommand("AT", "OK") ) {
        *log << "Error en el comando AT\n";
	log->flush();
        port->close();
        return 1;
    }
    
    if ( this->pin != "" && !pinEnviado ) {
	if ( sendCommand("AT+CPIN?" , "+CPIN: SIM PIN") ) {
	  if ( !sendCommand("AT+CPIN=" + this->pin, "OK") ) {
	      port->close();
	      return 1;
	  }
	  pinEnviado = 1;
	}
    }
    
    if ( !sendCommand("AT+CMGF=1", "OK") ) {
	if ( this->m_lastResponse.contains("+CME ERROR: SIM PIN required")  ) {
	  port->close();
	  return ERROR_PIN_REQUIRED;
	}
        port->close();
        return 1;
    }

    if ( !sendCommand("AT+CSCA=\"" + this->centro + "\"", "OK") ) {
        port->close();
        return 1;
    }

    if( !sendCommand("AT+CMGS=\"" + numero + "\"", ">") ) {
        port->close();
        return 1;
    }

    if( sendMessage(mensaje, "ERROR") ) {
        port->close();
    }

    port->close();
    emit sendAt(numero + ": " +mensaje);
    return 0;
}

bool smsat::read(const QString &waitfor) {
    char buff[1024];
    int numBytes;

    numBytes = port->bytesAvailable();

    if(numBytes > 0)   {
        if(numBytes > 1024) numBytes = 1024;

        int i = port->read(buff, numBytes);
        buff[i] = '\0';
        const QString msg = buff;
#ifdef QT_DEBUG
        *log << msg << "\n";

        *log << "bytes available: " << numBytes << "\n";
        *log << "received: " << i << "\n";
	log->flush();
#endif

	m_lastResponse = msg;

        if ( msg.contains(waitfor) ) return true;
        else return false;

    } else {
	if ( fixok ) return true; // Para el JabloCOM
        return false;
    }

    return true;
}

bool smsat::sendCommand(const QString &cmd, const QString &waitfor){
    QTextStream t(port);
    // t << cmd + "\x0D\x0A";
    t << cmd + "\x0D";
    t.flush();

    m_lastCmd = cmd;
    SLEEP(1);
    return read(waitfor);
}

bool smsat::sendMessage(const QString &cmd, const QString &waitfor){
    QTextStream t(port);
    t << cmd + "\032";
    t.flush();
    SLEEP(1);
    m_lastCmd = cmd;

    return read(waitfor);
}
