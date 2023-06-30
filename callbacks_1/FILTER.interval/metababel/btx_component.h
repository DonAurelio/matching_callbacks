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

// NEW:
typedef const char *
matching_callbacks_event_dispatcher_f(void *btx_handle, void *usr_data, const char *event_class_name);

// Struct stored in the component via `bt_self_component_set_data`
struct common_data_s {
  name_to_dispatcher_t *name_to_dispatcher;
  name_to_dispatcher_t *name_to_matching_dispatcher;
  // NEW: we need a function, defined by the user, that let us know 
  // the stream class name of a given event, since it is not provided 
  // by lttng but the model.yaml
  matching_callbacks_event_dispatcher_f *matching_callbacks_event_dispatcher;
  /* NEW: We need to preserve the event to access the event's fields data from matching callbacks */
  const bt_event *current_event;
  void *usr_data;
  const bt_value *params;
  btx_params_t *btx_params;
  /* Component's input port (weak) */
  const bt_component_filter *component;
  bt_trace *downstream_trace;
  /* Used by downstream.c */
  bt_self_message_iterator *self_message_iterator;
};

enum btx_filter_state_e {
  BTX_FILTER_STATE_INITIALIZING,
  BTX_FILTER_STATE_PROCESSING,
  BTX_FILTER_STATE_FINALIZING,
  BTX_FILTER_STATE_FINISHED,
  BTX_FILTER_STATE_ERROR,
};
typedef enum btx_filter_state_e btx_filter_state_t;

enum btx_filter_processing_state_e {
  BTX_FILTER_PROCESSING_STATE_READING,
  BTX_FILTER_PROCESSING_STATE_SENDING,
  BTX_FILTER_PROCESSING_STATE_FINISHED,
};
typedef enum btx_filter_processing_state_e btx_filter_processing_state_t;

typedef struct el_mi {
  /* Upstream message iterator (owned by this) */
  bt_message_iterator *message_iterator;
  struct el_mi *next, *prev;
} el_mi;

/* Message iterator's private data */
struct btx_message_iterator_s {
  /* (Weak) link to the component's private data */
  common_data_t *common_data;
  /*  Upstream messages iterator */
  el_mi *head_mi;
  btx_filter_state_t state;
  btx_filter_processing_state_t processing_state;
  /* Handling the downstream message queue */
  struct el *queue;
  struct el *pool;
};
typedef struct btx_message_iterator_s btx_message_iterator_t;


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

// NEW:
void btx_register_matching_callbacks_event_dispatcher(
    void *btx_handle, matching_callbacks_event_dispatcher_f *dispatcher);

#ifdef __cplusplus
}
#endif
