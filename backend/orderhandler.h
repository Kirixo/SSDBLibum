#ifndef ORDERHANDLER_H
#define ORDERHANDLER_H

#include <QtHttpServer/QHttpServer>
#include "orderrepository.h"  // Assuming you have an OrderRepository class for handling orders

class OrderHandler
{
public:
    OrderHandler();

    static QHttpServerResponse getOrder(const QHttpServerRequest &request);
    static QHttpServerResponse getOrderList(const QHttpServerRequest &request);
    static QHttpServerResponse getOrderWithMostBooksByAuthor(const QHttpServerRequest &request);
    static QHttpServerResponse getLargestOrdersByEmail(const QHttpServerRequest &request);
};

#endif // ORDERHANDLER_H
