#ifndef FPQUERYRESULTITEM_H
#define FPQUERYRESULTITEM_H

#include <QString>
#include <QDateTime>

#include <lqtutils_prop.h>

Q_NAMESPACE

L_BEGIN_CLASS(FPQueryResultItem)
L_RW_PROP_AS(QString, ID)
L_RW_PROP_AS(QString, UID)
L_RW_PROP_AS(QDateTime, TakenAt)
L_RW_PROP_AS(int, Year, -1)
L_RW_PROP_AS(int, Month, -1)
L_RW_PROP_AS(int, Day, -1)
L_RW_PROP_AS(QString, FileName)
L_RW_PROP_AS(QString, Name)
L_RW_PROP_AS(QString, Path)
L_RW_PROP_AS(QString, Hash)
L_RW_PROP_AS(QString, Type)
L_END_CLASS

#endif // FPQUERYRESULTITEM_H
