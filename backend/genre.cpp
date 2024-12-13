#include "genre.h"

Genre::Genre() {}

Genre::Genre(int id, const QString& name)
{
    id_ = id;
    name_ = name;
}

int Genre::id() const
{
    return id_;
}

void Genre::setId(int id)
{
    id_ = id;
}

const QString& Genre::name() const
{
    return name_;
}

void Genre::setName(const QString &name)
{
    name_ = name;
}

QJsonObject Genre::toJson() const
{
    QJsonObject json;
    json["id"] = id_;
    json["name"] = name_;
    return json;
}
