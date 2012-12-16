#include <QDebug>
#include <QPainter>
#include <QEvent>
#include "windowmananger.h"
#include "abstractwindow.h"



WindowMananger::WindowMananger(QObject *parent,QWidget *screen) :
	QObject(parent),
	mScreen(screen),
	current(NULL)
{
}

void WindowMananger::CreateWindow(QWidget *wnd , const QString*Name , const QString*Id , const QString*Data , const QRect*Rect  ){
	if(!wnd){
		throw AlertException("窗口管理器错误","NULL wnd ptr in WindowMananger::CreateWindow");
	}
	AbstractWindow *ptr = new AbstractWindow(this->screen(),wnd);
	ptr->setName( Name? *Name : QString::fromUtf8("匿名元件"));
	ptr->setID( Id? *Id : QString::fromUtf8("#Untitled"));
	ptr->setData( Data? *Data : "" );
	if(Rect){
		ptr->setRect(*Rect);
	}else{
		ptr->setPos(screen()->rect().topLeft()/2);
		ptr->setSize(QSize(100, 100));
	}
	ptr->show();
	wnd->show();
	
	//this->connect(ptr,SIGNAL(onClicked(AbstractWindow*)) , SLOT(onWindowClicked(AbstractWindow*)));
	this->connect(ptr,SIGNAL(onClose(AbstractWindow*)) , SLOT(onWindowClosed(AbstractWindow*)));
	this->connect(ptr,SIGNAL(onDataChange(AbstractWindow*)) , SLOT(onWindowChanged(AbstractWindow*)));
	this->connect(ptr,SIGNAL(onNameChange(AbstractWindow*)) , SLOT(onWindowChanged(AbstractWindow*)));
	this->connect(ptr,SIGNAL(onIDChange(AbstractWindow*)) , SLOT(onWindowChanged(AbstractWindow*)));
	this->connect(ptr,SIGNAL(onRectChange(AbstractWindow*)) , SLOT(onWindowChanged(AbstractWindow*)));
	this->connect(ptr,SIGNAL(onIconChange(AbstractWindow*)) , SLOT(onWindowChanged(AbstractWindow*)));
	
	this->connect(ptr,SIGNAL(onFocus(AbstractWindow*)) , SLOT(onWindowSelected(AbstractWindow*)));
	
	qDebug()<<"CreateWindow : "<<wnd<<','<<wnd->property("name")<<','<<wnd->property("id")<<','<<wnd->property("data")<<','<<wnd->property("rect");
	
	windows.push_back(ptr);
	WindowCreate(windows.length()-1);
	setCurrentWindow(ptr);
}

void WindowMananger::CloseWindow(int index){
	AbstractWindow* wnd = NULL;
	if(index<0){
		wnd = CurrentWindow();
	}else{
		wnd = windows.value(index);
	}
	qDebug()<<"CloseWindow : "<<index<<wnd;
	current = NULL;
	if(!wnd) return;
	wnd->close();
}

void WindowMananger::setCurrentWindow(int index){
	if( index != CurrentIndex() && index >= 0 && index < windows.size()){
		setCurrentWindow(windows.at(index));
	}
}

void WindowMananger::onWindowClicked(AbstractWindow*wnd){
	wnd->raise();
}

void WindowMananger::onWindowChanged(AbstractWindow*wnd){
	WindowChange(windows.indexOf(wnd));
}

void WindowMananger::onWindowClosed(AbstractWindow*wnd){
	int item = windows.indexOf(wnd);
	if(item > -1){
		qDebug()<<"Window Close: "<<wnd->getTitle()<<" (item : "<<wnd<<")";
		delete wnd->Content();
		delete wnd;
		windows.removeAt(item);
		emit WindowClose(item);
	}
}

void WindowMananger::onWindowSelected(AbstractWindow*wnd){
	if(wnd == current) return;
	if(current){
		current->setFocus(false);
	}
	current = wnd;
	WindowSelect(CurrentIndex());
}

void WindowMananger::setCurrentWindow(AbstractWindow* wnd){
	if(wnd == current) return;
	if(current){
		current->setFocus(false);
	}
	current = wnd;
	wnd->setFocus(true);
	WindowSelect(CurrentIndex());
}

QImage WindowMananger::Screenshot(){
	QImage   exportImage = QImage(this->screen()->size(), QImage::Format_ARGB32);
	exportImage.fill(QColor(0,0,0,0));   // fill transparent
	QPainter p;
	p.begin(&exportImage);
	
	for( int i=0 ; i<windows.length() ; i++ ){
		windows.at(i)->Draw(&exportImage);
	}
	p.end();
	return exportImage;
}

const QList<class AbstractWindow*>& WindowMananger::AllTask()const{
	return windows;
}

QString WindowMananger::SessionSave(){
	QString ret;
	QTextStream txt(&ret);
	AbstractWindow* wnd;
	foreach(wnd,windows){
		txt << "[BEGIN]" << endl;
		txt << "\tTYPE: " << wnd->property("type").toString() << endl;
		txt << "\tID: " <<wnd->property("id").toString() << endl;
		txt << "\tNAME: " << wnd->property("name").toString() << endl;
		txt << "\tDATA: " << wnd->property("data").toString() << endl;
		QRect tmpRect = wnd->getRect();
		txt << "\tRECT: " << SerializationRect(tmpRect) << endl;
		txt << "[END]" << endl;
	}
	return ret;
}

void WindowMananger::SessionResume(QString& data){
	QTextStream txt(&data);
	int start = false;
	
	struct ObjDef{
		QString type;
		QString id;
		QString name;
		QString data;
		QString rect;
	}obj;
	
	while(!txt.atEnd()){
		QString Line = txt.readLine();
		if( !start && Line == "[BEGIN]" ){
			start = true;
			continue;
		}
		if( start && Line == "[END]" ){
			start = false;
			QRect rect_def(UnSerializationRect(obj.rect));
			QWidget* wnd = static_cast<QWidget*>(CreateObject(obj.type));
			if(!wnd) throw AlertException("程序错误","无法实例化Area");
			CreateWindow(wnd , &obj.name , &obj.id , &obj.data , &rect_def);
			
			obj.type.clear();
			obj.id.clear();
			obj.name.clear();
			obj.data.clear();
			obj.rect.clear();
			continue;
		}
		if(!start) continue;
		
		Line = Line.trimmed();
		QString ID = Line.section(' ',0,0);
		
		if(ID == "TYPE:"){
			obj.type = Line.section(' ',1);
		}else if(ID == "ID:"){
			obj.id = Line.section(' ',1);
		}else if(ID == "NAME:"){
			obj.name = Line.section(' ',1);
		}else if(ID == "DATA:"){
			obj.data = Line.section(' ',1);
		}else if(ID == "RECT:"){
			obj.rect = Line.section(' ',1);
		}else{
			qDebug()<<"Unknown object define: "<<Line;
		}
	}
}

void WindowMananger::restart(){
	while(windows.length()){
		CloseWindow(0);
	}
}
