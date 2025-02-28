#ifndef AC_SIGNAL_H
#define AC_SIGNAL_H

#include <QObject>
#include <QQmlEngine>
#include<QJsonObject>

class AC_signal : public QObject
{
    Q_OBJECT
    QML_ELEMENT
private:
    explicit AC_signal(QObject *parent = nullptr);
public:
   static AC_signal* instance();
public slots:
    void nextPage();
    void word_show(qint32 id,qint32 type,QJsonObject query);
    void back_home();
    void back(QJsonObject query);
signals:
    void nextPage_sig();
     void word_show_sig(qint32 id,qint32 type,QJsonObject query);
     void learn_finish();
     void back_home_sig();
     void back_sig(QJsonObject query);
};
#endif // AC_SIGNAL_H
