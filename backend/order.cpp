#include "order.h"

Order::Order() : cost(0.0) {}

QJsonObject Order::toJson() const
{
    QJsonObject json;
    json["id"] = id;
    json["user"] = user.toJson();
    json["cost"] = cost >= 0 ? std::round(cost * 100) / 100.0 : QJsonValue::Null;
    json["date"] = date.toString("yyyy-MM-dd");
    json["info"] = info;

    QJsonArray booksArray;
    for (const auto& book : books) {
        booksArray.append(book.toJson());
    }
    json["books"] = booksArray;

    return json;
}

void Order::setUser(const User& newUser)
{
    user = newUser;
}

void Order::setCost(double newCost)
{
    cost = newCost;
}

void Order::setDate(const QDate& newDate)
{
    date = newDate;
}

void Order::setInfo(const QString& newInfo)
{
    info = newInfo;
}

void Order::addBook(const Book& newBook)
{
    books.append(newBook);
}

const User& Order::getUser() const
{
    return user;
}

double Order::getCost() const
{
    return cost;
}

const QDate& Order::getDate() const
{
    return date;
}

const QString& Order::getInfo() const
{
    return info;
}

const QList<Book>& Order::getBooks() const
{
    return books;
}

void Order::setId(qint64 newId)
{
    id = newId;
}
