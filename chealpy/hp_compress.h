#include <stdint.h>

typedef struct {
    int nside;
    ssize_t size;
    int64_t * pix;
    double * value;
} hp_sparse_t;

hp_sparse_t *
hp_sparse_from_dense(int nside, int64_t ipix_start, int64_t ipix_end, double * value);

hp_sparse_t *
hp_sparse_down_sample(hp_sparse_t * map);

double
hp_sparse_get(hp_sparse_t * map, int64_t ipix);

void
hp_sparse_free(hp_sparse_t * map);


