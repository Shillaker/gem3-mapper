/*
 * PROJECT: GEMMapper
 * FILE: output_map.h
 * DATE: 06/06/2013
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 */

#ifndef OUTPUT_MAP_H_
#define OUTPUT_MAP_H_

#include "utils/essentials.h"
#include "io/buffered_output_file.h"
#include "data_structures/sequence.h"
#include "archive/archive_search.h"
#include "matches/matches.h"
#include "matches/paired_matches.h"

/*
 * MAP Parameters
 */
typedef enum {
  map_format_v1 = 1, // GEMv1
  map_format_v2 = 2, // GEMv2
  map_format_v3 = 3, // GEMv3
} output_map_format_t;
typedef struct {
  /* MAP format  */
  output_map_format_t format_version;
} output_map_parameters_t;

/*
 * Setup
 */
void output_map_parameters_set_defaults(output_map_parameters_t* const restrict output_map_parameters);

/*
 * Utils
 */
void output_map_cigar(
    FILE* const restrict stream,
    match_trace_t* const restrict match_trace,
    matches_t* const restrict matches);
void output_map_alignment_pretty(
    FILE* const restrict stream,
    match_trace_t* const restrict match_trace,
    matches_t* const restrict matches,
    uint8_t* const restrict key,
    const uint64_t key_length,
    uint8_t* const restrict text,
    const uint64_t text_length,
    mm_stack_t* const restrict mm_stack);
/*
 * MAP fields
 */
void output_map_print_counters(
    buffered_output_file_t* const restrict buffered_output_file,
    matches_counters_t* const restrict matches_counter,
    const uint64_t mcs,
    const bool compact);
void output_map_print_match(
    buffered_output_file_t* const restrict buffered_output_file,
    const matches_t* const restrict matches,
    const match_trace_t* const restrict match_trace,
    const bool print_mapq,
    const output_map_format_t output_map_format);
void output_map_print_paired_match(
    buffered_output_file_t* const restrict buffered_output_file,
    const matches_t* const restrict matches_end1,
    const matches_t* const restrict matches_end2,
    const paired_map_t* const restrict paired_map,
    const output_map_format_t output_map_format);

/*
 * MAP SingleEnd
 */
void output_map_single_end_matches(
    buffered_output_file_t* const restrict buffered_output_file,
    archive_search_t* const restrict archive_search,
    matches_t* const restrict matches,
    output_map_parameters_t* const restrict output_map_parameters);

/*
 * MAP PairedEnd
 */
void output_map_paired_end_matches(
    buffered_output_file_t* const restrict buffered_output_file,
    archive_search_t* const restrict archive_search_end1,
    archive_search_t* const restrict archive_search_end2,
    paired_matches_t* const restrict paired_matches,
    output_map_parameters_t* const restrict output_map_parameters);

/*
 * FASTA/FASTQ
 */
void output_fastq(
    buffered_output_file_t* const restrict buffered_output_file,
    sequence_t* const restrict sequence);

#endif /* OUTPUT_MAP_H_ */
