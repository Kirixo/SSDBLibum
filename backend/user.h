#ifndef USER_H
#define USER_H
#include "dbcontroller.h"
#include <QtSql/QSqlQuery>
#include "jsonable.h"
#include "has_method.h"

class User : Jsonable
{
public:
    User();
    User(quint64 id);
    User(const QString& email, const QString& login, const QString& password);
    User(const User& user);
    User(User&& user);
    User& operator=(const User& user);
    ~User();

    bool saveInDB();
    // void saveAndroidID(QString& email, QString androidID);
    // bool checkAndroidID(QString& androidID);
    // void deleteAndroidID(QString& email);
    void authorize(const QString& email, const QString& password);
    void fetchByID(quint64 id);
    bool checkExistanceInDB();
    bool exists();


    void setId(qint64 newId);
    void setEmail(const QString &newEmail);
    void setLogin(const QString &newLogin);
    void setPassword(const QString &newPassword);

    qint64 id() const;
    QString email() const;
    QString login() const;



private:
    qint64 id_;
    QString email_;
    QString login_;
    QString password_;


    template <typename... T>
    requires (HasIsNull<T> && ...)
    bool checkNullVariables(T&&... variables) const;

    // Jsonable interface
public:
    QJsonObject toJson() const override;
};

#endif // USER_H
