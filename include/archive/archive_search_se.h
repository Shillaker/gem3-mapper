/*
 * PROJECT: GEMMapper
 * FILE: archive_search_se.h
 * DATE: 06/06/2012
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION: // TODO
 */

#ifndef ARCHIVE_SEARCH_SE_H_
#define ARCHIVE_SEARCH_SE_H_

#include "utils/essentials.h"
#include "archive/archive_search.h"
#include "matches/matches_predictors.h"

/*
 * Memory Injection (Support Data Structures)
 */
void archive_search_se_inject_mm(
    archive_search_t* const restrict archive_search,
    mm_search_t* const restrict mm_search);

/*
 * Archive Search SE Continue
 */
void archive_search_se_continue(
    archive_search_t* const restrict archive_search,
    matches_t* const restrict matches);

/*
 * Single-End Indexed Search (SE Online Approximate String Search)
 */
void archive_search_se(
    archive_search_t* const restrict archive_search,
    matches_t* const restrict matches);

/*
 * Compute Predictors
 */
void archive_search_se_compute_predictors(
    archive_search_t* const restrict archive_search,
    matches_t* const restrict matches,
    matches_predictors_t* const restrict predictors);

/*
 * Display
 */
void archive_search_se_print(
    FILE* const restrict stream,
    archive_search_t* const restrict archive_search,
    matches_t* const restrict matches);

#endif /* ARCHIVE_SEARCH_SE_H_ */
