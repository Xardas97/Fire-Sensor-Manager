#ifndef USERSMODEL_H
#define USERSMODEL_H

#include "database.h"

#include <memory>
#include <vector>
#include <QAbstractListModel>

class UsersModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool    isLoggedIn          READ isLoggedIn          NOTIFY loggedUserChanged)
    Q_PROPERTY(QString loggedUsername      READ loggedUsername      NOTIFY loggedUserChanged)
    Q_PROPERTY(bool    hasModPermissions   READ hasModPermissions   NOTIFY loggedUserChanged)
    Q_PROPERTY(bool    hasAdminPermissions READ hasAdminPermissions NOTIFY loggedUserChanged)

public:
    explicit UsersModel(std::shared_ptr<Database> database, QObject* parent = nullptr);

    bool    isLoggedIn() const;
    QString loggedUsername() const;
    bool    hasModPermissions() const;
    bool    hasAdminPermissions() const;

public slots:
    bool logIn(QString username, QString passphrase);
    void logOut();

    bool addUser(QString username, QString passphrase, Permissions permissions);
    bool updateUserPassphrase(QString username, QString oldPassphrase, QString newPassphrase);
    bool updateUserPermissions(QString username, Permissions permissions);
    bool removeUser(QString username);

signals:
    void loggedUserChanged();

private:
    std::optional<QString> m_loggedUsername;
    Permissions            m_permissions;

    std::shared_ptr<Database> m_database;

};

#endif // USERSMODEL_H
