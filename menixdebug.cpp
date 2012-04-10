#include "menixdebug.h"
#include "ui_menixdebug.h"
#include <QFile>
#include <QTextStream>

menixDebug::menixDebug(QString log,QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::menixDebug)
{
    m_ui->setupUi(this);
    fichero = log;
    verLog( m_ui->textEdit );
}

menixDebug::~menixDebug()
{
    delete m_ui;
}

void menixDebug::changeEvent(QEvent *e)
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

void menixDebug::verLog(QTextEdit *qte) {
	QFile *flog = new QFile( fichero );
	if ( flog->open( QIODevice::ReadOnly | QIODevice::Text ) ) {
		QTextStream in( flog );

		while ( !in.atEnd() )
			qte->append( in.readLine() );

		flog->close();
	}
}