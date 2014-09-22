/*
 * PROJECT: GEMMapper
 * FILE: sequence.c
 * DATE: 20/08/2012
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION: Simple data structure to store genomic reads
 */

#include "sequence.h"

#define SEQUENCE_TAG_INITIAL_LENGTH 80
#define SEQUENCE_TAG_ATTRIBUTE_INITIAL_LENGTH 40
#define SEQUENCE_INITIAL_LENGTH 200

/*
 * Constructor
 */
GEM_INLINE void sequence_init(sequence_t* const sequence) {
  string_init(&sequence->tag,SEQUENCE_TAG_INITIAL_LENGTH);
  string_init(&sequence->read,SEQUENCE_INITIAL_LENGTH);
  string_init(&sequence->qualities,SEQUENCE_INITIAL_LENGTH);
  sequence->attributes.end_info = SINGLE_END;
  string_init(&sequence->attributes.casava_tag,SEQUENCE_TAG_ATTRIBUTE_INITIAL_LENGTH);
  string_init(&sequence->attributes.extra_tag,SEQUENCE_TAG_ATTRIBUTE_INITIAL_LENGTH);
}
GEM_INLINE void sequence_init_mm(sequence_t* const sequence,mm_stack_t* const mm_stack) {
  string_init_mm(&sequence->tag,SEQUENCE_TAG_INITIAL_LENGTH,mm_stack);
  string_init_mm(&sequence->read,SEQUENCE_INITIAL_LENGTH,mm_stack);
  string_init_mm(&sequence->qualities,SEQUENCE_INITIAL_LENGTH,mm_stack);
  sequence->attributes.end_info = SINGLE_END;
  string_init_mm(&sequence->attributes.casava_tag,SEQUENCE_TAG_ATTRIBUTE_INITIAL_LENGTH,mm_stack);
  string_init_mm(&sequence->attributes.extra_tag,SEQUENCE_TAG_ATTRIBUTE_INITIAL_LENGTH,mm_stack);
}
GEM_INLINE void sequence_clear(sequence_t* const sequence) {
  SEQUENCE_CHECK(sequence);
  string_clear(&sequence->tag);
  string_clear(&sequence->read);
  string_clear(&sequence->qualities);
  sequence->attributes.end_info = SINGLE_END;
  string_clear(&sequence->attributes.casava_tag);
  string_clear(&sequence->attributes.extra_tag);
}
GEM_INLINE void sequence_destroy(sequence_t* const sequence) {
  SEQUENCE_CHECK(sequence);
  string_destroy(&sequence->tag);
  string_destroy(&sequence->read);
  string_destroy(&sequence->qualities);
  string_destroy(&sequence->attributes.casava_tag);
  string_destroy(&sequence->attributes.extra_tag);
}
/*
 * Accessors
 */
GEM_INLINE uint64_t sequence_get_length(sequence_t* const sequence) {
  SEQUENCE_CHECK(sequence);
  return string_get_length(&sequence->read);
}
GEM_INLINE void sequence_set_tag(sequence_t* sequence,char* const text,const uint64_t length) {
  SEQUENCE_CHECK(sequence);
  string_set_buffer(&sequence->tag,text,length);
}
GEM_INLINE char* sequence_get_tag(sequence_t* const sequence) {
  SEQUENCE_CHECK(sequence);
  return string_get_buffer(&sequence->tag);
}
GEM_INLINE void sequence_set_read(sequence_t* sequence,char* const text,const uint64_t length) {
  SEQUENCE_CHECK(sequence);
  string_set_buffer(&sequence->read,text,length);
}
GEM_INLINE char* sequence_get_read(sequence_t* const sequence) {
  SEQUENCE_CHECK(sequence);
  return string_get_buffer(&sequence->read);
}
GEM_INLINE void sequence_set_qualities(sequence_t* sequence,char* const text,const uint64_t length) {
  SEQUENCE_CHECK(sequence);
  string_set_buffer(&sequence->qualities,text,length);
}
GEM_INLINE char* sequence_get_qualities(sequence_t* const sequence) {
  SEQUENCE_CHECK(sequence);
  return string_get_buffer(&sequence->qualities);
}
GEM_INLINE bool sequence_has_qualities(const sequence_t* const sequence) {
  SEQUENCE_CHECK(sequence);
  return !string_is_null(&sequence->qualities);
}
GEM_INLINE bool sequence_has_casava_tag(const sequence_t* const sequence) {
  SEQUENCE_CHECK(sequence);
  return !string_is_null(&sequence->attributes.casava_tag);
}
GEM_INLINE bool sequence_has_extra_tag(const sequence_t* const sequence) {
  SEQUENCE_CHECK(sequence);
  return !string_is_null(&sequence->attributes.extra_tag);
}
GEM_INLINE sequence_end_t sequence_get_end_info(const sequence_t* const sequence) {
  SEQUENCE_CHECK(sequence);
  return sequence->attributes.end_info;
}
/*
 * Utils
 */
GEM_INLINE bool sequence_equals(sequence_t* const sequence_a,sequence_t* const sequence_b) {
  return string_equals(&sequence_a->read,&sequence_b->read);
}
GEM_INLINE void sequence_generate_reverse(sequence_t* const sequence,sequence_t* const rev_sequence) {
  // Prepare rc_string (Read)
  const uint64_t seq_buffer_length = string_get_length(&sequence->read);
  string_resize(&rev_sequence->read,seq_buffer_length);
  string_clear(&rev_sequence->read);
  // Reverse Read
  int64_t pos;
  const char* const seq_buffer = string_get_buffer(&sequence->read);
  for (pos=seq_buffer_length-1;pos>=0;--pos) {
    string_append_char(&rev_sequence->read,seq_buffer[pos]);
  }
  string_append_eos(&rev_sequence->read);
  // Reverse Qualities
  if (sequence_has_qualities(sequence)) {
    string_copy_reverse(&rev_sequence->qualities,&sequence->qualities);
  }
}
GEM_INLINE void sequence_generate_reverse_complement(sequence_t* const sequence,sequence_t* const rc_sequence) {
  // Prepare rc_string (Read)
  const uint64_t seq_buffer_length = string_get_length(&sequence->read);
  string_resize(&rc_sequence->read,seq_buffer_length);
  string_clear(&rc_sequence->read);
  // Reverse-Complement Read
  int64_t pos;
  const char* const seq_buffer = string_get_buffer(&sequence->read);
  for (pos=seq_buffer_length-1;pos>=0;--pos) {
    string_append_char(&rc_sequence->read,dna_complement(seq_buffer[pos]));
  }
  string_append_eos(&rc_sequence->read);
  // Reverse Qualities
  if (sequence_has_qualities(sequence)) {
    string_copy_reverse(&rc_sequence->qualities,&sequence->qualities);
  }
}

