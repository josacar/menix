#include "menix.h"
#include "ui_menix.h"
#include "server.h"
#include <QColorDialog>
#include <QToolBar>
#include <QMessageBox>
#include "menixprefs.h"
#include "menixdebug.h"

menix::menix(QWidget *parent) :
        QMainWindow(parent),
        m_ui(new Ui::menix) {
    m_ui->setupUi(this);


    QToolBar *toolbarIconos = addToolBar(tr("Iconos"));
    toolbarIconos->addAction(QIcon(":/preferences-system.png"),"Opciones",this,SLOT( abrirOpciones() ) );
    toolbarIconos->addAction(QIcon(":/terminal.png"),"Debug",this,SLOT(abrirDebug()));

    opciones = new opcionesGlobales();
    numero = opciones->NumeroDestino();
    puerto = opciones->Puerto();
    pin = opciones->Pin();
    centro = opciones->CentroMensajes();
    fixok = opciones->Fixok();
    
    ips = new QStringList( opciones->Ips() );

    file = new QFile( opciones->fileLog() );
    file->open( QIODevice::WriteOnly | QIODevice::Text );
    flog = new QTextStream( file );
    
    mensaje = new smsat(centro,puerto,flog,pin,fixok);
    crearAcciones();
    crearBandeja();
//    testPuerto( mensaje );
    Server *tcp = new Server(ips,this);
    connect(tcp,SIGNAL(sendMessage(QString)),this,SLOT(sendMessage(QString)));
    connect(mensaje,SIGNAL(sendAt(QString)),this,SLOT(sendedMessage(QString)));
    connect(mensaje,SIGNAL(displayMsg(QString)),this,SLOT(showPopup(QString)));
}

menix::~menix() {
    delete m_ui;
    delete flog;
    file->close();
}

void menix::abrirOpciones(){
    menixPrefs *preferencias = new menixPrefs(opciones,this);
    preferencias->show();
}

void menix::abrirDebug(){
    menixDebug *debug = new menixDebug(opciones->fileLog(),this);
    debug->show();
}

/*
void menix::testPuerto( smsat *mensaje ) {
   QColor color;
   if ( !mensaje->testPort() ) {
      color = Qt::red;
   } else {
      color = Qt::green;
   }

   QPalette palette( m_ui->lineEditPuerto->palette() );
   palette.setColor( QPalette::Base, color );
   m_ui->lineEditPuerto->setPalette(palette);
}

void menix::setNumero ( ) {
   numero = m_ui->lineEditNumero->text();
   delete mensaje;
   mensaje = new smsat(centro,puerto);
   connect(mensaje,SIGNAL(sendAt(QString)),this,SLOT(sendedMessage(QString)));
}

void menix::setPuerto ( ){
   puerto = m_ui->lineEditPuerto->text();
   delete mensaje;
   mensaje = new smsat("+34607003110",puerto);
   testPuerto( mensaje );
   connect(mensaje,SIGNAL(sendAt(QString)),this,SLOT(sendedMessage(QString)));
}*/

void menix::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void menix::on_pushButton_clicked(){
   if ( mensaje ) {
      mensaje->encolaSms(numero,m_ui->lineEdit->text() );
      m_ui->lcdCola->display(m_ui->lcdCola->intValue()+1);
   }
}

void menix::sendMessage( const QString& message){
   if ( mensaje ) {
      mensaje->encolaSms( numero,message );
      m_ui->lcdCola->display(m_ui->lcdCola->intValue()+1);
   }
}

void menix::showPopup( const QString& message){
  QMessageBox::critical(0, tr("Error"),
                              tr("%1.").arg(message));
}

void menix::sendedMessage( const QString& message){
   m_ui->textEdit->append(message);
   m_ui->lcdCola->display(m_ui->lcdCola->intValue()-1);
}

void menix::crearBandeja() {
    icon = new QIcon ( ":/menix.png" );
    trayIconMenu = new QMenu ( this );
    trayIconMenu->addAction ( restoreAction );
    trayIconMenu->addSeparator();
    trayIconMenu->addAction ( quitAction );

    trayIcon = new QSystemTrayIcon ( this );
    trayIcon->setContextMenu ( trayIconMenu );
    trayIcon->setIcon ( *icon );
    trayIcon->show();
    restoreAction->setEnabled ( false );

    connect ( trayIcon, SIGNAL ( messageClicked() ),
              this, SLOT ( show() ) );
    connect ( trayIcon, SIGNAL ( activated ( QSystemTrayIcon::ActivationReason ) ),
              this, SLOT ( accionesBandeja( QSystemTrayIcon::ActivationReason ) ) );
}

void menix::accionesBandeja( QSystemTrayIcon::ActivationReason reason ) {
    switch ( reason ) {
    case QSystemTrayIcon::Trigger:
        if ( isHidden() ) {
            restore();
        } else {
            hide();
            restoreAction->setEnabled ( true );
        }
    }
}


void menix::crearAcciones() {
    restoreAction = new QAction ( QIcon ( ":/top.png" ), tr ( "&Restaurar" ), this );
    restoreAction->setShortcut ( QKeySequence ( Qt::CTRL + Qt::Key_Enter ) );
    connect ( restoreAction, SIGNAL ( triggered() ), this, SLOT ( restore() ) );

    quitAction = new QAction ( QIcon ( ":/exit.png" ), tr ( "&Salir" ), this );
    connect ( quitAction, SIGNAL ( triggered() ), qApp, SLOT ( quit() ) );

}

void menix::restore() {
    this->show();
    restoreAction->setEnabled ( false );
}

void menix::closeEvent ( QCloseEvent *event ) {
    if ( trayIcon->isVisible() ) {
        hide();
        restoreAction->setEnabled ( true );
        event->ignore();
    }
}

