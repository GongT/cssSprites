#ifndef SIZEABLEIMAGEFRAME_H
#define SIZEABLEIMAGEFRAME_H

#include <QWidget>
#include <QMetaProperty>
#include <QImage>
#include <QIcon>
#include <QPixmap>
#include <QFile>
#include <QCryptographicHash>
#include "main.h"
#include <QBuffer>

class ImageFrame : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QString data READ getData WRITE setData )
	Q_PROPERTY(QVariant buffer READ getBuffer WRITE setBuffer )
	Q_PROPERTY(QIcon icon READ getIcon )
	Q_PROPERTY(QString type READ getType CONSTANT )
	
	public:
	explicit ImageFrame(QWidget *parent = 0);
	~ImageFrame();
	
	private:
	QImage img;
	QString mData;
	
	inline QString getData()const{
		return mData;
	}
	inline void setData(QString d){
		QFile file(d);
		if(file.exists()){
			d.replace('\\','/');
			mData = d.section('/',-1);
			file.copy(mData);
			img.load(mData);
			icon.addPixmap(QPixmap::fromImage(img));
			this->repaint();
		}else{
			qWarning()<<"File Not Found :"<<d;
		}
	}
	
	inline QVariant getBuffer()const{
		return QVariant();
	}
	inline void setBuffer(QVariant d){
		img = qvariant_cast<QImage>(d);
		icon.addPixmap(QPixmap::fromImage(img));
		
		QByteArray bits;
		QBuffer buffer(&bits);
		img.save(&buffer,"PNG");
		QCryptographicHash hasher(QCryptographicHash::Md5);
		hasher.addData(bits);
		mData = hasher.result().toHex() + ".png";
		qDebug()<<(mData)<<
				  img.save(mData,"PNG");
		this->repaint();
	}
	
	inline const QIcon& getIcon()const{
		return icon;
	}//no setter
	
	inline QString getType()const{
		return mType;
	}
	
	protected:
	const QString mType;
	void paintEvent(QPaintEvent *);
	QIcon icon;
};

#endif // SIZEABLEIMAGEFRAME_H
