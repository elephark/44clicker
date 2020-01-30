#include "prefdialog.h"
#include "ui_prefdialog.h"

PrefDialog::PrefDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrefDialog)
{
	ui->setupUi(this);

//	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
//	connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

PrefDialog::~PrefDialog()
{
	delete ui;
}
