#include "widget.h"
#include<QSqlError>
#include<QStandardPaths>
#include<QDir>

Widget::Widget(QWidget *parent)
    : QWidget(parent) , Tire(30000, std::vector<int>(27, 0))  // 使用初始化列表初始化vector
    , cnt(660000, -1)
{

    QFile file(":/json/CET4_3.json");

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qCritical()<<"Failed to open file"<<file.errorString()<<"   "<<file.error();
        return ;
    }else{
        qDebug()<<"文件CET4_3打开成功";
    }
    Initsqlite();
    QTextStream in(&file);
    QJsonParseError parseError;
    while(!in.atEnd()){
        QString line=in.readLine();
        QJsonDocument doc=QJsonDocument::fromJson(line.toUtf8(),&parseError);
        if(parseError.error==QJsonParseError::NoError){
            if(doc.isObject()){
                QJsonObject obj=doc.object();
                QStringList expand_list;
                QJsonObject question_info;
                InitWord_Table(obj,Id,expand_list);
                QJsonObject contents=obj["content"].toObject();
                contents=contents["word"].toObject();
                contents=contents["content"].toObject();
                get_phrase(contents, Id);  // 现在传入的是正确层级的对象
                InitSentence_Table(contents,question_info);
                InitQuestion_Table(question_info,Id,expand_list);

            }
            else{
                qDebug()<<"Json已经读取完毕剩下内容没有json";
            }
        }
        else{
            qCritical()<<parseError.errorString()<<"  "<<parseError.error;
        }
        linecount++;
        Id++;
    }
        QMapIterator<QString,int>it(SentenceMap);
        while(it.hasNext()){
            it.next();
            QString Sentence=it.key();
            int id=it.value();
            Relation(Sentence,id);
        }
        db.close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

}

Widget::~Widget() {}

void Widget::Initsqlite()
{
    QString databasePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/CET4.db";

    if(!QFile::exists(databasePath)){
        if(!QFile::copy(":/database/CET4.db",databasePath))
        {
            qDebug()<<"文件复制失败";
            return;
        }else{
            qDebug()<<"复制成功";
        }
    }
    QFile::setPermissions(databasePath,QFileDevice::WriteOwner|QFileDevice::ReadOwner);
    // 打开数据库
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);

    if (!db.open()) {
        qDebug() << "无法打开数据库:" << db.lastError().text();
    } else {
        qDebug() << "数据库连接成功，路径：" << databasePath;
    }
}


void Widget::InitWord_Table(const QJsonObject &obj,const qint32 id,QStringList &expand)
{
     QSqlQuery query(db);
    int Word_ID=id;
    int Question_ID=Word_ID;
    if(obj.contains("headWord")){
        QString Word=obj["headWord"].toString();
        expand.append(Word);
        if(obj.contains("content")){
            QJsonObject content=obj["content"].toObject();
            content=content["word"].toObject();
            content=content["content"].toObject();
            QString ukphone=content["ukphone"].toString();
            QString usphone=content["usphone"].toString();
            QString ukspeech=content["ukspeech"].toString();
            QString usspeech=content["usspeech"].toString();
            QJsonArray trans=content["trans"].toArray();
            QStringList categorys;
            for(int i=0;i<trans.size();i++){
                QJsonObject transobj=trans[i].toObject();
                QString Meaning=transobj["tranCn"].toString();
                QString Category=transobj["pos"].toString();
                categorys.append(Category);
                query.prepare("insert into Mean_Table(ID,word,mean,category)values(:id,:word,:mean,:category)");
                query.bindValue(":id",id);
                query.bindValue(":word",Word);
                query.bindValue(":mean",Meaning);
                query.bindValue(":category",Category);
                if(!query.exec()){
                    qDebug()<<"mean_table 插入失败";
                    qDebug()<<query.lastError();
                    qDebug()<<query.lastQuery();
                }
                else{
                    qDebug()<<"插入成功";
                    wordid++;
                }
            }

            int Score=0;
            int Flag=0;
            qDebug()<<Word_ID<<" 1"<<Word<<" 2"<<Question_ID<<" 3"<<Score<<" 6"<<Flag<<" 7"<<ukphone<<" 8"<<
                usphone<<" 9"<<ukspeech<<" 10"<<usspeech<<" 11";
            query.prepare("INSERT INTO All_Word_Table (Id, Word, QuestionId, Score, Flag, ukphone, usphone, ukspeech, usspeech) "
                          "VALUES (:id, :word, :questionId, :score, :flag, :ukphone, :usphone, :ukspeech, :usspeech)");

            query.bindValue(":id", Word_ID);
            query.bindValue(":word", Word);
            query.bindValue(":questionId", Question_ID);
            query.bindValue(":score", Score);
            query.bindValue(":flag", Flag);
            query.bindValue(":ukphone", ukphone);
            query.bindValue(":usphone", usphone);
            query.bindValue(":ukspeech", ukspeech);
            query.bindValue(":usspeech", usspeech);
            if (!db.isOpen()) {
                qDebug() << "Database is not open!";
                return;
            }
            if (!query.exec()) {
                qDebug() << " ALL_Word_Table 插入数据失败:" << query.lastError().text();
                qDebug() << "SQL:" << query.lastQuery();
                qDebug() << "Bound Values:" << query.boundValues();
            } else {
                qDebug() << "插入数据成功";
                WordRestore(Word,Word_ID,categorys,expand);
            }
        }
        else{
            qDebug()<<"当前单词没有内容";
        }
    }
    else{
        qDebug()<<"没有包含单词当前Json中";
    }

}

void Widget::InitSentence_Table(const QJsonObject &obj,QJsonObject &question)
{

    QSqlQuery query(db);
    QJsonObject content=obj["sentence"].toObject();
    QJsonArray sentence_arry=content["sentences"].toArray();
    for(int i=0;i<sentence_arry.size();i++){
        QJsonObject sentence_obj=sentence_arry[i].toObject();
        QString sentence=sentence_obj["sContent"].toString();
        QString sentence_trans=sentence_obj["sCn"].toString();
        if(i==0){
            question["sentence"]=sentence;
            question["sentence_trans"]=sentence_trans;
        }
        query.prepare("insert into Sentence(SentenceId,Sentence,Sentence_Trans)values(:Id,:sentence,:sentence_trans)");
        query.bindValue(":Id",SentenceId);
        query.bindValue(":sentence",sentence);
        query.bindValue(":sentence_trans",sentence_trans);
        if (!db.isOpen()) {
            qDebug() << "Database is not open!";
            return;
        }
        if(!query.exec()){
            qCritical()<<"sentence表插入失败"<<query.lastError().text();
        }
        else{
           qDebug()<<"sentence表插入成功"<<sentence<<" "<<sentence_trans;
            SentenceMap[sentence]=SentenceId;
            SentenceId++;
        }
    }
    if(question.isEmpty()){
        qCritical()<<"一条句子都没有";
    }
}

void Widget::InitQuestion_Table(const QJsonObject &obj,const qint32 id,const QStringList &expand)
{
    QSqlQuery query(db);
    if(obj.isEmpty()){
        qDebug()<<"前面句子获取异常";
    }else{
        QString origin_sentence=obj["sentence"].toString();
        QStringList anwer_list=Sentence_Separate(origin_sentence);
        QString Question;
        QString anwer;
        QString explain=obj["sentence_trans"].toString();
        int flag=1;
        for(const QString &it:anwer_list){
            for(const QString &word:expand){
                QString goal = it.toLower(); // 将句子中的单词转换为小写
                QString lowerWord = word.toLower(); // 将扩展列表中的单词转换为小写
                if(goal == lowerWord){
                    int change_index = origin_sentence.toLower().indexOf(goal); // 查找小写匹配的位置
                    if (change_index != -1) {
                        Question=origin_sentence.replace(change_index,it.length(),"________");
                        flag=0;
                        anwer=it;
                        break;
                    }

                }
            }
        }
        if(flag==1){
            qDebug()<<"句子或者单词推理出错，没有单词";
            qDebug()<<"问题句子："<<origin_sentence;
            qDebug()<<"拆分之后的句子"<<anwer_list;
            qDebug()<<"拓展单词"<<expand;
        }
        qint32 Id=id;
        query.prepare("insert into Question_Table(Question_ID,Question,explain,anwer)values(:Id,:question,:explain,:anwer)");
        query.bindValue(":Id",Id);
        query.bindValue(":question",Question);
        query.bindValue(":explain",explain);
        query.bindValue(":anwer",anwer);
        if (!db.isOpen()) {
            qDebug() << "Database is not open!";
            return;
        }
        if(!query.exec()){
            qCritical()<<"Question_Table表插入失败"<<query.lastError().text();
        }

    }

}

void Widget::Relation(const QString &sentence,const int &id)
{
    QSqlQuery query(db);
    //将句子切割成单个单词
    QStringList wordlist=Sentence_Separate(sentence);
    int big=1;
    for(QString &word:wordlist){
        if(big==1){
            big=0;
            word=word.toLower();
        }
        //WordMap之前存放推算出来的所有形态单词的map
        if(WordMap.contains(word)){
            query.prepare("insert into Relation(Sentence_ID,Word_ID,sentence,word)values(:sentence_id,:word_id,:sentence,:word)");
            query.bindValue(":sentence_id",id);
            query.bindValue(":word_id",WordMap[word]);
            query.bindValue(":sentence",sentence);
            query.bindValue(":word",word);
            if(!query.exec()){
                qCritical()<<"relation表插入失败";
                qCritical()<<word<<"    "<<wordlist;
                qCritical()<<query.lastError().text();
                qCritical()<<query.lastQuery();
            }
            else{
                qDebug()<<"数据插入成功";
            }
        }
    }
}

void Widget::WordRestore(QString word,int id,const QStringList categorys,QStringList &expand)
{
    WordMap[word]=id;
    QString change_word;
    int len=word.size();
    QString end1,end2;
    end1=word.right(1);
    end2=word.right(2);
    //遍历它的词性并根据词性处理
    for(int i=0;i<categorys.size();i++){
        //名词形式，去除不规则的情况，根据不同结尾不同处理
        if(categorys[i]=="n"){
            if(Nounsirregular.contains(word)){
                change_word=Nounsirregular[word];
            }
            else if(end1=="s"||end1=="x"||end1=="z"||end1=="o"
                ||end2=="ch"||end2=="sh"){
                change_word=word.append("es");
            }
            else if(end1=="y")
            {
                QChar ch=word[len-2];
                if(isVowel(ch)){
                    change_word=word.replace(len-1,1,"ies");
                }
            }
            else if(end1=="f"){
                QSet<QString> normalFWords = {"roof", "cliff", "chief", "belief"};
                if(normalFWords.contains(word)) {
                    change_word = word + "s";
                }
                change_word=word.replace(len-1,1,"ves");
            }
            else if(end2=="fe"){
                change_word=word.replace(len-1,2,"ves");
            }
            WordMap[change_word]=id;
            expand.append(change_word);
        }
        else if(categorys[i][0]=='v'){
            //过去式和过去分词
            //以e结尾直接加
            //以y结尾加辅音字母结尾变y为ied
            //以一个辅音字母结尾的重读闭音节动词双写辅音字母，再加-ed
            //直接加ed
            //不规则动词变换
            if(Verbsirregular.contains(word)){
                for(const QString &it:Verbsirregular[word]){
                    WordMap[it]=id;
                    Creat_Tire(it);
                    expand.append(it);
                }
            }
            else {
                if(end1=="e"){
                    change_word=word.append("d");
                }
                else if(end1=="y"&&isVowel(word[len-2])){
                    change_word=word.replace(len-1,1,"ied");
                }
                else if(doubleConsonantVerbs.contains(word)){
                        change_word=word+end1+"ed";
                }
                else{
                    change_word=word+"ed";
                }
                WordMap[change_word]=id;
                expand.append(change_word);
            }
                //现在分词
                //以e结尾的去e加ing
                //双写最后一个字母+ing
                //+ing
            if(end1=="e"){
                change_word=word.replace(len-1,1,"ing");
            }
            else if(doubleConsonantVerbs.contains(word)){
                 change_word=word+end1+"ing";
            }
            else{
                change_word=word+"ing";
            }
            WordMap[change_word]=id;

            expand.append(change_word);
                //第三人称单数
                //以s,x,z,o,ch,sh结尾的直接加es
                //以y加辅音字母结尾的变y为i+es
                //直接加s
                if(end1=="s"||end1=="x"||end1=="z"||end1=="o"
                         ||end2=="ch"||end2=="sh"){
                    change_word=word.append("es");
                }
                else if(end1=="y"&&isVowel(word[len-2])){
                    change_word=word.replace(len-1,1,"ies");
                }
                else{
                    change_word=word.append("s");
                }
                WordMap[change_word]=id;

                expand.append(change_word);
            }


        else if(categorys[i]=="adj"){
            if(end1=="e"){
                change_word=word.append("r");
                WordMap[change_word]=id;
                Creat_Tire(change_word);
                expand.append(change_word);
                change_word=word.append("st");
                WordMap[change_word]=id;
                Creat_Tire(change_word);
                expand.append(change_word);
            }
            //判断是否是以一个辅音字母结尾的重读闭音节动词双写辅音字母
            else if(!isVowel(word[len-2])){
                if(isVowel(word[len-1])){
                    change_word=word.append(end1);
                    change_word=change_word.append("er");
                    WordMap[change_word]=id;
                    Creat_Tire(change_word);
                    expand.append(change_word);
                  change_word=change_word.append("est");
                    WordMap[change_word]=id;
                  Creat_Tire(change_word);
                    expand.append(change_word);
                }else{
                    change_word=word.append("er");
                    WordMap[change_word]=id;
                    Creat_Tire(change_word);
                    expand.append(change_word);
                    change_word=word.append("est");
                    WordMap[change_word]=id;
                    Creat_Tire(change_word);
                    expand.append(change_word);
                }
    }
}
    }
}

void Widget::get_phrase(const QJsonObject &obj,const qint32 &id)
{
    QSqlQuery query(db);
    if(obj.contains("phrase")){
        qDebug()<<"get_phrasehave";
        QJsonObject phrase_obj=obj["phrase"].toObject();
        QJsonArray phrases=phrase_obj["phrases"].toArray();
        qDebug()<<"phrasesize:   "<<phrases.size();
        for(int i=0;i<phrases.size();i++){
            QJsonObject phrase=phrases[i].toObject();
            QString pcontent=phrase["pContent"].toString();
            QString pCn=phrase["pCn"].toString();

            query.prepare("insert into Phrase(id,phrase,pcn) values (:id,:phrase,:pcn)");
            query.bindValue(":id",id);
            query.bindValue(":phrase",pcontent);
            query.bindValue(":pcn",pCn);
            if(!query.exec()){
                qCritical()<<"Phrased "<<query.lastError().text();
                return;
            }
        }
    }else{
        qDebug()<<"get_phrase not contains ";
        qDebug()<<obj;
    }

}

bool Widget::isVowel(QChar ch)
{
   return (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u');
}

void Widget::Creat_Tire(const QString word)
{
    int pos=0;
    for(int i=0;i<word.size();i++){
        int j=word[i].unicode()-'a';
        if(!Tire[pos][j]){
            Tire[pos][j]=++index;
        }
         pos=Tire[pos][j];
    }
    cnt[index]=1;
}

bool Widget::query_Tire(const QString word)
{
    int pos=0;
    for(int i=0;i<word.size();i++){
        int j=word[i].unicode()-'a';
        if(!Tire[pos][j]){
            return -1;
        }
        pos=Tire[pos][i];
    }
    return cnt[pos];
}

QStringList Widget::Sentence_Separate(QString sentence)
{
    sentence=sentence.toLower();
    for(int i=0;i<sentence.size();i++){

        if(!sentence[i].isLetter())
            sentence.replace(i, 1, " ");
    }
    QStringList lis=sentence.split(" ",Qt::SkipEmptyParts);
    return lis;
}







