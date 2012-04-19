#ifndef SQLITEKEYVALUESTORE_H
#define SQLITEKEYVALUESTORE_H

#include <QSqlDatabase>
#include <QByteArray>
#include <QSharedPointer>

class SQLiteKeyValueStore
{
public:
    SQLiteKeyValueStore();
    ~SQLiteKeyValueStore();

    /**
     * @brief Open a database for read/write. Returns true on success false on failure.
     * Takes the name of the database to open and an optional error string pointer.
     *
     * @param filename the filename of the database
     * @param error optional error string pointer if you need to know what went wrong.
     * @return bool true on success, false on failure
     */
    bool open(const QString& filename, QString * error=0);

    bool close(QString * error=0);

    bool store(const QByteArray& key, const QByteArray& value, QString * error=0);

    bool retrieve(const QByteArray& key, QByteArray * valueOutput, QString * error=0);

private:
    QSqlDatabase _db;
};

#endif // SQLITEKEYVALUESTORE_H
