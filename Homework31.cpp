#include <stdio.h>
#include <stdlib.h>
#include <math.h>


static double Square(double A)
{
    double Result = (A*A);
    return Result;
}

static double RadiansFromDegrees(double Degrees)
{
    double Result = 0.01745329251994329577f * Degrees;
    return Result;
}

static double ReferenceHaversine(double X0, double Y0, double X1, double Y1, double EarthRadius)
{
    double lat1 = Y0;
    double lat2 = Y1;
    double lon1 = X0;
    double lon2 = X1;

    double dLat = RadiansFromDegrees(lat2 - lat1);
    double dLon = RadiansFromDegrees(lon2 - lon1);
    lat1 = RadiansFromDegrees(lat1);
    lat2 = RadiansFromDegrees(lat2);
    
    double a = Square(sin(dLat/2.0)) + cos(lat1)*cos(lat2)*Square(sin(dLon/2));
    double c = 2.0*asin(sqrt(a));
    
    double Result = EarthRadius * c;
    
    return Result;
}

int main()
{   
    const int PointCount = 10;
    const double EarthRadius = 6372.8;
    double sum = 0;

    double x[PointCount];
    double y[PointCount];

    FILE *fptr = fopen("filename.txt", "a");
    fprintf(fptr, "{\"pairs\":[");

    for (int i = 0; i < PointCount; i++)
    { 
        double rx = (double)rand() / (double)RAND_MAX;
        rx = rx * 360;
        rx = rx - 180;

        double ry = (double)rand() / (double)RAND_MAX;
        ry = ry * 180;
        ry = ry - 90;
        
        x[i] = rx;
        y[i] = ry;

        if(i == PointCount - 1)
        {
            double haversine = ReferenceHaversine(x[i-1], y[i-1], x[i], y[i], EarthRadius);
            sum += haversine;
            fprintf(fptr, "{\"x0\":%f, \"y0\":%f, \"x1\":%f, \"y1\":%f}", x[i-1], y[i-1], x[i], y[i]);
        }
        else if(i % 2 == 1)
        {
            double haversine = ReferenceHaversine(x[i-1], y[i-1], x[i], y[i], EarthRadius);
            sum += haversine;
            fprintf(fptr, "{\"x0\":%f, \"y0\":%f, \"x1\":%f, \"y1\":%f},", x[i-1], y[i-1], x[i], y[i]);
        }
    }

    double haversineAverage = sum / PointCount*2;
    printf("The haversine average of this generator is: %f\n", haversineAverage);

    fprintf(fptr, "]");
    fprintf(fptr, "}");
    fclose(fptr);
    return 0;
}