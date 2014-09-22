/*
 * PROJECT: GEMMapper
 * FILE: essentials.h
 * DATE: 06/06/2012
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION: Basic includes for GEM
 */
#ifndef ESSENTIALS_H_
#define ESSENTIALS_H_

// Common constants/SysIncludes
#include "commons.h"

// Error/Msg handling module
#include "report.h"
#include "errors.h"

// GThreads
#include "gthread.h"

// File Manager
#include "fm.h"

// Memory Manager module
#include "mm.h"
#include "mm_slab.h"
#include "mm_stack.h"
#include "mm_pool.h"

// Basic Data Structures
#include "vector.h"
#include "priority_queue.h"
#include "segmented_vector.h"
#include "hash.h"
#include "string_buffer.h"
#include "dna_text.h"

// Stats Structures
#include "stats_vector.h"
#include "stats_matrix.h"

// Basic Profiling
#include "profiler.h"

// Mapper Profiling
#include "mapper_profile.h"

#endif /* ESSENTIALS_H_ */
