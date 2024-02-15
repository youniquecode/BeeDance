#include "coresld.h"

double CoreSLD::calculate_sld()
{
    double a_formula(0);
    double b_formula(0);
    auto vec_elem = chem_formula->getElements();

    for(auto it(vec_elem.cbegin()); it != vec_elem.cend(); it++)
    {
        a_formula += it->index() * dt_sld->getElement(it->symbol(), it->nucleons()).getMass();
        b_formula += it->index() * dt_sld->getElement(it->symbol(), it->nucleons()).getCohb().real();

    }
    return (density_ * b_formula) / (a_formula * 1.660153907);
}

bool CoreSLD::isMultiFormula()
{
    long count(0);

    for(auto it = str_formula.cbegin(); it != str_formula.cend(); it++) if((*it) == ']') count++;

    if(count > 1) return true;
    return false;
}

CoreSLD::CoreSLD()
{
    read = new ReadDataFiles();
    chem_formula = new ChemicalFormula("H2O");
    dt_sld = new DataTableSLD();
    dt_sld->setReadData(read);
}

void CoreSLD::setFormula(QString formula)
{
    str_formula = formula;
    chem_formula->setFormula(formula);
}

double CoreSLD::get_sld()
{
    return calculate_sld();
}
