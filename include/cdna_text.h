/*
 * PROJECT: GEMMapper
 * FILE: compact_dna_text.h
 * DATE: 06/06/2013
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 *   Provides functionality to handle a compact representation of a 8-alphabet text // TODO
 */

#ifndef CDNA_TEXT_H_
#define CDNA_TEXT_H_

#include "essentials.h"

#include "segmented_vector.h"
#include "cdna_bitwise_text.h"

/*
 * Checkers
 */
#define CDNA_TEXT_CHECK(cdna_text) \
  GEM_CHECK_NULL(cdna_text)
#define CDNA_TEXT_ITERATOR_CHECK(cdna_text_iterator) \
  GEM_CHECK_NULL(cdna_text_iterator)
#define CDNA_TEXT_ITERATOR_HUB_CHECK(iterator_hub) \
  GEM_CHECK_NULL(iterator_hub)

/*
 * Compact DNA Text
 */
typedef struct {
  /* Text */
  svector_t* text;         // Compacted 3bits-character text (21 chars in each 8Bytes)
  uint64_t text_length;    // Total text length (number of characters)
  /* Iterator */
  svector_iterator_t text_iterator;
  /* Internals (Builder) */
  uint64_t position_mod21;
  uint64_t current_word;
  /* MM */
  mm_slab_t* mm_slab;
} cdna_text_t;
typedef struct {
  /* Text */
  cdna_text_t* cdna_text;
  svector_iterator_t text_iterator;
  bool eoi;
  /* Internals (Iterator) */
  uint64_t position;
  uint64_t position_mod21;
  uint64_t current_word;
} cdna_text_iterator_t;
typedef struct {
  /* Text(s) */
  uint64_t num_texts;
  /* Iterators */
  cdna_text_iterator_t* iterators;
  uint64_t* limits;
  /* Current State */
  uint64_t text_position;
  uint64_t iterator_num;
  bool eoi;
} cdna_text_iterator_hub_t;

extern const uint64_t cdna_text_block_mask_left[256];

/*
 * CDNA Text (Builder incorporated)
 */
GEM_INLINE cdna_text_t* cdna_text_new(mm_slab_t* const mm_slab);
GEM_INLINE void cdna_text_delete(cdna_text_t* const cdna_text);
GEM_INLINE void cdna_text_add_char(cdna_text_t* const cdna_text,const uint8_t enc_char);
GEM_INLINE void cdna_text_close(cdna_text_t* const cdna_text);

GEM_INLINE void cdna_text_write_as_bitwise_text(fm_t* const file_manager,cdna_text_t* const cdna_text);

/*
 * CDNA Accessors
 */
GEM_INLINE uint64_t cdna_text_get_length(cdna_text_t* const cdna_text);

/*
 * CDNA Text Iterator
 */
GEM_INLINE void cdna_text_iterator_init(
    cdna_text_iterator_t* const iterator,cdna_text_t* const cdna_text,const uint64_t position);
GEM_INLINE bool cdna_text_iterator_eoi(cdna_text_iterator_t* const iterator);
GEM_INLINE uint8_t cdna_text_iterator_get_char_encoded(cdna_text_iterator_t* const iterator);
GEM_INLINE void cdna_text_iterator_next_char(cdna_text_iterator_t* const iterator);

/*
 * CDNA Text Reverse Iterator
 *   Note that:
 *    - Only works with an open cdna_text (cdna_text_close(.) not called)
 *    - Traverses the text from last added character until the first
 */
GEM_INLINE void cdna_text_reverse_iterator_init(
    cdna_text_iterator_t* const iterator,cdna_text_t* const cdna_text,const uint64_t position);
GEM_INLINE bool cdna_text_reverse_iterator_eoi(cdna_text_iterator_t* const iterator);
GEM_INLINE uint8_t cdna_text_reverse_iterator_get_char_encoded(cdna_text_iterator_t* const iterator);
GEM_INLINE void cdna_text_reverse_iterator_next_char(cdna_text_iterator_t* const iterator);

/*
 * CDNA Text Compare
 */
GEM_INLINE void cdna_text_block_iterator_init(cdna_text_iterator_t* const iterator,cdna_text_t* const cdna_text,const uint64_t position);
GEM_INLINE void cdna_text_block_iterator_next_block(cdna_text_iterator_t* const iterator);
GEM_INLINE uint64_t cdna_text_block_iterator_get_block(cdna_text_iterator_t* const iterator);
GEM_INLINE int cdna_text_block_cmp(cdna_text_iterator_t* const iterator_a,cdna_text_iterator_t* const iterator_b);

/*
 * CDNA Text (Iterator-HUB)
 */
GEM_INLINE cdna_text_iterator_hub_t* cdna_text_iterator_hub_new(
    cdna_text_t** cdna_texts,const uint64_t num_texts,
    const uint64_t starting_position);
GEM_INLINE void cdna_text_iterator_hub_delete(cdna_text_iterator_hub_t* const iterator_hub);
GEM_INLINE bool cdna_text_iterator_hub_eoi(cdna_text_iterator_hub_t* const iterator_hub);
GEM_INLINE uint8_t cdna_text_iterator_hub_get_char_encoded(cdna_text_iterator_hub_t* const iterator_hub);
GEM_INLINE void cdna_text_iterator_hub_next_char(cdna_text_iterator_hub_t* const iterator_hub);

/*
 * Display
 */
GEM_INLINE void cdna_text_print(FILE* const stream,cdna_text_t* const cdna_text);

/*
 * Errors
 */
#define GEM_ERROR_CDNA_INDEX_OUT_OF_RANGE "Compact DNA-Text. Requested index (%lu) out of range [0,%lu)"
#define GEM_ERROR_CDNA_NOT_VALID_CHARACTER "Compact DNA-Text Builder. Invalid character provided ASCII='%d'"

#endif /* CDNA_TEXT_H_ */
