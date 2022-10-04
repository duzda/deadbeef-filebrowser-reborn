#include "modelcolumns.hpp"

using namespace GUI;

ModelColumns::ModelColumns() {
    this->add(this->ColumnIcon);
    this->add(this->ColumnName);
    this->add(this->ColumnURI);
    this->add(this->ColumnTooltip);
    this->add(this->ColumnVisibility);
}