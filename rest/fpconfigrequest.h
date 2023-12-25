#ifndef FPCONFIGREQUEST_H
#define FPCONFIGREQUEST_H

#include <QObject>

class FPConfigRequest : public QObject
{
    Q_OBJECT
public:
    explicit FPConfigRequest(QObject* parent = nullptr);

public slots:
    void request();

signals:
    void requestFailed();
    void requestSucceeded();
};

#endif // FPCONFIGREQUEST_H
