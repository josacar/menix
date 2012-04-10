#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QStringList>
#include <QSettings>

class Connection;

class Server : public QTcpServer {
	Q_OBJECT

public:
        Server ( QStringList *lista,QObject *parent = 0 );

signals:
   void sendMessage( const QString& );

protected slots:
   void incomingConnection ( );
   void updateServer();
   void displayError(QAbstractSocket::SocketError);

private:
   QTcpSocket *tcpServerConnection;
   QStringList *ips;

};

#endif
