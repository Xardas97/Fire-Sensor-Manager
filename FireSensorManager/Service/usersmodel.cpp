#include "usersmodel.h"

UsersModel::UsersModel(std::shared_ptr<Database> database, QObject* parent)
    : QAbstractListModel(parent), m_permissions(Permissions::None),
      m_database(database)
{
    m_users = m_database->users();
}

QVariant UsersModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || (role != Roles::UsernameRole && role != Roles::PermissionsRole))
        return QVariant();

    auto& user = m_users[index.row()];

    if (role == Roles::UsernameRole)
        return user.username;

    return user.permissions;
}

QHash<int, QByteArray> UsersModel::roleNames() const
{
    static QHash<int, QByteArray> mapping {{Roles::UsernameRole, "username"}, {Roles::PermissionsRole, "permissions"}};
    return mapping;
}

int UsersModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_users.size();
}

bool UsersModel::isLoggedIn() const
{
    return m_loggedUsername.has_value();
}

QString UsersModel::loggedUsername() const
{
    if (!m_loggedUsername.has_value())
        return QString{};

    return *m_loggedUsername;
}

bool UsersModel::hasModPermissions() const
{
    return m_permissions == Permissions::Admin ||
           m_permissions == Permissions::Moderator;
}

bool UsersModel::hasAdminPermissions() const
{
    return m_permissions == Permissions::Admin;
}

bool UsersModel::logIn(QString username, QString passphrase)
{
    auto authenticated = m_database->authenticateUser(username, passphrase, m_permissions);
    if (!authenticated)
        return false;

    m_loggedUsername = username;
    emit loggedUserChanged();
    return true;
}

void UsersModel::logOut()
{
    qDebug() << "Logging out...";

    m_loggedUsername.reset();
    m_permissions = Permissions::None;

    emit loggedUserChanged();
}

bool UsersModel::addUser(QString username, QString passphrase, Permissions permissions)
{
    auto added = m_database->addUser(username, passphrase, permissions);
    if (!added)
        return false;

    beginInsertRows(QModelIndex(), m_users.size(), m_users.size());
    m_users.emplace_back(username, permissions);
    endInsertRows();

    return true;
}

bool UsersModel::updateUserPermissions(QString username, Permissions permissions)
{
    auto found = find(username);
    if (found == m_users.end())
        return false;

    if (found->permissions == permissions)
        return true;

    auto updated = m_database->updateUserPermissions(username, permissions);
    if (!updated)
        return false;

    found->permissions = permissions;
    auto row = found - m_users.begin();
    emit dataChanged(index(row), index(row));

    return true;
}

bool UsersModel::updateUserPassphrase(QString username, QString oldPassphrase, QString newPassphrase)
{
    Permissions temp;
    auto authenticated = m_database->authenticateUser(username, oldPassphrase, temp);
    if (!authenticated)
        return false;

    return m_database->updateUserPassphrase(username, newPassphrase);
}

bool UsersModel::removeUser(QString username)
{
    auto found = find(username);
    if (found == m_users.end())
        return false;

    auto removed = m_database->removeUser(username);
    if (!removed)
        return false;

    auto row = found - m_users.begin();

    beginRemoveRows(QModelIndex(), row, row);
    m_users.erase(found);
    endRemoveRows();

    return true;
}

std::vector<User>::iterator UsersModel::find(const QString& username)
{
    return std::find_if(m_users.begin(),
                        m_users.end(),
                        [&username](const User& user) { return user.username == username; });
}
