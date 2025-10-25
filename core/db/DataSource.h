#pragma once
#include <string>
#include <soci/soci.h>

class DataSource
{
public:
    explicit DataSource(const std::string& db,
        const soci::backend_factory& factory, size_t poolSize=8);
    DataSource(const DataSource&) = delete;
    DataSource operator=(const DataSource&) = delete;
    soci::connection_pool& pool();
private:
    soci::connection_pool _pool;
};

inline soci::connection_pool& DataSource::pool()
{
    return _pool;
}