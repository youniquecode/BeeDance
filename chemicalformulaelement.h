#ifndef CHEMICALFORMULAELEMENT_H
#define CHEMICALFORMULAELEMENT_H
#include "element.h"

class ChemicalFormulaElement
{
    Element *element_ = nullptr;
    double index_;
public:
    ChemicalFormulaElement(Element element, double index = 0.0);
    ~ChemicalFormulaElement();

    Element* element() { return element_; }
    double index() { return index_; }
    double mass() { return index_ * element_->get_mass(); }

    void set_element(Element element) { element_ = new Element(element); }
    void set_index(double index) { index_ = index; }
    void plus(double add) { index_ += add; }
    void mult(double mul) { index_ *= mul; }

    bool has_element() { return element_ != nullptr; }
};

#endif // CHEMICALFORMULAELEMENT_H