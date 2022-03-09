#include "ImageUtility.h"

ImageUtility::ImageUtility::ImageUtility()
{
}

ImageUtility::ImageUtility::~ImageUtility()
{
}

bool ImageUtility::is_vertical(cv::Mat src_image)
{
	if (!is_horizontal(src_image))
		return true;
	return false;
}

bool ImageUtility::is_horizontal(cv::Mat src_image)
{
	auto size = src_image.size();

	if (size.width > size.height)
		return true;
	return false;
}

cv::Mat ImageUtility::rotate(cv::Mat src, int angle)
{
	// get rotation matrix for rotating the image around its center in pixel coordinates
	cv::Point2f center((src.cols - 1) / 2.0, (src.rows - 1) / 2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	// determine bounding rectangle, center not relevant
	cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), src.size(), angle).boundingRect2f();
	// adjust transformation matrix
	rot.at<double>(0, 2) += bbox.width / 2.0 - src.cols / 2.0;
	rot.at<double>(1, 2) += bbox.height / 2.0 - src.rows / 2.0;

	cv::Mat dst;
	cv::warpAffine(src, dst, rot, bbox.size());
	return dst;
}

cv::Mat ImageUtility::crop(cv::Mat src_image, int x1, int y1, int x2, int y2)
{

	// rect is create a box, x1 and y1 is the start pointer, x2 and y2 is create range
	cv::Rect rect(x1, y1, x2 - x1, y2 - y1);   
								   
	cv::Mat crop_image = src_image(rect);

	return crop_image.clone();
}

cv::Mat ImageUtility::crop(cv::Mat src_image, std::vector<int> size)
{
	if (size.size() != 4)
	{
		std::cout << "ImageUtility: crop error, the vector size is not true\nthe vector size is " << size.size();
	}
	return crop(src_image, size.at(0), size.at(1), size.at(2), size.at(3));
}


