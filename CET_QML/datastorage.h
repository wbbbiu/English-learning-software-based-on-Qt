#ifndef DATASTORAGE_H
#define DATASTORAGE_H
#include<QList>
#include<QString>
#include <QObject>
#include<QSqlDatabase>
#include<unordered_map>
#include<QStandardPaths>
#include<QVariantList>
#include <QtQml/qqml.h> // 包含 QML_ELEMENT 宏
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QNetworkRequest>
#include<QMediaPlayer>
#include<QBuffer>
#include<QDateTime>
#include<QSettings>
#include <QAudioOutput>
#include<QJsonObject>
#include"ac_signal.h"
#include<random>
using namespace std;
class DataStorage : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(qint32 today_learn_count READ get_today_learn_count WRITE setToday_learn_count NOTIFY today_learn_countChanged FINAL)
    Q_PROPERTY(qint32 today_review_count READ get_today_review_count WRITE setToday_review_count NOTIFY today_review_countChanged FINAL)
    Q_PROPERTY(qint32 flag_count READ get_flag_count WRITE setFlag_count NOTIFY flag_countChanged FINAL)
    Q_PROPERTY(qint32 review_count READ get_review_count WRITE setReview_count NOTIFY review_countChanged FINAL)
    Q_PROPERTY(qint32 sentence_flag READ get_sentence_flag WRITE setSentence_flag NOTIFY sentence_flagChanged FINAL)
    Q_PROPERTY(qint32 init_flag READ get_init_flag WRITE setInit_flag NOTIFY init_flagChanged FINAL)
    Q_PROPERTY(qint32 type READ get_Type WRITE setType NOTIFY typeChanged FINAL)
private:
    explicit DataStorage(QObject *parent = nullptr);
    //网络请求用于发音请求处理
    QNetworkAccessManager *network;
    //用于播放发音
    QMediaPlayer *player;
    //设置音频格式声音大小
    QAudioOutput *output;
    qint32 get_random(qint32 start,qint32 end);//固定范围内获取随机数
    QStringList sentence_Separate(QString sentence);//将句子切割成单词

    //学习，复习单词组，句子组
    QList<qint32> review_word_group;
    QList<qint32> review_sentence_group;
     QList<qint32> now_word_group;

    //持久化存储，学习，复习单词组，句子组的下标
    qint32 review_word_index;
    qint32 review_sentence_index;
     qint32 word_index=0;


    //使用次id遍历所有单词，已经学习的跳过，未学习的展开学习
    qint32 now_word_id=1;

    //各持久化文件存储路径，当前背诵的单词的单词组，为了避免退出应用后这个单词组消失我们采用持久化存储，将内容存储在wordgrounp.dat中
    //然后wordgrounp.dat的路径就是wordgrounp_path；
    QString wordgroup_path;
    QString sentencegorup_path;
    QString configuration_path;
    QString review_sentence_path;
    QString review_word_path;

    //当前复习组数，如果长时间没复习就会有多个复习组
    qint32 group_count=0;

    //判断当前单词是否学习过
     unordered_map<int,int> learn_flag;

public:
    //唯一实例访问接口
    static DataStorage *intance();
    //当前模式，是学习模式还是复习，学习是1，复习是2
    qint32 type;

    //当前学习组相关句子
    QList<qint32> sentence_group;
    qint32 sentence_index=0;//访问下标

    //学习进度相关变量
    qint32 now_learn_count=0;//当前总共学习多少
    qint32 today_learn_count=0;//今天已经学习的数目
    qint32 today_review_count=0;//今天已经复习的数目
    qint32 flag_count=15;//今天的学习目标
    qint32 review_count=0;//今天需要复习的总数
    qint32 sentence_flag=0;//学习句子
    qint32 review_accomplish=0;
    qint32 review_logo=0;

    //用于判断是否需要获取新的单词组，如果上次背诵已经完成那就是新的分组，否则就是上一次继续
    int init_flag=0;

    //数据库对象
    QSqlDatabase db;
    //学习阶段
    qint32 stage=1;

    //函数部分
    //Q_Property函数
    qint32 get_today_learn_count();
    void  setToday_learn_count(qint32 value);

    qint32 get_today_review_count();
    void setToday_review_count(qint32 value);

    qint32 get_flag_count();
    void setFlag_count(qint32 value);

    qint32 get_review_count()
    {
        return review_word_group.size();
    }
    void setReview_count(qint32 value);

    qint32 get_sentence_flag();
    void setSentence_flag(qint32 value);

    qint32 get_init_flag();
    void setInit_flag(qint32 value);

    qint32 get_Type();
    void setType(qint32 value);
    //关闭应用时保存当前数据
    void SaveData();

    //该阶段信息获取，QML调用获取该阶段信息
    Q_INVOKABLE QVariantMap stage_one_query(qint32 word_id);
    Q_INVOKABLE QJsonObject stage_two_query(qint32 word_id);
    Q_INVOKABLE QJsonObject stage_three_query(qint32 word_id);
    Q_INVOKABLE QJsonObject stage_four_query(qint32 word_id);
    Q_INVOKABLE QJsonObject stage_five_query(qint32 word_id);

    //阶段判断
    Q_INVOKABLE QVariantMap stage_state(qint32 type);
    Q_INVOKABLE QVariantMap sentence_state(qint32 type);

    //学习完毕和复习完毕的处理，学习完成之后对学习内容存储到复习表
    Q_INVOKABLE bool review_word_finish();
    Q_INVOKABLE bool review_sentence_finish();
    Q_INVOKABLE bool learn_finish();

    //阶段控制
    Q_INVOKABLE bool upgrade(qint32 word_id);
     Q_INVOKABLE bool sentence_next();
    Q_INVOKABLE bool learn_end(qint32 word_id);
    Q_INVOKABLE bool review_end(qint32 word_id);

    //收藏响应
    Q_INVOKABLE bool collect(qint32 word_id);
    Q_INVOKABLE bool cancel_collect(qint32 word_id);

    //数据初始化
    Q_INVOKABLE void data_init();

    //复习检查
    Q_INVOKABLE  bool review_check();

    //发音请求和处理
    Q_INVOKABLE void fetchPronunciation(QString word,qint32 type);
    Q_INVOKABLE void handlerResponse(QNetworkReply *reply);

    //单词信息界面展示
    Q_INVOKABLE QJsonObject get_word_info(qint32 id);

    //复习是两种情况真对用户反应的两种情况做出处理
     Q_INVOKABLE bool review_1(qint32 id);
     Q_INVOKABLE bool review_2(qint32 id);

private:
     //创建应用目录还有相关初始化，存入数据库，并打开数据库，使用db访问
    void open_sqlite();
    public slots:
    //学习初始化，这个函数代表当我点击开始学习的时候的初始化处理，看是否需要获得新的单词组
    void learn_init();
    signals:
    void today_learn_countChanged();
    void review_countChanged();
    void flag_countChanged();
    void today_review_countChanged();
    void sentence_flagChanged();
    void init_flagChanged();
    void typeChanged();
};

#endif // DATASTORAGE_H
