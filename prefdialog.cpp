#include "prefdialog.h"
#include "ui_prefdialog.h"
#include "mainwindow.h"
#include <QtWidgets> // Need this so we can use the restore defaults button.

PrefDialog::PrefDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrefDialog)
{
	ui->setupUi(this);

	setWindowTitle("Preferences");

	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(savePrefs()));
	connect(ui->buttonBox->button(QDialogButtonBox::RestoreDefaults), SIGNAL(clicked()),
	        this, SLOT(restoreDefaults()));

	// Let's grab the prefs, if they're valid (which should be always).
	if (qobject_cast<MainWindow*>(parent)->prefs() != nullptr) {
		_prefs = qobject_cast<MainWindow*>(parent)->prefs();

		// Populate the values in the widgets.
		ui->md1SpinBox->setValue(_prefs->MDWeight(0));
		ui->md2SpinBox->setValue(_prefs->MDWeight(1));
		ui->md3SpinBox->setValue(_prefs->MDWeight(2));
		ui->multiClickSpinBox->setValue(_prefs->multiClickWeight());

		// If you want to do a >10min freestyle...tough.
		ui->totalTimeSpinBox->setMaximum(600);
		// If you want to do a <1sec freestyle...tough.
		ui->totalTimeSpinBox->setMinimum(1);
		// If you want to do a freestyle with a non-integer length in seconds...tough.
		ui->totalTimeSpinBox->setValue(_prefs->totalTimeSetting() / 1000);
	}
	else {
//		qDebug() << "Error: PrefDialog: Parent invalid, preferences aborting!";
	}
}

PrefDialog::~PrefDialog()
{
	delete ui;
}

/**
 * @brief Copy the settings back once we're done making changes.
 */
void PrefDialog::savePrefs()
{
	if (_prefs)
	{
		// Copy back the dialog contents.
		_prefs->setMDWeight(0, ui->md1SpinBox->value());
		_prefs->setMDWeight(1, ui->md2SpinBox->value());
		_prefs->setMDWeight(2, ui->md3SpinBox->value());
		_prefs->setMultiClickWeight(ui->multiClickSpinBox->value());
		_prefs->setTotalTimeSetting(ui->totalTimeSpinBox->value() * 1000);

		// Save them to disk.
		_prefs->writePrefs();
	}
	else {
//		qDebug() << "Error: PrefDialog::savePrefs(): No prefs found!?";
	}
}

/**
 * @brief Repopulate the fields with default values, which the user can then save or discard.
 */
void PrefDialog::restoreDefaults()
{
	ui->md1SpinBox->setValue(DEFAULT_MD_LV1_WEIGHT);
	ui->md2SpinBox->setValue(DEFAULT_MD_LV2_WEIGHT);
	ui->md3SpinBox->setValue(DEFAULT_MD_LV3_WEIGHT);
	ui->multiClickSpinBox->setValue(DEFAULT_MULTICLICK_WEIGHT);
	ui->totalTimeSpinBox->setValue(DEFAULT_FREESTYLE_LENGTH / 1000);
	// todo: Other settings? ie what if something else gets corrupted?
}
