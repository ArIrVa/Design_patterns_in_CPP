// Задание 1 В программу рендеринга добавить новые классы для декорирования текста

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class SimpleText
{
public:
    virtual void render(const string &text)
    {
        cout << text;
    }
};

class DecoratedText : public SimpleText
{
public:
    SimpleText *_text;
    DecoratedText(SimpleText *s_text) : _text(s_text) {}
};

class ItalicText : public DecoratedText
{
public:
    ItalicText(SimpleText *s_text) : DecoratedText(s_text) {}
    void render(const string &text) override
    {
        cout << "<i>";
        _text->render(text);
        cout << "</i>" << endl;
    }
};

class Paragraph : public DecoratedText
{
public:
    Paragraph(SimpleText *s_text) : DecoratedText(s_text) {}
    void render(const string &text) override
    {
        cout << "<p>";
        _text->render(text);
        cout << "</p>" << endl;
    }
};

class Reversed : public DecoratedText
{
public:
    Reversed(SimpleText *s_text) : DecoratedText(s_text) {}
    void render(const string &text) override
    {
        string t = text;
        reverse(t.begin(), t.end());
        _text->render(t);
        cout << endl;
    }
};

class Link : public DecoratedText
{
public:
    Link(SimpleText *s_text) : DecoratedText(s_text) {}
    void render(const string &text1, const string &text2)
    {
        string d_text = "<a href=" + text1 + ">" + text2 + "</a>";
        _text->render(d_text);
        cout << endl;
    }
};

int main()
{
    SimpleText original_text;
    original_text.render("Hello world!");
    cout << endl;

    auto it_text = new ItalicText(&original_text);
    it_text->render("Hello Italic world!");

    auto paragraph_text = new Paragraph(&original_text);
    paragraph_text->render("Hello Paragraph world!");

    auto reversed_text = new Reversed(&original_text);
    reversed_text->render("Hello world!");

    auto link_text = new Link(&original_text);
    link_text->render("netology.ru", "Hello world!");

    return 0;
}