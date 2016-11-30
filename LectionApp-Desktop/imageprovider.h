#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H
#include <QAbstractItemModel>
#include <QTreeView>
//#include <QSqlDatabase>
#include <QStringList>

enum dataType {ROOT, SEMESTR, COURSE, LECTURE, IMAGE};

struct DataWrapper // обертка
{
    int id;
    dataType type;
    void* data;
    int number;
    DataWrapper* parentPointer;
    QList <DataWrapper*> children;
    int childrenCount;
};

struct IData // Image Data
{
    int id;
    int p_id; // parentId
    int number;
    QString path; // path to image
    QString comment;
    QVector <QString> tags;
};

struct HData // Href Data
{
    int id;
    int p_id;
    QString type;
    QString name;
    QString comment;
    int number;
};

class ImageProvider : public QAbstractItemModel
{
private:
    DataWrapper root {0, ROOT, nullptr, 0, nullptr, {}, 0};
    int getChildCount(dataType type, int parentId) const;
    const DataWrapper* dataForIndex(const QModelIndex &index) const;
    //DataWrapper* dataForIndex(const QModelIndex &index);

public:
    ImageProvider(QString dbname, QObject *parent = nullptr);

    ~ImageProvider();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const; // QVariant обертка, в нее можно завернуть объект любого типа
    virtual QModelIndex parent(const QModelIndex &child) const;
    void fetchAll(const QModelIndex &parent);
    void fetchMore(const QModelIndex &parent);
    bool canFetchMore(const QModelIndex &parent) const;
};

#endif // IMAGEPROVIDER_H
