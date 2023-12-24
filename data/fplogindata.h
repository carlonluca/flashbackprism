#ifndef FPLOGINDATA_H
#define FPLOGINDATA_H

#include <QObject>

#include <lqtutils/lqtutils_prop.h>

Q_NAMESPACE

L_BEGIN_CLASS(FPLoginCredentials)
L_RW_PROP_AS(QString, username)
L_RW_PROP_AS(QString, password)
L_END_CLASS

L_BEGIN_CLASS(FPLoginResponse)
L_RW_PROP_AS(QString, id)
L_END_CLASS

#endif // FPLOGINDATA_H
