#include "mainwindow.h"
#include "main.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>

MainWindow* w;

class MyApplication : public QApplication{
	public:
	inline MyApplication(int &argc, char **argv):QApplication(argc,argv){}
	
	inline bool notify(QObject *a, QEvent *b){
		try{
			return QApplication::notify(a,b);
		}catch(AlertException e){
			e.Alert();
			return true;
		}catch(std::exception e){
			if( QMessageBox::Abort == QMessageBox::question(w,UTF8("发生未捕获的错误"),
														UTF8("忽略错误可能导致意外行为，可以尝试通过另存为保存当前工作。\n错误描述：")+e.what(),
														QMessageBox::Abort|QMessageBox::Retry,
														QMessageBox::Abort) ){
				exit(-1);
			}
			return false;
		}
	}
};

int main(int argc, char *argv[])
{
	MyApplication a(argc, argv);
	w = new MainWindow;
	w->show();
	
	QString cwd;
	//命令行打开文件
	if(argc > 1){
		QString file = QString(argv[1]);
		w->OpenFile( file );
		cwd = file;
	}else{
		cwd = QString(argv[0]);
		cwd.replace('\\','/');
		cwd = cwd.section('/',0,-2);
		cwd += TEMP_FOLDER;
		QDir c(cwd);
		if( !c.exists(cwd) ){
			c.mkdir(cwd);
		}
		qDebug()<<"CHDIR "<<QDir::setCurrent(cwd);
	}
	
	
	return a.exec();
}

void alert(const char *const msg , const char *const title){
	QMessageBox::critical(w,UTF8(title?title:"错误"),UTF8(msg),QMessageBox::Ok);
}

void message(const QString& msg ,const QString& title){
	QMessageBox::information(w,title.length()?title:UTF8("提示"),msg,QMessageBox::Ok);
}

#include "imageframe.h"
#include <QLabel>
void* CreateObject(const char*type){
	if(strcmp(type,"Image")==0)
		return new ImageFrame(0);
	if(strcmp(type,"Area")==0){
		QLabel* wnd = new QLabel(0);
		wnd->setProperty("type","Area");
		return wnd;
	}else{
		qDebug(type);
		return NULL;
	}
}

void* CreateObject(const QString& type){
	return CreateObject(type.toStdString().data());
}
