// Задание 2 Расширение методов AddWhere и AddColumns
// для добавления в запрос нескольких полей

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

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

    SqlSelectQueryBuilder &AddColumns(const vector<std::string> &colms) noexcept
    {
        // query.columns.clear(); // Очистим предыдущие колонки, если были заданы ранее
        query.columns.resize(colms.size());
        copy(colms.begin(), colms.end(), query.columns.begin());
        return *this;
    }

    SqlSelectQueryBuilder &AddFrom(const string &tableName) noexcept
    {
        query.fromTable = tableName;
        return *this;
    }

    SqlSelectQueryBuilder &AddWhere(const map<string, string> &cv) noexcept
    {
        for (const auto &pair : cv)
        {
            const string &fieldName = pair.first;
            const string &value = pair.second;

            if (!query.whereCondition.empty())
            {
                query.whereCondition += " AND ";
            }

            query.whereCondition += fieldName + "=" + value;
        }

        return *this;
    }

private:
    SqlSelectQuery query;
};


int main()
{
    SqlSelectQueryBuilder query_builder("workers");

    query_builder.AddColumns({"name", "phone"});
    query_builder.AddFrom("students");
    query_builder.AddWhere({{"id", "42"}, {"name", "John"}});

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