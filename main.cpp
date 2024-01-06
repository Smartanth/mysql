#include <iostream>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/exception.h>

int main() {
    try {
        std::unique_ptr<sql::Connection> con;
        std::unique_ptr<sql::Statement> stmt;
        std::unique_ptr<sql::ResultSet> res;
        std::unique_ptr<sql::PreparedStatement> pstmt;

        // Connect to the database server
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        con.reset(driver->connect("tcp://localhost:3306", "root", "root")); // TODO: Replace by your own

        // Create the database if it doesn't exist
        const std::string databaseName = "test_mysql_for_embedded";
        stmt.reset(con->createStatement());
        stmt->execute("CREATE DATABASE IF NOT EXISTS " + databaseName);

        // Connect to the specific database
        con->setSchema(databaseName);

        // Create the table if it doesn't exist
        const std::string tableName = "test_table";
        const std::string tableCreationQuery =
            "CREATE TABLE IF NOT EXISTS " + tableName + " ("
            "id INT AUTO_INCREMENT PRIMARY KEY, "
            "name VARCHAR(255), "
            "age INT)";
        stmt->execute(tableCreationQuery);

        // Create (INSERT)
        pstmt.reset(con->prepareStatement("INSERT INTO " + tableName + " (name, age) VALUES (?, ?)"));
        pstmt->setString(1, "mimi");
        pstmt->setInt(2, 5);
        pstmt->executeUpdate();

        // Read (SELECT)
        pstmt.reset(con->prepareStatement("SELECT * FROM " + tableName + " WHERE name = ?"));
        pstmt->setString(1, "mimi");
        res.reset(pstmt->executeQuery());
        while (res->next()) {
            std::cout << "Read from DB: Name: " << res->getString("name") << ", Age: " << res->getInt("age") << std::endl;
        }

        // Update (UPDATE)
        pstmt.reset(con->prepareStatement("UPDATE " + tableName + " SET age = ? WHERE name = ?"));
        pstmt->setInt(1, 6);
        pstmt->setString(2, "mimi");
        const int updateCount = pstmt->executeUpdate();
        std::cout << "Updated " << updateCount << " row(s)." << std::endl;

        // Delete (DELETE)
        pstmt.reset(con->prepareStatement("DELETE FROM " + tableName + " WHERE name = ?"));
        pstmt->setString(1, "mimi");
        const int deleteCount = pstmt->executeUpdate();
        std::cout << "Deleted " << deleteCount << " row(s)." << std::endl;

    } catch (sql::SQLException &e) {
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return 1;
    }

    return 0;
}