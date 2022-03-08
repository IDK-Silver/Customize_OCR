#include "OCR_Image.h"

OCR_Image::OCR_Image()
{
}

OCR_Image::~OCR_Image()
{
}

void OCR_Image::load_image(cv::Mat image)
{
	this->image = image;
	return;
}

void OCR_Image::load_image(QString image_path)
{
    this->image_path = image_path.toLocal8Bit().toStdString();
}


cv::Mat OCR_Image::QImage2cvMat(QImage image)
{
    cv::Mat mat;
    //qDebug() << image.format();
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        cv::Mat mat_temp = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        cvtColor(mat_temp, mat, cv::COLOR_BGRA2BGR);
    }
    break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

std::vector<std::vector<std::string>> OCR_Image::ocr_src_image()
{
    cv::Mat srcimg = cv::imread(this->image_path, cv::IMREAD_COLOR);

    auto det = new DBDetector(ocr_config->det_model_dir, ocr_config->use_gpu, ocr_config->gpu_id,
                                            ocr_config->gpu_mem, ocr_config->cpu_threads, ocr_config->enable_mkldnn,
                                            ocr_config->max_side_len, ocr_config->det_db_thresh, ocr_config->det_db_thresh,
                                            ocr_config->det_db_unclip_ratio, ocr_config->use_polygon_score,
                                            ocr_config->visualize, ocr_config->use_tensorrt, ocr_config->precision
                                        );

    Classifier *cls = nullptr;
    //Classifier *cls = nullptr;
    if (ocr_config->use_angle_cls == true) {
        cls = new Classifier(ocr_config->cls_model_dir, ocr_config->use_gpu, ocr_config->gpu_id,
            ocr_config->gpu_mem, ocr_config->cpu_threads,
            ocr_config->enable_mkldnn, ocr_config->cls_thresh,
            ocr_config->use_tensorrt, ocr_config->precision);
    }

    auto char_list_file = ocr_config->char_list_file;
    if (ocr_config->benchmark)
        auto char_list_file = ocr_config->char_list_file.substr(6);

    auto rec = new CRNNRecognizer(ocr_config->rec_model_dir, ocr_config->use_gpu, ocr_config->gpu_id,
        ocr_config->gpu_mem, ocr_config->cpu_threads,
        ocr_config->enable_mkldnn, char_list_file,
        ocr_config->use_tensorrt, ocr_config->precision, ocr_config->rec_batch_num);

    std::vector<std::vector<std::vector<int>>> boxes;
    std::vector<double> det_times;
    det->Run(srcimg, boxes, &det_times);


    std::vector<cv::Mat> img_list;

    for (int j = 0; j < boxes.size(); j++) {
        cv::Mat crop_img;
        crop_img = Utility::GetRotateCropImage(srcimg, boxes[j]);
        if (cls != nullptr) {
            crop_img = cls->Run(crop_img);
        }
        img_list.push_back(crop_img);
    }

    std::vector<std::vector<std::string>> im_strs;
    std::vector<double> rec_times;
    rec->Run(img_list, &rec_times, &im_strs);

    delete det;
    delete rec;
    delete cls;

    return im_strs;
}

std::string OCR_Image::ocr_part_image(cv::Mat input_image, bool run_det)
{
    cv::Mat srcimg = input_image;


    DBDetector* det = nullptr;

    if (run_det)
    {
        det = new DBDetector(ocr_config->det_model_dir, ocr_config->use_gpu, ocr_config->gpu_id,
            ocr_config->gpu_mem, ocr_config->cpu_threads, ocr_config->enable_mkldnn,
            ocr_config->max_side_len, ocr_config->det_db_thresh, ocr_config->det_db_thresh,
            ocr_config->det_db_unclip_ratio, ocr_config->use_polygon_score,
            ocr_config->visualize, ocr_config->use_tensorrt, ocr_config->precision
        );
    }
        

    Classifier* cls = nullptr;
    //Classifier *cls = nullptr;
    if (ocr_config->use_angle_cls == true) {
        cls = new Classifier(ocr_config->cls_model_dir, ocr_config->use_gpu, ocr_config->gpu_id,
            ocr_config->gpu_mem, ocr_config->cpu_threads,
            ocr_config->enable_mkldnn, ocr_config->cls_thresh,
            ocr_config->use_tensorrt, ocr_config->precision);
    }

    auto char_list_file = ocr_config->char_list_file;
    if (ocr_config->benchmark)
        auto char_list_file = ocr_config->char_list_file.substr(6);

    auto rec = new CRNNRecognizer(ocr_config->rec_model_dir, ocr_config->use_gpu, ocr_config->gpu_id,
        ocr_config->gpu_mem, ocr_config->cpu_threads,
        ocr_config->enable_mkldnn, char_list_file,
        ocr_config->use_tensorrt, ocr_config->precision, ocr_config->rec_batch_num);

    std::vector<std::vector<std::vector<int>>> boxes;
    std::vector<cv::Mat> img_list;
    if (run_det)
    {
        qDebug() << "Run Det";
        std::vector<double> det_times;
        det->Run(srcimg, boxes, &det_times);

        for (int j = 0; j < boxes.size(); j++) {
            cv::Mat crop_img;
            crop_img = Utility::GetRotateCropImage(srcimg, boxes[j]);
            if (cls != nullptr) {
                crop_img = cls->Run(crop_img);
            }
            img_list.push_back(crop_img);
        }
    }
    else
    {
        img_list.push_back(srcimg);
    }


    std::vector<std::vector<std::string>> im_strs;
    std::vector<double> rec_times;
    rec->Run(img_list, &rec_times, &im_strs);

    delete det;
    delete rec;
    delete cls;

    // return im_strs;

    auto single_line = merge_src_string(im_strs);
    return single_line;
}

std::string OCR_Image::merge_src_string(std::vector<std::string> input_str)
{
    std::string split_str;
    for (int index = 0; index < input_str.size(); index++)
    {
        split_str += input_str.at(index);
    }
    return split_str;
}

std::string OCR_Image::merge_src_string(std::vector<std::vector<std::string>> stack_str)
{
    std::vector<std::string> strs;

    for (int index = 0; index < stack_str.size(); index++)
    {
        std::string single_line;
        std::vector<std::string> split_line = stack_str.at(index);
        for (int split_index = 0; split_index < split_line.size(); split_index++)
        {
            single_line += split_line.at(split_index);
        }
        strs.push_back(single_line);
    }
    return merge_src_string(strs);
}
