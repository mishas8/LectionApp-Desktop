#include "imageprovider.h"
#include <stdexcept>
#include "QSql"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

using namespace std;

ImageProvider::ImageProvider(QString dbname, QObject *parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbname);
    if (!db.open()){
        throw invalid_argument("Cannot open db");
    }
    fetchAll (QModelIndex());
}

ImageProvider::~ImageProvider()
{
    db.close();
}

int ImageProvider::rowCount(const QModelIndex &parent) const
{
    const DataWrapper *parent_pointer = dataForIndex (parent);
    return parent_pointer->children_count;
}

int ImageProvider::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED (parent);
    return 1;
}

QModelIndex ImageProvider::index(int row, int column, const QModelIndex &parent) const
{
    if (column != 0) {
            return QModelIndex();
        }
        if (!parent.isValid()) {
            return createIndex (row, column, root.children[row]);
        }
        const DataWrapper *parent_pointer = dataForIndex (parent);
        if (parent_pointer->type == IMAGE) {
            return QModelIndex();
        }
        if (parent_pointer->children.size() <= row){
            return QModelIndex();
        }
        return createIndex (row, column, parent_pointer->children[row]);
}

const DataWrapper *ImageProvider::dataForIndex(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return &root;
    }
    return static_cast<DataWrapper *>(index.internalPointer());
}

DataWrapper *ImageProvider::dataForIndex (const QModelIndex &index)
{
    if (!index.isValid()) {
        return &root;
    }
    return static_cast<DataWrapper *> (index.internalPointer());
}

QVariant ImageProvider::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return {};
    if (role == Qt::DisplayRole) {
         if (index.isValid()) {
             const DataWrapper *elem = dataForIndex (index);
             switch (elem->type) {
                 case SEMESTR:
                 case SUBJECT:
                 case LECTURE:
                     return static_cast<HData*> (elem->data)->name;
                 case IMAGE:
                     return static_cast<IData*> (elem->data)->path;
                 default:
                     break;
             }
         }
     }
    if (role == Qt::DecorationRole||Qt::SizeHintRole){
         if (index.isValid()) {
             const DataWrapper *elem = dataForIndex (index);
                 if (elem->type == IMAGE){
                     QPixmap pix;
                     pix.load(static_cast<IData*> (elem->data)->path);
                     if (role == Qt::DecorationRole){
                         return pix;
                     }
                     else {
                         return pix.size()/2;
                     }
             }
         }
     }
    return QVariant();
}

int ImageProvider::getChildrenCount (h_type type, int p_id) const
{
    QSqlQuery query;
    switch (type) {
    case ROOT:
    case SEMESTR:
    case SUBJECT:
        query.prepare ("SELECT COUNT (*) from hierarchy where pid = :id ");
        break;
    case LECTURE:
        query.prepare ("SELECT COUNT (*) from images where pid = :id ");
        break;
    case IMAGE:
        return 0;
    default:
        break;
    }
    query.bindValue (":id", p_id);
    query.exec();
    query.next();
    //qDebug() << query.executedQuery();
    //qDebug() << query.lastError();
    int count = query.value (0).toInt();
    return count;
}

QModelIndex ImageProvider::parent(const QModelIndex &child) const
{
    if (!child.isValid()) return QModelIndex();
    const DataWrapper *child_pointer = dataForIndex (child);
    if (!child_pointer->parent_pointer) return QModelIndex();
    return createIndex (child_pointer->parent_pointer->number, 0,
                        static_cast<void *> (child_pointer->parent_pointer));

}

void ImageProvider::fetchMore (const QModelIndex &parent)
{
    fetchAll (parent);
}

bool ImageProvider::canFetchMore (const QModelIndex &parent) const
{
    const DataWrapper *data = dataForIndex (parent);
    return data->children.size() < data->children_count;
}

void ImageProvider::fetchAll (const QModelIndex &parent)
{
    DataWrapper *data = dataForIndex (parent);
    data->children.clear();
    QSqlQuery query;
    if (data->type != LECTURE) {
        query.prepare ("SELECT * from hierarchy where pid = :id ORDER BY number");
    } else {
        query.prepare ("SELECT * from images where pid = :id ORDER BY number");
    }
    query.bindValue (":id", data->id);
    query.exec();
    while (query.next()) {
        auto id = query.value("id").toUInt();
        auto comment = query.value("Comment").toString();
        //QStringList tags = query.value("Tags").toStringList();
        //auto number = query.value("Number").toInt();
        QStringList tags;
        int number;

        if (data->type == SEMESTR)
            tags = query.value("Tags").toStringList();
        if (data->type != SEMESTR)
            number = query.value("Number").toInt();
        else
            number = query.value("No").toInt();

        switch (data->type) {
        case ROOT:
        case SEMESTR:
        case SUBJECT: {
            auto type = query.value ("Type").toInt();
            auto name = query.value ("Name").toString();
            data->children.append (
                        new DataWrapper{id, (h_type)type,
                                    new HData{type, name, comment},
                                    number, data, {}, getChildrenCount((h_type)type, id)});
            break;
        }
        case LECTURE: {
            auto path = query.value ("path").toString();
            data->children.append (
                        new DataWrapper{id, IMAGE, new IData{path, comment, tags},
                                    number, data, {}, getChildrenCount(IMAGE, id)});
            break;
        }
        default:
            break;
        }
    }
    data->children_count = data->children.size();
}
