#include "orderrepository.h"
#include "dbcontroller.h"
#include "logger.h"
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qsqlquery.h>

OrderRepository::OrderRepository() {}

std::optional<Order> OrderRepository::fetchOrderById(int id)
{
    QSqlDatabase db = DBController::getDatabase();
    if (!db.isOpen()) {
        Logger::instance().log(QString("[fetchOrderById] Database not open!"), Logger::LogLevel::Warning);
        return std::nullopt;
    }

    QString queryString = R"(
        SELECT o.id, o.user_id, u.login AS user_name, o.cost, o.date, o.info
        FROM orders AS o
        LEFT JOIN users AS u ON o.user_id = u.id
        WHERE o.id = :id
    )";

    QSqlQuery query(db);
    query.prepare(queryString);
    query.bindValue(":id", id);

    if (!query.exec() || !query.next()) {
        Logger::instance().log(QString("[fetchOrderById] Database query error!")
                                   .append(query.lastError().text()), Logger::LogLevel::Warning);
        return std::nullopt;
    }

    // Retrieve the order details
    Order order;
    User user;
    order.setId(query.value("id").toInt());
    user.setId(query.value("user_id").toInt());
    user.setLogin(query.value("user_name").toString());
    order.setUser(user);
    order.setCost(query.value("cost").toDouble());
    order.setDate(query.value("date").toDate());
    order.setInfo(query.value("info").toString());

    // Fetch the list of books in the order
    QList<Book> books = fetchBooksForOrder(order.getUser().id());  // Pass the user_id as order_id here if needed

    for (const Book& book : books) {
        order.addBook(book);
    }

    return order;
}

QList<Book> OrderRepository::fetchBooksForOrder(int orderId)
{
    QSqlDatabase db = DBController::getDatabase();
    if (!db.isOpen()) {
        Logger::instance().log(QString("[fetchBooksForOrder] Database not open!"), Logger::LogLevel::Warning);
        return {};
    }

    QString queryString = R"(
        SELECT b.id, b.title
        FROM orders_content AS oc
        LEFT JOIN books AS b ON oc.book_id = b.id
        WHERE oc.order_id = :orderId
    )";

    QSqlQuery query(db);
    query.prepare(queryString);
    query.bindValue(":orderId", orderId);

    QList<Book> books;
    if (query.exec()) {
        while (query.next()) {
            Book book;
            book.setId(query.value("id").toInt());
            book.setTitle(query.value("title").toString());
            books.append(book);
        }
    } else {
        Logger::instance().log(QString("[fetchBooksForOrder] Database query error!")
                                   .append(query.lastError().text()), Logger::LogLevel::Warning);
    }

    return books;
}

QList<Order> OrderRepository::fetchOrders(int limit, int page)
{
    QSqlDatabase db = DBController::getDatabase();
    if (!db.isOpen()) {
        Logger::instance().log(QString("[fetchOrders] Database not open!"), Logger::LogLevel::Warning);
        return {};
    }

    QString queryString = R"(
        SELECT o.id, o.user_id, u.login AS user_name, o.cost, o.date, o.info
        FROM orders AS o
        LEFT JOIN users AS u ON o.user_id = u.id
        ORDER BY o.id DESC
        LIMIT :limit OFFSET :offset
    )";

    QSqlQuery query(db);
    query.prepare(queryString);
    query.bindValue(":limit", limit);
    query.bindValue(":offset", (page - 1) * limit);

    QList<Order> orders;
    if (query.exec()) {
        while (query.next()) {
            Order order;
            User user;
            order.setId(query.value("id").toInt());
            user.setId(query.value("user_id").toInt());
            user.setLogin(query.value("user_name").toString());
            order.setUser(user);
            order.setCost(query.value("cost").toDouble());
            order.setDate(query.value("date").toDate());
            order.setInfo(query.value("info").toString());

            // Fetch the books for the order
            QList<Book> books = fetchBooksForOrder(order.getUser().id());  // Pass user_id as order_id
            for (const Book& book : books) {
                order.addBook(book);
            }

            orders.append(std::move(order));
        }
    } else {
        Logger::instance().log(QString("[fetchOrders] Database query error!")
                                   .append(query.lastError().text()), Logger::LogLevel::Warning);
    }

    return orders;
}

int OrderRepository::getOrdersCount()
{
    QSqlDatabase db = DBController::getDatabase();

    QString queryString = R"(
        SELECT COUNT(1)
        FROM orders;
    )";

    QSqlQuery query(db);
    query.prepare(queryString);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}

std::optional<int> OrderRepository::getOrderWithMostBooksByAuthor(const QString& authorName)
{
    QSqlDatabase db = DBController::getDatabase();
    if (!db.isOpen()) {
        Logger::instance().log(QString("[getOrderWithMostBooksByAuthor] Database not open!"), Logger::LogLevel::Warning);
        return std::nullopt;
    }

    QString queryString = R"(
        SELECT get_order_with_most_books_by_author(:authorName) AS order_id;
    )";

    QSqlQuery query(db);
    query.prepare(queryString);
    query.bindValue(":authorName", authorName);

    if (query.exec() && query.next()) {
        int orderId = query.value("order_id").toInt();
        if (orderId > 0) {
            return orderId;
        }
    }

    return std::nullopt;
}

QList<Order> OrderRepository::getLargestOrdersByEmail(const QString& userEmail)
{
    QSqlDatabase db = DBController::getDatabase();
    if (!db.isOpen()) {
        Logger::instance().log(QString("[getLargestOrdersByEmail] Database not open!"), Logger::LogLevel::Warning);
        return {};
    }

    QString queryString = R"(
        SELECT order_id, cost, date FROM get_largest_orders_by_email(:userEmail);
    )";

    QSqlQuery query(db);
    query.prepare(queryString);
    query.bindValue(":userEmail", userEmail);

    QList<Order> orders;
    if (query.exec()) {
        while (query.next()) {
            Order order;
            order.setCost(query.value("cost").toDouble());
            order.setDate(query.value("date").toDate());
            orders.append(order);
        }
    } else {
        Logger::instance().log(QString("[getLargestOrdersByEmail] Database query error!")
                                   .append(query.lastError().text()), Logger::LogLevel::Warning);
    }

    return orders;
}
