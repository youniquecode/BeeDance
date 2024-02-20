#ifndef CORESLD_H
#define CORESLD_H

#include "readdatafiles.h"
#include "chemicalformula.h"
#include "datatablesld.h"

class CoreSLD
{
private:
    double density_;
    ReadDataFiles *read = nullptr;
    ChemicalFormula *chem_formula = nullptr;
    DataTableSLD *dt_sld;
    QString str_formula;


    double full_mass(QString const);

    double calculate_sld();
    double calculate_multisld();
    bool isMultiFormula();


public:
    CoreSLD();
    void setFormula(QString);
    void setDensity(double density) { density_ = density; };
    double get_sld();
};

#endif // CORESLD_H
