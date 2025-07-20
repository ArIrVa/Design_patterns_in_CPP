// Задание 2 Реализовать класс OneShotDB

#include <iostream>
#include <string>
#include <map>

using namespace std;

class VeryHeavyDatabase
{
public:
    string GetData(const string &key) const noexcept
    {
        return "Very Big Data String: " + key;
    }
};

class CacheProxyDB : VeryHeavyDatabase
{
public:
    explicit CacheProxyDB(VeryHeavyDatabase *real_object) : real_db_(real_object) {}
    string GetData(const string &key) noexcept
    {
        if (cache_.find(key) == cache_.end())
        {
            cout << "Get from real object\n";
            cache_[key] = real_db_->GetData(key);
        }
        else
        {
            cout << "Get from cache\n";
        }
        return cache_.at(key);
    }

private:
    map<string, string> cache_;
    VeryHeavyDatabase *real_db_;
};

class TestDB : VeryHeavyDatabase
{
public:
    explicit TestDB(VeryHeavyDatabase *real_object) : real_db_(real_object) {}
    string GetData(const string &key) noexcept
    {
        return "test_data\n";
    }

private:
    VeryHeavyDatabase *real_db_;
};

// Реализация класса OneShotDB
class OneShotDB : VeryHeavyDatabase
{
public:
    explicit OneShotDB(VeryHeavyDatabase *real_object, size_t shots = 1) : real_db_(real_object), shots_(shots) {}

    string GetData(const string &key) noexcept
    {
        ++callCount_;
        if (callCount_ > shots_)
        {
            return "error";
        }
        else
            return "value";
    }

private:
    VeryHeavyDatabase *real_db_;
    size_t shots_;
    int callCount_ = 0;
};

int main()
{
    auto real_db = VeryHeavyDatabase();
    auto cached_db = CacheProxyDB(addressof(real_db));
    auto test_db = TestDB(addressof(real_db));
    cout << cached_db.GetData("key") << endl;
    cout << cached_db.GetData("key") << endl;
    cout << test_db.GetData("key") << endl;

    auto limit_db = OneShotDB(addressof(real_db), 2);
    cout << limit_db.GetData("key") << endl;
    cout << limit_db.GetData("key") << endl;
    cout << limit_db.GetData("key") << endl;

    return 0;
}