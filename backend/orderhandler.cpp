#include "orderhandler.h"
#include <qjsondocument.h>
#include "logger.h"
#include "responsefactory.h"
#include "orderrepository.h"  // Assuming you have an OrderRepository class

OrderHandler::OrderHandler() {}

QHttpServerResponse OrderHandler::getOrder(const QHttpServerRequest &request)
{
    bool ok;
    int orderId = request.query().queryItemValue("id").toInt(&ok);

    Logger::instance().log(QString("[getOrder request]: id = %1").arg(orderId), Logger::LogLevel::Info);

    if (!ok) {
        return ResponseFactory::createResponse("Invalid order ID.", QHttpServerResponse::StatusCode::BadRequest);
    }

    std::optional<Order> order = OrderRepository::fetchOrderById(orderId);

    if (order) {
        QByteArray responseData = QJsonDocument(order->toJson()).toJson(QJsonDocument::Compact);
        return ResponseFactory::createJsonResponse(responseData, QHttpServerResponse::StatusCode::Ok);
    }

    return ResponseFactory::createResponse("Order not found.", QHttpServerResponse::StatusCode::NotFound);
}

QHttpServerResponse OrderHandler::getOrderList(const QHttpServerRequest &request)
{
    const int defaultLimit = 24;
    const int defaultPage = 1;

    bool isLimitOk;
    int limit = request.query().queryItemValue("limit").toInt(&isLimitOk);
    if (!isLimitOk) {
        limit = defaultLimit;
    }

    bool isPageOk;
    int page = request.query().queryItemValue("page").toInt(&isPageOk);
    if (!isPageOk) {
        page = defaultPage;
    }

    Logger::instance().log(QString("[getOrderList request]: limit = %1, page = %2").arg(limit).arg(page), Logger::LogLevel::Info);

    QList<Order> orders = OrderRepository::fetchOrders(limit, page);

    if (!orders.isEmpty()) {
        QJsonArray orderArray;
        for (const Order &order : orders) {
            orderArray.append(order.toJson());
        }

        QJsonObject responseJson;
        responseJson["total_count"] = OrderRepository::getOrdersCount();
        responseJson["orders"] = orderArray;

        QByteArray responseData = QJsonDocument(responseJson).toJson(QJsonDocument::Compact);

        return ResponseFactory::createJsonResponse(responseData, QHttpServerResponse::StatusCode::Ok);
    }

    Logger::instance().log(QString("[getOrderList request] Not Found: limit = %1, page = %2").arg(limit).arg(page), Logger::LogLevel::Warning);

    return ResponseFactory::createResponse("Orders not found.", QHttpServerResponse::StatusCode::NotFound);
}

QHttpServerResponse OrderHandler::getOrderWithMostBooksByAuthor(const QHttpServerRequest &request)
{
    QString authorName = request.query().queryItemValue("author");

    Logger::instance().log(QString("[getOrderWithMostBooksByAuthor request]: author = ").append(authorName), Logger::LogLevel::Info);

    std::optional<int> orderId = OrderRepository::getOrderWithMostBooksByAuthor(authorName);

    if (!orderId) {
        Logger::instance().log(QString("[getOrderWithMostBooksByAuthor] No order found for author: ").append(authorName), Logger::LogLevel::Warning);
        return ResponseFactory::createResponse("No orders found for the specified author.", QHttpServerResponse::StatusCode::NotFound);
    }

    QJsonObject responseJson;
    responseJson["order_id"] = *orderId;

    QByteArray responseData = QJsonDocument(responseJson).toJson(QJsonDocument::Compact);
    return ResponseFactory::createJsonResponse(responseData, QHttpServerResponse::StatusCode::Ok);
}

QHttpServerResponse OrderHandler::getLargestOrdersByEmail(const QHttpServerRequest &request)
{
    QString userEmail = request.query().queryItemValue("email");

    Logger::instance().log(QString("[getLargestOrdersByEmail request]: email = ").append(userEmail), Logger::LogLevel::Info);

    QList<Order> orders = OrderRepository::getLargestOrdersByEmail(userEmail);

    if (orders.isEmpty()) {
        Logger::instance().log(QString("[getLargestOrdersByEmail] No orders found for user with email: ").append(userEmail), Logger::LogLevel::Warning);
        return ResponseFactory::createResponse("No orders found for the specified email.", QHttpServerResponse::StatusCode::NotFound);
    }

    QJsonArray orderArray;
    for (const Order &order : orders) {
        orderArray.append(order.toJson());
    }

    QJsonObject responseJson;
    responseJson["orders"] = orderArray;

    QByteArray responseData = QJsonDocument(responseJson).toJson(QJsonDocument::Compact);
    return ResponseFactory::createJsonResponse(responseData, QHttpServerResponse::StatusCode::Ok);
}

