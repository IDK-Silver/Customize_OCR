//
// Created by idk on 2022/11/17.
//

#include "XlntControl.h"


XlntControl::XlntControl(const std::string& open_file_path) {
    this->workbook = std::make_shared<xlnt::workbook>();
    workbook->load(open_file_path);
}

[[maybe_unused]] bool XlntControl::open_file(const std::string &openfile_path) {
    this->workbook->load(openfile_path);
    return true;
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
    sheet = this->workbook->sheet_by_index(index);
}

int XlntControl::find_element_inRow(const int& row_index, const std::string &element) {
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

int XlntControl::find_element_inCol(const int& col_index, const std::string& element) {
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

void XlntControl::write_data(const int& row, const int& col, const std::string& data)
{
    this->sheet.cell(row + 1, col + 1).value(data);
}


void XlntControl::save_file(const std::string &file_path) {
    this->workbook->save(file_path);
}