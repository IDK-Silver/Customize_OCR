#ifndef ImageUtility_LIB
#define ImageUtility_LIB

#include <opencv2/opencv.hpp>



namespace ImageUtility
{
	cv::Mat crop(cv::Mat, int, int, int, int);

	cv::Mat crop(cv::Mat, std::vector<int>);

	bool is_vertical(cv::Mat);

	bool is_horizontal(cv::Mat);

	cv::Mat rotate(cv::Mat, int angle);

	class ImageUtility
	{
	public:
		ImageUtility();
		~ImageUtility();

	private:

	};
}





#endif // !ImageUtility_LIB

