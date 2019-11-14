/*
 *  GEM-Mapper v3 (GEM3)
 *  Copyright (c) 2011-2017 by Santiago Marco-Sola  <santiagomsola@gmail.com>
 *
 *  This file is part of GEM-Mapper v3 (GEM3).
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * PROJECT: GEM-Mapper v3 (GEM3)
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 *   Mapper module provides high-level functions to run
 *   the standard mapper workflow (SE/PE)
 */

#include <faasm/core.h>

#include "mapper/mapper.h"
#include "mapper/mapper_io.h"
#include "archive/search/archive_search_se.h"
#include "archive/search/archive_search_pe.h"
#include "text/sequence_bisulfite.h"
#include "io/output_sam.h"
#include "profiler/profiler.h"

/*
 * Profile Level
 */
#define PROFILE_LEVEL PHIGH

/*
 * Debug
 */
//#define DEBUG_MAPPER_DISPLAY_EACH_READ_TIME

/*
 * Error Report
 */
mapper_search_t* g_mapper_searches; // Global searches on going
pthread_mutex_t mapper_error_report_mutex = PTHREAD_MUTEX_INITIALIZER;
void mapper_error_report_cmd(
    FILE* stream,
    mapper_parameters_t* const mapper_parameters) {
  // Display header
  fprintf(stream,"GEM::Version %s\n",mapper_parameters->gem_version);
  fprintf(stream,"GEM::CMD");
  // Print CMD line used
  uint64_t i;
  for (i=0;i<mapper_parameters->argc;++i) {
    fprintf(stream," %s",mapper_parameters->argv[i]);
  }
  fprintf(stream,"\n");
}
void mapper_error_report_input_state(
    FILE* stream,
    buffered_input_file_t* const buffered_fasta_input,
    const sequence_t* const sequence) {
  // Display header
  fprintf(stream,"GEM::Input.State\n");
  // Check NULL
  if (sequence==NULL) { fprintf(stream,"Sequence is NULL\n"); return; }
  if (buffered_fasta_input==NULL) { fprintf(stream,"Buffered_fasta_input is NULL\n"); return; }
  // Dump FASTA/FASTQ read
  if (!string_is_null(&sequence->tag) && !string_is_null(&sequence->read)) {
    const bool has_qualities = sequence_has_qualities(sequence);
    char* const end_tag =
        (sequence->end_info == paired_end1) ? "/1" :
      ( (sequence->end_info == paired_end2) ? "/2" : " " );
    fprintf(stream,"Sequence (File '%s' Line '%"PRIu64"')\n",
        buffered_input_file_get_file_name(buffered_fasta_input),
        buffered_fasta_input->current_buffer_line_no - (has_qualities ? 4 : 2));
    if (has_qualities) {
      if (!string_is_null(&sequence->qualities)) {
        fprintf(stream,"@%"PRIs"%s\n%"PRIs"\n+\n%"PRIs"\n",
            PRIs_content(&sequence->tag),end_tag,
            PRIs_content(&sequence->read),
            PRIs_content(&sequence->qualities));
      } else {
        fprintf(stream,"@%"PRIs"%s\n%"PRIs"\n+\n<<Null Qualities>>\n",
            PRIs_content(&sequence->tag),end_tag,
            PRIs_content(&sequence->read));
      }
    } else {
      fprintf(stream,">%"PRIs"%s\n%"PRIs"\n",
          PRIs_content(&sequence->tag),end_tag,
          PRIs_content(&sequence->read));
    }
  } else {
    fprintf(stream,"Current sequence is <<Empty>>\n");
  }
}
void mapper_error_report(FILE* stream) {
  // Select thread
  const uint64_t thread_id = gem_thread_get_thread_id();
  if (thread_id==0) {
    mapper_parameters_t* const mapper_parameters = g_mapper_searches->mapper_parameters;
    MUTEX_BEGIN_SECTION(mapper_parameters->error_report_mutex) {
      fprintf(stream,"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
      fprintf(stream,"GEM::Unexpected error occurred. Sorry for the inconvenience\n"
                     "     Feedback and bug reporting it's highly appreciated,\n"
                     "     => Please report or email (gem.mapper.dev@gmail.com)\n");
      fprintf(stream,"GEM::Running-Thread (threadID = MASTER)\n");
      fprintf(stream,"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
      mapper_error_report_cmd(stream,mapper_parameters); // Display CMD used
    } MUTEX_END_SECTION(mapper_parameters->error_report_mutex);
  } else {
    mapper_search_t* const mapper_search = g_mapper_searches + (thread_id-1); // Thread
    mapper_parameters_t* const mapper_parameters = mapper_search->mapper_parameters;
    MUTEX_BEGIN_SECTION(mapper_parameters->error_report_mutex) {
      fprintf(stream,"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
      fprintf(stream,"GEM::Unexpected error occurred. Sorry for the inconvenience\n"
                     "     Feedback and bug reporting it's highly appreciated,\n"
                     "     => Please report or email (gem.mapper.dev@gmail.com)\n");
      fprintf(stream,"GEM::Running-Thread (threadID = %"PRIu64")\n",thread_id);
      fprintf(stream,"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
      mapper_error_report_cmd(stream,mapper_parameters); // Display CMD used
      fprintf(stream,"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
      // Display Input State
      mapper_io_handler_t* const mapper_io_handler = mapper_search->mapper_io_handler;
      if (!mapper_search->mapper_io_handler->paired_end) {
        const sequence_t* const sequence = *mapper_search->sequence_end1;
        mapper_error_report_input_state(stream,mapper_io_handler->buffered_fasta_input_end1,sequence);
      } else {
        const sequence_t* const sequence_end1 = *mapper_search->sequence_end1;
        const sequence_t* const sequence_end2 = *mapper_search->sequence_end2;
        mapper_error_report_input_state(stream,mapper_io_handler->buffered_fasta_input_end1,sequence_end1);
        mapper_error_report_input_state(stream,mapper_io_handler->buffered_fasta_input_end2,sequence_end2);
      }
    } MUTEX_END_SECTION(mapper_parameters->error_report_mutex);
  }
}

int read_faasm_func_input() {
    int res = 0;
    faasmGetInput((unsigned char*)&res, sizeof(int));
    return res;
}

/*
 * SE Mapper
 */
void mapper_se_thread(mapper_search_t *mapper_search) {
  // GEM-thread error handler
  // Apparently only used for profiling so can ignore
  // gem_thread_register_id(mapper_search->thread_id+1);

  // Parameters
  printf("Preparing parameters\n");
  mapper_parameters_t* const parameters = mapper_search->mapper_parameters;
  search_parameters_t* const search_parameters = &mapper_search->mapper_parameters->search_parameters;
  archive_t* const archive = parameters->archive;
  mapper_io_handler_t* mapper_io_handler;
  archive_search_handlers_t* archive_search_handlers;
  archive_search_t* archive_search;
  matches_t* matches;
  sequence_t* sequence;

  // Init search structures
  printf("Init search structures \n");
  archive_search_handlers = archive_search_handlers_new(archive);
  archive_search_se_new(search_parameters,false,&archive_search);
  matches = matches_new();

  // Init I/O handler
  printf("Init I/O handler \n");
  mapper_io_handler = mapper_io_handler_new_se(
      parameters,parameters->io.input_buffer_size,
      archive_search_handlers->mm_allocator);

  // Set structures pointers (DEBUG)
  mapper_search->mapper_io_handler = mapper_io_handler;
  mapper_search->sequence_end1 = &sequence;

  // FASTA/FASTQ reading loop
  printf("FASTA/ FASTQ reading loop \n");
  uint64_t reads_processed = 0;
  while (mapper_read_sequence(mapper_io_handler,true,&sequence)) {
//    // DEBUG
//    if (gem_streq(sequence->tag.buffer,"Sim.Illumina.l100.0000009233")) {
//      printf("HERE\n");
//    }

    // Prepare Search
    archive_search_handlers_prepare_se(archive_search,sequence,archive_search_handlers);

    // Search into the archive
#ifdef DEBUG_MAPPER_DISPLAY_EACH_READ_TIME
    gem_timer_t timer;
    TIMER_RESTART(&timer); archive_search_se(archive_search,matches); TIMER_STOP(&timer);
    fprintf(stderr,"Done %s in %2.4f ms.\n",sequence->tag.buffer,TIMER_GET_TOTAL_MS(&timer));
#else
    archive_search_se(archive_search,matches);
#endif

    // Output matches
    mapper_io_handler_output_matches(mapper_io_handler,
        archive_search,matches,mapper_search->mapping_stats);

    // Update processed
    if (++reads_processed == parameters->io.mapper_ticker_step) {
      printf("Step ticker mutex with %i reads \n", reads_processed);
      ticker_update_mutex(mapper_search->ticker,reads_processed);
      reads_processed=0;
    }

    // Clear (these are not needed => just wipe clean all mm_allocator)
    // archive_search_destroy(archive_search);
    // filtering_candidates_clear(&archive_search_handlers->filtering_candidates_end1,true);
    // Clear
    archive_search_handlers_clear(archive_search_handlers);
    matches_clear(matches);
  }
  // Update processed
  printf("Updating ticker mutex with %i reads \n", reads_processed);
  ticker_update_mutex(mapper_search->ticker,reads_processed);

  // Clean up
  matches_delete(matches);
  archive_search_delete(archive_search);
  archive_search_handlers_delete(archive_search_handlers);
  mapper_io_handler_delete(mapper_io_handler);

  // Running single threaded so we ignore this
  // pthread_exit(0);

  free(mapper_search);
}

/*
 * PE Mapper
 */
void mapper_pe_thread(mapper_search_t *mapper_search) {
  // GEM-thread error handler
  gem_thread_register_id(mapper_search->thread_id+1);

  // Parameters
  mapper_parameters_t* const parameters = mapper_search->mapper_parameters;
  search_parameters_t* const search_parameters = &mapper_search->mapper_parameters->search_parameters;
  archive_t* const archive = parameters->archive;
  mapper_io_handler_t* mapper_io_handler;
  archive_search_handlers_t* archive_search_handlers;
  archive_search_t* archive_search_end1;
  archive_search_t* archive_search_end2;
  paired_matches_t* paired_matches;
  sequence_t* sequence_end1;
  sequence_t* sequence_end2;

  // Init search structures
  archive_search_handlers = archive_search_handlers_new(archive);
  archive_search_pe_new(search_parameters,false,&archive_search_end1,&archive_search_end2);
  paired_matches = paired_matches_new();

  // Init I/O handler
  mapper_io_handler = mapper_io_handler_new_pe(
      parameters,parameters->io.input_buffer_size,
      archive_search_handlers->mapper_stats,
      archive_search_handlers->mm_allocator);

  // Set structures pointers (DEBUG)
  mapper_search->mapper_io_handler = mapper_io_handler;
  mapper_search->sequence_end1 = &sequence_end1;
  mapper_search->sequence_end2 = &sequence_end2;

  // FASTA/FASTQ reading loop
  uint64_t reads_processed = 0;
  while (mapper_read_paired_sequence(mapper_io_handler,true,&sequence_end1,&sequence_end2)) {
//    // DEBUG
//    if (gem_streq(sequence_end1->tag.buffer,"Sim.Illumina.l100.0000491385/1")) {
//      printf("HERE\n");
//    }

    // Prepare Search
    archive_search_handlers_prepare_pe(
        archive_search_end1,archive_search_end2,
        sequence_end1,sequence_end2,
        archive_search_handlers);

    // Search into the archive
#ifdef DEBUG_MAPPER_DISPLAY_EACH_READ_TIME
    gem_timer_t timer;
    TIMER_RESTART(&timer); archive_search_pe(archive_search_end1,archive_search_end2,paired_matches); TIMER_STOP(&timer);
    fprintf(stderr,"Done %s in %2.4f ms.\n",sequence_end1->tag.buffer,TIMER_GET_TOTAL_MS(&timer));
#else
    archive_search_pe(archive_search_end1,archive_search_end2,paired_matches);
#endif

    // Output matches
    mapper_io_handler_output_paired_matches(
        mapper_io_handler,archive_search_end1,archive_search_end2,
        paired_matches,mapper_search->mapping_stats);
    //output_fastq(mapper_search->buffered_output_file,&archive_search_end1->sequence);
    //output_fastq(mapper_search->buffered_output_file,&archive_search_end2->sequence);

    // Update processed
    if (++reads_processed == parameters->io.mapper_ticker_step) {
      ticker_update_mutex(mapper_search->ticker,reads_processed);
      reads_processed=0;
    }

    // Clear (these are not needed => just wipe clean all mm_allocator)
//    archive_search_destroy(archive_search_end1);
//    archive_search_destroy(archive_search_end2);
//    filtering_candidates_clear(&archive_search_handlers->filtering_candidates_end1,true);
//    filtering_candidates_clear(&archive_search_handlers->filtering_candidates_end2,true);
    // Clear
    archive_search_handlers_clear(archive_search_handlers);
    paired_matches_clear(paired_matches,true);
  }
  // Update processed
  ticker_update_mutex(mapper_search->ticker,reads_processed);

  // Clean up
  archive_search_delete(archive_search_end1);
  archive_search_delete(archive_search_end2);
  paired_matches_delete(paired_matches);
  archive_search_handlers_delete(archive_search_handlers);
  mapper_io_handler_delete(mapper_io_handler);
  pthread_exit(0);
  free(mapper_search);
}

/*
 * SE/PE runnable
 */
void mapper_run(mapper_parameters_t* const mapper_parameters,const bool paired_end) {
  // Load GEM-Index
  mapper_load_index(mapper_parameters);
  gem_cond_fatal_error_msg(
      paired_end && mapper_parameters->archive->text->run_length,
      "Archive RL-text not supported for paired-end mode (use standard index)");
  gem_cond_fatal_error_msg(
      paired_end && (mapper_parameters->archive->type == archive_dna_forward),
      "Archive no-complement not supported for paired-end mode (use standard index)");
  if (mapper_parameters->archive->gpu_index) {
    gem_warn_msg(
        "Running CPU-mode using GPU-index; "
        "CPU-index can achieve better performance "
        "(gem-indexer --gpu-index=false)");
  }

  // Set error-report function
  // Note that the global reference to mapper_searches doesn't work in the Faasm context
  g_mapper_searches = NULL;
  MUTEX_INIT(mapper_parameters->error_report_mutex);
  gem_error_set_report_function(mapper_error_report);

  // Prepare output file/parameters (SAM headers)
  archive_t* const archive = mapper_parameters->archive;
  const bool bisulfite_index = (archive->type == archive_dna_bisulfite);
  if (mapper_parameters->io.output_format==SAM) {
    output_sam_print_header(
        mapper_parameters->output_file,archive,&mapper_parameters->io.sam_parameters,
        mapper_parameters->argc,mapper_parameters->argv,mapper_parameters->gem_version);
    mapper_parameters->io.sam_parameters.bisulfite_output = bisulfite_index;
  }

  // Setup Ticker
  ticker_t ticker;
  ticker_count_reset(&ticker,mapper_parameters->misc.verbose_user,
      paired_end ? "PE::Mapping Sequences" : "SE::Mapping Sequences",0,
      mapper_parameters->io.mapper_ticker_step,false);
  ticker_add_process_label(&ticker,"#","sequences processed");
  ticker_add_finish_label(&ticker,"Total","sequences processed");
  ticker_mutex_enable(&ticker);

  // Global mapping stats here is null by default
  mapping_stats_t* const mstats = NULL;

  PROF_START(GP_MAPPER_MAPPING);

  // pthread_handler_t mapper_thread;
  int faasmFuncIdx;
  if (paired_end) {
    faasmFuncIdx = 2;
  } else {
    faasmFuncIdx = 1;
  }

  PROFILE_VTUNE_START(); // Vtune

  // Set up worker params (only have one worker)
  mapper_search_t ms;
  ms.thread_id = 1;
  ms.thread_data = mm_alloc(pthread_t);
  ms.mapper_parameters = mapper_parameters;
  ms.ticker = &ticker;
  ms.mapping_stats = NULL;

  // Call function to do work (this is where it used to be threaded)
  mapper_se_thread(&ms);

  PROFILE_VTUNE_STOP(); // Vtune
  ticker_finish(&ticker);
  ticker_mutex_cleanup(&ticker);
  PROF_STOP(GP_MAPPER_MAPPING);
	// Merge report stats
	if (mstats) {
		 merge_mapping_stats(mapper_parameters->global_mapping_stats,mstats,1);
		 mm_free(mstats);
	}
  // Clean up
	MUTEX_DESTROY(mapper_parameters->error_report_mutex);
}
void mapper_se_run(mapper_parameters_t* const mapper_parameters) {
  mapper_run(mapper_parameters,false);
}
void mapper_pe_run(mapper_parameters_t* const mapper_parameters) {
  mapper_run(mapper_parameters,true);
}
