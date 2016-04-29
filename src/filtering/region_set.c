/*
 * PROJECT: GEMMapper
 * FILE: region_set.c
 * DATE: 06/06/2013
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 */

#include "filtering/region_set.h"

/*
 * Constants
 */
#define REGION_SET_NUN_INITIAL_INTERVALS 20

/*
 * Setup
 */
void region_set_init(region_set_t* const restrict region_set) {
  region_set->region_intervals = vector_new(REGION_SET_NUN_INITIAL_INTERVALS,region_interval_t);
}
void region_set_clear(region_set_t* const restrict region_set) {
  vector_clear(region_set->region_intervals);
}
void region_set_destroy(region_set_t* const restrict region_set) {
  vector_delete(region_set->region_intervals);
}
/*
 * Adding
 */
void region_set_add(
    region_set_t* const restrict region_set,
    const uint64_t begin_position,
    const uint64_t end_position) {
  // Allocate
  region_interval_t* region_interval;
  vector_alloc_new(region_set->region_intervals,region_interval_t,region_interval);
  // Add
  region_interval->begin_position = begin_position;
  region_interval->end_position = end_position;
}
/*
 * Sort
 */
void region_set_sort(region_set_t* const restrict region_set) {
  // TODO
}
/*
 * Set operators
 */
bool region_set_is_contained(
    region_set_t* const restrict region_set,
    const uint64_t begin_position,
    const uint64_t end_position) {
  // TODO
  return true;
}
