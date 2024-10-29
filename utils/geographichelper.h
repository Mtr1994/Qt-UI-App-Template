#ifndef GEOGRAPHICHELPER_H
#define GEOGRAPHICHELPER_H

class GeographicHelper
{
public:
    GeographicHelper();

    // 将经纬度转换为 Mercator 坐标的函数
    static void latLonToMercator(double lat, double lon, double& x, double& y);

};

#endif // GEOGRAPHICHELPER_H
