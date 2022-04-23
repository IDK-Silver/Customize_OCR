#ifndef EXCEL_WRITE_LIB_C

#define EXCEL_WRITE_LIB_C

#pragma execution_character_set("utf-8")

#include <qstring.h>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include "../QXlsx/header/xlsxdocument.h"

namespace excel_write
{
	
	class ExcelDoc : public QXlsx::Document
	{
	public:
		ExcelDoc(QString input_path);
		~ExcelDoc() = default;

		int search(QString sheet, int col, QString target);
		int search(int col, QString target);

		int str_col_en2num(QString ens)
		{
			std::shared_ptr<std::map<char, int>> en2num_dict = nullptr;
			// init dict
			if (en2num_dict == nullptr)
			{
				en2num_dict = std::make_shared<std::map<char, int>>();
				char albet[26]{};

				// most 'A' can't use "A"
				std::iota(std::begin(albet), std::end(albet), 'A');

				for (int index = 0; index < 26; index++)
				{
					en2num_dict->insert(std::pair<char, int>(albet[index], index));
				}
			}

			std::vector<int> nums;

			for (auto en : ens.toLocal8Bit().toStdString())
			{
				nums.push_back(en2num_dict->at(toupper(en)));
			}

			std::reverse(nums.begin(), nums.end());


			while (nums.size() > 1)
			{
				int last_num = nums.at(nums.size() - 1);
				nums.pop_back();
				nums.at(nums.size() - 1) *= last_num;
			}

			return nums.at(0);
		}

	private:

	};

	



};





#endif // !EXCEL_WRITE_LIB

