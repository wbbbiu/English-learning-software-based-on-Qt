#include "ac_signal.h"

AC_signal::AC_signal(QObject *parent)
    : QObject{parent}
{}

AC_signal *AC_signal::instance()
{
    static AC_signal ac_signal;
    return &ac_signal;
}

void AC_signal::nextPage()
{
  emit nextPage_sig();
}

void AC_signal::word_show(qint32 id,qint32 type,QJsonObject query)
{
   emit word_show_sig(id,type,query);
}

void AC_signal::back_home()
{
    qDebug()<<"返回主界面";
   emit back_home_sig();
}

void AC_signal::back(QJsonObject query)
{
    emit back_sig(query);
}
