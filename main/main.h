#ifndef MAIN_H
#define MAIN_H

#include <QMetaProperty>
#include <QDir>

#define COLLISION(point,rect) (rect.left() < point.x() && rect.right() > point.x() && rect.top() < point.y() && rect.bottom() > point.y()) 

#define UTF8 QString::fromUtf8

#define TEMP_FOLDER "/css-temp/"
#define STUFF_FOLDER ".files/"

void* CreateObject(const char* type);
void* CreateObject(const QString& type);

inline QString singleLine(QString str){
	str.replace('\\',"\\\\");
	str.replace('/',"\\/");
	str.replace('\n',"\\n");
	return str;
}

inline QString multiLine(QString str){
	str.replace(QString("\\n"),"\n");
	str.replace(QString("\\/"),"/");
	str.replace(QString("\\\\"),"\\");
	return str;
}

#include <QRect>
#include <QTextStream>
inline QString SerializationRect(const QRect& ret){
	return QString("(+%1+%2,%3x%4)").arg(ret.x()).arg(ret.y()).arg(ret.width()).arg(ret.height());
}


inline QRect UnSerializationRect(QString str){
	QTextStream read(&str);
	QRect ret;
	int num;
	
	read.read(2);
	read>>num;
	ret.setX(num);
	
	read.read(1);
	read>>num;
	ret.setY(num);
	
	
	read.read(1);
	read>>num;
	ret.setWidth(num);
	
	read.read(1);
	read>>num;
	ret.setHeight(num);
	
	return ret;
}

#include "alertexception.h"

#endif // MAIN_H
