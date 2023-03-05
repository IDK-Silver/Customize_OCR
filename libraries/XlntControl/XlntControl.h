//
// Created by idk on 2022/11/17.
//

#ifndef CUSTOMIZE_OCR_XLNTCONTROL_H
#define CUSTOMIZE_OCR_XLNTCONTROL_H

#include <cctype>
#include <string>
#include <memory>
#include <xlnt/xlnt.hpp>
#include <iostream>


std::string fuzzy_reference(const std::string& cell_string);

class XlntControl : public xlnt::workbook {

public:
    XlntControl() = default;
    ~XlntControl() = default;

    void set_sheet(int index);
    void print();

    int find_element_inRow(const unsigned int& row_index, const std::string& element);
    int find_element_inCol(const unsigned int& col_index, const std::string& element);

    void write_data(const unsigned int& col, const unsigned int& row, const std::string& data);



    xlnt::worksheet sheet;
    std::string file_path;

    unsigned int max_row_cursor = 0;
private:
};


#endif //CUSTOMIZE_OCR_XLNTCONTROL_H
