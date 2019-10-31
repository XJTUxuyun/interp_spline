#include "interp.h"

int main(int argc, char **argv)
{
    double x[] = {1, 2, 3, 4, 5};
    double y[] = {1, 4, 9, 16, 25};
    struct interp_spline_s is;
    interp_spline_init(&is, x, y, 5);
    double x1 = 2.5;
    double y1 = interp_spline(&is, x1);
    printf("interp %f -> %f\n", x1, y1);
    return 0;
}
