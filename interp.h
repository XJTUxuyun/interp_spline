#ifndef __SRC_INTERP_H
#define __SRC_INTERP_H

struct interp_spline_s
{
    double *x;
    double *y;
    double *h;
    double *m;
    int len;
};

int interp_spline_init(struct interp_spline_s *is, const double *x,const double *y, const int len);

int interp_spline_free(struct interp_spline_s *is);

double interp_spline(const struct interp_spline_s *is, const double x);

#endif
