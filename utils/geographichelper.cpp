#include "geographichelper.h"

#include <iostream>
#include <cmath>

// 常量定义
const double R = 6378137.0; // 地球半径，单位：米
const double PI = 3.14159265358979323846;

GeographicHelper::GeographicHelper()
{

}

void GeographicHelper::latLonToMercator(double lat, double lon, double& x, double& y)
{
    // 将角度转换为弧度
    lat = lat * PI / 180.0;
    lon = lon * PI / 180.0;

    // Mercator 投影公式
    x = R * lon;
    y = R * std::log(std::tan(PI / 4.0 + lat / 2.0));
}
