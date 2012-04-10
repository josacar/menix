#include <QtNetwork>
#include "server.h"
#include <QMessageBox>

Server::Server ( QStringList *lista, QObject *parent ) : QTcpServer ( parent ) {
    ips = lista;
    if (!listen( QHostAddress::Any, (qint16)31313 )) {
        QMessageBox::critical(0, tr("Menix TCP Server"),
                              tr("No se puede abrir el socket: %1.").arg(errorString()));
        close();
        return;
    }
    connect(this, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
}

void Server::incomingConnection () {
    tcpServerConnection = nextPendingConnection();

    if ( ! ips->contains( tcpServerConnection->peerAddress().toString() ) ) {
#ifdef QT_DEBUG
        qDebug() << "Conexion no aceptada desde IP: " << tcpServerConnection->peerAddress().toString();
#endif
        tcpServerConnection->close();
    }
    connect(tcpServerConnection, SIGNAL(readyRead()), this, SLOT(updateServer()));
    connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

}

void Server::updateServer() {
    QByteArray mensaje = tcpServerConnection->readAll();
#ifdef QT_DEBUG
    int bytesReceived = (int)tcpServerConnection->bytesAvailable();
    qDebug() << "Recibidos " << bytesReceived << " bytes : " << mensaje;
#endif

    emit sendMessage(mensaje);
    if ( tcpServerConnection->isOpen() )
        tcpServerConnection->close();
}

void Server::displayError(QAbstractSocket::SocketError socketError) {
    if (socketError == QTcpSocket::RemoteHostClosedError)
        return;
#ifdef QT_DEBUG
    qDebug() <<  tr("Error de red")
            << tr("Se produjo el siguiente error: %1.").arg(tcpServerConnection->errorString());
#endif

    tcpServerConnection->close();
    close();
}


