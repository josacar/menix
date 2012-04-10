#include "menixprefs.h"
#include "ui_menixprefs.h"
#include <QInputDialog>

menixPrefs::menixPrefs ( opcionesGlobales *opciones,QWidget *parent ) :
        QDialog ( parent ),
        m_ui ( new Ui::menixPrefs ) {
    m_ui->setupUi ( this );
    _opciones = opciones;

    // Primer panel
    m_ui->lineEditNumero->setText ( opciones->NumeroDestino() );
    m_ui->lineEditCentro->setText ( opciones->CentroMensajes() );
    m_ui->lineEditPuerto->setText ( opciones->Puerto() );
    m_ui->checkBoxFixok->setChecked ( opciones->Fixok() );
    m_ui->lineEditPin->setText ( opciones->Pin() );

    // Segundo panel
    QStringList *lista = new QStringList ( opciones->Ips() );
    for ( int i=0;i< lista->size();++i ) {
        m_ui->listWidgetIps->addItem ( lista->at ( i ) );
    }

    connect ( m_ui->buttonBox,SIGNAL ( accepted() ),this,SLOT ( updateValues() ) );

    connect ( m_ui->pushButtonAdd,SIGNAL ( clicked ( bool ) ),this,SLOT ( addIp ( bool ) ) );
    connect ( m_ui->pushButtonDel,SIGNAL ( clicked ( bool ) ),this,SLOT ( delIp ( bool ) ) );

    connect ( m_ui->listWidgetPanel,
              SIGNAL ( currentItemChanged ( QListWidgetItem *, QListWidgetItem * ) ),
              this, SLOT ( changePage ( QListWidgetItem *, QListWidgetItem* ) ) );
}

menixPrefs::~menixPrefs() {
    delete m_ui;
}

void menixPrefs::changeEvent ( QEvent *e ) {
    QWidget::changeEvent ( e );
    switch ( e->type() ) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi ( this );
        break;
    default:
        break;
    }
}

void menixPrefs::addIp ( bool checked ) {
    bool ok;
    QString text = QInputDialog::getText ( this, "Añadir IP",
                                           "IP:", QLineEdit::Normal,
                                           "", &ok );
    if ( ok && !text.isEmpty() )
        m_ui->listWidgetIps->addItem ( text );
}

void menixPrefs::delIp ( bool checked ) {
    QListWidget *lista = m_ui->listWidgetIps;
    QList<QListWidgetItem *> *items = new QList<QListWidgetItem *> ( lista->selectedItems() );
    if ( !items->empty() )
        for ( int i=0;i<items->size();++i )
            lista->removeItemWidget ( items->at ( i ) );
}

void menixPrefs::changePage ( QListWidgetItem *current, QListWidgetItem *previous ) {
    if ( !current )
        current = previous;

    m_ui->stackedWidgetOpciones->setCurrentIndex ( m_ui->listWidgetPanel->row ( current ) );
}

void menixPrefs::updateValues() {
    // Panel 1
    _opciones->setCentroMensajes ( m_ui->lineEditCentro->text() );
    _opciones->setNumeroDestino ( m_ui->lineEditNumero->text() );
    _opciones->setPuerto ( m_ui->lineEditPuerto->text() );
    _opciones->setFixok ( m_ui->checkBoxFixok->isChecked() );
    _opciones->setPin( m_ui->lineEditPin->text() );
    // Panel 2
    QStringList *lista = new QStringList();
    for ( int i=0;i< m_ui->listWidgetIps->count();++i ) {
        lista->append ( m_ui->listWidgetIps->item ( i )->text() );
    }
    _opciones->setIps ( *lista );
    _opciones->writeSettings();
    deleteLater();
}
