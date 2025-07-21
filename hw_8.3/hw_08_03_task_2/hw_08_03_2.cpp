// Задание 2. Паттерн «Наблюдатель»

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>

using namespace std;

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void onWarning(const string &) {}
    virtual void onError(const string &) {}
    virtual void onFatalError(const string &) {}
};

class Observable
{
private:
    mutable vector<weak_ptr<Observer>> observers_;

public:
    void addObserver(shared_ptr<Observer> observer)
    {
        observers_.push_back(observer);
    }

    void warning(const string &message) const
    {
        notifyObservers([this, &message](const auto &obs)
                        { obs->onWarning(message); });
    }

    void error(const string &message) const
    {
        notifyObservers([this, &message](const auto &obs)
                        { obs->onError(message); });
    }

    void fatalError(const string &message) const
    {
        notifyObservers([this, &message](const auto &obs)
                        { obs->onFatalError(message); });
    }

private:
    template <typename Callback>
    void notifyObservers(Callback cb) const
    {
        for (auto it = observers_.begin(); it != observers_.end();)
        {
            if (auto obs = (*it).lock())
            {
                cb(obs.get());
                ++it;
            }
            else
            {
                it = observers_.erase(it);
            }
        }
    }
};

class WarningLogger : public Observer
{
public:
    void onWarning(const string &message) override
    {
        cout << "WARNING: " << message << endl;
    }
};

class ErrorFileLogger : public Observer
{
private:
    string filePath_;

public:
    explicit ErrorFileLogger(const string &path) : filePath_(path) {}

    void onError(const string &message) override
    {
        ofstream log(filePath_, ios_base::app);
        if (log.is_open())
        {
            log << "ERROR: " << message << endl;
            log.close();
        }
    }
};

class FatalErrorLogger : public Observer
{
private:
    string filePath_;

public:
    explicit FatalErrorLogger(const string &path) : filePath_(path) {}

    void onFatalError(const string &message) override
    {
        cout << "FATAL ERROR: " << message << endl;

        ofstream log(filePath_, ios_base::app);
        if (log.is_open())
        {
            log << "FATAL ERROR: " << message << endl;
            log.close();
        }
    }
};


int main()
{
    auto warning_logger = make_shared<WarningLogger>();
    auto error_file_logger = make_shared<ErrorFileLogger>("errors.log");
    auto fatal_error_logger = make_shared<FatalErrorLogger>("fatal_errors.log");

    Observable observable;

    observable.addObserver(warning_logger);
    observable.addObserver(error_file_logger);
    observable.addObserver(fatal_error_logger);

    observable.warning("Это предупреждение.");
    observable.error("Это ошибка.");
    observable.fatalError("Это фатальная ошибка.");

    return 0;
}