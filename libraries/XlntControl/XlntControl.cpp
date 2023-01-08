//
// Created by idk on 2022/11/17.
//

#include "XlntControl.h"



std::string fuzzy_reference(const std::string& cell_string)
{
    bool is_pha = true;
    for (const auto &word : cell_string)
    {
        if (isdigit(word))
        {
            is_pha = false;
            break;
        }
    }
    if (is_pha)
    {
        return  std::string(cell_string).append("0");
    }
    return  cell_string;
}

void XlntControl::print() {

    for (const auto& cell_vec : sheet)
    {
        for (auto cell : cell_vec)
        {
            if (cell.has_value())
                std::cout  << cell.to_string() << " ";
        }
        std::cout << "\n";
    }
}

void XlntControl::set_sheet(int index) {
    sheet = this->sheet_by_index(index);
    this->next_row_index = this->sheet.highest_row() + 1;
}

int XlntControl::find_element_inRow(const unsigned int& row_index, const std::string &element) {
    int index = 0;
    int col_index = 0;
    for (const auto& cell_vec : sheet)
    {
        if (index != row_index)
        {
            index++;
            continue;
        }

        for (auto cell : cell_vec)
        {
            if (cell.to_string() == element)
                return col_index;
            col_index++;
        }
    }
    return -1;
}

int XlntControl::find_element_inCol(const unsigned int& col_index, const std::string& element) {
    int row_index = 0;
    for (const auto& cell_vec : sheet)
    {
        int index = 0;
        for (auto cell : cell_vec)
        {
            if (index != col_index)
            {
                index++;
                continue;
            }

            if (cell.to_string() == element)
                return row_index;
        }
        row_index++;
    }
    return -1;
}

void XlntControl::write_data(const unsigned int& col, const unsigned int& row, const std::string& data)
{
    this->sheet.cell(col + 1, row + 1).value(data);
    this->next_row_index = row + 1;
}

