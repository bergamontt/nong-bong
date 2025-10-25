#include "DataSource.h"

DataSource::DataSource(const std::string &db,
    const soci::backend_factory& factory, const size_t poolSize)
    : _pool(poolSize)
{
    for (std::size_t i = 0; i < poolSize; ++i)
    {
        soci::session &sql = pool().at(i);
        sql.open(factory, db);
    }
}
