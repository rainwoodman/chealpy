#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h> // For fprintf, stderr
#include <limits.h> // For SSIZE_MAX

#include "Python.h" // For Python C-API error handling

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
    hp_sparse_t * map_out = NULL;
    int64_t * pix = NULL;
    double * value = NULL;

    if (!map_in) {
        PyErr_SetString(PyExc_ValueError, "Input map is NULL in hp_sparse_down_sample");
        return NULL;
    }
    if (map_in->nside <= 0 || (map_in->nside % 2 != 0)) {
        PyErr_SetString(PyExc_ValueError, "Input map nside must be positive and even");
        return NULL;
    }

    map_out = malloc(sizeof(hp_sparse_t));
    if (!map_out) {
        PyErr_NoMemory();
        return NULL;
    }
    map_out->nside = map_in->nside / 2;
    map_out->pix = NULL; // Initialize to allow cleanup
    map_out->value = NULL; // Initialize to allow cleanup

    ssize_t max_size = map_in->size;

    if (max_size < 0) { // Should not happen if map_in->size is ssize_t and positive
        PyErr_Format(PyExc_ValueError, "Input map size is negative: %zd", max_size);
        goto error_exit;
    }
    // Check for potential overflow before multiplication for initial buffers
    if (max_size > 0 && ( (SSIZE_MAX / sizeof(pix[0])) < (size_t)max_size ) ) {
        PyErr_Format(PyExc_OverflowError, "Input map size too large for pix buffer allocation: %zd", max_size);
        goto error_exit;
    }
    pix = malloc(sizeof(pix[0]) * max_size);
    if (!pix && max_size > 0) { // Check max_size > 0 because malloc(0) can be NULL or valid
        PyErr_NoMemory();
        goto error_exit;
    }

    if (max_size > 0 && ( (SSIZE_MAX / sizeof(value[0])) < (size_t)max_size ) ) {
        PyErr_Format(PyExc_OverflowError, "Input map size too large for value buffer allocation: %zd", max_size);
        goto error_exit;
    }
    value = malloc(sizeof(value[0]) * max_size);
    if (!value && max_size > 0) {
        PyErr_NoMemory();
        goto error_exit;
    }

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
        if(last_n == 4) { 
            PyErr_SetString(PyExc_RuntimeError, "Buffer overflow in hp_sparse_down_sample (last_n)");
            goto error_exit; // Ensure cleanup
        }
        last_value[last_n] = map_in->value[i];
        last_n++;
    }
    map_out->size = j;

    if (map_out->size < 0) { // j should not be negative if logic is correct
        PyErr_Format(PyExc_RuntimeError, "Calculated map_out size is negative: %zd", map_out->size);
        goto error_exit;
    }

    // Check for potential overflow for final map_out allocations
    if (map_out->size > 0 && ( (SSIZE_MAX / sizeof(map_out->pix[0])) < (size_t)map_out->size ) ) {
        PyErr_Format(PyExc_OverflowError, "Output map size too large for pix buffer: %zd", map_out->size);
        goto error_exit;
    }
    map_out->pix = malloc(sizeof(map_out->pix[0]) * map_out->size);
    if (!map_out->pix && map_out->size > 0) {
        PyErr_NoMemory();
        goto error_exit;
    }

    if (map_out->size > 0 && ( (SSIZE_MAX / sizeof(map_out->value[0])) < (size_t)map_out->size ) ) {
        PyErr_Format(PyExc_OverflowError, "Output map size too large for value buffer: %zd", map_out->size);
        goto error_exit;
    }
    map_out->value = malloc(sizeof(map_out->value[0]) * map_out->size);
    if (!map_out->value && map_out->size > 0) {
        PyErr_NoMemory();
        goto error_exit;
    }

    for(i = 0; i < map_out->size; i ++) {
        map_out->pix[i] = pix[i];
        map_out->value[i] = value[i];
    }
    free(pix); // pix is temporary buffer, now copied or not needed
    free(value); // value is temporary buffer
    return map_out;

error_exit:
    free(pix);
    free(value);
    if (map_out) {
        free(map_out->pix); // These might be NULL if not allocated yet
        free(map_out->value);
        free(map_out);
    }
    return NULL;
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
