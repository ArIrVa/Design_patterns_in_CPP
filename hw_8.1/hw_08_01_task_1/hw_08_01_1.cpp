// Задание 1 Построение простых SELECT-запросов с применением класса SqlSelectQueryBuilder

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct SqlSelectQuery
{
    vector<string> columns; // Список выбранных колонок
    string fromTable;       // Имя таблицы
    string whereCondition;  // Условие WHERE
};

class SqlSelectQueryBuilder
{
public:
    // Основной конструктор принимает таблицу
    explicit SqlSelectQueryBuilder(const string &tableName)
    {
        query.fromTable = tableName;
    }

    // Сборка готового SQL-запроса
    string BuildQuery() noexcept
    {
        stringstream sqlStream;

        sqlStream << "SELECT ";

        if (query.columns.empty())
        {
            sqlStream << "*"; // Если колонки не указаны, используем *
        }
        else
        {
            for (size_t i = 0; i < query.columns.size(); ++i)
            {
                sqlStream << query.columns[i];
                if (i != query.columns.size() - 1)
                    sqlStream << ", ";
            }
        }

        sqlStream << " FROM " << query.fromTable;

        if (!query.whereCondition.empty())
        {
            sqlStream << " WHERE " << query.whereCondition;
        }

        // Завершаем запрос символом ';'
        sqlStream << ";";

        return sqlStream.str();
    }

    SqlSelectQueryBuilder &AddColumn(const string &columnName) noexcept
    {
        query.columns.push_back(columnName);
        return *this;
    }

    SqlSelectQueryBuilder &AddFrom(const string &tableName) noexcept
    {
        query.fromTable = tableName;
        return *this;
    }

    SqlSelectQueryBuilder &AddWhere(const string &condition, const string &value) noexcept
    {
        if (!query.whereCondition.empty())
            query.whereCondition += " AND "; // Логическое соединение условий
        query.whereCondition += condition + "=" + value;
        return *this;
    }

private:
    SqlSelectQuery query;
};

int main()
{
    SqlSelectQueryBuilder query_builder("workers");

    query_builder.AddColumn("name").AddColumn("phone");
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");

    // Построение финального запроса
    string query = query_builder.BuildQuery();

    // Проверка корректности построенного запроса
    string queryCorrect = "SELECT name, phone FROM students WHERE id=42 AND name=John;";

    if (query != queryCorrect)
    {
        cout << "Ошибка!\n";
    }
    cout << query << endl;

    return 0;
}