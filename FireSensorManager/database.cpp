#include "database.h"

#include "Communication/sensor.h"

#include <QDebug>
#include <QBuffer>
#include <QPixmap>
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

    auto success = createMapsTable() || createSensorsTable();
    if (!success)
    {
        qWarning() << "Creating tables failed!";
        return false;
    }

    qDebug() << "Finished creating missing tables";
    return true;
}

bool Database::createMapsTable()
{
    auto queryText = "CREATE TABLE IF NOT EXISTS maps "
                     "("
                     "  id    INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "  floor INTEGER,"
                     "  image BLOB"
                     ")";

    QSqlQuery query;
    auto res = query.exec(queryText);
    if (!res)
    {
        qWarning() << "Creating maps table failed: " << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::createSensorsTable()
{
    auto queryText = "CREATE TABLE IF NOT EXISTS sensors "
                     "("
                     "  uuid                  TEXT        PRIMARY KEY,"
                     "  capabilities          INTEGER,"
                     "  address               TEXT,"
                     "  port                  INTEGER,"
                     "  name                  TEXT, "
                     "  temperature_threshold INTEGER,"
                     "  co2_threshold         INTEGER,"
                     "  pollution_threshold   INTEGER"
                     ")";

    QSqlQuery query;
    auto res = query.exec(queryText);
    if (!res)
    {
        qWarning() << "Creating sensors table failed: " << query.lastError().text();
        return false;
    }

    return true;
}

void Database::loadMaps(std::function<void(int, const QPixmap&)> addFunc)
{
    std::unordered_map<int, std::vector<QPixmap>> maps;

    QSqlQuery query;
    query.prepare("SELECT * FROM maps");

    auto success = query.exec();
    if (!success)
    {
        qWarning() << "Failed to load map from database!";
        return;
    }

    while (query.next())
    {
        auto floor = query.value("floor").toInt();

        auto imageBytes = query.value("image").toByteArray();
        QPixmap map;
        map.loadFromData(imageBytes);

        addFunc(floor, map);
    }

    return;
}

void Database::saveMaps(const std::unordered_map<int, std::vector<QPixmap>>& maps)
{
    qDebug() << "Deleting current database maps...";

    auto success = QSqlQuery{}.exec("DELETE FROM maps");
    if (!success)
    {
        qWarning() << "Failed to delete maps from the dabatase!";
        return;
    }

    qDebug() << "Inserting new maps into the database...";

    for (auto& floorMaps: maps)
    {
        auto floor = floorMaps.first;
        for (auto& map: floorMaps.second)
        {
            saveMap(floor, map);
        }
    }

    qDebug() << "Finished adding maps to database";
}

void Database::saveMap(int floor, const QPixmap& pixmap)
{
    auto queryText = "INSERT INTO maps "
                     "(floor, image) "
                     "VALUES (:floor, :image)";

    QByteArray inByteArray;
    QBuffer inBuffer {&inByteArray};
    inBuffer.open(QIODevice::WriteOnly);
    pixmap.save(&inBuffer, "PNG");

    QSqlQuery query;
    query.prepare(queryText);
    query.bindValue(":floor", floor);
    query.bindValue(":image", inByteArray);

    auto success = query.exec();
    if (!success)
    {
        qWarning() << "Failed to insert map into the dabatase: " << query.lastError().text();
        return;
    }
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

    auto success = QSqlQuery{}.exec("DELETE FROM sensors");
    if (!success)
    {
        qWarning() << "Failed to delete sensors from the dabatase!";
        return;
    }

    qDebug() << "Inserting new sensors into the database...";

    for (auto& sensor: sensors)
    {
        saveSensor(*sensor);
    }

    qDebug() << "Finished inserting sensors into the databse";
}

bool Database::loadSensorData(Sensor& sensor)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM sensors WHERE uuid = :uuid");
    query.bindValue(":uuid", sensor.uuid());

    auto success = query.exec();
    if (!success)
        return false;

    if (query.next())
    {
        sensor.setTemperatureThreshold(query.value("temperature_threshold").toInt());
        sensor.setCo2ConcentrationThreshold(query.value("co2_threshold").toInt());
        sensor.setPollutionThreshold(query.value("pollution_threshold").toInt());
        return true;
    }

    return false;
}

void Database::saveSensorData(const Sensor& sensor)
{
    QSqlQuery query;
    query.prepare("DELETE FROM sensors WHERE uuid = :uuid");
    query.bindValue(":uuid", sensor.uuid());
    query.exec();

    saveSensor(sensor);
}

void Database::saveSensor(const Sensor& sensor)
{
    auto queryText = "INSERT INTO sensors "
                     "(uuid, capabilities, address, port, name, temperature_threshold, co2_threshold, pollution_threshold) "
                     "VALUES (:uuid, :capabilities, :address, :port, :name, :temperature_threshold, :co2_threshold, :pollution_threshold)";

    QSqlQuery query;
    query.prepare(queryText);
    query.bindValue(":uuid",                  sensor.uuid());
    query.bindValue(":capabilities",          sensor.capabilities().toInt());
    query.bindValue(":address",               sensor.address().toString());
    query.bindValue(":port",                  sensor.port());
    query.bindValue(":name",                  sensor.name());
    query.bindValue(":temperature_threshold", sensor.temperatureThreshold());
    query.bindValue(":co2_threshold",         sensor.co2ConcentrationThreshold());
    query.bindValue(":pollution_threshold",   sensor.pollutionThreshold());

    auto success = query.exec();
    if (!success)
    {
        qWarning() << "Failed to insert new sensors into the dabatase: " << query.lastError().text();
        return;
    }
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

