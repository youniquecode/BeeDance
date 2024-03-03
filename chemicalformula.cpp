#include "chemicalformula.h"
#include <QRegularExpression>
#include <deque>
#include <stack>

QString ChemicalFormula::getLine(QString::ConstIterator cbegin, QString::ConstIterator cend)
{
    QString result;

    if(cbegin == nullptr || cend == nullptr) return result;

    for(auto it(cbegin); it != cend; it++) result.push_back(*it);
    return result;
}

ChemicalFormula::ChemicalFormula(QString line)
{
    setFormula(line);
}
QString::ConstIterator ChemicalFormula::it_on_SecondElement(const QString &line)
{
    auto it = line.cbegin();
    if(line.cbegin() == line.cend()) return it;
    for(; it != line.cend(); it++) // находим заглавный символ первого элемента
        if((*it).isUpper()) break;
    it++;
    for(; it != line.cend(); it++)
        if((*it).isUpper() || (*it) == '^') break;

    return it;
}
SimpleFormulaElement ChemicalFormula::readIsotope(QString line)
{
    if(line.isEmpty()) return SimpleFormulaElement("", 0, 0.0);

    if(line[0] == '^') line.remove(0, 1); // удаляем знак ^ если есть.

    QString nucl;
    auto it(line.cbegin());
    for(; it->isDigit(); it++) nucl.push_back(*it);
    long nucleons = nucl.toLong();

    QString element;
    for(; it->isLetter(); it++) element.push_back(*it);

    QString id(getLine(it, line.cend()));
    double index = id.toDouble();
    if(id.isEmpty()) index = 1.0;

    SimpleFormulaElement el(element, nucleons);
    el.setindex(index);
    return el;

}
bool ChemicalFormula::hasBrackets(QString line) const
{
    static QRegularExpression open_brackets("[(]");
    static QRegularExpression close_brackets("[)]");

    for(auto it(line.cbegin()); it != line.cend(); it++)
    {
        if(QString(*it).contains(open_brackets)) return true;
        if(QString(*it).contains(close_brackets)) return true;
    }
    return false;
}

bool ChemicalFormula::hasSquareBrackets(QString line) const
{
    return line.contains(QRegularExpression("[\\[]"));
}
std::vector<SimpleFormulaElement> ChemicalFormula::getElements(QString subformula)
{
    std::vector<SimpleFormulaElement> result;
    QString temp_formula = subformula;


    while(hasBrackets(temp_formula))
    {
        auto iters_brackets_with_index = (brackets_index_inside(temp_formula)); // получаем выражение со скобками и индексом
        QString brackets_with_index = getLine(iters_brackets_with_index); // получаем полное выражение
        temp_formula.erase(iters_brackets_with_index.first, iters_brackets_with_index.second); // удаляем из исходной строки
        QString inner_brackets = getLine(brackets_inside(brackets_with_index)); // получаем внутреннее выражение
        double brackets_index = get_index(brackets_with_index); // получаем индекс
        std::vector<SimpleFormulaElement> temp_res = getElements(inner_brackets);
        multiple_by_index(temp_res, brackets_index);
        result.insert(result.end(), temp_res.begin(), temp_res.end());
    }

    while(!temp_formula.isEmpty())
    {
        result.push_back(get_firstElement(temp_formula));
        temp_formula = del_firstElement(temp_formula);
    }

    return result;
}

bool ChemicalFormula::is_multiformula(QString line)
{
    for(auto it = line.cbegin(); it != line.cend(); it++) if((*it) == ']') return true;
    return false;
}
std::pair<QString::ConstIterator, QString::ConstIterator> ChemicalFormula::brackets_index_inside(QString const &line)
{
    std::pair<QString::ConstIterator, QString::ConstIterator> result(nullptr, nullptr);
    static QRegularExpression open_brackets("[(]");
    static QRegularExpression close_brackets("[)]");

    std::deque<QChar> deq_brackets;

    QString::ConstIterator it;
    for(it = line.begin(); it != line.end(); it++)
    {
        if (QString(*it).contains(open_brackets))
        {
            if(deq_brackets.empty())
            {
                result.first = it;
                result.second = it;
            }
            deq_brackets.push_back(*it);
            deq_brackets.push_back(')');
        }
        if (QString(*it).contains(close_brackets))
        {
            deq_brackets.pop_back();
            deq_brackets.pop_back();

            if(deq_brackets.empty()) break;
        }
    }
    for(it++; it != line.end(); it++)
    {
        if(it->isDigit() || *it == '.') continue;
        break;
    }
    result.second = it;
    return result;
}
std::pair<QString::ConstIterator, QString::ConstIterator> ChemicalFormula::brackets_inside(const QString &line)
{
    std::pair<QString::ConstIterator, QString::ConstIterator> result(nullptr, nullptr);
    if(!hasBrackets(line)) return result; // if line doesn't have any brackets - return nullptr,nullptr

    static QRegularExpression open_brackets("[(]");
    static QRegularExpression close_brackets("[)]");

    std::deque<QChar> deq_brackets;


    QString::ConstIterator it;
    for(it = line.begin(); it != line.end(); it++)
    {
        if (QString(*it).contains(open_brackets))
        {
            if(deq_brackets.empty())
            {
                result.first = it;
                result.second = it;
            }
            deq_brackets.push_back(*it);
            deq_brackets.push_back(')');
        }
        if (QString(*it).contains(close_brackets))
        {
            deq_brackets.pop_back();
            deq_brackets.pop_back();

            if(deq_brackets.empty()) break;
        }
    }
    result.first++;
    result.second = it;
    return result;

}
double ChemicalFormula::get_index(const QString &line)
{
    if(!hasBrackets(line)) return 0.0;

    QString::ConstIterator temp_it = line.cend();
    for(auto it(line.cbegin()); it != line.cend(); it++) { if(*it == ')') temp_it = it; }

    temp_it++;

    if(temp_it == line.cend()) return 1.0;

    QString str_index = getLine(temp_it, line.cend());

    return str_index.toDouble();

}

// line is correct. In other way - undefined behavior
std::vector<SimpleFormulaElement> ChemicalFormula::getElements()
{
    return getElements(formula);
}

bool ChemicalFormula::is_correct()
{
    return check_format()
           && check_roundbrackets()
           && check_sqbrackets();
}

bool ChemicalFormula::check_roundbrackets(QString line) const
{
    static QRegularExpression open_brackets("[(]");
    static QRegularExpression close_brackets("[)]");

    std::stack<QChar> b_stack;

    for(auto it(line.cbegin()); it != line.cend(); it++)
    {
        if(QString(*it).contains(open_brackets))
            b_stack.push(*it);

        if(QString(*it).contains(close_brackets))
        {
            if(b_stack.empty())
                return false;

            if(QString(b_stack.top()).contains(open_brackets))
                    b_stack.pop();
            else
                return false;
        }
    }
    return b_stack.empty();
}

bool ChemicalFormula::check_format(QString line) const
{
    static QRegularExpression re_element("([\\^][0-9]+)?[A-Z][a-z]*([0-9]+[.]?[0-9]*|[0-9]*[.][0-9]+)?");
    static QRegularExpression re_squarebrackets("[\\[]([0-9]+[.]?[0-9]*|[0-9]*[.][0-9]+)[\\]]");
    static QRegularExpression re_brackets("[(][)]([0-9]+[.]?[0-9]*|[0-9]*[.][0-9]+)?");

    if(!line.contains(re_element)) return false;

    line = line.remove(re_squarebrackets);
    line = line.remove(re_element);
    line = line.remove(re_brackets);

    if(line.isEmpty()) return true;
    return false;
}

bool ChemicalFormula::check_sqbrackets(QString line) const
{
    if(!hasSquareBrackets(line)) return true;
    return line.contains(QRegularExpression("[\\[]([0-9]+[.]?[0-9]*|[0-9]*[.][0-9]+)[\\]]$"));
}

bool ChemicalFormula::is_correct(QString line)
{
    return check_format(line)
           && check_roundbrackets(line)
           && check_sqbrackets(line);

}

SimpleFormulaElement ChemicalFormula::get_firstElement(const QString line)
{
    return readIsotope(getLine(line.begin(), it_on_SecondElement(line)));
}
QString ChemicalFormula::del_firstElement(QString const line)
{
    return getLine(it_on_SecondElement(line), line.cend());
}
