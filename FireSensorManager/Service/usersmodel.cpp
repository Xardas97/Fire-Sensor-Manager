#include "usersmodel.h"

UsersModel::UsersModel(std::shared_ptr<Database> database, QObject* parent)
    : QObject(parent), m_permissions(Permissions::None),
      m_database(database)
{ }

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
    return m_database->addUser(username, passphrase, permissions);
}

bool UsersModel::updateUserPermissions(QString username, Permissions permissions)
{
    return m_database->updateUserPermissions(username, permissions);
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
    return m_database->removeUser(username);
}
