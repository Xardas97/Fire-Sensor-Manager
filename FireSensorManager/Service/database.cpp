#include "database.h"

#include "Map/mapentry.h"
#include "Communication/sensor.h"

#include <vector>
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
    loadData();
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

    auto success = createMapsTable() && createSensorsTable();
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
                     "  pollution_threshold   INTEGER,"
                     "  xPlacement            INTEGER,"
                     "  yPlacement            INTEGER,"
                     "  mapId                 INTEGER"
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

void Database::loadData()
{
    qDebug() << "Loading all data from the database...";
    loadMapsAndTheirSensors();
    loadMaplessSensors();
    qDebug() << "Finished loading all data from the database";
}

void Database::loadMapsAndTheirSensors()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM maps");

    auto success = query.exec();
    if (!success)
    {
        qWarning() << "Failed to load maps from the database: " << query.lastError().text();
        return;
    }

    while (query.next())
    {
        loadMapItsSensors(query.record());
    }
}

void Database::loadMapItsSensors(const QSqlRecord& record)
{
    auto id = record.value("id").toInt();
    auto floor = record.value("floor").toInt();

    auto imageBytes = record.value("image").toByteArray();
    QPixmap mapImage;
    mapImage.loadFromData(imageBytes);

    auto floorMapsIterator = m_maps.find(floor);
    if (floorMapsIterator == m_maps.end())
    {
        floorMapsIterator = m_maps.emplace(std::make_pair(floor, std::vector<MapEntry>{})).first;
    }

    MapEntry& entry = floorMapsIterator->second.emplace_back(id, floor, mapImage);
    loadSensorsForMap(entry);
}

void Database::loadSensorsForMap(MapEntry& map)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM sensors WHERE mapId = :mapId");
    query.bindValue(":mapId", map.id());

    auto success = query.exec();
    if (!success)
    {
        qWarning() << "Failed to load sensors from the dabatase: " << query.lastError().text();
        return;
    }

    while (query.next())
    {
        std::shared_ptr<Sensor> sensor {Sensor::fromSqlRecord(query.record())};
        map.addSensor(sensor);
        m_sensors.push_back(sensor);
    }
}

void Database::loadMaplessSensors()
{
    QSqlQuery query;
    auto success = query.exec("SELECT * FROM sensors WHERE mapId IS NULL");
    if (!success)
    {
        qWarning() << "Failed to load mapless sensors from the dabatase: " << query.lastError().text();
        return;
    }

    while (query.next())
    {
        m_sensors.push_back({Sensor::fromSqlRecord(query.record())});
    }
}

std::unordered_map<int, std::vector<MapEntry>>& Database::maps()
{
    return m_maps;
}

std::vector<std::shared_ptr<Sensor>>& Database::sensors()
{
    return m_sensors;
}

void Database::saveData()
{
    saveMaps();
    saveSensors();
}

void Database::saveMaps()
{
    qDebug() << "Deleting current database maps...";

    QSqlQuery query;
    auto success = query.exec("DELETE FROM maps");
    if (!success)
    {
        qWarning() << "Failed to delete maps from the dabatase: " << query.lastError().text();
        return;
    }

    qDebug() << "Inserting new maps into the database...";

    for (auto& floorMaps: m_maps)
    {
        auto floor = floorMaps.first;
        for (auto& map: floorMaps.second)
        {
            saveMap(floor, map);
        }
    }

    qDebug() << "Finished adding maps to database";
}

void Database::saveMap(int floor, MapEntry& map)
{
    auto queryText = "INSERT INTO maps "
                     "(floor, image) "
                     "VALUES (:floor, :image)";

    QByteArray inByteArray;
    QBuffer inBuffer {&inByteArray};
    inBuffer.open(QIODevice::WriteOnly);
    map.pixmap().save(&inBuffer, "PNG");

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

    map.setId(query.lastInsertId().toInt());
}

void Database::saveSensors()
{
    qDebug() << "Deleting current database sensors...";

    QSqlQuery query;
    auto success = query.exec("DELETE FROM sensors");
    if (!success)
    {
        qWarning() << "Failed to delete sensors from the dabatase: " << query.lastError().text();
        return;
    }

    qDebug() << "Inserting new sensors into the database...";

    for (auto& sensor: m_sensors)
    {
        saveSensor(*sensor);
    }

    qDebug() << "Finished inserting sensors into the databse";
}

void Database::saveSensor(Sensor& sensor)
{
    auto queryText = "INSERT INTO sensors "
                     "(uuid, capabilities, address, port, name, temperature_threshold, co2_threshold, pollution_threshold, xPlacement, yPlacement, mapId) "
                     "VALUES (:uuid, :capabilities, :address, :port, :name, :temperature_threshold, :co2_threshold, :pollution_threshold, :xPlacement, :yPlacement, :mapId)";

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
    query.bindValue(":xPlacement",            sensor.x());
    query.bindValue(":yPlacement",            sensor.y());

    if (sensor.map())
        query.bindValue(":mapId", sensor.map()->id());
    else
        query.bindValue(":mapId", QVariant{});


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
    saveData();
    close();
}

