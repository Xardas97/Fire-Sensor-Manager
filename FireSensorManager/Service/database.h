#ifndef DATABASE_H
#define DATABASE_H

#include <tuple>
#include <vector>
#include <QObject>
#include <functional>
#include <unordered_map>

class Sensor;
class MapEntry;
class QSqlRecord;

class FailedToUseDatabaseException : public std::exception {};

enum Permissions : unsigned short { None = 0, Moderator = 1, Admin = 2 };

struct UserSettings
{
    int   alarm    = 0;
    float volume   = 0.5;
    int   uiTheme  = 1;
    int   uiAccent = 1;
};

struct User {
    User(QString username, Permissions permissions) : username(username), permissions(permissions) {}
    QString username;
    Permissions permissions;
};

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    auto maps() -> std::unordered_map<int, std::vector<std::shared_ptr<MapEntry>>>&;
    auto sensors() -> std::vector<std::shared_ptr<Sensor>>&;
    auto users() -> std::vector<User>;

    bool authenticateUser(QString username, QString passphrase, Permissions& permissions);
    bool addUser(QString username, QString passphrase, Permissions permissions);
    bool updateUserPermissions(QString username, Permissions permissions);
    bool updateUserPassphrase(QString username, QString passphrase);
    bool removeUser(QString username);

    void saveData();

    auto loadUserSettings(QString username) -> UserSettings;
    void saveAlarmData(QString username, int alarm, float volume);
    void saveAppearanceData(QString username, int theme, int accent);

private:
    bool open();
    void close();

    bool createTables();
    bool createMapsTable();
    bool createUsersTable();
    bool createSensorsTable();

    bool createDefaultAdminUser();

    void loadData();
    void loadMapsAndTheirSensors();
    void loadMapItsSensors(const QSqlRecord& record);
    void loadSensorsForMap(MapEntry& map);
    void loadMaplessSensors();

    void saveMaps();
    void saveMap(int floor, MapEntry& pixmap);
    void saveSensors();
    void saveSensor(Sensor& sensor);

    QByteArray hashPassphrase(const QString& passphrase);

    std::vector<std::shared_ptr<Sensor>> m_sensors;
    std::unordered_map<int, std::vector<std::shared_ptr<MapEntry>>> m_maps;
};

#endif // DATABASE_H
