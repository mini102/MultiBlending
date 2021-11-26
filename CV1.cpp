// CV1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
const int PYR_LEVEL = 6;


std::vector<Mat> buildGaussianPyramid(const Mat& src) {
    std::vector<Mat> GaussianPyramid(PYR_LEVEL);
    GaussianPyramid[0] = src;
    for (auto i = 1;i < PYR_LEVEL;i++) {
        pyrDown(GaussianPyramid[i - 1], GaussianPyramid[i]);
    }
    return GaussianPyramid;
}

std::vector<Mat> buildLaplacianPyramid(const Mat& src) {

    std::vector<Mat> LP(PYR_LEVEL);
    LP[0] = src;
    for (int i = 1;i < PYR_LEVEL;i++) {
        pyrDown(LP[i - 1], LP[i]);
        Mat temp;
        pyrUp(LP[i], temp, LP[i - 1].size());
        LP[i - 1] -= temp;
    }
    return LP;
}
Mat reconstruct(const std::vector<Mat>& apple, const std::vector<Mat>& orange, std::vector<Mat>& mask, std::vector<Mat>& reverse) {
    Mat out = apple.back(); //제일 낮은 것
    Mat outy = orange.back();
    std::vector<Mat> res(PYR_LEVEL);
    std::vector<Mat> reso(PYR_LEVEL);
    std::vector<Mat> pyramid(PYR_LEVEL);
    /*cv::multiply(apple[0], mask[0], res[0]);
    cv::imshow("res0", res[0]);
    cv::waitKey();*/
    //cv::imshow("res", reverse[0]);
    //cv::waitKey();
    for (int i = int(apple.size() - 1);i >= 0;i--) {  
        std::string windowName = "res" + std::to_string(i);
        cv::multiply(apple[i], mask[i], res[i]); //apple
        cv::multiply(orange[i], reverse[i], reso[i]); //orange
        add(res[i], reso[i], pyramid[i]); //adding
        //cv::imshow(windowName, res[i]);
        //cv::waitKey();
    }
    Mat result = pyramid.back();
    //reconstuction
    for (int i = int(pyramid.size()) - 2;i >= 0;i--) {
        pyrUp(result, result, pyramid[i].size());
        result += pyramid[i];
    }
    return result;
}
int main(int argc, const char* argv[])
{
    Mat apple = imread("C://Users//서채민//source//img//burt_apple.png");
    Mat orange = imread("C://Users//서채민//source//img//burt_orange.png");
    Mat mask = imread("C://Users//서채민//source//img//burt_mask.png");

    Mat G;
    Mat M;
    Mat Apple;
    apple.convertTo(Apple, CV_32F, 1 / 255.f);
    Mat Orange;
    orange.convertTo(Orange, CV_32F, 1 / 255.f);

    mask.convertTo(M, CV_32F, 1 / 255.f);
    mask.convertTo(G, CV_32F, 1 / 255.f);

    std::vector<Mat> apple_Pyramid = buildLaplacianPyramid(Apple);
    std::vector<Mat> orange_Pyramid = buildLaplacianPyramid(Orange);
    std::vector<Mat> Mask = buildGaussianPyramid(G);
    //imshow("before", apple_Pyramid[0]);
    M = Scalar(1, 1, 1) - G;
    //imshow("after", M);
    std::vector<Mat> ReverseMask = buildGaussianPyramid(M);
    //std::vector<Mat> Pyramid;
    Mat res = reconstruct(apple_Pyramid, orange_Pyramid, Mask, ReverseMask);
    imshow("res", res);
    waitKey();
    return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
