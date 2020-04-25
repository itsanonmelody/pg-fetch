#include <iostream>
#include <algorithm>

#include "pgf/dbf.hpp"

#include "pqxx/pqxx"
#include "shapefil.h"

int main()
{
    try
    {
        pqxx::connection C{"hostaddr=127.0.0.1 port=5432 dbname=postgres"};
        std::cout << "Connected to " << C.dbname() << std::endl;
        pqxx::work W{C};

        pqxx::result R{W.exec("SELECT name, salary FROM employee")};

        std::cout << "Found " << R.size() << " employees:\n";
        std::cout << "Serializing to DBF format..." << std::endl;
        pgf::SerializeToDBF("test.dbf", R);

        std::cout << "Doubling all employees' salaries...\n";
        W.exec0("UPDATE employee SET salary = salary*2");

        std::cout << "Making changes definite: ";
        W.commit();
        std::cout << "OK.\n";
    }
    catch (std::exception const &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}