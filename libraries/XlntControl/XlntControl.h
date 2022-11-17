//
// Created by idk on 2022/11/17.
//

#ifndef CUSTOMIZE_OCR_XLNTCONTROL_H
#define CUSTOMIZE_OCR_XLNTCONTROL_H

#include <string>
#include <memory>
#include <xlnt/xlnt.hpp>
#include <iostream>

class XlntControl {
public:
    XlntControl() = default;
    explicit XlntControl(const std::string& open_file_path);
    ~XlntControl() = default;

    [[maybe_unused]] bool open_file(const std::string& openfile_path);
    [[maybe_unused]] void set_sheet(int index);
    [[maybe_unused]] void print();

    [[maybe_unused]] int find_element_inRow(const int& row_index, const std::string& element);
    [[maybe_unused]] int find_element_inCol(const int& col_index, const std::string& element);

    void write_data(const int& row, const int& col, const std::string& data);
    void save_file(const std::string& file_path);
private:
    std::shared_ptr<xlnt::workbook> workbook;
    xlnt::worksheet sheet;

};


#endif //CUSTOMIZE_OCR_XLNTCONTROL_H
