#include "coresld.h"

double CoreSLD::full_mass(QString const line)
{
    auto temp_chem_formula = new ChemicalFormula("");
    temp_chem_formula->setFormula(line);
    auto vec_elem = temp_chem_formula->getElements();

    double mass(0.0);

    for(auto it(vec_elem.cbegin()); it != vec_elem.cend(); it++)
        mass += it->index() * dt_sld->getElement(it->symbol(), it->nucleons()).get_mass();

    return mass;
}

double CoreSLD::calculate_sld(QString line)
{
    if(!isMultiFormula(line)) return calculate_singlesld(line);
    qDebug() << "multiformula";
    return calculate_multisld(line);

    return 0.0;
}

double CoreSLD::calculate_multisld(QString line)
{
    QString temp_line = line;
    temp_line.chop(1);

    QStringList list_formuls = temp_line.split(']');
    // qDebug() << "size: " << list_formuls.size();

    std::vector<SimpleFormulaElement> formula_for_single;

    for(auto it(list_formuls.cbegin()); it != list_formuls.cend(); it++)
    {
        auto temp_elements = subcalculate_subline(*it);
        formula_for_single.insert(formula_for_single.cend(), temp_elements.cbegin(), temp_elements.cend());
    }

    return calculate_singlesld(formula_for_single);

}


double CoreSLD::calculate_singlesld(QString line)
{
    double a_formula(0);
    double b_formula(0);
    chem_formula->setFormula(line);
    auto vec_elem = chem_formula->getElements();

    for(auto it(vec_elem.cbegin()); it != vec_elem.cend(); it++)
    {
        a_formula += it->index() * dt_sld->getElement(it->symbol(), it->nucleons()).get_mass();
        b_formula += it->index() * dt_sld->getElement(it->symbol(), it->nucleons()).get_bc().real();

    }
    return (density_ * b_formula) / (a_formula * 1.660153907);

}

double CoreSLD::calculate_singlesld(std::vector<SimpleFormulaElement> elem_vec)
{
    double a_formula(0);
    double b_formula(0);
    auto vec_elem = elem_vec;

    for(auto it(vec_elem.cbegin()); it != vec_elem.cend(); it++)
    {
        a_formula += it->index() * dt_sld->getElement(it->symbol(), it->nucleons()).get_mass();
        b_formula += it->index() * dt_sld->getElement(it->symbol(), it->nucleons()).get_bc().real();

    }
    return (density_ * b_formula) / (a_formula * 1.660153907);

}

bool CoreSLD::isMultiFormula(QString line)
{
    long count(0);

    for(auto it = line.cbegin(); it != line.cend(); it++) if((*it) == ']') count++;

    if(count > 0) return true;
    return false;
}

double CoreSLD::read_value_squarebrackets(QString line)
{
    QString number;
    bool inside_brackets(false);
    for(auto it(line.cbegin()); it != line.cend(); it++)
    {
        if(inside_brackets) number.push_back(*it);
        if((*it) == '[') { inside_brackets = true; }
    }
    qDebug() << number.toDouble();
    return number.toDouble();

}

std::vector<SimpleFormulaElement> CoreSLD::subcalculate_subline(QString line)
{
    double brackets_value = read_value_squarebrackets(line);
    QString formula = delete_squarebrackets(line);
    double mass = full_mass(formula);
    double koef = brackets_value / mass;
    qDebug() << "k = " << koef;
    ChemicalFormula chem_f(formula);
    auto elements = chem_f.getElements();
    chem_f.multiple_on_index(elements, koef);

    for(auto it(elements.begin()); it != elements.end(); it++)
    {
        qDebug() << it->symbol() << " " << it->index();
    }
    return elements;

}

QString CoreSLD::delete_squarebrackets(QString line)
{
    auto del_it = line.cend();
    for(auto it(line.cbegin()); it != line.cend(); it++)
    {
        if((*it) == '[') {del_it = it; break; }
    }
    line.erase(del_it, line.cend());

    qDebug() << "delete_squarebrackets: " << line;
    return line;
}

CoreSLD::CoreSLD()
{
    chem_formula = new ChemicalFormula("");
    dt_sld = new DataTableSLD();
}

void CoreSLD::setFormula(QString formula)
{
    str_formula = formula;
    chem_formula->setFormula(formula);
}
double CoreSLD::get_sld()
{
    return calculate_sld(str_formula);
}
double CoreSLD::get_sld(QString line)
{
    return calculate_sld(line);
}

bool CoreSLD::check_roundbrackets(QString)
{

}

bool CoreSLD::check_valueexitst(QString)
{

}
