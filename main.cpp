#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui_c.h"

#include <iostream>
using namespace cv;
void my_filter(Mat in,int size){// Мой фильтр
    int sum;

    for(int i =0; i < in.rows; i++){
        for(int k = 0; k <in.cols; k ++){
            for(int j = size/-2;j<=size/2;j++){
                for(int l = size/-2;l<=size/2;l++){
                    sum = sum+in.at<uchar>(i+j,k+l);

                }
            }
            in.at<uchar>(i,k) = sum/(size*size);
            sum = 0;

        }
    }
}
Mat show_spectrum_magnitude(Mat I){
    Mat padded;
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols );
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);
    dft(complexI, complexI);

    split(complexI, planes);
    magnitude(planes[0], planes[1], planes[0]);
    Mat magI = planes[0];
    magI += Scalar::all(1);
    log(magI, magI);

    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols/2;
    int cy = magI.rows/2;
    Mat q0(magI, Rect(0, 0, cx, cy));
    Mat q1(magI, Rect(cx, 0, cx, cy));
    Mat q2(magI, Rect(0, cy, cx, cy));
    Mat q3(magI, Rect(cx, cy, cx, cy));
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
    normalize(magI, magI, 0, 1, CV_MINMAX);

    return magI;

}
int main(int argc, char ** argv)
{
    Mat I = imread("/home/tokjan/ClionProjects/Furie/lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);//Путь

    imshow("Orignial"       , I   );    //До
    imshow("spectrum magnitude-Original", show_spectrum_magnitude(I));

    my_filter(I,9);//Фильтр
    imshow("Filter"       , I   );   //После
    imshow("spectrum magnitude-Filter", show_spectrum_magnitude(I));

    waitKey();
    return 0;
}