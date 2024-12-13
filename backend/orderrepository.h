#ifndef ORDERREPOSITORY_H
#define ORDERREPOSITORY_H

#include "order.h"
#include "book.h"
#include <optional>
#include <QList>

class OrderRepository
{
public:
    OrderRepository();
    static std::optional<Order> fetchOrderById(int id);
    static QList<Order> fetchOrders(int limit, int page);
    static int getOrdersCount();

    // New methods to interact with SQL functions
    static std::optional<int> getOrderWithMostBooksByAuthor(const QString& authorName);
    static QList<Order> getLargestOrdersByEmail(const QString& userEmail);

private:
    static QList<Book> fetchBooksForOrder(int orderId);
};

#endif // ORDERREPOSITORY_H
