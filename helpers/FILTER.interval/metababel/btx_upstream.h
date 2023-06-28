#pragma once
#include <metababel/btx_component.h>
#ifdef __cplusplus
extern "C" {
#endif

// Dispatcher
typedef void(dispatcher_t)(UT_array *callbacks, common_data_t *common_data,
                           const bt_message *upstream_message, bool *is_callback_called);

typedef void lttng_host_callback_f(
    void *btx_handle,
    void *usr_data, const char*, int64_t, uint64_t, int64_t, int64_t, const char*, uint64_t, bt_bool);
void btx_register_callbacks_lttng_host(
    void *btx_handle, lttng_host_callback_f *callback);

void btx_delete_dispatchers(common_data_t *common_data);

// Matching dispatcher

struct condition_to_callback_s {
  void *condition;
  void *callback;
};
typedef struct condition_to_callback_s condition_to_callback_t;

typedef void matching_condition_f(
    void *btx_handle, void *usr_data, const char *event_class_name,
    bool *matched);

typedef void matched_callback_f(
    void *btx_handle, void *usr_data, const char * event_class_name);

void btx_register_matching_callback(
    void *btx_handle, matching_condition_f *condition,
    matched_callback_f *callback);

void btx_delete_matching_dispatchers(common_data_t *common_data);

// NEW:
void btx_event_get_common_context_member_field_by_name_const(
  void *btx_handle, const char *name, const bt_field **event_cc_f_m, bool *succeed);

// NEW:
void btx_event_common_context_field_integer_signed_get_value(
  void *btx_handle, const char *name, int64_t *value, bool *succeed);

// NEW:
void btx_event_common_context_field_integer_unsigned_get_value(
  void *btx_handle, const char *name, uint64_t *value, bool *succeed);

// NEW:
void btx_event_common_context_field_string_get_value(
  void *btx_handle, const char *name, const char *value, bool *succeed);

// NEW:
void btx_event_get_payload_field_by_name_const(
  void *btx_handle, const char *name, const bt_field **event_cc_f_m, bool *succeed);

// NEW:
void btx_event_payload_field_integer_unsigned_get_value(
    void *btx_handle, const char *name, uint64_t *value, bool *succeed);

#ifdef __cplusplus
}
#endif
