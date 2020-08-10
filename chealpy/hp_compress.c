#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "chealpix.h"
#include "hp_compress.h"

static int64_t
pix2pix_nest(int64_t ipix, int64_t nside1, int64_t nside2)
{
    double vec[3];
    pix2vec_nest64(nside1, ipix, vec);
    vec2pix_nest64(nside2, vec, &ipix);
    return ipix;
}

hp_sparse_t *
hp_sparse_from_dense(int nside, int64_t ipix_start, int64_t ipix_end, double * value)
{
    hp_sparse_t * map = malloc(sizeof(hp_sparse_t));
    map->nside = nside;
    map->pix = malloc(sizeof(map->pix[0]) * (ipix_end - ipix_start));
    map->value = malloc(sizeof(map->value[0]) * (ipix_end - ipix_start));
    map->size = ipix_end - ipix_start;
    ptrdiff_t i;
    for(i = 0; i < map->size; i ++) {
        map->pix[i] = ipix_start + i;
        map->value[i] = value[i];
    }
    return map;
}

hp_sparse_t *
hp_sparse_down_sample(hp_sparse_t * map_in)
{
    hp_sparse_t * map_out = malloc(sizeof(hp_sparse_t));
    map_out->nside = map_in->nside / 2;
    ssize_t max_size = map_in->size;
    int64_t * pix = malloc(sizeof(pix[0]) * max_size);
    double * value = malloc(sizeof(value[0]) * max_size);
    double last_value[4];
    int64_t last_ip2 = -1;
    int last_n = 0;
    ptrdiff_t i;
    ptrdiff_t j = -1;
    for (i = 0; i <= map_in->size; i ++) {
        int harvest = 0;
        int64_t ip2;
        if(i < map_in->size) {
            ip2 = pix2pix_nest(map_in->pix[i], map_in->nside, map_out->nside);
            if(ip2 != last_ip2) {
                harvest = 1;
            }
        } else {
            harvest = 1;
        }
        if(harvest) {
            if (j >= 0) {
                pix[j] = last_ip2;
                double sum = 0;
                int k;
                for(k = 0; k < last_n; k ++) {
                    sum += last_value[k];
                }
                value[j] = sum / last_n;
            }
            j++;
        } 
        if(i < map_in->size) {
            if(harvest) {
                last_n = 0;
                last_ip2 = ip2;
            }
        } else {
            break;
        }
        if(last_n == 4) { raise(); }
        last_value[last_n] = map_in->value[i];
        last_n++;
    }
    map_out->size = j;
    map_out->pix = malloc(sizeof(map_out->pix[0]) * map_out->size);
    map_out->value = malloc(sizeof(map_out->value[0]) * map_out->size);
    for(i = 0; i < map_out->size; i ++) {
        map_out->pix[i] = pix[i];
        map_out->value[i] = value[i];
    }
    free(pix);
    free(value);
    return map_out;
}

void
hp_sparse_free(hp_sparse_t * map)
{
    free(map->pix);
    free(map->value);
    free(map);
}

static int
int64_cmp(const void * p1, const void * p2)
{
    const int64_t *i1 = p1;
    const int64_t *i2 = p2;
    return (*i2 < *i1) - (*i1 < *i2);
}

double
hp_sparse_get(hp_sparse_t * map, int64_t ipix)
{
    int64_t * ptr = bsearch(&ipix, map->pix, map->size, sizeof(int64_t), int64_cmp);
    if(ptr == NULL) {
        return 0;
    }
    ptrdiff_t idx = ptr - map->pix;
    return map->value[idx];
}
