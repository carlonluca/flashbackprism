#ifndef FPYEARLISTBACKEND_H
#define FPYEARLISTBACKEND_H

#include <QObject>

class FPYearListBackend : public QObject
{
    Q_OBJECT
public:
    explicit FPYearListBackend(QObject* parent = nullptr);
};

#endif // FPYEARLISTBACKEND_H
