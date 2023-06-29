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

typedef void test_stream_class_callback_condition_f(
    void *btx_handle, void *usr_data, const char *event_class_name,
    bool *matched, const char*, int64_t, uint64_t, int64_t, int64_t);

typedef void test_stream_class_conditioned_callback_f(
    void *btx_handle, void *usr_data,
    const char *
        event_class_name, const char*, int64_t, uint64_t, int64_t, int64_t);

void btx_register_matching_callback_test_stream_class(
    void *btx_handle, test_stream_class_callback_condition_f *condition,
    test_stream_class_conditioned_callback_f *callback);

void btx_delete_matching_dispatchers(common_data_t *common_data);

#ifdef __cplusplus
}
#endif
