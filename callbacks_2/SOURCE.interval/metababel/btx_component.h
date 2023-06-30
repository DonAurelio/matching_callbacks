#pragma once

#include "utarray.h"
#include "uthash.h"
#include <babeltrace2/babeltrace.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration of common_data
struct common_data_s;
typedef struct common_data_s common_data_t;

// Params
struct btx_params_s {
  char _dummy;

}; 
typedef struct btx_params_s btx_params_t;

void btx_populate_params(common_data_t *common_data_t);


struct name_to_dispatcher_s {
  const char *name;
  void *dispatcher;
  UT_array *callbacks;
  UT_hash_handle hh;
};
typedef struct name_to_dispatcher_s name_to_dispatcher_t;

// Structure for Downstream Message
struct el {
  const bt_message *message;
  struct el *next, *prev;
};

// Struct stored in the component via `bt_self_component_set_data`
struct common_data_s {
  name_to_dispatcher_t *name_to_dispatcher;
  void *usr_data;
  const bt_value *params;
  btx_params_t *btx_params;
  bt_trace *downstream_trace;
  /* Used by downstream.c */
  bt_self_message_iterator *self_message_iterator;
};

enum btx_source_state_e {
  BTX_SOURCE_STATE_INITIALIZING,
  BTX_SOURCE_STATE_PROCESSING,
  BTX_SOURCE_STATE_FINALIZING,
  BTX_SOURCE_STATE_FINISHED,
  BTX_SOURCE_STATE_ERROR,
};
typedef enum btx_source_state_e btx_source_state_t;

/* Message iterator's private data */
struct btx_message_iterator_s {
  /* (Weak) link to the component's private data */
  common_data_t *common_data;
  btx_source_state_t state;

  /* Handling the downstream message queue */
  struct el *queue;
  struct el *pool;
};
typedef struct btx_message_iterator_s btx_message_iterator_t;

enum btx_source_status_e {
  BTX_SOURCE_END,
  BTX_SOURCE_OK,
};
typedef enum btx_source_status_e btx_source_status_t;


void btx_register_usr_callbacks(void *btx_handle);

typedef void
initialize_usr_data_callback_f(void *btx_handle,
                                   void **);
void btx_register_callbacks_initialize_usr_data(
    void *btx_handle, initialize_usr_data_callback_f *callback);

void btx_call_callbacks_initialize_usr_data(
    common_data_t *common_data, void **);
typedef void
finalize_usr_data_callback_f(void *btx_handle,
                                   void *);
void btx_register_callbacks_finalize_usr_data(
    void *btx_handle, finalize_usr_data_callback_f *callback);

void btx_call_callbacks_finalize_usr_data(
    common_data_t *common_data, void *);
typedef void
read_params_callback_f(void *btx_handle,
                                   void *, btx_params_t *);
void btx_register_callbacks_read_params(
    void *btx_handle, read_params_callback_f *callback);

void btx_call_callbacks_read_params(
    common_data_t *common_data, void *, btx_params_t *);
typedef void
push_usr_messages_callback_f(void *btx_handle,
                                   void *, btx_source_status_t*);
void btx_register_callbacks_push_usr_messages(
    void *btx_handle, push_usr_messages_callback_f *callback);

void btx_call_callbacks_push_usr_messages(
    common_data_t *common_data, void *, btx_source_status_t*);

#ifdef __cplusplus
}
#endif
