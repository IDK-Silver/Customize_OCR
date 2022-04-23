#include <iostream>
#include <QtCore>
#include "../ExcelWrite.h"
#include "../QXlsx/header/xlsxdocument.h"
#pragma execution_character_set("utf-8")
int main()
{
    //QXlsx::Document xlsx("test-data.xlsx");
    
    excel_write::ExcelDoc doc("test-data.xlsx");
    
    qDebug() << doc.search("工作表2", 2, "王靖元");
    qDebug() << doc.search(2, "王靖元");
    
    


   




}