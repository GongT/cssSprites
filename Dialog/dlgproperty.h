#ifndef DLGPROPERTY_H
#define DLGPROPERTY_H

#include <QDialog>
#include "ui_dlgproperty.h"
#include <QDebug>
#include "main.h"

namespace Ui {
	class DlgProperty;
}

class DlgProperty : public QDialog
{
	Q_OBJECT
	public:
	explicit DlgProperty(QWidget *parent = 0);
	~DlgProperty();
	void show();
	
	private:
	
	signals:
	void Apply();
	
	protected:
	void changeEvent(QEvent *);
	void ResumeUI();
	
	private slots:
	void onAnyChange();
	
	private:
	Ui::DlgProperty *ui;
	
#define T_PROPERTY( _TYPE_ , _TER_ )\
	inline const _TYPE_& get##_TER_()const{return m##_TER_;} \
	inline void set##_TER_(const _TYPE_ data){/*qDebug()<<"Set "#_TER_;*/m##_TER_ = data; emit _TER_##Change(data);} \
		inline void def##_TER_(){set##_TER_(m##_TER_##_default);} \
	private: _TYPE_ m##_TER_ , m##_TER_##_default;
	
	signals: void ScreenHeightChange(int);
	private slots: void onScreenHeightChange(int);
	public: T_PROPERTY( int , ScreenHeight )
	Q_PROPERTY( int  screenheight READ getScreenHeight WRITE setScreenHeight RESET defScreenHeight NOTIFY ScreenHeightChange )
	
	signals: void ScreenWidthChange(int);
	private slots: void onScreenWidthChange(int);
	public: T_PROPERTY( int , ScreenWidth )
	Q_PROPERTY( int  screenwidth READ getScreenWidth WRITE setScreenWidth RESET defScreenWidth NOTIFY ScreenWidthChange )
	
	signals: void URLChange(QString);
	private slots: void onURLChange(const QString&);
	public: T_PROPERTY( QString , URL )
	Q_PROPERTY( QString  url READ getURL WRITE setURL RESET defURL NOTIFY URLChange )
	
	signals: void TemplateChange(QString);
	private slots: void onTemplateChange(const QString &);
	public: T_PROPERTY( QString , Template )
	Q_PROPERTY( QString  template READ getTemplate WRITE setTemplate RESET defTemplate NOTIFY TemplateChange )
	
	signals: void NameSpaceChange(QString);
	private slots: void onNameSpaceChange(const QString &);
	public: T_PROPERTY( QString , NameSpace )
	Q_PROPERTY( QString  namespace READ getNameSpace WRITE setNameSpace RESET defNameSpace NOTIFY NameSpaceChange )
	
	signals: void BgSelectorChange(QString);
	private slots: void onBgSelectorChange(const QString&);
	public: T_PROPERTY( QString , BgSelector )
	Q_PROPERTY( QString  bgselector READ getBgSelector WRITE setBgSelector RESET defBgSelector NOTIFY BgSelectorChange )
	
	signals: void GenerateBgImageChange(bool);
	private slots: void onGenerateBgImageChange(bool);
	public: T_PROPERTY( bool , GenerateBgImage )
	Q_PROPERTY( bool  generatebgimage READ getGenerateBgImage WRITE setGenerateBgImage RESET defGenerateBgImage NOTIFY GenerateBgImageChange )
	
	private slots:
	void on_btnsMain_clicked(QAbstractButton *button);
	
};

#endif // DLGPROPERTY_H
