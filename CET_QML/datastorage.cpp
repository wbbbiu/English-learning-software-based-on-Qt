 #include "datastorage.h"
#include<QFile>
#include<QDebug>
#include<QSqlError>
#include<QSqlQuery>
#include<QDir>
#include<QDataStream>
#include<QJsonArray>
using namespace std;
DataStorage::DataStorage(QObject *parent)
    : QObject{parent}
{
    open_sqlite();
    network=new QNetworkAccessManager(this);
    connect(network,&QNetworkAccessManager::finished,this,&DataStorage::handlerResponse);

}

qint32 DataStorage::get_random(qint32 start, qint32 end)
{
    random_device dr;
    mt19937 gen(dr());
    uniform_int_distribution dis(start,end);
    return dis(gen);
}

QStringList DataStorage::sentence_Separate(QString sentence)
{

    for(int i=0;i<sentence.size();i++){
        if(!sentence[i].isLetter()){
            sentence[i]=' ';
        }
    }
    return sentence.split(" ",Qt::SkipEmptyParts);

}

bool DataStorage::review_check()
{
    QSettings setting(configuration_path,QSettings::IniFormat);
    QDateTime now=QDateTime::currentDateTime();
    QDateTime today=now.date().startOfDay();
    QList<qint32> id_list;
    if(!setting.contains("time")){
        setting.setValue("time",today);
        return true;
    }else{
        QDateTime last_time=setting.value("time").toDateTime();
        if(now>=last_time.addDays(0)){
            qDebug()<<"review check is";
            QSqlQuery query(db);
             qint32 last_logo=-1;
            QDateTime sevenDaysAgo = now.addDays(-7);
            QDateTime fifteenDaysAgo = now.addDays(-15);
            QDateTime thirtyDaysAgo = now.addDays(-30);
            QDateTime ninetyDaysAgo = now.addDays(-90);
            query.prepare("SELECT logo,id FROM Review_Table WHERE "
                          "(time <=:sevenDaysAgo AND stage = 1 AND state=0) OR "
                          "(time <=:fifteenDaysAgo AND stage = 2 AND state=0) OR "
                          "(time <=:thirtyDaysAgo AND stage = 4 AND state=0) OR "
                          "(time <=:ninetyDaysAgo AND stage = 5 AND state=0) "
                          "ORDER BY logo ASC, time ASC");
            query.bindValue(":sevenDaysAgo", sevenDaysAgo.toString(Qt::ISODate));
            query.bindValue(":fifteenDaysAgo", fifteenDaysAgo.toString(Qt::ISODate));
            query.bindValue(":thirtyDaysAgo", thirtyDaysAgo.toString(Qt::ISODate));
            query.bindValue(":ninetyDaysAgo", ninetyDaysAgo.toString(Qt::ISODate));
            qDebug() << "Prepared SQL:" << query.lastQuery();
            qDebug() << "Bound values:" << query.boundValues();
             if(!query.exec()){
                qDebug()<<"review_check word is failed"<<query.lastError().text();
                 return false;
             }else{
                qDebug() << "SQL executed successfully, checking for results...";
                 while(query.next()){

                     qint32 current_logo=query.value(0).toInt();

                     if(last_logo!=current_logo){

                         if (last_logo != -1) { // 如果不是第一个logo，也就是不是同一组复习，插入分隔符-1
                             review_word_group.append(-1);
                             review_sentence_group.append(-1);
                             group_count++;
                         }
                         last_logo=current_logo;
                         QSqlQuery query1(db);
                         query1.prepare("select sentence_id from Review_Sentence where logo = :logo");
                         query1.bindValue(":logo", current_logo);
                         if(!query1.exec()){
                             qDebug()<<"review_check sentence is failed"<<query1.lastError().text();
                             return false;
                         }else{
                             while(query1.next()){
                                 qint32 sentence_id=query1.value(0).toInt();
                                 review_sentence_group.append(sentence_id);
                             }
                         }
                     }

                     qint32 id=query.value(1).toInt();
                     qDebug()<<"word id is"<<id;
                     id_list.append(id);
                     review_word_group.append(id);
                 }
             }
             if(!id_list.isEmpty()){
                 QStringList placeholders;
                 for (int i = 0; i < id_list.size(); ++i) {
                     placeholders << "?"; // 使用QStringList来收集占位符
                 }
                 QSqlQuery updatequery(db);
                 //设置当前是复习状态不在参与下一次复习检查
                 updatequery.prepare(QString("UPDATE Review_Table SET state=1 WHERE id IN (%1)").arg(placeholders.join(","))); // 使用join方法拼接占位符
                 for (const auto& id : id_list) {
                     updatequery.addBindValue(id);
                 }
                 if (!updatequery.exec()) {
                     qDebug() << "review_check batch update is failed" << updatequery.lastError().text();
                     return false;
                 }

        }
    }
    setting.setValue("time",today);

    return true;
}
}

bool DataStorage::review_1(qint32 id)
{
    QSqlQuery query1(db);
    query1.prepare("update All_Word_Table set Score=7 where Id=:id");
    query1.bindValue(":id",id);
    if(!query1.exec()){
        qCritical()<<"learn_end update is failed "<<query1.lastError().text();
        return false;
    }
    return true;
}

bool DataStorage::review_2(qint32 id)
{
    QSqlQuery query1(db);
    query1.prepare("update All_Word_Table set Score=11 where Id=:id");
    query1.bindValue(":id",id);
    if(!query1.exec()){
        qCritical()<<"learn_end update is failed "<<query1.lastError().text();
        return false;
    }
    return true;
}
DataStorage *DataStorage::intance()
{
    static DataStorage storage;
    return &storage;
}
//获取当前学习单词第一阶段需要的信息

QVariantMap DataStorage::stage_one_query(qint32 word_id)
{
    qDebug()<<"进入第一阶段查询函数";
    QSqlQuery query_one(db);

    QVariantMap result;
    query_one.prepare("select Word,ukphone,ukspeech,usphone,usspeech,Id from All_Word_Table where Id=:id");
    query_one.bindValue(":id",word_id);
    if(!query_one.exec()){
        qCritical()<<"stage_one 查询失败"<<query_one.lastError().text();
    }else{
        qDebug()<<"stage_one 查询成功";
        while(query_one.next()){

            result["word"]=query_one.value(0).toString();
            result["ukphone"]=query_one.value(1).toString();
            result["ukspeech"]=query_one.value(2).toString();
            result["usphone"]=query_one.value(3).toString();
            result["usspeech"]=query_one.value(4).toString();
            result["id"]=query_one.value(5).toInt();
        }
    }
    if(result.isEmpty()){
        qCritical()<<"stage_one_query is failed result is empty";
    }
    qDebug()<<"stage_one result: "<<result;
    return result;
}
//如果是单词学习阶段，获取当前这个单词的学习阶段，来确定加载那个页面，并返回这个单词的相关信息
QVariantMap DataStorage::stage_state(qint32 type)
{

    QList<qint32> group;
    qint32 index_;
    //判断是学习状态还是复习状态
    if(type==1){
        group=now_word_group;
        index_=word_index;
    }else{
        group=review_word_group;
        index_=review_word_index;
    }
    QSqlQuery query(db);
    QVariantMap info;
    //当前单词组中存在一些单词已经学习完成，就需要循环寻找下一个没有学习过的单词
    int flag=1;
    while(flag){
        query.prepare("select Score,Id from All_Word_Table where Id=:id");
        query.bindValue(":id",group[index_]);
        qDebug()<<query.lastQuery();
        if(!query.exec()){
            qCritical()<<"stage_state failed: "<<query.lastError().text();
        }else{
            //获取当前单词的阶段
            while(query.next()){
                info["stage"]=query.value(0).toInt();
                info["id"]=query.value(1).toInt();
            }
            //除了5,9,10这几个阶段是学习和复习完成的跳过
            if(info["stage"]!=5&&info["stage"]!=10&&info["stage"]!=15){
                 flag=0;
            }
        }
        index_=(index_+1)%group.size();
    }
    if(type==1){
       word_index=index_;
    }else{
       review_word_index=index_;
    }
    qDebug()<<"stage:"<<info;
        return info;
}

QVariantMap DataStorage::sentence_state(qint32 type)
{
    QList<qint32> group;
    qint32 index_;
    if(type==1){
        group=sentence_group;
        index_=sentence_index;
    }else{
        group=review_sentence_group;
        index_=review_sentence_index;
    }

    QVariantMap info;
    QString sentence;
     QStringList word_list;
    if(index_>=group.size()){
        sentence_flag=2;
    }else{
        int x=group[index_];
         QSqlQuery query(db);
        query.prepare("select sentence from Relation where Sentence_ID=?");
        query.addBindValue(x);
        if(!query.exec()){
            qCritical()<<"stage_six failed: "<<query.lastError().text();
        }else{

            while(query.next()){
                sentence=query.value(0).toString();
            }
        }
        word_list=sentence_Separate(sentence);
        info["words"]=word_list;
        QSqlQuery query1(db);
        query1.prepare("select Word_ID,word from Relation where Sentence_ID=?");
        query1.addBindValue(x);
        if(!query1.exec()){
            qCritical()<<"stage_six failed query1 : "<<query1.lastError().text();
        }else{
            QJsonArray cet4s;
            //获取当前单词的阶段
            while(query1.next()){
                QJsonObject cet4;
                QString word=query1.value(1).toString();
                for(int i=0;i<word_list.size();i++){
                    QString gold=word_list[i].toLower();
                    if(gold==word){
                        cet4["id"]=query1.value(0).toInt();
                        cet4["index"]=i;
                        cet4s.append(cet4);
                    }
                }

            }
            info["cet4s"]=cet4s;
        }
    }
    index_=index_+1;
    if(type==1){
       sentence_index=index_;
    }else{
        review_sentence_index=index_;
    }
    qDebug()<<info;
    return info;
}

bool DataStorage::learn_end(qint32 word_id)
{

    QSqlQuery query1(db);
    query1.prepare("update All_Word_Table set Score=5 where Id=:id");
    query1.bindValue(":id",word_id);
    if(!query1.exec()){
        qCritical()<<"learn_end update is failed "<<query1.lastError().text();
        return false;
    }
    today_learn_count++;
    if(today_learn_count>=now_word_group.size()){
    sentence_flag=1;
    }
    return true;
}

bool DataStorage::review_end(qint32 word_id)
{
    qDebug()<<"review_end";
            QSqlQuery query1(db);
            query1.prepare("update All_Word_Table set Score=15 where Id=:id");
            query1.bindValue(":id",word_id);
            if(!query1.exec()){
                qCritical()<<"review_end update is failed "<<query1.lastError().text();
                return false;
            }
            today_review_count++;
            if(today_review_count==review_count){
                sentence_flag=1;
            }

        return true;

}

bool DataStorage::upgrade(qint32 word_id)
{
    QSqlQuery query(db);
    qint32 stage;
    query.prepare("select Score from All_Word_Table where Id=:id");
    query.bindValue(":id",word_id);
    if(!query.exec()){
        qCritical()<<"upgrad select is failed "<<query.lastError().text();
        return false;
    }else{
        while(query.next()){
            stage=query.value(0).toInt()+1;
            QSqlQuery  query1(db);
            query1.prepare("update All_Word_Table set Score=:stage where Id=:id");
            query1.bindValue(":stage",stage);
            query1.bindValue(":id",word_id);
            if(!query1.exec()){
                qCritical()<<"updata is failed "<<query1.lastError().text();
                return false;
            }else{
                qDebug()<<"Phase update seccess: "<<stage<<" today_learn: "<<today_learn_count;
            if(stage==5){
                //如果进入阶段六那就代表这个单词的学习是完成了的，今日学习数加1
                today_learn_count++;
                //如果学习数已经等于目标，那么就代表这个单词学习完成开始单词复习部分
                if(today_learn_count>=now_word_group.size()){
                    sentence_flag=1;
                }
            }
            else if(stage==10||stage==15){
                today_review_count++;
                qDebug()<<"today_review is"<<today_review_count;
                if(today_review_count==review_count){
                    sentence_flag=1;
                }
            }
            }

        }

        return true;
    }


}

//收藏函数
bool DataStorage::collect(qint32 word_id)
{
    QSqlQuery query(db);
    query.prepare("insert into Collect(word_id) values(word_id=:word_id)");
    query.bindValue(":word_id",word_id);
    if(!query.exec()){
        qCritical()<<"collect is failed: "<<query.lastError().text();
        return false;
    }else{
        return true;
    }
}

//取消收藏的函数
bool DataStorage::cancel_collect(qint32 word_id)
{
    QSqlQuery query(db);
    query.prepare("delete from Collect where word_id=:word_id");
    query.bindValue(":word_id",word_id);
    if(!query.exec()){
        qCritical()<<"cancel_collect is failed: "<<query.lastError().text();
        return false;
    }else{
        return true;
    }
}

//创建应用目录还有相关初始化，存入数据库，并打开数据库，使用db访问
void DataStorage::open_sqlite()
{
    //QStandardPaths标准路径，打开一个可写的系统标准的应用数据存储的路径
    //在 Windows 上，通常指向 C:\Users\<username>\AppData\Roaming\<YourApp>
    QString appPath=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString sqlPath=appPath+"/CET4.db";
     wordgroup_path=appPath+"/wordgrounp.dat";
    sentencegorup_path=appPath+"/sentencegroup.dat";
     review_sentence_path=appPath+"/review_sentence.dat";
    review_word_path=appPath+"/review_word.dat";
     configuration_path=appPath+"/time.ini";
    QDir dir;
        // 确保这个应用目录是存在的
    if(!dir.exists(appPath)){
        //不存在就创建一个这个应用的目录
        if(dir.mkpath(appPath)){
            qDebug()<<"应用数据目录创建成功";
        }
        else{
             qCritical()<<"应用数据目录不存在，并且创建失败";
        }
    }
    QFile file;
        //在应用目录存在的情况下查看这个存放单词的数据库是否存在
    if(!file.exists(sqlPath)){
        //如果不存在的话就进行复制将项目中的数据库复制过去
        if(QFile::copy(":/database/CET4.db",sqlPath)){
            qDebug()<<"复制数据库到应用数据目录下";
        }else{
             qCritical()<<"该应用目录下没有数据库文件，并且复制失败";
        }
    }
    //添加读写权限
    file.setPermissions(sqlPath,QFileDevice::WriteOwner|QFileDevice::ReadOwner);
    //指定数据库驱动我们此处使用的是sqlite数据库
    db=QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库链接内容
    db.setDatabaseName(sqlPath);
    if(!db.open()){
         qCritical()<<"数据库打开失败"<<db.lastError().text();
    }
    else{
        qDebug()<<"数据库打开成功"<<"打开路径为："<<sqlPath;
    }

}
//初始化入now_word_id的这些哪怕关闭app也需要存储下来的变量，将他们取出,也就是持久化储存的内容
void DataStorage::data_init()
{
    //将wordgrounp.dat中持久化存储的单词组取出来
    QFile file(wordgroup_path);
    now_word_group.clear();
        if(file.exists()&&file.open(QIODevice::ReadOnly)){
            QDataStream in(&file);
            in>>now_word_group;
             qDebug() << "读取到学习组，大小为:" << now_word_group.size();
            file.close();
        }
        else{
             qCritical()<<"文件你创建失败";
        }
        QFile file1(sentencegorup_path);
        sentence_group.clear();
        if(file1.exists()&&file1.open(QIODevice::ReadOnly)){
            QDataStream in(&file1);
            in>>sentence_group;
            qDebug() << "读取到学习组，大小为:" << sentence_group.size();
            file1.close();
        }
        QFile file2(review_sentence_path);
        if(file2.exists()&&file2.open(QIODevice::ReadOnly)){
            QDataStream in(&file2);
            in>>review_sentence_group;
            file2.close();
        }
        QFile file3(review_word_path);
        if(file3.exists()&&file3.open(QIODevice::ReadOnly)){
            QDataStream in(&file3);
            in>>review_word_group;
            file3.close();
        }

    //取出已经学习过的单词的标记，放入unordermap表示学习过了
    QSqlQuery query(db);
    query.prepare("select Id from All_Word_Table where flag=1");
    if(!query.exec()){
         qCritical()<<"data_init learn flag is failed"<<query.lastError().text();
    }else{
        while(query.next()){
            learn_flag[query.value(0).toInt()]=1;
        }
    }
    QSqlQuery query1(db);
    //取出其他需要持久化存储的放入数据库的内容
    query1.prepare("select now_word_id,now_learn_count,flag_count,init_flag,`index`,sentence_index,today_learn_count,today_review_count,sentence_flag,review_word_index,review_sentence_index,review_logo from Init_data");
    qDebug()<<query1.lastQuery();
    if(!query1.exec()){
         qCritical()<<"data_init is failed "<<query1.lastError().text();
    }
    else{
        qDebug()<<"init_data success";
        while(query1.next()){
            now_word_id=query1.value(0).toInt();
            now_learn_count=query1.value(1).toInt();
            flag_count=query1.value(2).toInt();
            init_flag=query1.value(3).toInt();
            word_index=query1.value(4).toInt();
            sentence_index=query1.value(5).toInt();
            today_learn_count=query1.value(6).toInt();
            today_review_count=query1.value(7).toInt();
            sentence_flag=query1.value(8).toInt();
            review_word_index=query1.value(9).toInt();
            review_sentence_index=query1.value(10).toInt();
            review_logo=query1.value(9).toInt();
        }
    }
}
void DataStorage::handlerResponse(QNetworkReply *reply)
{
    if(reply->error()==QNetworkReply::NoError){
        qDebug()<<"响应请求";
        QByteArray data=reply->readAll();
        output=new QAudioOutput(this);
       player=new QMediaPlayer;
        player->setAudioOutput(output);

        QBuffer *buffer=new QBuffer(player);
        buffer->setData(data);
        buffer->open(QIODevice::ReadOnly);
        QUrl setUrl("audio.mp3");
        player->setSourceDevice(buffer,setUrl);
        player->play();
        qDebug()<<"音频开始播放";
    }else{
        qCritical()<<"handler is failed "<<reply->errorString();
    }
    reply->deleteLater();
}

QJsonObject DataStorage::get_word_info(qint32 id)
{
    QJsonObject info;
    {
    QSqlQuery query(db);
    query.prepare("select mean,category from Mean_Table where ID=?");
    query.addBindValue(id);
    if(!query.exec()){
        qCritical()<<"get_word_info is failed mean"<<query.lastError().text();
    }else{
        QJsonArray means;
        while(query.next()){
            QJsonObject mean;
            mean["mean"]=query.value(0).toString();
            mean["category"]=query.value(1).toString();
            means.append(mean);
        }
        info["means"]=means;
    }
    }
    {
        QSqlQuery query(db);
        query.prepare("select a.Sentence,a.Sentence_Trans from Sentence a inner join Relation b on a.SentenceId=b.Sentence_ID where b.Word_ID=? limit 4");
        query.addBindValue(id);
        if(!query.exec()){
            qCritical()<<"get_word_info is failed sentence"<<query.lastError().text();
        }else{
            QJsonArray sentences;
            while(query.next()){
                QJsonObject sentence;
                sentence["sentence"]=query.value(0).toString();
                sentence["sentence_trans"]=query.value(1).toString();
                sentences.append(sentence);
            }
            info["sentences"]=sentences;
        }
    }

    {
        QSqlQuery query(db);
        query.prepare("select phrase,pcn from Phrase  where id=? limit 4");
        query.addBindValue(id);
        if(!query.exec()){
            qCritical()<<"get_word_info is failed Phrase"<<query.lastError().text();
        }else{
            QJsonArray phrases;

            while(query.next()){
                QJsonObject phrase;
                phrase["phrase"]=query.value(0).toString();
                phrase["phrase_trans"]=query.value(1).toString();
                phrases.append(phrase);

            }
            info["phrases"]=phrases;
        }
    }
    {
        QSqlQuery query(db);
        query.prepare("select Word,ukphone,usphone from All_Word_Table where Id=?");
        query.addBindValue(id);
        if(!query.exec()){
            qCritical()<<"get_word_info is failed word"<<query.lastError().text();
        }else{
            while(query.next()){
                QJsonObject word;
                word["word"]=query.value(0).toString();
                word["ukphone"]=query.value(1).toString();
                word["usphone"]=query.value(2).toString();
                info["word"]=word;
            }

        }
    }
    return info;
}

QJsonObject DataStorage::stage_two_query(qint32 word_id)
{
    QSqlQuery query1(db);
    QString word;
    query1.prepare("select Word from All_Word_Table where Id=?");
    query1.addBindValue(word_id);
    if(!query1.exec()){
        qCritical()<<"stage_tow  word is failed "<<query1.lastError().text();
    }else{
        while(query1.next()){
            word=query1.value(0).toString();
        }
    }
    QSqlQuery query(db);
    QJsonObject info;
    query.prepare("select a.category,a.mean,a.word,b.ukphone from Mean_Table a inner join  All_Word_Table b on a.ID=b.Id where b.Word=?");
    query.addBindValue(word);
    if(!query.exec()){
        qCritical()<<"stage_tow_query is failed "<<query.lastError().text();
    }else{
        int index=get_random(0,3);
        info["anwer"]=index;
        QJsonArray anwers;
        QJsonArray categorys;
        while(query.next()){
            info["word"]=query.value(2).toString();
            for(int i=0;i<=3;i++){
                QJsonObject anwer;

                if(index==i){
                    anwer["anwer"]=query.value(0).toString()+". "+query.value(1).toString();
                }else{
                    int x=get_random(0,2000);
                    qDebug()<<"random is "<<x;
                    QSqlQuery query1(db);
                    query1.prepare("select category,mean from Mean_Table where ID=?");
                    query1.addBindValue(x);
                    if(!query1.exec()){
                        qCritical()<<"stage_tow_query is failed "<<query1.lastError().text();
                    }else{
                        while(query1.next()){
                            anwer["anwer"]=query1.value(0).toString()+". "+query1.value(1).toString();
                        }
                    }
                }
                anwers.append(anwer);
            }
            info["anwers"]=anwers;
            info["categorys"]=categorys;
            info["ukphone"]=query.value(3).toString();
        }
    }
    return info;
}
QJsonObject DataStorage::stage_three_query(qint32 word_id)
{
    QSqlQuery query(db);
    QJsonObject info;
    query.prepare("select Question,explain,anwer from Question_Table where Question_ID=?");
    query.addBindValue(word_id);
    if(!query.exec()){
        qCritical()<<"stage_three_query is failed "<<query.lastError().text();
    }else{
        int index=get_random(0,3);
        info["anwer"]=index;
        QJsonArray anwers;
        while(query.next()){
            for(int i=0;i<=3;i++){
                QJsonObject anwer;
                if(index==i){
                    anwer["anwer"]=query.value(2).toString();
                }else{
                    int x=get_random(0,2000);
                    QSqlQuery query1(db);
                    query1.prepare("select Word from All_Word_Table where Id=?");
                    query1.addBindValue(x);
                    if(!query1.exec()){
                        qCritical()<<"stage_three_query is failed "<<query1.lastError().text();
                    }else{
                        while(query1.next()){
                            anwer["anwer"]=query1.value(0).toString();
                        }
                    }
                }
                 anwers.append(anwer);
            }
            info["anwers"]=anwers;
            info["question"]=query.value(0).toString();
            info["explain"]=query.value(1).toString();
        }

    }
    return info;
}

QJsonObject DataStorage::stage_four_query(qint32 word_id)
{
    QSqlQuery query1(db);
    QString word;
     QJsonObject info;
    query1.prepare("select Word from All_Word_Table where Id=?");
    query1.addBindValue(word_id);
    if(!query1.exec()){
        qCritical()<<"stage_four  word is failed "<<query1.lastError().text();
    }else{

        while(query1.next()){
            word=query1.value(0).toString();
            info["word"]=word;
        }
    }
    QSqlQuery query(db);
    query.prepare("select category,mean from Mean_Table where word=?");
    query.addBindValue(word);
    if(!query.exec()){
        qCritical()<<"stage_four is mean failed "<<query.lastError().text();
    }else{
        QString mean;
        while(query.next()){
            mean=query.value(0).toString()+". "+query.value(1).toString()+"\n";
        }
        info["mean"]=mean;
    }
    return info;
}

QJsonObject DataStorage::stage_five_query(qint32 word_id)
{
    QSqlQuery query(db);
    QJsonObject info;
    query.prepare("select Word,usphone from All_Word_Table where Id=?");
    query.addBindValue(word_id);
    if(!query.exec()){
        qCritical()<<"stage_five is failed "<<query.lastError().text();
    }else{

        while(query.next()){
            info["word"]=query.value(0).toString();
            info["usphone"]=query.value(1).toString();
        }
    }
    return info;
}

bool DataStorage::sentence_next()
{
    qint32 index;
    QList<qint32>group;
    if(type==1){
        index=sentence_index;
        group=sentence_group;
    }else{
        index=review_sentence_index;
        group=review_sentence_group;
    }
    if(index>=group.size()||group[index]==-1){
        qDebug()<<"学习完成";
        sentence_flag=0;
        emit AC_signal::instance()->learn_finish();
        return false;
    }
    qDebug()<<"学习未完成";
    return true;
}

bool DataStorage::review_word_finish()
{
   review_word_index+=1;
    if(review_word_index>=review_word_group.size()||review_word_group[review_word_index]==-1){
        sentence_flag=1;
        for(int i=0;i<review_word_index;i++){
            QSqlQuery query(db);
            query.prepare("update Review_Table set state=1,stage=stage+1 where id=?");
            query.addBindValue(review_word_group[i]);
            if(!query.exec()){
                qDebug()<<"review_word_finish is failed"<<query.lastError().text();
            }
            QSqlQuery query1(db);
            query1.prepare("updata All_Word_Table set Score=6 where id=?");
            query1.addBindValue(review_word_group[i]);
            if(!query1.exec()){
                qDebug()<<"review_word_finish is failed"<<query1.lastError().text();
            }
        }
        if(review_word_index>=review_word_group.size()){
            review_word_group.erase(review_word_group.begin(),review_word_group.end());
        }else{
            review_word_group.erase(review_word_group.begin(),review_word_group.begin()+review_word_index+1);
        }
        review_word_index=0;
        return true;
    }
    return false;
}

bool DataStorage::review_sentence_finish()
{

        review_sentence_index+=1;
        if(review_sentence_index>=review_sentence_group.size()||review_sentence_group[review_sentence_index]==-1){
            if(review_sentence_index>=review_sentence_group.size()){
                review_sentence_group.erase(review_sentence_group.begin(),review_sentence_group.end());
            }else{
                review_sentence_group.erase(review_sentence_group.begin(),review_sentence_group.begin()+review_sentence_index+1);
            }

            review_sentence_index=0;
            review_logo--;
            return true;
        }
        return false;

}

bool DataStorage::learn_finish()
{
    for(qint32 &id:now_word_group){
        QSqlQuery query(db);
        query.prepare("insert into Review_Table(logo,id,time,stage,state) values(?,?,datetime('now','localtime'),?,?)");
        query.addBindValue(review_logo);
        query.addBindValue(id);
        query.addBindValue(1);
        query.addBindValue(0);
        if(!query.exec()) {
            qDebug() << " learn_finish word Error inserting data:" << query.lastError().text();
            return false;
        } else {
            qDebug() << "Data inserted successfully!";
        }
        QSqlQuery query1(db);
        query1.prepare("update All_Word_Table set Score=Score+1 where Id=?");
        query1.addBindValue(id);
        if(!query1.exec()) {
            qDebug() << " learn_finish word Error scoreupdate data:" << query1.lastError().text();
            return false;
        } else {
            qDebug() << "Data inserted successfully!";
        }
    }
    for(const qint32 &sentence_id:sentence_group){
        QSqlQuery query(db);
        query.prepare("insert into Review_Sentence(logo,sentence_id) values(?,?)");
        query.addBindValue(review_logo);
        query.addBindValue(sentence_id);
        if(!query.exec()) {
            qDebug() << " learn_finish word Error inserting data:" << query.lastError().text();
        } else {
            qDebug() << "Data inserted successfully!";
        }
    }
    review_logo++;
    return true;
}

void DataStorage::fetchPronunciation(QString word,qint32 type)
{
    qDebug()<<"发音请求";
    QString typestr=QString::number(type);
    QString url="https://dict.youdao.com/dictvoice?audio="+word+"&type="+typestr;
    qDebug() << "请求 URL: " << url;
    network->get(QNetworkRequest(QUrl(url)));
    return;
}
//这个函数代表当我点击开始学习的时候的初始化处理，看是否需要获得新的单词组
void DataStorage::learn_init()
{
    QSqlQuery query(db);
    //如果这次一次新的背诵我们就需要获取新的背诵组
    qDebug()<<"learn_init used";
    if(init_flag==0){
        qDebug()<<"learn_init begin";
        now_word_group.clear();
        sentence_group.clear();
        int now_count=0;
        //当前学习组加入的单词如果数目超过今天的目标就不在增加
        while(now_count<=flag_count){

            while(learn_flag[now_word_id]!=0&&now_word_id<=2607){
                now_word_id++;
            }
                ;//查询一个当前没有没有学习过的单词
            qDebug()<<"当前单词id是 "<< now_word_id;
            //查询出和当前单词有关的句子
            query.prepare("select Sentence_ID from Relation where Word_ID=:id");//找出包含这个句子的单词
            query.bindValue(":id",now_word_id);
            learn_flag[now_word_id]=1;
            now_word_group.append(now_word_id);

            if(!query.exec()){
                 qCritical()<<"learn_init failed:"<<query.lastError().text();
            }else
            {
                qDebug()<<"learn_init success";
                //这些和这个单词有关的句子，取出句子id
                while(query.next()){//去遍历这些句子
                    int sentence_id=query.value(0).toInt();
                    QSqlQuery query_tow(db);
                    //找出这些句子中包含的四级单词对于已经学习过的可以直接忽略，没有学习过的进行标记
                    query_tow.prepare("select Word_ID from Relation where Sentence_ID=:sentence_id");
                    query_tow.bindValue(":sentence_id",sentence_id);
                    int add_flag=0;//用于检验这个句子是否含有其他没有学习的单词，如果有的话加入学习句子组
                    if(!query_tow.exec()){
                        qCritical()<<"learn_init query_two is failed";
                    }else{
                        while(query_tow.next()){
                            int word_id=query_tow.value(0).toInt();

                            //如果这个句子中有还没有学习过的单词，那就加入学习组并且更新数据库，这个单词已经学习
                            if(!learn_flag[word_id]){
                                QSqlQuery query_three(db);
                                query_three.prepare("update All_Word_Table set flag=1 where Id=:id");
                                query_three.bindValue(":id",word_id);
                                if(!query_three.exec()){
                                    qCritical()<<"learn_init updata failed";
                                }else{
                                    now_word_group.append(word_id);
                                    learn_flag[word_id]=1;
                                    add_flag=1;
                                    now_count++;
                                }
                            }
                        }

                    }
                    if(add_flag==1){
                        sentence_group.append(sentence_id);
                    }
                }
            }
        }
    }
    else{
        qDebug()<<"NO need learn_init";
    }
}

qint32 DataStorage::get_today_learn_count()
{
    return today_learn_count;
}

void DataStorage::setToday_learn_count(qint32 value)
{
    today_learn_count=value;
    emit today_learn_countChanged();
}

qint32 DataStorage::get_today_review_count()
{
    return today_review_count;
}

void DataStorage::setToday_review_count(qint32 value)
{
    today_review_count=value;
    emit today_review_countChanged();
}

qint32 DataStorage::get_flag_count()
{
    return flag_count;
}

void DataStorage::setFlag_count(qint32 value)
{
    flag_count=value;
    emit flag_countChanged();
}



void DataStorage::setReview_count(qint32 value)
{
    review_count=value;
    emit review_countChanged();
}

qint32 DataStorage::get_sentence_flag()
{
    return sentence_flag;
}

void DataStorage::setSentence_flag(qint32 value)
{
    sentence_flag=value;
    emit sentence_flagChanged();
}

qint32 DataStorage::get_init_flag()
{
    return init_flag;
}

void DataStorage::setInit_flag(qint32 value)
{
    init_flag=value;
    emit init_flagChanged();
}

qint32 DataStorage::get_Type()
{
    return type;
}

void DataStorage::setType(qint32 value)
{
    type=value;
    emit typeChanged();
}

void DataStorage::SaveData()
{
    QSqlQuery query(db);
    query.prepare("update init_data set now_word_id=:now_word_id,now_learn_count=:now_learn_count,"
"flag_count=:flag_count,init_flag=:init_flag,`index`=:index,sentence_index=:sentence_index,today_learn_count=:today_learn_count,"
                  "today_review_count=:today_review_count,sentence_flag=:sentence_flag，review_word_index=:review_word_index"
                  ",review_sentence_index=:review_sentence_index,review_logo=:review_logo");
    query.bindValue(":now_word_id",now_word_id);
    query.bindValue(":now_learn_count",now_learn_count);
    query.bindValue(":flag_count",flag_count);
    query.bindValue(":init_flag",init_flag);
    query.bindValue(":index",word_index);
    query.bindValue(":sentence_index",sentence_index);
    query.bindValue(":today_learn_count",today_learn_count);
    query.bindValue(":today_review_count",today_review_count);
    query.bindValue(":sentence_flag",sentence_flag);
    query.bindValue(":review_word_id",review_word_index);
    query.bindValue(":review_sentence_index",review_sentence_index);
    query.bindValue(":review_logo",review_logo);
    if(!query.exec()){
        qCritical()<<"SaveData is failed "<<query.lastError().text();
        return;
    }else{
        qDebug()<<"the data is save success";
        QFile file(wordgroup_path);
        if(file.open(QIODevice::WriteOnly)){
            QDataStream in(&file);
            in<<now_word_group;
            file.close();
        }
        QFile file1(sentencegorup_path);
        if(file1.open(QIODevice::WriteOnly)){
            QDataStream in(&file1);
            in<<sentence_group;
            file1.close();
        }
        QFile file2(review_sentence_path);
        if(file2.open(QIODevice::WriteOnly)){
            QDataStream in(&file2);
            in<<review_sentence_group;
            file2.close();
        }
        QFile file3(review_word_path);
        if(file3.open(QIODevice::WriteOnly)){
            QDataStream in(&file3);
            in<<review_word_group;
            file3.close();
        }

    }
}
