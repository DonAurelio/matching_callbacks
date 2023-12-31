#include "utarray.h"
#include "uthash.h"
#include <assert.h>
#include <babeltrace2/babeltrace.h>
#include <metababel/btx_component.h>
#include <metababel/btx_upstream.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

static void btx_dispatch_lttng_host(
    UT_array *callbacks, common_data_t *common_data,
    const bt_message *upstream_message, bool *is_callback_called) {

  const char* hostname;
  int64_t vpid;
  uint64_t vtid;
  int64_t ts;
  int64_t backend;
  const char* name;
  uint64_t dur;
  bt_bool err;

  const bt_event *event = bt_message_event_borrow_event_const(upstream_message);
  {
    const bt_field *event_cc_f = bt_event_borrow_common_context_field_const(event);
    {
      const bt_field *event_cc_f_m_0 = bt_field_structure_borrow_member_field_by_index_const(event_cc_f, 0);
      hostname = (const char*)bt_field_string_get_value(event_cc_f_m_0);
    }
    {
      const bt_field *event_cc_f_m_1 = bt_field_structure_borrow_member_field_by_index_const(event_cc_f, 1);
      vpid = (int64_t)bt_field_integer_signed_get_value(event_cc_f_m_1);
    }
    {
      const bt_field *event_cc_f_m_2 = bt_field_structure_borrow_member_field_by_index_const(event_cc_f, 2);
      vtid = (uint64_t)bt_field_integer_unsigned_get_value(event_cc_f_m_2);
    }
    {
      const bt_field *event_cc_f_m_3 = bt_field_structure_borrow_member_field_by_index_const(event_cc_f, 3);
      ts = (int64_t)bt_field_integer_signed_get_value(event_cc_f_m_3);
    }
    {
      const bt_field *event_cc_f_m_4 = bt_field_structure_borrow_member_field_by_index_const(event_cc_f, 4);
      backend = (int64_t)bt_field_integer_signed_get_value(event_cc_f_m_4);
    }
  }
  {
    const bt_field *event_p_f = bt_event_borrow_payload_field_const(event);
    {
      const bt_field *event_p_f_m_0 = bt_field_structure_borrow_member_field_by_index_const(event_p_f, 0);
      name = (const char*)bt_field_string_get_value(event_p_f_m_0);
    }
    {
      const bt_field *event_p_f_m_1 = bt_field_structure_borrow_member_field_by_index_const(event_p_f, 1);
      dur = (uint64_t)bt_field_integer_unsigned_get_value(event_p_f_m_1);
    }
    {
      const bt_field *event_p_f_m_2 = bt_field_structure_borrow_member_field_by_index_const(event_p_f, 2);
      err = (bt_bool)bt_field_bool_get_value(event_p_f_m_2);
    }
  }


  // Call all the callbacks who where registered
  // Their type are declared in 'upstream.h'
  lttng_host_callback_f **p = NULL;
  while ((p = utarray_next(callbacks, p))) {
    (*p)((void *)common_data,
         common_data
             ->usr_data, hostname, vpid, vtid, ts, backend, name, dur, err);
  }


  if (callbacks)
    *is_callback_called = true;
}

void btx_register_callbacks_lttng_host(
    void *btx_handle, lttng_host_callback_f *callback) {
  // Look-up our dispatcher
  name_to_dispatcher_t *s = NULL;
  name_to_dispatcher_t **name_to_dispatcher =
      &((common_data_t *)btx_handle)->name_to_dispatcher;
  HASH_FIND_STR(*name_to_dispatcher, "lttng:host", s);
  if (!s) {
    // We didn't find the dispatcher, so we need to:
    // 1. Create it
    s = (name_to_dispatcher_t *)malloc(sizeof(name_to_dispatcher_t));
    s->name = "lttng:host";
    s->dispatcher = (void *)&btx_dispatch_lttng_host;
    utarray_new(s->callbacks, &ut_ptr_icd);
    // 2. Register it
    HASH_ADD_KEYPTR(hh, *name_to_dispatcher, s->name, strlen(s->name), s);
  }
  // Add the callbacks to the array
  utarray_push_back(s->callbacks, &callback);
}

void btx_matching_dispatch_test_stream_class(UT_array *callbacks,
                                                   common_data_t *common_data,
                                                   const bt_message *upstream_message,
                                                   bool *is_callback_called) {

  const char* hostname;
  int64_t vpid;
  uint64_t vtid;
  int64_t ts;
  int64_t backend;
  const bt_event *event = bt_message_event_borrow_event_const(upstream_message);
    {
    const bt_field *event_cc_f = bt_event_borrow_common_context_field_const(event);
    {
      const bt_field *event_cc_f_m_0 = bt_field_structure_borrow_member_field_by_index_const(event_cc_f, 0);
      hostname = (const char*)bt_field_string_get_value(event_cc_f_m_0);
    }
    {
      const bt_field *event_cc_f_m_1 = bt_field_structure_borrow_member_field_by_index_const(event_cc_f, 1);
      vpid = (int64_t)bt_field_integer_signed_get_value(event_cc_f_m_1);
    }
    {
      const bt_field *event_cc_f_m_2 = bt_field_structure_borrow_member_field_by_index_const(event_cc_f, 2);
      vtid = (uint64_t)bt_field_integer_unsigned_get_value(event_cc_f_m_2);
    }
    {
      const bt_field *event_cc_f_m_3 = bt_field_structure_borrow_member_field_by_index_const(event_cc_f, 3);
      ts = (int64_t)bt_field_integer_signed_get_value(event_cc_f_m_3);
    }
    {
      const bt_field *event_cc_f_m_4 = bt_field_structure_borrow_member_field_by_index_const(event_cc_f, 4);
      backend = (int64_t)bt_field_integer_signed_get_value(event_cc_f_m_4);
    }
  }

  const bt_event_class *event_class = bt_event_borrow_class_const(event);
  const char *event_class_name = bt_event_class_get_name(event_class);

  condition_to_callback_t **p = NULL;
  while ((p = utarray_next(callbacks, p))) {
    test_stream_class_callback_condition_f *condition =
        (test_stream_class_callback_condition_f *)((*p)->condition);
    test_stream_class_conditioned_callback_f *callback =
        (test_stream_class_conditioned_callback_f *)((*p)->callback);
    bool is_condition_met = false;
    condition(
        (void *)common_data, common_data->usr_data, event_class_name,
        &is_condition_met, hostname, vpid, vtid, ts, backend);
    if (is_condition_met) {
      callback(
          (void *)common_data, common_data->usr_data,
          event_class_name, hostname, vpid, vtid, ts, backend);
      *is_callback_called = true;
    }
  }
}

void btx_register_matching_callback_test_stream_class(
    void *btx_handle, test_stream_class_callback_condition_f *condition,
    test_stream_class_conditioned_callback_f *callback) {
  // Look-up our dispatcher
  name_to_dispatcher_t *s = NULL;
  name_to_dispatcher_t **name_to_matching_dispatcher =
      &((common_data_t *)btx_handle)->name_to_matching_dispatcher;
  HASH_FIND_STR(*name_to_matching_dispatcher, "test_stream_class", s);
  if (!s) {
    // We didn't find the dispatcher, so we need to:
    // 1. Create it
    s = (name_to_dispatcher_t *)malloc(sizeof(name_to_dispatcher_t));
    s->name = "test_stream_class";
    s->dispatcher = (void *)&btx_matching_dispatch_test_stream_class;
    utarray_new(s->callbacks, &ut_ptr_icd);
    // 2. Register it
    HASH_ADD_KEYPTR(hh, *name_to_matching_dispatcher, s->name, strlen(s->name),
                    s);
  }
  // Add the callbacks to the array
  condition_to_callback_t *m =
      (condition_to_callback_t *)malloc(sizeof(condition_to_callback_t));
  m->condition = (void *)condition;
  m->callback = (void *)callback;
  utarray_push_back(s->callbacks, &m);
}

void btx_delete_dispatchers(common_data_t *common_data) {
  name_to_dispatcher_t *current, *tmp;
  HASH_ITER(hh, common_data->name_to_dispatcher, current, tmp) {
    HASH_DEL(common_data->name_to_dispatcher, current);
    utarray_free(current->callbacks);
    free(current);
  }
}

void btx_delete_matching_dispatchers(common_data_t *common_data) {
  name_to_dispatcher_t *current, *tmp;
  HASH_ITER(hh, common_data->name_to_matching_dispatcher, current, tmp) {
    // Removes the item from the hash table.
    HASH_DEL(common_data->name_to_matching_dispatcher, current);
    // Deletes every condition, callback pair.
    condition_to_callback_t **p = NULL;
    while ((p = utarray_next(current->callbacks, p))) {
      free(*p);
    }
    utarray_free(current->callbacks);
    free(current);
  }
}
