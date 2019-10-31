#include "interp.h"

#include <string.h>
#include <stdlib.h>

struct tuple_s
{
    double a;
    double b;
    double c;
};

static int get_index(const double *x, const int len, const double v)
{
    // 理论弹道比较均匀还是啥
    // 算了，就用二分查找吧
    register int low = 0;
    register int hig = len - 1;
    register int mid;

    if (v < x[0])
    {
        return -1;
    }

    if (v > x[hig])
    {
        return len;
    }

    while (low != hig - 1)
    {
        mid = (low + hig) / 2;
        if (v < x[mid])
        {
            hig = mid;
        }

        if (v > x[mid])
        {
            low = mid;
        }

        if (v == x[mid])
        {
            return mid;
        }
    }

    return low;
}

int interp_spline_init(struct interp_spline_s *is, const double *x, const double *y, const int len)
{
    if (!is || !x || !y || len<=2)
    {
        return -1;
    }

    is->x = malloc(len * sizeof(double));
    if (!is->x)
    {
        goto fail;
    }

    memcpy(is->x, x, len * sizeof(double));
    is->len = len;

    is->y = malloc(sizeof(double) * len);
    if (!is->y)
    {
        goto fail;
    }

    memcpy(is->y, y, len * sizeof(double));

    double *h = malloc(sizeof(double) * (len - 1));
    if (!h)
    {
        goto fail;
    }

    int i;
    for (i=0; i<len-1; i++)
    {
        h[i] = x[i+1] - x[i];
    }

    double *m = malloc(sizeof(double) * len);
    if (!h)
    {
        goto fail;
    }

    memset(m, 0x00, sizeof(double) * len);

    struct tuple_s *t = malloc(sizeof(struct tuple_s) * len);
    if (!t)
    {
        goto fail;
    }

    for (i=1; i<len-1; i++)
    {
        t[i].a = h[i-1];
        t[i].b = 2 * (h[i-1] + h[i]);
        t[i].c = h[i];
    }

    t[0].a = 0;
    t[0].b = 1;
    t[0].c = 0;

    t[len-1].a = 0;
    t[len-1].b = 1;
    t[len-1].c = 0;

    double *r = malloc(sizeof(double) * len);
    if (!r)
    {
        goto fail;
    }

    for (i=1; i<len-1; i++)
    {
        r[i] = ((y[i+1] - y[i])/h[i] - (y[i] - y[i-1])/h[i-1]) * 6;
    }

    r[0] = 0;
    r[len-1] = 0;

    // the trigiagonal matrix algorithm
    for (i=1; i<len; i++)
    {
        t[i].b -= t[i].a / t[i-1].b * t[i-1].c;
        r[i] -= t[i].a / t[i-1].b * r[i-1];
    }

    m[len-1] = r[len-1] / t[len-1].b;
    for (i=len-2; i>=0; i--)
    {
        m[i] = (r[i] - t[i].c * m[i+1]) / t[i].b;
    }

    m[0] = 0;
    //m[len-1] = 0;
    is->m = m;
    is->h = h;

    free(t);

    return 0;

fail:
    if (is->x)
    {
        free(is->x);
    }

    if (is->y)
    {
        free(is->y);
    }

    if (is->h)
    {
        free(is->h);
    }

    if (is->m)
    {
        free(is->m);
    }
    
    if (t)
    {
        free(t);
    }

    return -1;

}

int interp_spline_free(struct interp_spline_s *is)
{
    if (!is)
    {
        return -1;
    }

    if (is->x)
    {
        free(is->x);
    }

    if (is->y)
    {
        free(is->y);
    }

    if (is->h)
    {
        free(is->h);
    }

    if (is->m)
    {
        free(is->m);
    }

    return 0;
}

double interp_spline(const struct interp_spline_s *is, const double x)
{
    int idx = get_index(is->x, is->len, x);

    printf("x-> %f idx->%d\n", x, idx);
    if (idx == -1)
    {
        idx = 0;
    }

    if (idx == is->len)
    {
        idx = is->len - 1;
    }

    double a = is->y[idx];
    double b = (is->y[idx+1] - is->y[idx]) / is->h[idx] -       \
                is->h[idx] * is->m[idx] / 2 - is->h[idx] *      \
                (is->m[idx+1] - is->m[idx]) / 6;
    double c = is->m[idx] / 2;
    double d = (is->m[idx+1] - is->m[idx]) / (6 * is->h[idx]);

    double e = x - is->x[idx];
    return ((((d * e) + c) * e) + b) * e + a;
}
