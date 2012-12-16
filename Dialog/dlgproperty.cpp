#include "dlgproperty.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>

#include <QMetaProperty>

//#include 

/*const QString keyNameSpace("g_ns");
const QString keyURL("g_url");
const QString keyBgSelector("g_bs");
const QString keyGenerateBgImage("g_ggi");
const QString keyTemplate("g_tpl");*/

DlgProperty::DlgProperty(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DlgProperty)
{
	ui->setupUi(this);
	ui->btnsMain->setFocus();
	
	mScreenHeight_default = ui->txtHeight->value();
	mScreenWidth_default = ui->txtWidth->value();
	mURL_default = ui->txtURL->text();
	mTemplate_default = ui->txtTemplate->toPlainText();
	mNameSpace_default = ui->txtNameSpace->text();
	mBgSelector_default = ui->txtBgSelector->text();
	mGenerateBgImage_default = ui->chkGenerateBgImage->isChecked();
	
	defBgSelector();
	defGenerateBgImage();
	defNameSpace();
	defTemplate();
	defURL();
	
	this->connect( ui->txtHeight , SIGNAL(valueChanged(int)) , SLOT(onAnyChange()) );
	this->connect( ui->txtWidth , SIGNAL(valueChanged(int)) , SLOT(onAnyChange()) );
	this->connect( ui->txtURL , SIGNAL(textChanged(QString)) , SLOT(onAnyChange()) );
	this->connect( ui->txtTemplate , SIGNAL(textChanged()) , SLOT(onAnyChange()) );
	this->connect( ui->txtNameSpace , SIGNAL(textChanged(QString)) , SLOT(onAnyChange()) );
	this->connect( ui->txtBgSelector , SIGNAL(textChanged(QString)) , SLOT(onAnyChange()) );
	this->connect( ui->chkGenerateBgImage , SIGNAL(toggled(bool)) , SLOT(onAnyChange()) );
}

void DlgProperty::ResumeUI(){
	ui->txtHeight->setValue(mScreenHeight);
	ui->txtWidth->setValue(mScreenWidth);
	ui->txtURL->setText(mURL);
	ui->txtTemplate->setPlainText(mTemplate);
	ui->txtNameSpace->setText(mNameSpace);
	ui->txtBgSelector->setText(mBgSelector);
	ui->chkGenerateBgImage->setChecked(mGenerateBgImage);
	
	QPushButton* btn = ui->btnsMain->button(QDialogButtonBox::Apply);
	btn->setEnabled(false);
}

DlgProperty::~DlgProperty()
{
	delete ui;
}

void DlgProperty::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void DlgProperty::show()
{
	ResumeUI();
	QDialog::show();
}

void DlgProperty::onAnyChange(){
	QPushButton* btn = ui->btnsMain->button(QDialogButtonBox::Apply);
	btn->setEnabled(true);
}

void DlgProperty::onGenerateBgImageChange(bool data){
	ui->chkGenerateBgImage->setChecked(data);
}

void DlgProperty::onBgSelectorChange(const QString& data){
	ui->txtBgSelector->setText(data);
}

void DlgProperty::onNameSpaceChange(const QString& data){
	ui->txtNameSpace->setText(data);
}

void DlgProperty::onTemplateChange(const QString& data){
	ui->txtTemplate->setPlainText(data);
}

void DlgProperty::onURLChange(const QString& data){
	ui->txtURL->setText(data);
}

void DlgProperty::onScreenWidthChange(int data){
	ui->txtWidth->setValue(data);
}

void DlgProperty::onScreenHeightChange(int data){
	ui->txtHeight->setValue(data);
}

void DlgProperty::on_btnsMain_clicked(QAbstractButton *button)
{
	switch( ui->btnsMain->standardButton(button) ){
	case QDialogButtonBox::Reset:
		qDebug()<<"QDialogButtonBox::Reset";
		ResumeUI();
		break;
	case QDialogButtonBox::RestoreDefaults:
		qDebug()<<"QDialogButtonBox::RestoreDefaults";
		defBgSelector();
		defGenerateBgImage();
		defNameSpace();
		defTemplate();
		defURL();
		ResumeUI();
		break;
	case QDialogButtonBox::Apply:
		qDebug()<<"QDialogButtonBox::Apply";
		setScreenHeight( ui->txtHeight->value() );
		setScreenWidth( ui->txtWidth->value() );
		setURL( ui->txtURL->text() );
		setTemplate( ui->txtTemplate->toPlainText() );
		setNameSpace( ui->txtNameSpace->text() );
		setBgSelector( ui->txtBgSelector->text() );
		setGenerateBgImage( ui->chkGenerateBgImage->isChecked() );
		break;
	case QDialogButtonBox::Close:
		qDebug()<<"QDialogButtonBox::Close";
		break;
	default:
		qDebug()<<"Unknown button : "<<button;
	}
}
