#ifndef WIDGET_H
#define WIDGET_H
#include<QFile>
#include<QErrorMessage>
#include<QDebug>
#include<QJsonObject>
#include<QJsonDocument>
#include<QByteArray>
#include<QJsonArray>
#include <cstdint> // C++ 版本
#include<QSqlDatabase>
#include<QSqlQuery>
#include <QWidget>
#include<QMap>
#include<string>
#include<vector>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    static const int MAX_NODES = 30000;
    static const int MAX_CHARS = 27;
    static const int MAX_WORDS = 660000;
    void Initsqlite();//初始化数据库
    void InitWord_Table(const QJsonObject &obj,const qint32 id,QStringList &expand);//初始化单词表
    void InitSentence_Table(const QJsonObject &obj,QJsonObject &question);//初始化句子表
    void InitQuestion_Table(const QJsonObject &obj,const qint32 id,const QStringList &expand);//初始化问题表
    void Relation(const QString &sentence,const int &id);//初始化单词句子关系表
    void WordRestore(const QString word,int id,const QStringList categorys,QStringList &expand);//处理单词变形
    void get_phrase(const QJsonObject &obj,const qint32 &id);
    bool isVowel(QChar ch);
    void Creat_Tire(const QString word);
    bool query_Tire(const QString word);
    QStringList Sentence_Separate(QString sentence);
    // 不规则名词复数形式 (添加更多常见的不规则名词)
    const QMap<QString, QString> Nounsirregular{
        {"man", "men"},
        {"woman", "women"},
        {"child", "children"},
        {"foot", "feet"},
        {"tooth", "teeth"},
        {"mouse", "mice"},
        {"person", "people"},
        {"goose", "geese"},
        {"ox", "oxen"},
        // 添加以下常见不规则名词
        {"criterion", "criteria"},
        {"phenomenon", "phenomena"},
        {"analysis", "analyses"},
        {"crisis", "crises"},
        {"thesis", "theses"},
        {"datum", "data"},
        {"medium", "media"},
        {"bacterium", "bacteria"},
        {"curriculum", "curricula"},
        {"formula", "formulae"},
        {"index", "indices"},
        {"matrix", "matrices"},
        {"vertex", "vertices"},
        {"axis", "axes"},
        {"hypothesis", "hypotheses"},
        {"parenthesis", "parentheses"},
        {"stimulus", "stimuli"},
        {"alumnus", "alumni"},
        {"cactus", "cacti"},
        {"fungus", "fungi"},
        {"nucleus", "nuclei"},
        {"radius", "radii"},
        {"syllabus", "syllabi"},
        {"antenna", "antennae"},
        {"formula", "formulae"},
        {"nebula", "nebulae"},
        {"vertebra", "vertebrae"}
    };

    // 不规则动词变化形式 (添加更多常见的不规则动词)
    const QMap<QString, QStringList> Verbsirregular{
        {"be", {"am", "is", "are", "was", "were", "been", "being"}},
        {"have", {"has", "had", "having"}},
        {"do", {"does", "did", "done", "doing"}},
        {"go", {"goes", "went", "gone", "going"}},
        {"say", {"says", "said", "saying"}},
        {"make", {"makes", "made", "making"}},
        {"know", {"knows", "knew", "known", "knowing"}},
        {"take", {"takes", "took", "taken", "taking"}},
        {"see", {"sees", "saw", "seen", "seeing"}},
        {"come", {"comes", "came", "coming"}},
        {"think", {"thinks", "thought", "thinking"}},
        {"give", {"gives", "gave", "given", "giving"}},
        // 添加以下常见不规则动词
        {"break", {"breaks", "broke", "broken", "breaking"}},
        {"choose", {"chooses", "chose", "chosen", "choosing"}},
        {"drink", {"drinks", "drank", "drunk", "drinking"}},
        {"drive", {"drives", "drove", "driven", "driving"}},
        {"eat", {"eats", "ate", "eaten", "eating"}},
        {"fall", {"falls", "fell", "fallen", "falling"}},
        {"fly", {"flies", "flew", "flown", "flying"}},
        {"get", {"gets", "got", "gotten", "getting"}},
        {"grow", {"grows", "grew", "grown", "growing"}},
        {"hide", {"hides", "hid", "hidden", "hiding"}},
        {"keep", {"keeps", "kept", "kept", "keeping"}},
        {"leave", {"leaves", "left", "left", "leaving"}},
        {"lose", {"loses", "lost", "lost", "losing"}},
        {"mean", {"means", "meant", "meant", "meaning"}},
        {"meet", {"meets", "met", "met", "meeting"}},
        {"put", {"puts", "put", "put", "putting"}},
        {"run", {"runs", "ran", "run", "running"}},
        {"sell", {"sells", "sold", "sold", "selling"}},
        {"send", {"sends", "sent", "sent", "sending"}},
        {"sing", {"sings", "sang", "sung", "singing"}},
        {"sit", {"sits", "sat", "sat", "sitting"}},
        {"sleep", {"sleeps", "slept", "slept", "sleeping"}},
        {"speak", {"speaks", "spoke", "spoken", "speaking"}},
        {"swim", {"swims", "swam", "swum", "swimming"}},
        {"teach", {"teaches", "taught", "taught", "teaching"}},
        {"tell", {"tells", "told", "told", "telling"}},
        {"write", {"writes", "wrote", "written", "writing"}}
    };
     // 需要双写末尾辅音的动词集合
    const QSet<QString> doubleConsonantVerbs{
        // 单音节动词
        "ban", "beg", "chop", "drop", "fit", "grab", "hug", "nod", "pat",
        "rub", "shop", "slip", "stop", "tip","clap", "drag", "plan", "rob", "skip", "slam", "spin", "step",
        "trap", "trim", "win", "wrap"
        // 多音节末尾重音动词
        "admit", "begin", "commit", "compel", "concur", "confer", "defer",
        "excel", "expel", "forget", "incur", "occur", "prefer", "rebel",
        "recur", "refer", "regret", "repel", "submit","control", "distil", "fulfil", "instil", "patrol", "propel",
        "refill", "uplift", "worship",
        // 特殊情况（传统用法）
        "cancel", "counsel", "dial", "equal", "fuel", "label", "level",
        "marvel", "model", "quarrel", "signal", "travel", "program", "target", "limit", "format", "benefit", "focus"
    };
    // 不规则形容词变化形式 (添加更多常见的不规则形容词)
    const QMap<QString, QPair<QString, QString>> Adjectivesirregular{
        {"good", {"better", "best"}},
        {"bad", {"worse", "worst"}},
        {"far", {"farther", "farthest"}},
        {"little", {"less", "least"}},
        {"many", {"more", "most"}},
        {"much", {"more", "most"}},
        // 添加以下不规则形容词
        {"old", {"elder", "eldest"}},
        {"late", {"later", "latest"}},
        {"well", {"better", "best"}},
        {"ill", {"worse", "worst"}},
        {"far", {"further", "furthest"}}  // alternative form
    };

    // 特殊规则：以-f或-fe结尾变为-ves的名词
    const QSet<QString> fToVesNouns{
        "leaf", "sheaf", "loaf", "thief", "wife", "wolf", "calf", "half",
        "elf", "self", "shelf", "life", "knife", "wife"
    };

    // 特殊规则：保持-f/-fs的名词
    const QSet<QString> keepFNouns{
        "roof", "belief", "chief", "cliff", "proof", "safe", "gulf"
    };
    int SentenceId=1;//句子id
    qint32 Id=1;//单词id
    int linecount=0;//读到第几个json对象
    int wordid=1;//mean_table表的id
    QMap<QString,int>WordMap;//单词和对应的id组成
    QMap<QString,int>SentenceMap;//句子和id
    QSqlDatabase db;
    int index=0;//当前最大序号
    std::vector<std::vector<int>> Tire;
    std::vector<int> cnt;

};
#endif // WIDGET_H
