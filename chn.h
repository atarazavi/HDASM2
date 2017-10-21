#ifndef CHN_H
#define CHN_H

#include<QFile>
#include <QString>
#include <QDataStream>
#include <QChar>
class CHN
{
public:

    CHN(QString file_name);
    QChar header[32];
    QChar footer[512];
    qint32 data[32768]={0};
    qint64 file_size;
    qint64 data_size;


};

#endif // CHN_H
