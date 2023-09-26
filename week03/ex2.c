#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct Point{
    float x;
    float y;
};

float distance(struct Point p1, struct Point p2){
    return sqrt( (p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y) );
};

float area(struct Point A, struct Point B, struct Point C){
    return 0.5 * abs( A.x * B.y - B.x * A.y + B.x * C.y - C.x * B.y + C.x * A.y - A.x * C.y );
}; 

int main()
{
    struct Point A = {2.5, 6};
    struct Point B = {1, 2.2};
    struct Point C = {10, 6};
    
    printf("%f\n", distance(A, B));
    
    printf("%f", area(A,B,C));
    
    return 0;
};
