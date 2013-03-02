#ifndef ALERTEXCEPTION_H
#define ALERTEXCEPTION_H

#include <QtConcurrent/QtConcurrent>

class AlertException : public QtConcurrent::Exception
{
	public:
	AlertException(const QString Title, const QString Message);
	AlertException(const char*Title, const char*Message);
	~AlertException()throw(){}
	void Alert()const;
	const char* what() const throw();

	QString mMessage;
	QString mTitle;
};

#endif // ALERTEXCEPTION_H
