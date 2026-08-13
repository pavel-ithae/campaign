#include <iostream>
#include <campaign/data_source.hpp>

void flag_updated(bool previous, bool current)
{
    std::cout << "Callback!" << '\n';
}

int main()
{
    std::cout << "Hello World!" << '\n';

    campaign::DataSource dataSource(1);

    dataSource.init();

    std::cout << dataSource.getFlag(0, 0x1) << '\n';

    dataSource.registerFlagCallback(0, 0x1, flag_updated);

    dataSource.setFlag(0, 0x1, true);

    std::cout << dataSource.getFlag(0, 0x1) << '\n';

    return 0;
}