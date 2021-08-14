#include "prefdialog.h"
#include "ui_prefdialog.h"

#include "mainwindow.h"

#include <QDebug>

PrefDialog::PrefDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrefDialog)
{
	ui->setupUi(this);

	setWindowTitle("Preferences");

	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(savePrefs()));
//	connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));


//	ClickPrefs *p;

	// We'll work with a copy of the prefs.
	// todo: We may not need _tmpPrefs after all.
	if (dynamic_cast<MainWindow*>(parent)->prefs() != nullptr) {
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

		// Populate the values in the widgets.
		ui->md1SpinBox->setValue(_tmpPrefs->MDWeight(0));
		ui->md2SpinBox->setValue(_tmpPrefs->MDWeight(1));
		ui->md3SpinBox->setValue(_tmpPrefs->MDWeight(2));
		ui->multiClickSpinBox->setValue(_tmpPrefs->multiClickWeight());

		// If you want to do a >10min freestyle...tough.
		ui->totalTimeSpinBox->setMaximum(600);
		// If you want to do a freestyle with a non-integer length in seconds...tough.
		ui->totalTimeSpinBox->setValue(_tmpPrefs->totalTimeSetting() / 1000);
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

/**
 * @brief Copy the settings back once we're done making changes.
 */
void PrefDialog::savePrefs()
{
	if (p)
	{
		// Copy back the dialog contents.
		p->setMDWeight(0, ui->md1SpinBox->value());
		p->setMDWeight(1, ui->md2SpinBox->value());
		p->setMDWeight(2, ui->md3SpinBox->value());
		p->setMultiClickWeight(ui->multiClickSpinBox->value());
		p->setTotalTimeSetting(ui->totalTimeSpinBox->value() * 1000);

		qDebug() << "PrefDialog::savePrefs(): Saved prefs";
	}
	else {
		qDebug() << "Error: PrefDialog::savePrefs(): No prefs found!?";
	}
}
