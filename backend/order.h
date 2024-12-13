#ifndef ORDER_H
#define ORDER_H

#include <QObject>
#include <QUrl>
#include <QDate>
#include "book.h"
#include "jsonable.h"
#include "user.h"

class Order : public Jsonable
{
public:
    Order();

    // Serialization
    QJsonObject toJson() const override;

    // Setters
    void setUser(const User& newUser);
    void setCost(double newCost);
    void setDate(const QDate& newDate);
    void setInfo(const QString& newInfo);
    void addBook(const Book& newBook);  // Add a book to the order

    // Getters
    const User& getUser() const;
    double getCost() const;
    const QDate& getDate() const;
    const QString& getInfo() const;
    const QList<Book>& getBooks() const;  // Return the list of books

    void setId(qint64 newId);

private:
    qint64 id;
    User user;
    double cost;
    QDate date;
    QString info;
    QList<Book> books;  // List of books in the order
};

#endif // ORDER_H
