#include "reaktorData.h"

//コンストラクタ
reaktorData::reaktorData()
{
    bgr = cv::Vec3b(0, 0, 0);
    reaktor_idx = 0;
}
reaktorData::reaktorData(cv::Vec3b b, unsigned int id)
    : bgr(b), reaktor_idx(id)
{}