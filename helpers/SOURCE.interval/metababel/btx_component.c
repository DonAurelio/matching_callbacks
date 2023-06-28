#include "utarray.h"
#include "uthash.h"
#include <assert.h>
#include <babeltrace2/babeltrace.h>
#include <metababel/btx_component.h>
// stdio needed because `params_definition` contain fprintf
#include <stdio.h>

void btx_populate_params(common_data_t *common_data) {
  const bt_value *params = common_data->params;
  btx_params_t *usr_params = common_data->btx_params;
  (void)params;
  (void)usr_params;
}


typedef void btx_dispatch_initialize_usr_data_f(
    UT_array *callbacks, common_data_t *common_data,
    void **);

static void btx_dispatch_initialize_usr_data(
    UT_array *callbacks, common_data_t *common_data,
    void ** usr_data_p) {

  // Call all the callbacks who where registered
  // Their type are declared in 'upstream.h'
  initialize_usr_data_callback_f **p = NULL;
  while ((p = utarray_next(callbacks, p))) {
    (*p)((void *)common_data, usr_data_p);
  }
}

void btx_register_callbacks_initialize_usr_data(
    void *btx_handle, initialize_usr_data_callback_f *callback) {
  // Look-up our dispatcher
  name_to_dispatcher_t *s = NULL;
  name_to_dispatcher_t **name_to_dispatcher =
      &((common_data_t *)btx_handle)->name_to_dispatcher;
  HASH_FIND_STR(*name_to_dispatcher, "initialize_usr_data", s);
  if (!s) {
    // We didn't find the dispatcher, so we need to:
    // 1. Create it
    s = (name_to_dispatcher_t *)malloc(sizeof(name_to_dispatcher_t));
    s->name = "initialize_usr_data";
    s->dispatcher = (void *)&btx_dispatch_initialize_usr_data;
    utarray_new(s->callbacks, &ut_ptr_icd);
    // 2. Register it
    HASH_ADD_KEYPTR(hh, *name_to_dispatcher, s->name, strlen(s->name), s);
    // 3. Add the callbacks to the array
    utarray_push_back(s->callbacks, &callback);
  } else {
    assert(false && "Only one callbacks for initialize_usr_data");
  }
}

void btx_call_callbacks_initialize_usr_data(
    common_data_t *common_data,
    void ** usr_data_p) {
  name_to_dispatcher_t *s = NULL;
  HASH_FIND_STR(common_data->name_to_dispatcher, "initialize_usr_data", s);

  if (s)
    (*((btx_dispatch_initialize_usr_data_f(*))(s->dispatcher)))(
        s->callbacks, common_data, usr_data_p);
}

typedef void btx_dispatch_finalize_usr_data_f(
    UT_array *callbacks, common_data_t *common_data,
    void *);

static void btx_dispatch_finalize_usr_data(
    UT_array *callbacks, common_data_t *common_data,
    void * usr_data) {

  // Call all the callbacks who where registered
  // Their type are declared in 'upstream.h'
  finalize_usr_data_callback_f **p = NULL;
  while ((p = utarray_next(callbacks, p))) {
    (*p)((void *)common_data, usr_data);
  }
}

void btx_register_callbacks_finalize_usr_data(
    void *btx_handle, finalize_usr_data_callback_f *callback) {
  // Look-up our dispatcher
  name_to_dispatcher_t *s = NULL;
  name_to_dispatcher_t **name_to_dispatcher =
      &((common_data_t *)btx_handle)->name_to_dispatcher;
  HASH_FIND_STR(*name_to_dispatcher, "finalize_usr_data", s);
  if (!s) {
    // We didn't find the dispatcher, so we need to:
    // 1. Create it
    s = (name_to_dispatcher_t *)malloc(sizeof(name_to_dispatcher_t));
    s->name = "finalize_usr_data";
    s->dispatcher = (void *)&btx_dispatch_finalize_usr_data;
    utarray_new(s->callbacks, &ut_ptr_icd);
    // 2. Register it
    HASH_ADD_KEYPTR(hh, *name_to_dispatcher, s->name, strlen(s->name), s);
    // 3. Add the callbacks to the array
    utarray_push_back(s->callbacks, &callback);
  } else {
    assert(false && "Only one callbacks for finalize_usr_data");
  }
}

void btx_call_callbacks_finalize_usr_data(
    common_data_t *common_data,
    void * usr_data) {
  name_to_dispatcher_t *s = NULL;
  HASH_FIND_STR(common_data->name_to_dispatcher, "finalize_usr_data", s);

  if (s)
    (*((btx_dispatch_finalize_usr_data_f(*))(s->dispatcher)))(
        s->callbacks, common_data, usr_data);
}

typedef void btx_dispatch_read_params_f(
    UT_array *callbacks, common_data_t *common_data,
    void *, btx_params_t *);

static void btx_dispatch_read_params(
    UT_array *callbacks, common_data_t *common_data,
    void * usr_data, btx_params_t * usr_params) {

  // Call all the callbacks who where registered
  // Their type are declared in 'upstream.h'
  read_params_callback_f **p = NULL;
  while ((p = utarray_next(callbacks, p))) {
    (*p)((void *)common_data, usr_data, usr_params);
  }
}

void btx_register_callbacks_read_params(
    void *btx_handle, read_params_callback_f *callback) {
  // Look-up our dispatcher
  name_to_dispatcher_t *s = NULL;
  name_to_dispatcher_t **name_to_dispatcher =
      &((common_data_t *)btx_handle)->name_to_dispatcher;
  HASH_FIND_STR(*name_to_dispatcher, "read_params", s);
  if (!s) {
    // We didn't find the dispatcher, so we need to:
    // 1. Create it
    s = (name_to_dispatcher_t *)malloc(sizeof(name_to_dispatcher_t));
    s->name = "read_params";
    s->dispatcher = (void *)&btx_dispatch_read_params;
    utarray_new(s->callbacks, &ut_ptr_icd);
    // 2. Register it
    HASH_ADD_KEYPTR(hh, *name_to_dispatcher, s->name, strlen(s->name), s);
    // 3. Add the callbacks to the array
    utarray_push_back(s->callbacks, &callback);
  } else {
    assert(false && "Only one callbacks for read_params");
  }
}

void btx_call_callbacks_read_params(
    common_data_t *common_data,
    void * usr_data, btx_params_t * usr_params) {
  name_to_dispatcher_t *s = NULL;
  HASH_FIND_STR(common_data->name_to_dispatcher, "read_params", s);

  if (s)
    (*((btx_dispatch_read_params_f(*))(s->dispatcher)))(
        s->callbacks, common_data, usr_data, usr_params);
}

typedef void btx_dispatch_push_usr_messages_f(
    UT_array *callbacks, common_data_t *common_data,
    void *, btx_source_status_t*);

static void btx_dispatch_push_usr_messages(
    UT_array *callbacks, common_data_t *common_data,
    void * usr_data, btx_source_status_t* status) {

  // Call all the callbacks who where registered
  // Their type are declared in 'upstream.h'
  push_usr_messages_callback_f **p = NULL;
  while ((p = utarray_next(callbacks, p))) {
    (*p)((void *)common_data, usr_data, status);
  }
}

void btx_register_callbacks_push_usr_messages(
    void *btx_handle, push_usr_messages_callback_f *callback) {
  // Look-up our dispatcher
  name_to_dispatcher_t *s = NULL;
  name_to_dispatcher_t **name_to_dispatcher =
      &((common_data_t *)btx_handle)->name_to_dispatcher;
  HASH_FIND_STR(*name_to_dispatcher, "push_usr_messages", s);
  if (!s) {
    // We didn't find the dispatcher, so we need to:
    // 1. Create it
    s = (name_to_dispatcher_t *)malloc(sizeof(name_to_dispatcher_t));
    s->name = "push_usr_messages";
    s->dispatcher = (void *)&btx_dispatch_push_usr_messages;
    utarray_new(s->callbacks, &ut_ptr_icd);
    // 2. Register it
    HASH_ADD_KEYPTR(hh, *name_to_dispatcher, s->name, strlen(s->name), s);
    // 3. Add the callbacks to the array
    utarray_push_back(s->callbacks, &callback);
  } else {
    assert(false && "Only one callbacks for push_usr_messages");
  }
}

void btx_call_callbacks_push_usr_messages(
    common_data_t *common_data,
    void * usr_data, btx_source_status_t* status) {
  name_to_dispatcher_t *s = NULL;
  HASH_FIND_STR(common_data->name_to_dispatcher, "push_usr_messages", s);

  if (s)
    (*((btx_dispatch_push_usr_messages_f(*))(s->dispatcher)))(
        s->callbacks, common_data, usr_data, status);
}
