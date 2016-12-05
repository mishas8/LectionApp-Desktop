#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H
#include <QAbstractItemModel>
#include <QTreeView>
#include <QSqlDatabase>
#include <QStringList>

enum h_type {ROOT=0, SEMESTR=1, SUBJECT=2, LECTURE=3, IMAGE=4};

struct DataWrapper // обертка
{
    unsigned int id;
    h_type type;
    void* data; // HData or IData
    int number; // index number
    DataWrapper* parent_pointer;
    QList <DataWrapper*> children;
    int children_count;
};

struct IData // Image Data
{
    QString path; // path to image
    QString comment; // about image
    QStringList tags;
};

struct HData // Hierarchy Data
{
    int type; // semestr, subject, lecture
    QString name;
    QString comment;
};

class ImageProvider : public QAbstractItemModel
{
private:
    DataWrapper root {0, ROOT, nullptr, 0, nullptr, {}, -1};
    int getChildrenCount(h_type type, int p_id) const;
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
