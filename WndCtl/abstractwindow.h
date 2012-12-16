#ifndef IITEMS_H
#define IITEMS_H
#include <QFrame>
#include "main.h"
#include <QIcon>

class AbstractWindow : public QWidget
{
	Q_OBJECT
	/* 单独Get/Set的方法 */
	Q_PROPERTY(QString id READ getID WRITE setID NOTIFY onIDChange)
	Q_PROPERTY(QString name READ getName WRITE setName NOTIFY onNameChange)
	Q_PROPERTY(QString data READ getData WRITE setData NOTIFY onDataChange)
	Q_PROPERTY(QString type READ getType STORED false)
	Q_PROPERTY(QIcon icon READ getIcon STORED false)
	Q_PROPERTY(QString title READ getTitle STORED false)
	Q_PROPERTY(QRect rect READ getRect WRITE setRect NOTIFY onRectChange STORED false)
	Q_PROPERTY(QSize size READ getSize WRITE setSize NOTIFY onRectChange STORED false)
	Q_PROPERTY(QPoint pos READ getPos WRITE setPos NOTIFY onRectChange STORED false)
	
	public:
	//getter
	inline QString getType()const{return mContent->property("type").toString();}
	inline QString getID()const{return mID;}
	inline QString getName()const{return mName;}
	inline QString getData()const{return mContent->property("data").toString();}
	inline QIcon getIcon()const{return qvariant_cast<QIcon>(mContent->property("icon"));}
	inline QString getTitle()const{ return '['+getType()+']'+getName()+'('+getID()+')'; }
	inline QRect getRect()const{return getContentRect();}
	inline QSize getSize()const{return getContentRect().size();}
	inline QPoint getPos()const{return getContentRect().topLeft();}
	//setter
	inline void setID(const QString&data){if(mID==data)return;mID=data;emit onIDChange(this);}
	inline void setName(const QString&data){if(mName==data)return;mName=data;emit onNameChange(this);}
	inline void setData(const QString&data){mContent->setProperty("data",data);emit onDataChange(this);}
	inline void setRect(const QRect&data){setSize(data.size());setPos(data.topLeft());}
	inline void setSize(const QSize&data){ResizeAbsolute(data);}
	inline void setPos(const QPoint&data){MoveAbsolute(data);}
	
	private: //property
	QString mID;
	QString mName;
	//属性定义结束
	
    public:
		AbstractWindow(QWidget *screen,QWidget *content);
        virtual ~AbstractWindow() {}
		
		/* Window Info */
		void ChangeInfo(const QString*,const QString*,const QString*,const QRect*);
		
		/* 内容元件 */
		QWidget* Content()const;
		QWidget* setContent(QWidget*);
		QRect getContentRect()const;
		
		public slots:
		/* 绘图 */
		void Draw(QPaintDevice *dev);
		/* 焦点 */
		void focusInEvent(QFocusEvent *);
		void focusOutEvent(QFocusEvent *);
		void setFocus(bool);
		inline bool isFocus(){return mFocus;}
		
		
		
	public slots:
		void MenuRequested();
		
	signals:
		void onOpen(AbstractWindow*);
		void onClose(AbstractWindow*);
		void onFocus(AbstractWindow*);
		void onClicked(AbstractWindow*);
		
		void onIDChange(AbstractWindow*);
		void onNameChange(AbstractWindow*);
		void onDataChange(AbstractWindow*);
		void onRectChange(AbstractWindow*);
		void onIconChange(AbstractWindow*);
		void onBorderChange(AbstractWindow*);
		
	protected:
		void paintEvent(QPaintEvent *);
		void mouseMoveEvent(QMouseEvent *);
		void mousePressEvent(QMouseEvent *);
		void mouseReleaseEvent(QMouseEvent *);
		void resizeEvent(QResizeEvent *);
		void moveEvent(QMoveEvent *);
		void closeEvent(QCloseEvent *);
		
	private:
		bool mFocus; //是否拥有焦点，用来决定是否绘制边框
		const QWidget*const mScreen; //窗口所在“屏幕”
		QWidget* mContent; //窗口内容
		QPoint TrackingPoint; //鼠标按下时的位置，用来计算位移
		bool isMouseDown;
		
		enum TrackingType{
			None = 0,
			LeftTop,Top,RightTop,
			Left,Center,Right,
			LeftBottom,Bottom,RightBottom
		}Tracking;//鼠标按下时，记录当前追踪方式
		void TestPosition(int x,int y);//测试鼠标处于哪个位置
		void MoveDispatch(const QPoint &p); //根据当前追踪的方式分配形状变化
		
		/* 窗口大小、位置函数 */
		void MoveRelative(const QPoint&);
		void MoveRelative(const int& x , const int& y);
		void MoveAbsolute(const QPoint&);
		void ResizeRelative(const QSize&);
		void ResizeRelative(const int& w , const int& h);
		void ResizeAbsolute(const QSize&);
		
		class QRect mTitleButton;//标题菜单按钮
		
	public:
		inline long getBorderTop()const{return mBorderTop;}
		inline long getBorderLeft()const{return mBorderLeft;}
		inline long getBorderRight()const{return mBorderRight;}
		inline long getBorderBottom()const{return mBorderBottom;}
		inline QSize getBorderSize()const{return QSize(mBorderLeft+mBorderRight,mBorderTop+mBorderBottom);}
		inline QPoint getBorderPos()const{return QPoint(mBorderLeft,mBorderTop);}
		inline void setBorderTop(int set){mBorderTop = set;mContent->move(getBorderPos());emit onRectChange(this);}
		inline void setBorderLeft(int set){mBorderLeft = set;mContent->move(getBorderPos());emit onRectChange(this);}
		inline void setBorderRight(int set){mBorderRight = set;mContent->resize(this->size() - getBorderSize());emit onRectChange(this);}
		inline void setBorderBottom(int set){mBorderBottom = set;mContent->resize(this->size() - getBorderSize());emit onRectChange(this);}
		
	private:
		/* 边框宽度 变量 设置获取函数 */
		long mBorderTop;
		long mBorderLeft;
		long mBorderRight;
		long mBorderBottom;
};
#endif // IITEMS_H
