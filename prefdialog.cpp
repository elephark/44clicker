#include "prefdialog.h"
#include "ui_prefdialog.h"

#include "mainwindow.h"

#include <QDebug>

PrefDialog::PrefDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrefDialog)
{
	ui->setupUi(this);

//	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
//	connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));


	ClickPrefs *p;

	// We'll work with a copy of the prefs.
	if (dynamic_cast<MainWindow*>(parent)->prefs()) {
		// Note: Don't try to use qobject_cast, since MainWindow is a grandchild class of QObject.
		p = dynamic_cast<MainWindow*>(parent)->prefs();
		_tmpPrefs = new ClickPrefs;

		_tmpPrefs->setMultiClickWeight(p->multiClickWeight());
		_tmpPrefs->setTotalTimeSetting(p->totalTimeSetting());
		_tmpPrefs->setTimerDisplayRefresh(p->timerDisplayRefresh());
		_tmpPrefs->setMDWeight(0, p->MDWeight(0));
		_tmpPrefs->setMDWeight(1, p->MDWeight(1));
		_tmpPrefs->setMDWeight(2, p->MDWeight(2));

		qDebug() << "_tmpPrefs->totalTimeSetting =" << _tmpPrefs->totalTimeSetting();
		qDebug() << "p->totalTimeSetting =" << p->totalTimeSetting();

	}
	else {
		qDebug() << "Error: PrefDialog: Parent invalid, preferences aborting!";
	}


	qDebug() << "Parent of PrefDialog is" << parent;


	qDebug() << "PrefDialog::PrefDialog()";
}

PrefDialog::~PrefDialog()
{
	delete ui;
	qDebug() << "PrefDialog::~PrefDialog()";
}
