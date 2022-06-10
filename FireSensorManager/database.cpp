#include "database.h"

#include "Communication/sensor.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QStandardPaths>

Database::Database(QObject *parent)
    : QObject{parent}
{
    open();
    createTables();
}

bool Database::open()
{
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    auto databaseName = "/database.db";
    auto fullPath = path + databaseName;

    qDebug() << "Opening database at: " << fullPath;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fullPath);

    bool ok = db.open();
    if (!ok)
    {
        auto error = db.lastError();
        qDebug() << "Error: " << error;
        return false;
    }

    qDebug() << "Database successfully opened!";
    return true;
}

bool Database::createTables()
{
    qDebug() << "Creating missing database tables...";

    auto queryText = "CREATE TABLE IF NOT EXISTS sensors "
                     "("
                     "  uuid                  TEXT        PRIMARY KEY, "
                     "  capabilities          INTEGER,"
                     "  address               TEXT,"
                     "  port                  INTEGER,"
                     "  name                  TEXT, "
                     "  temperature_threshold INTEGER,"
                     "  co2_threshold         INTEGER,"
                     "  pollution_threshold   INTEGER"
                     ")";

    auto res = QSqlQuery{}.exec(queryText);
    if (!res)
    {
        qWarning() << "Creating sensors table failed!";
        return false;
    }

    qDebug() << "Finished creating missing tables";
    return true;
}

std::vector<std::unique_ptr<Sensor>> Database::loadSensors()
{
    std::vector<std::unique_ptr<Sensor>> sensors;

    QSqlQuery query;
    auto success = query.exec("SELECT * FROM sensors");
    if (!success)
    {
        qWarning() << "Failed to load sensors from the dabatase!";
        return sensors;
    }

    while (query.next())
    {
        sensors.push_back(Sensor::fromSqlRecord(query.record()));
    }

    qDebug() << "Finished loading sensors from the database";
    return sensors;
}

void Database::saveSensors(const std::vector<std::shared_ptr<Sensor>>& sensors)
{
    qDebug() << "Deleting current database sensors...";

    QSqlQuery query;
    auto success = query.exec("DELETE FROM sensors");
    if (!success)
    {
        qWarning() << "Failed to delete sensors from the dabatase!";
        return;
    }

    qDebug() << "Inserting new sensors into the database...";

    for (auto& sensor: sensors)
    {
        auto queryText = "INSERT INTO sensors "
                         "(uuid, capabilities, address, port, name, temperature_threshold, co2_threshold, pollution_threshold) "
                         "VALUES (:uuid, :capabilities, :address, :port, :name, :temperature_threshold, :co2_threshold, :pollution_threshold)";

        query.prepare(queryText);
        query.bindValue(":uuid",                sensor->uuid());
        query.bindValue(":capabilities",        sensor->capabilities().toInt());
        query.bindValue(":address",             sensor->address().toString());
        query.bindValue(":port",                sensor->port());
        query.bindValue(":name",                sensor->name());
        query.bindValue(":temperature_threshold", sensor->temperatureThreshold());
        query.bindValue(":co2_threshold",       sensor->co2ConcentrationThreshold());
        query.bindValue(":pollution_threshold", sensor->pollutionThreshold());

        success = query.exec();
        if (!success)
        {
            qWarning() << "Failed to insert new sensors into the dabatase: " << query.lastError().text();
            return;
        }
    }

    qDebug() << "Finished inserting sensors into the databse";
}

void Database::close()
{
    qDebug() << "Closing and removing database...";

    auto db = QSqlDatabase::database();

    db.close();
    QSqlDatabase::removeDatabase(db.databaseName());

    qDebug() << "Database closed and removed!";
}

Database::~Database()
{
    close();
}

