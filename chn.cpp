#include "chn.h"


CHN::CHN(QString file_name)
{
    QFile chn_file(file_name);
    chn_file.open(QIODevice::ReadOnly);
    QDataStream in(&chn_file);
    this->file_size=chn_file.size();
    for(int i=0;i<32;i++){
        in>>this->header[i];
    }
    this->data_size=(this->file_size-(512+32))/4;
    for(int i=0;i<this->data_size;i++){
        union{
            unsigned char byte[4];
            int data;
        }m_test;
        in>>m_test.byte[0];
        in>>m_test.byte[1];
        in>>m_test.byte[2];
        in>>m_test.byte[3];
        this->data[i]=m_test.data;

    }
    for(int i=0;i<512;i++){
        in>>this->footer[i];
    }
}
