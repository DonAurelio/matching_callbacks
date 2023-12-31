#pragma once
#include <babeltrace2/babeltrace.h>
#include <metababel/btx_component.h>
#ifdef __cplusplus
extern "C" {
#endif
void btx_downstream_move_messages(
    btx_message_iterator_t *message_iterator_private_data,
    bt_message_array_const messages, uint64_t capacity, uint64_t *count);

void btx_downstream_push_message(
    btx_message_iterator_t *message_iterator_private_data,
    const bt_message *message);

bt_trace_class *
btx_downstream_trace_class_create_rec(bt_self_component *self_component);

bt_trace *btx_downstream_trace_create_rec(bt_trace_class *trace_class);

void btx_streams_get_ref(bt_trace *trace);
void btx_streams_put_ref(bt_trace *trace);

void btx_push_messages_stream_beginning(
    bt_self_message_iterator *self_message_iterator,
    btx_message_iterator_t *message_iterator_private_data);
void btx_push_messages_stream_end(
    bt_self_message_iterator *self_message_iterator,
    btx_message_iterator_t *message_iterator_private_data);

void btx_push_message_lttng_host(
    void *
        btx_handle, const char* hostname, int64_t vpid, uint64_t vtid, int64_t ts, int64_t backend, const char* name, uint64_t dur, bt_bool err);
#ifdef __cplusplus
}
#endif
