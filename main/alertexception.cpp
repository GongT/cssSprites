#include "main.h"
#include <QMessageBox>

AlertException::AlertException(const char* Title, const char* Message):
	mMessage(UTF8(Message)),
	mTitle(UTF8(Title))
{
	mMessage = UTF8(Message);
	mTitle = UTF8(Title);
}

AlertException::AlertException(const QString Title, const QString Message):
	mMessage(Message),
	mTitle(Title)
{
	mMessage = Message;
	mTitle = Title;
}

void AlertException::Alert()const{
	QMessageBox::critical(0,mTitle,mMessage,QMessageBox::Ok);
}


const char* AlertException::what() const throw(){
	Alert();
	QString ret(mTitle+':'+mMessage);
	return ret.toStdString().data();
}
