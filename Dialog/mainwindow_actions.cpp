#include "ui_mainwindow.h"
#include <QAction>
#include <QFileDialog>
#include <QUrl>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include "windowmananger.h"
#include "imageframe.h"
#include "abstractwindow.h"
#include "mainwindow.h"
#include "dlgproperty.h"

const QString StartIndent("/*@css/sprites");
const QString EndIndent("@end sprites@*/");
const QString AutoGenIndent("/*AUTOGEN-ID_START DO NOT REMOVE*/");
const QString AutoGenEndIndent("/*AUTOGEN-ID_END*/");
const QString CharsetIndent("@CHARSET ");

void MainWindow::onMenuClicked(QAction* e){
	int action = e->property("action").toInt();
	switch (action) {
	case 0://打开
	try{
		QString openPath = QFileDialog::getOpenFileName(  this,UTF8("打开文件"),QString(),QString("*.css")   );
		if(openPath.length()) OpenFile(openPath);
	}catch(AlertException e){
		e.Alert();
	}
		break;
	case 1://保存
		if( CurrentFilePath.length() ){
			SaveFile(CurrentFilePath);
			break;
		}
		//no break;
	case 2://另存为
	{	QString savePath = QFileDialog::getSaveFileName(  this,UTF8("打开文件"),QString(),QString("*.css")   );
		if(savePath.length()){
			SaveFile(savePath);
		}
	}	break;
	case 3://退出
		this->close();
		break;
	case 100://添加图形
	{	QWidget* wnd = static_cast<QWidget*>(CreateObject("Image"));
		if(!wnd) throw AlertException("程序错误","无法实例化Image");
		wm->CreateWindow(wnd);
	}	break;
	case 101://添加区域
	{	QWidget* wnd = static_cast<QWidget*>(CreateObject("Area"));
		if(!wnd) throw AlertException("程序错误","无法实例化Area");
		wm->CreateWindow(wnd);
	}	break;
	case 200://图像设定
		CurrentSetting->show();
		break;
	case 201://导出PNG
	{
		QImage image = wm->Screenshot();
		
		QString savePath = QFileDialog::getSaveFileName(  this,UTF8("导出图片"),QString(),QString("*.png")   );
		if(savePath.length()){
			qDebug()<<"SaveTo"<<savePath;
			image.save(savePath);
		}
	}
		break;
	case 203://导出CSS到自身
		if( CurrentFilePath.length() ){
			ExportCss(CurrentFilePath);
			break;
		}
		// NO break;
	case 202://导出CSS
	{	QString exportPath = QFileDialog::getSaveFileName(  this,UTF8("打开文件"),QString(),QString("*.css")   );
		if(exportPath.length()){
			ExportCss(exportPath);
		}
	}	break;
	case 300://打开右边栏
	{	static bool isShow = false;
		if(isShow){
			ui->frmProperty->show();
		}else{
			ui->frmProperty->hide();
		}
		isShow = !isShow;
	}	break;
	default:
		if(action>=1000){
			wm->setCurrentWindow(action-1000);
		}else{
			qWarning()<<"Unknown Action "<<action;
		}
		break;
	}
}

void MainWindow::OpenFile(const QString& file){
	bool test_ok = false;
	
	QFile fp(file);
	test_ok = fp.exists();
	if(!test_ok) throw AlertException(UTF8("打开失败"),UTF8("文件不存在:\n")+file);
	test_ok = fp.open(QFile::ReadOnly);
	if(!test_ok) throw AlertException(UTF8("打开失败"),UTF8("无法读取文件:\n")+file);
	
	QTextStream CurrentFile(&fp);
	//读取文件中的配置
	QString Line;
	QString OData;
	QTextStream ProjectData(&OData);
	bool switcher = false;
	while( !CurrentFile.atEnd() ){
		Line = CurrentFile.readLine();
		if( !switcher ){ //非 配置or导出 部分
			if(Line.startsWith(StartIndent) || Line.startsWith(AutoGenIndent)){
				switcher = true;
				continue;
			}
		}else{ //配置or导出 部分
			switcher = !(Line.startsWith(EndIndent) || Line.startsWith(AutoGenEndIndent));
			ProjectData<<Line<<endl;
		}
	}
	
	bool start = false;
	while(!ProjectData.atEnd()){
		QString Line = ProjectData.readLine();
		if( Line == "[GLOBAL]" ){
			start = true;
			continue;
		}
		if( Line == "[END]" ){
			start = false;
			continue;
		}
		if(!start) continue;
		
		Line = Line.trimmed();
		QString ID = Line.section(' ',0,0).toLower();
		ID.truncate(ID.length()-1);
		QString Value = multiLine( Line.section(' ',1) );
		
		CurrentSetting->setProperty(ID.toStdString().data(),Value);
		
	}
	
	if(!test_ok) throw AlertException(UTF8("打开失败"),UTF8("无法在文件中识别本程序的格式"));
	
	ProjectData.reset();
	wm->restart();
	
	//移动当前目录
	QString stuff_dir = file + STUFF_FOLDER;
	QDir stuff(stuff_dir);
	if( !stuff.exists() ){
		stuff.mkdir(stuff_dir);
	}
	stuff.setCurrent(stuff_dir);
	
	qDebug()<<"[OPEN FILE]Current dir is "<<QDir::currentPath();
	
	wm->SessionResume(OData);
	
	CurrentFilePath = file;
	this->setWindowTitle(file+QString(" - cssSprites"));
}

void MainWindow::SaveFile(const QString& file){
	QFile fp(file);
	fp.open(QFile::ReadOnly);
	QTextStream CurrentFile(&fp);
	QString OData;
	QTextStream SaveingFile(&OData);
	SaveingFile.setCodec("UTF-8");
	
	SaveingFile << StartIndent <<endl;
	
	SaveingFile <<endl;
	SaveingFile<< "[GLOBAL]" <<endl;
	SaveingFile<<"\tSCREENHEIGHT: "<<singleLine( CurrentSetting->property("screenheight").toString() )<<endl;
	SaveingFile<<"\tSCREENWIDTH: "<<singleLine( CurrentSetting->property("screenwidth").toString() )<<endl;
	SaveingFile<<"\tURL: "<<singleLine( CurrentSetting->property("url").toString() )<<endl;
	SaveingFile<<"\tTEMPLATE: "<<singleLine( CurrentSetting->property("template").toString() )<<endl;
	SaveingFile<<"\tNAMESPACE: "<<singleLine( CurrentSetting->property("namespace").toString() )<<endl;
	SaveingFile<<"\tBGSELECTOR: "<<singleLine( CurrentSetting->property("bgselector").toString() )<<endl;
	SaveingFile<<"\tGENERATEBGIMAGE: "<<singleLine( CurrentSetting->property("generatebgimage").toString() )<<endl;
	SaveingFile<<"[END]"<<endl;
	SaveingFile <<endl;
	
	SaveingFile << wm->SessionSave();
	SaveingFile << endl << EndIndent <<endl;
	
	//读取文件中的其他部分
	QString Line;
	bool switcher = false;
	while( !CurrentFile.atEnd() ){
		Line = CurrentFile.readLine();
		if( !switcher ){ //非 配置or导出 部分
			if(Line.startsWith(StartIndent) || Line.startsWith(AutoGenIndent)){
				switcher = true;
				continue;
			}
			SaveingFile<<Line<<endl;
		}else{ //配置or导出 部分
			switcher = !(Line.startsWith(EndIndent) || Line.startsWith(AutoGenEndIndent));
		}
	}
	
	fp.close();
	if(!fp.open(QFile::WriteOnly)) throw AlertException(UTF8("保存失败"),UTF8("无法写入文件"));
	CurrentFile.reset();
	
	CurrentFilePath = file;
	this->setWindowTitle(file+QString(" - cssSprites"));
	
	qDebug()<<"File Save"<<file;
	CurrentFile<<OData;
	fp.close();
	
	//转移文件
	QDir fsd;
	QString target = fsd.absoluteFilePath(CurrentFilePath);
	target += STUFF_FOLDER;
	fsd.setPath(target);
	if( !fsd.exists() ){
		if(! fsd.mkdir(target) ) throw AlertException(UTF8("保存错误"),("目标路径无法写入（请检查权限）\n")+target);
	}
	QStringList stuff = QDir::current().entryList(QDir::Files);
	QString fName;
	foreach(fName,stuff){
		qWarning(QString("Move File ================\n\t\tfrom %1(%2)\n\t\t to %3").arg(fName).arg(QDir::currentPath()).arg(target+fName).toStdString().data());
		QFile::copy(fName,target+fName);
		//QFile::remove(fName);
	}
	//移动当前目录
	fsd.setCurrent(target);
	qDebug()<<"[SAVE FILE]Current dir is "<<QDir::currentPath();
}

void MainWindow::ExportCss(const QString& file){
	QFile fp(file);
	fp.open(QFile::ReadOnly);
	QTextStream CurrentFile(&fp);
	
	QString OData;//存放导出文件内容
	QTextStream SaveingFile(&OData);
	SaveingFile.setCodec("UTF-8");
	SaveingFile<<CharsetIndent<<"\"UTF-8\""<<endl;
	
	QString Line;
	bool switcher = false;
	while( !CurrentFile.atEnd() ){
		Line = CurrentFile.readLine();
		if( !switcher ){ //非 配置or导出 部分
			if(Line.startsWith(AutoGenIndent)){
				switcher = true;
				continue;
			}
			if(Line.startsWith(CharsetIndent)) continue;//逃过字符集声明
			SaveingFile<<Line<<endl;
		}else{ //配置or导出 部分
			switcher = !Line.startsWith(AutoGenEndIndent);
		}
	}
	
	SaveingFile<<endl<<AutoGenIndent<<endl;
	//开始实际导出
	QString Template( CurrentSetting->getTemplate() );
	const QString NS(CurrentSetting->getNameSpace() );
	const QString URL(CurrentSetting->getURL() );
	const QString BgStyle = QString("\n\tdisplay: block;\n\tbackground-repeat: no-repeat;\n\tbackground-image:url( %1 );\n").arg(URL);
	if( CurrentSetting->getGenerateBgImage() ){
		SaveingFile<<NS<<' '<<CurrentSetting->getBgSelector()<<UTF8("{//背景图")<<BgStyle<<"}\n\n";
	}else{
		Template = Template.insert(Template.lastIndexOf('}')-1,BgStyle);
	}
	Template.replace("%ns",NS+' ');
	Template.replace("%i","%1"); //	%i - 元件选择器
	Template.replace("%r","%2");//	%r - 元件名称
	Template.replace("%t","%3");//	%t - 元件类型
	Template.replace("%x","%4");//	%x - X位置
	Template.replace("%y","%5");//	%y - Y位置
	Template.replace("%w","%6");//	%w - 宽度
	Template.replace("%h","%7");//	%h - 高度
	qDebug()<<Template;
	const QList<AbstractWindow*>& windows = wm->AllTask();
	AbstractWindow* wnd = NULL;
	foreach(wnd,windows){
		QRect temp = wnd->getRect();
		SaveingFile<<(Template
					  .arg(wnd->getID())
					  .arg(wnd->getName())
					  .arg(wnd->getType())
					  .arg(temp.x())
					  .arg(temp.y())
					  .arg(temp.width())
					  .arg(temp.height())
					  )<<endl;
	}
	SaveingFile<<endl<<AutoGenEndIndent<<endl;
	
	fp.close();
	if(!fp.open(QFile::WriteOnly)) throw AlertException(UTF8("保存失败"),UTF8("无法写入文件"));
	CurrentFile.reset();
	
	CurrentFile<<OData;
	fp.close();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e){
	const QMimeData* mime = e->mimeData();
	if( mime->hasImage() ){
		e->acceptProposedAction();
	}else if( mime->hasUrls() ){
		e->acceptProposedAction();
		
	}else{
		e->ignore();
	}
}

void MainWindow::dropEvent(QDropEvent *e){
	const QMimeData* mime = e->mimeData();
	if( mime->hasImage() ){
		QWidget* nWin = static_cast<QWidget*>(CreateObject("Image"));
		nWin->setProperty("buffer", mime->imageData() );
		wm->CreateWindow(nWin);
		e->acceptProposedAction();
	}else if( mime->hasUrls() ){
		QList<QUrl> urls = mime->urls();
		if( urls.length() == 1 && urls[0].toString().endsWith(".css") ){
			OpenFile(urls[0].toLocalFile());
			//测试保存
			return;
		}
		QUrl url;
		QString error;
		for( int i=0 ; i<urls.length() ; i++ ){
			url  = urls.at(i);
			QImage temp;
			if(url.isLocalFile() && temp.load(url.toLocalFile())){
				QWidget* nWin = static_cast<QWidget*>(CreateObject("Image"));
				nWin->setProperty("data", url.toLocalFile() );
				wm->CreateWindow(nWin);
			}else{
				error += url.toString();
				error += '\n';
			}
		}
		if(error.length()){
			message(error,UTF8("以下文件协议不支持"));
		}
		e->acceptProposedAction();
	}else{
		e->ignore();
	}
}
