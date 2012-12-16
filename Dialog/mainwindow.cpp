#include "mainwindow.h"
#include "windowmananger.h"
#include "abstractwindow.h"
#include <QDebug>
#include <QTextStream>

#include "ui_mainwindow.h"
#include "dlgproperty.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	wm(new WindowMananger(this)),
	CurrentSetting(new DlgProperty(this))
{
	ui->setupUi(this);
	
	this->connect(wm,SIGNAL(WindowCreate(int)),SLOT(WndCreate(int)));
	this->connect(wm,SIGNAL(WindowChange(int)),SLOT(WndChange(int)));
	this->connect(wm,SIGNAL(WindowClose(int)),SLOT(WndClose(int)));
	this->connect(wm,SIGNAL(WindowSelect(int)),SLOT(WndSelect(int)));
	
	wm->setScreen(ui->frmMain);
	wm->connect(ui->lstObjects,SIGNAL(currentRowChanged(int)),SLOT(setCurrentWindow(int)));
	
	ui->frmMain->setBackgroundRole(QPalette::Base);
	ui->scrollArea->setBackgroundRole(QPalette::Dark);
	
#define CONNECT(NAME) this->connect(CurrentSetting,SIGNAL(NAME) ,SLOT(on##NAME) )
	CONNECT( ScreenHeightChange(int) );
	CONNECT( ScreenWidthChange(int) );
	CurrentSetting->defScreenHeight();
	CurrentSetting->defScreenWidth();
	
}

void MainWindow::setStatusTip(const QString& tip){
	ui->lblStatus->showMessage(tip);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::ChangeInput(const AbstractWindow*const ptr){
	ui->txtData->setText(ptr->getData());
	ui->txtID->setText(ptr->getID());
	ui->txtName->setText(ptr->getName());
	QRect rect = ptr->getRect();
	ui->txtLeft->setValue(rect.x());
	ui->txtTop->setValue(rect.y());
	ui->txtWidth->setValue(rect.width());
	ui->txtHeight->setValue(rect.height());
}

void MainWindow::WndCreate(int index){
	AbstractWindow* wnd_ptr = wm->getWindow(index);
	QIcon icon(qvariant_cast<QIcon>(wnd_ptr->property("icon")));
	
	//添加菜单项目
	QAction* btn = new QAction(ui->mnuObjects);
	btn->setProperty("action", QVariant(1000+index));
	btn->setText(wnd_ptr->getTitle());
	btn->setIcon(icon);
	btn->setCheckable(true);
	btn->setIconVisibleInMenu(true);	
	ui->mnuObjects->addAction(btn);
	
	//添加列表项目
	QListWidgetItem* new_item = new QListWidgetItem(wnd_ptr->getTitle());
	new_item->setIcon(icon);
	ui->lstObjects->addItem(new_item);
	
}

void MainWindow::WndChange(int index){
	AbstractWindow* wnd_ptr = wm->getWindow(index);
	//文本框
	ChangeInput(wnd_ptr);
	QIcon icon = wnd_ptr->getIcon();
	QString title = wnd_ptr->getTitle();
	
	//列表
	QListWidgetItem*row = ui->lstObjects->item(index);
	row->setIcon(icon);
	row->setText(title);
	
	//菜单
	QAction* btn = ui->mnuObjects->actions().value(index+2);
	btn->setIcon(icon);
	btn->setText(title);
}

void MainWindow::WndClose(int index){
	qDebug()<<"MainWindow::WndClose Cleanup: "<<index;
	delete ui->mnuObjects->actions().value(index+2);
	delete ui->lstObjects->takeItem(index);
}

void MainWindow::WndSelect(int index){
	static QAction* btn = NULL;
	if(btn)btn->setChecked(false);
	btn = ui->mnuObjects->actions().value(index);
	btn->setChecked(true);
	ui->lstObjects->setCurrentRow(index);
	
	ChangeInput(wm->CurrentWindow());
}

void MainWindow::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void MainWindow::on_btnOk_pressed(){
	AbstractWindow* wnd_ptr = wm->CurrentWindow();
	wnd_ptr->blockSignals(true);
	wnd_ptr->setData(ui->txtData->text());
	wnd_ptr->setID(ui->txtID->text());
	wnd_ptr->setName(ui->txtName->text());
	wnd_ptr->setRect( QRect( ui->txtLeft->value(),ui->txtTop->value() ,
						 ui->txtWidth->value(),ui->txtHeight->value() ));
	wnd_ptr->blockSignals(false);
}

void MainWindow::on_btnClose_pressed(){
    wm->CloseWindow();
}

void MainWindow::closeEvent(QCloseEvent *e){
	//测试是否保存
	e->accept();
}

void MainWindow::onScreenHeightChange(int h){
	qDebug()<<"Height change "<<h;
	ui->frmMain->resize(ui->frmMain->width(),h);
}

void MainWindow::onScreenWidthChange(int w){
	qDebug()<<"Width change "<<w;
	ui->frmMain->resize(w,ui->frmMain->height());
}
