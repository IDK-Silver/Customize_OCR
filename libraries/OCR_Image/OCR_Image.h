#ifndef OCR_Image_Part
#define OCR_Image_Part

#include <memory>
#include <string>

#include <opencv2/opencv.hpp>
#include <qstring.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qdebug.h>

#include <include/ocr_det.h>
#include <include/ocr_cls.h>
#include <include/ocr_rec.h>
#include <include/utility.h>

using namespace PaddleOCR;

struct OCR_Data
{
	cv::Mat image;
	std::string string;
};


struct OCR_Config
{
	bool use_gpu = false;
	int gpu_id = 0;
	int gpu_mem = 0;
	int cpu_threads = 64;
	bool enable_mkldnn = true;
	bool use_tensorrt = false;
	std::string precision = "fp32";
	bool benchmark = false;
	
	// detection related
	std::string image_dir = "";
	std::string det_model_dir = "model/ch_PP-OCRv2_det_infer";
	int max_side_len = 960;
	double det_db_thresh = 0.0;
	double det_db_box_thresh = 0.5;
	double det_db_unclip_ratio = 1.6; // 1.6
	bool use_polygon_score = false;
	bool visualize = true;

	// classification related
	bool use_angle_cls = true;
	std::string cls_model_dir = "model/ch_ppocr_mobile_v2.0_cls_slim_infer";
	double cls_thresh = 0.9;

	// recognition related
	std::string rec_model_dir = "model/chinese_cht_mobile_v2.0_rec_infer";
	std::string en_rec_model_dir = "model/en_number_mobile_v2.0_rec_slim_infer";
	int rec_batch_num = 50;
	std::string char_list_file = "utils/dict/chinese_cht_dict.txt";

};

class OCR_Image
{
public:
	OCR_Image();
	~OCR_Image();

	//variable
	OCR_Config* ocr_config = new OCR_Config;

	void load_image(cv::Mat image);
	void load_image(QString);

	cv::Mat QImage2cvMat(QImage);
	QPixmap Mat2QPixmap(cv::Mat);

	std::vector<std::vector<std::string>> ocr_src_image();
	std::string ocr_part_image(cv::Mat src_image, bool run_dat, std::string choose_rec_model_dir);

	std::string merge_src_string(std::vector<std::string> input_str);
	std::string merge_src_string(std::vector<std::vector<std::string>>);

private:
	cv::Mat image;
	cv::String image_path;

};


#endif