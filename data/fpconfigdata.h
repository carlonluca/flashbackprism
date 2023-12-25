#ifndef FPCONFIGDATA_H
#define FPCONFIGDATA_H

#include <QObject>

class FPConfigData : public QObject
{
    Q_OBJECT
public:
    explicit FPConfigData(QObject *parent = nullptr);
};

#endif // FPCONFIGDATA_H
