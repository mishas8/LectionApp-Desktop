#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H
#include <QAbstractItemModel>
#include <QTreeView>
#include <QSqlDatabase>
#include <QStringList>

enum h_type {ROOT, SEMESTR, SUBJECT, LECTURE, IMAGE};

struct DataWrapper // обертка
{
    int id;
    h_type type;
    void* data; // HData or IData
    int number; // index number
    DataWrapper* parent_pointer;
    QList <DataWrapper*> children;
    int children_count;
};

struct IData // Image Data
{
    int id;
    int p_id; // parent id
    int number; // index number
    QString path; // path to image
    QString comment; // about image
    QVector <QString> tags;
};

struct HData // Hierarchy Data
{
    int id; // Primary key
    int p_id; // Parent id
    QString type; // semestr, subject, lecture
    QString name;
    QString comment;
    int number; // index number
};

class ImageProvider : public QAbstractItemModel
{
private:
    DataWrapper root {0, ROOT, nullptr, 0, nullptr, {}, 0};
    int getChildCount(h_type type, int parentId) const;
    const DataWrapper* dataForIndex(const QModelIndex &index) const;
    DataWrapper* dataForIndex(const QModelIndex &index);
    QSqlDatabase db;

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
