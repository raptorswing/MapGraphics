#include "SQLiteKeyValueStore.h"

#include <QtDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>


SQLiteKeyValueStore::SQLiteKeyValueStore()
{
}

SQLiteKeyValueStore::~SQLiteKeyValueStore()
{
}

bool SQLiteKeyValueStore::open(const QString &filename, QString *error)
{
    if (_db.isOpen())
        return true;

    _db = QSqlDatabase::addDatabase("QSQLITE",filename);
    _db.setDatabaseName(filename);

    if (!_db.open())
    {
        if (error)
            *error = "Failed to open database.";
        return false;
    }

    if (!_db.isOpen())
    {
        if (error)
            *error = "Still failed to open database";
        return false;
    }

    QSqlQuery query("CREATE TABLE IF NOT EXISTS keyValueStore (key BLOB PRIMARY KEY NOT NULL, value BLOB NOT NULL);",_db);
    if (!query.exec())
    {
        if (error)
            *error = "Failed to initialize database.";
        qDebug() << query.lastError();
        return false;
    }

    QSqlQuery query2("PRAGMA locking_mode = EXCLUSIVE;",_db);
    if (!query2.exec())
    {
        if (error)
            *error = "Failed to enable exclusive locking";
        return false;
    }

    QSqlQuery query3("PRAGMA auto_vacuum = FULL;",_db);
    if (!query3.exec())
    {
        if (error)
            *error = "Failed to enable auto_vacuum mode";
        return false;
    }

    QSqlQuery query4("VACUUM;",_db);
    if (!query4.exec())
    {
        if (error)
            *error = "Failed to do vaccuum";
        return false;
    }
    return true;
}

bool SQLiteKeyValueStore::close(QString *error)
{
    Q_UNUSED(error);

    if (!_db.isOpen())
        return true;

    _db.close();
    return true;
}

bool SQLiteKeyValueStore::store(const QByteArray &key, const QByteArray &value, QString *error)
{
    if (!_db.isOpen())
    {
        if (error)
            *error = "Database is not open/ready";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO keyValueStore (key, value) VALUES (:key,:value);");
    query.bindValue(":key",QVariant(key));
    query.bindValue(":value",QVariant(value));
    if (!query.exec())
    {
        if (error)
            *error = "Failed to store values. SQL error:" + query.lastError().text();
    }
    return true;
}

bool SQLiteKeyValueStore::retrieve(const QByteArray &key, QByteArray *valueOutput, QString *error)
{
    if (!_db.isOpen())
    {
        if (error)
            *error = "Database is not open/ready";
        return false;
    }

    if (!valueOutput)
    {
        if (error)
            *error = "Invalid null parameter";
        return false;
    }

    QSqlQuery query;
    if (!query.prepare("SELECT value FROM keyValueStore WHERE key == :key;"))
    {
        if (error)
            *error = "Failed to prepare query";
        return false;
    }

    query.bindValue(":key",QVariant(key));

    if (!query.exec())
    {
        if (error)
            *error = "Failed to retrieve key. SQL error:" + query.lastError().text();
        return false;
    }

    if (!query.isSelect() || !query.isActive())
    {
        if (error)
            *error = "Query error";
        return false;
    }

    if (!query.next())
    {
        if (error)
            *error = "No such key";
        return false;
    }

    *valueOutput = query.value(0).toByteArray();
    return true;
}
