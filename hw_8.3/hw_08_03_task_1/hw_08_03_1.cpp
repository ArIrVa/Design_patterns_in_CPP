// Задание 1 Паттерн «Команда»

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class LogCommand
{
public:
    virtual ~LogCommand() = default;
    virtual void print(const string &message) = 0;
};

class ConsoleLogger : public LogCommand
{
public:
    explicit ConsoleLogger() {}
    void print(const string &message) override
    {
        cout << message << endl;
    }
};

class FileLogger : public LogCommand
{
public:
    explicit FileLogger(const string &path) : filePath_(path) {}
    void print(const string &message) override
    {
        ofstream file(filePath_, ios_base::app);
        if (!file.is_open())
        {
            cerr << "Ошибка открытия файла." << endl;
            return;
        }
        file << message << endl;
        file.close();
    }

private:
    string filePath_;
};

void print(LogCommand &logger)
{
    logger.print("Hello world!");
}


int main()
{
    ConsoleLogger consoleLog;
    FileLogger fileLog("log.txt");

    print(consoleLog);
    print(fileLog);

    return 0;
}
