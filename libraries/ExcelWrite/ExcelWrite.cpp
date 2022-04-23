#include "ExcelWrite.h"
#pragma execution_character_set("utf-8")


excel_write::ExcelDoc::ExcelDoc(QString input_path) : QXlsx::Document(input_path)
{

}


int excel_write::ExcelDoc::search(QString sheet_name, int col, QString target)
{
    auto current_sheet = this->currentSheet();
    
    this->selectSheet(sheet_name);
    int row_index = 1;
    if (QXlsx::Cell* cell = this->cellAt(row_index, col))
    {
        if (cell->value().toString().toLocal8Bit() == target)
            return row_index;

        while (cell->value().toString() != "")
        {
            if (cell->value().toString().toLocal8Bit() == QString(target).toLocal8Bit())
            {
                this->selectSheet(current_sheet->sheetName());
                return row_index;
            }
            row_index++;
            cell = this->cellAt(row_index, col);
        }
    }
    this->selectSheet(current_sheet->sheetName());
    return 0;
}

int excel_write::ExcelDoc::search(int col, QString target)
{
    return this->search(this->currentSheet()->sheetName(), col, target);
}
