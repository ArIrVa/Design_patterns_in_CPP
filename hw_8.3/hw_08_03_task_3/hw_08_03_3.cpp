// Задание 3 Паттерн «Цепочка ответственности»

#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>

using namespace std;

enum class Type
{
    WARNING,
    ERROR,
    FATAL_ERROR,
    UNKNOWN_MESSAGE
};

class LogMessage
{
private:
    Type type_;
    string message_;

public:
    explicit LogMessage(Type type, const string &message)
        : type_(type), message_(message) {}

    Type type() const { return type_; }

    const string &message() const { return message_; }
};

class HandlerInterface
{
protected:
    shared_ptr<HandlerInterface> next_handler_;

public:
    virtual ~HandlerInterface() = default;

    void setNext(shared_ptr<HandlerInterface> handler)
    {
        this->next_handler_ = handler;
    }

    void handle(const LogMessage &log_msg)
    {
        if (!handleMessage(log_msg))
        {
            if (next_handler_)
            {
                next_handler_->handle(log_msg);
            }
            else
            {
                throw runtime_error("Необработанное сообщение журнала");
            }
        }
    }

    virtual bool handleMessage(const LogMessage &log_msg) = 0;
};

class FatalErrorHandler : public HandlerInterface
{
public:
    bool handleMessage(const LogMessage &log_msg) override
    {
        if (log_msg.type() != Type::FATAL_ERROR)
            return false;

        cerr << "Fatal error occurred: " << log_msg.message() << endl;
        throw runtime_error(log_msg.message());
    }
};

class ErrorHandler : public HandlerInterface
{
private:
    string file_path_;

public:
    explicit ErrorHandler(const string &filePath)
        : file_path_(filePath) {}

    bool handleMessage(const LogMessage &log_msg) override
    {
        if (log_msg.type() != Type::ERROR)
            return false;

        ofstream out(file_path_, ios_base::app);
        if (!out.is_open())
        {
            cerr << "Ошибка открытия файла.\n";
            return true;
        }

        out << "Error logged: " << log_msg.message() << endl;
        out.close();
        return true;
    }
};

class WarningHandler : public HandlerInterface
{
public:
    bool handleMessage(const LogMessage &log_msg) override
    {
        if (log_msg.type() != Type::WARNING)
            return false;

        cout << "Warning: " << log_msg.message() << endl;
        return true;
    }
};

class UnknownMessageHandler : public HandlerInterface
{
public:
    bool handleMessage(const LogMessage &log_msg) override
    {
        if (log_msg.type() != Type::UNKNOWN_MESSAGE)
            return false;

        throw runtime_error("Unknown log message received: " + log_msg.message());
    }
};

shared_ptr<HandlerInterface> buildChain()
{
    auto fatalErrorHandler = make_shared<FatalErrorHandler>();
    auto errorHandler = make_shared<ErrorHandler>("errors.log");
    auto warningHandler = make_shared<WarningHandler>();
    auto unknownMessageHandler = make_shared<UnknownMessageHandler>();

    fatalErrorHandler->setNext(errorHandler);
    errorHandler->setNext(warningHandler);
    warningHandler->setNext(unknownMessageHandler);

    return fatalErrorHandler;
}

void sendLog(const LogMessage &msg)
{
    auto chain = buildChain();
    try
    {
        chain->handle(msg);
    }
    catch (const exception &ex)
    {
        cerr << ex.what() << endl;
    }
}


int main()
{
    sendLog(LogMessage{Type::WARNING, "Это предупреждение."});
    sendLog(LogMessage{Type::ERROR, "Произошла ошибка."});
    sendLog(LogMessage{Type::FATAL_ERROR, "Фатальная ошибка! Программа завершена."});
    sendLog(LogMessage{Type::UNKNOWN_MESSAGE, "Нераспознанное сообщение журнала."});

    return 0;
}