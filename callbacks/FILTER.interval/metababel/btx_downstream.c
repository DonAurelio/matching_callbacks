#include "utlist.h"
#include <babeltrace2/babeltrace.h>
#include <metababel/btx_component.h>
#include <metababel/btx_downstream.h>
#include <stdlib.h>

void btx_downstream_move_messages(
    btx_message_iterator_t *message_iterator_private_data,
    bt_message_array_const messages, uint64_t capacity, uint64_t *count) {

  // Set count to min(capacity, common_data->queue.size())
  // Count time, pop the head of the queue and put it in messages
  for (*count = 0; *count < capacity && message_iterator_private_data->queue;
       (*count)++) {
    struct el *elt = message_iterator_private_data->queue;
    messages[*count] = elt->message;
    DL_DELETE(message_iterator_private_data->queue, elt);
    // Put it back to the bool of chain for reuse
    DL_APPEND(message_iterator_private_data->pool, elt);
  }
}

void btx_downstream_push_message(
    btx_message_iterator_t *message_iterator_private_data,
    const bt_message *message) {

  struct el *elt;
  if (message_iterator_private_data->pool) {
    elt = message_iterator_private_data->pool;
    DL_DELETE(message_iterator_private_data->pool, elt);
  } else {
    elt = (struct el *)malloc(sizeof *elt);
  }
  elt->message = message;
  DL_APPEND(message_iterator_private_data->queue, elt);
}

bt_trace_class *
btx_downstream_trace_class_create_rec(bt_self_component *self_component) {
  bt_trace_class *trace_class;
  trace_class = bt_trace_class_create(self_component);
  {
    bt_stream_class *trace_class_sc_0;
    trace_class_sc_0 = bt_stream_class_create(trace_class);
    bt_stream_class_set_name(trace_class_sc_0, "test_stream_class");
    {
      bt_field_class *trace_class_sc_0_ecc_fc;
      trace_class_sc_0_ecc_fc = bt_field_class_structure_create(trace_class);
      {
        bt_field_class *trace_class_sc_0_ecc_fc_m_0;
        trace_class_sc_0_ecc_fc_m_0 = bt_field_class_string_create(trace_class);
        bt_field_class_structure_append_member(trace_class_sc_0_ecc_fc, "hostname", trace_class_sc_0_ecc_fc_m_0);
        bt_field_class_put_ref(trace_class_sc_0_ecc_fc_m_0);
      }
      {
        bt_field_class *trace_class_sc_0_ecc_fc_m_1;
        trace_class_sc_0_ecc_fc_m_1 = bt_field_class_integer_signed_create(trace_class);
        bt_field_class_integer_set_field_value_range(trace_class_sc_0_ecc_fc_m_1, 64);
        bt_field_class_structure_append_member(trace_class_sc_0_ecc_fc, "vpid", trace_class_sc_0_ecc_fc_m_1);
        bt_field_class_put_ref(trace_class_sc_0_ecc_fc_m_1);
      }
      {
        bt_field_class *trace_class_sc_0_ecc_fc_m_2;
        trace_class_sc_0_ecc_fc_m_2 = bt_field_class_integer_unsigned_create(trace_class);
        bt_field_class_integer_set_field_value_range(trace_class_sc_0_ecc_fc_m_2, 64);
        bt_field_class_structure_append_member(trace_class_sc_0_ecc_fc, "vtid", trace_class_sc_0_ecc_fc_m_2);
        bt_field_class_put_ref(trace_class_sc_0_ecc_fc_m_2);
      }
      {
        bt_field_class *trace_class_sc_0_ecc_fc_m_3;
        trace_class_sc_0_ecc_fc_m_3 = bt_field_class_integer_signed_create(trace_class);
        bt_field_class_integer_set_field_value_range(trace_class_sc_0_ecc_fc_m_3, 64);
        bt_field_class_structure_append_member(trace_class_sc_0_ecc_fc, "ts", trace_class_sc_0_ecc_fc_m_3);
        bt_field_class_put_ref(trace_class_sc_0_ecc_fc_m_3);
      }
      {
        bt_field_class *trace_class_sc_0_ecc_fc_m_4;
        trace_class_sc_0_ecc_fc_m_4 = bt_field_class_integer_signed_create(trace_class);
        bt_field_class_integer_set_field_value_range(trace_class_sc_0_ecc_fc_m_4, 64);
        bt_field_class_structure_append_member(trace_class_sc_0_ecc_fc, "backend", trace_class_sc_0_ecc_fc_m_4);
        bt_field_class_put_ref(trace_class_sc_0_ecc_fc_m_4);
      }
      bt_stream_class_set_event_common_context_field_class(trace_class_sc_0, trace_class_sc_0_ecc_fc);
      bt_field_class_put_ref(trace_class_sc_0_ecc_fc);
    }
    {
      bt_event_class *trace_class_sc_0_ec_0;
      trace_class_sc_0_ec_0 = bt_event_class_create(trace_class_sc_0);
      bt_event_class_set_name(trace_class_sc_0_ec_0, "lttng:host");
      {
        bt_field_class *trace_class_sc_0_ec_0_p_fc;
        trace_class_sc_0_ec_0_p_fc = bt_field_class_structure_create(trace_class);
        {
          bt_field_class *trace_class_sc_0_ec_0_p_fc_m_0;
          trace_class_sc_0_ec_0_p_fc_m_0 = bt_field_class_string_create(trace_class);
          bt_field_class_structure_append_member(trace_class_sc_0_ec_0_p_fc, "name", trace_class_sc_0_ec_0_p_fc_m_0);
          bt_field_class_put_ref(trace_class_sc_0_ec_0_p_fc_m_0);
        }
        {
          bt_field_class *trace_class_sc_0_ec_0_p_fc_m_1;
          trace_class_sc_0_ec_0_p_fc_m_1 = bt_field_class_integer_unsigned_create(trace_class);
          bt_field_class_integer_set_field_value_range(trace_class_sc_0_ec_0_p_fc_m_1, 64);
          bt_field_class_structure_append_member(trace_class_sc_0_ec_0_p_fc, "dur", trace_class_sc_0_ec_0_p_fc_m_1);
          bt_field_class_put_ref(trace_class_sc_0_ec_0_p_fc_m_1);
        }
        {
          bt_field_class *trace_class_sc_0_ec_0_p_fc_m_2;
          trace_class_sc_0_ec_0_p_fc_m_2 = bt_field_class_bool_create(trace_class);
          bt_field_class_structure_append_member(trace_class_sc_0_ec_0_p_fc, "err", trace_class_sc_0_ec_0_p_fc_m_2);
          bt_field_class_put_ref(trace_class_sc_0_ec_0_p_fc_m_2);
        }
        bt_event_class_set_payload_field_class(trace_class_sc_0_ec_0, trace_class_sc_0_ec_0_p_fc);
        bt_field_class_put_ref(trace_class_sc_0_ec_0_p_fc);
      }
      bt_event_class_put_ref(trace_class_sc_0_ec_0);
    }
    bt_stream_class_put_ref(trace_class_sc_0);
  }

  return trace_class;
}

bt_trace *btx_downstream_trace_create_rec(bt_trace_class *trace_class) {
  bt_trace *trace = bt_trace_create(trace_class);
  {
    bt_stream_class *stream_class =
        bt_trace_class_borrow_stream_class_by_index(trace_class, 0);
    bt_stream_put_ref(bt_stream_create(stream_class, trace));
  }
  return trace;
}

// Workarround of a bug where gepping trace alive (and not the stream) provoke
// some segfault
void btx_streams_get_ref(bt_trace *trace) {
  {
    bt_stream *stream = bt_trace_borrow_stream_by_index(trace, 0);
    bt_stream_get_ref(stream);
  }
}

void btx_streams_put_ref(bt_trace *trace) {
  {
    bt_stream *stream = bt_trace_borrow_stream_by_index(trace, 0);
    bt_stream_put_ref(stream);
  }
}

void btx_push_messages_stream_beginning(
    bt_self_message_iterator *self_message_iterator,
    btx_message_iterator_t *message_iterator_private_data) {

  bt_trace *trace =
      message_iterator_private_data->common_data->downstream_trace;
  {
    bt_stream *stream = bt_trace_borrow_stream_by_index(trace, 0);
    bt_message *message =
        bt_message_stream_beginning_create(self_message_iterator, stream);
    btx_downstream_push_message(message_iterator_private_data, message);
  }
}

void btx_push_messages_stream_end(
    bt_self_message_iterator *self_message_iterator,
    btx_message_iterator_t *message_iterator_private_data) {

  bt_trace *trace =
      message_iterator_private_data->common_data->downstream_trace;
  {
    bt_stream *stream = bt_trace_borrow_stream_by_index(trace, 0);
    bt_message *message =
        bt_message_stream_end_create(self_message_iterator, stream);
    btx_downstream_push_message(message_iterator_private_data, message);
  }
}

static void btx_set_message_lttng_host(
    bt_event *
        event, const char* hostname, int64_t vpid, uint64_t vtid, int64_t ts, int64_t backend, const char* name, uint64_t dur, bt_bool err) {
  {
    bt_field *event_cc_f = bt_event_borrow_common_context_field(event);
    {
      bt_field *event_cc_f_m_0 = bt_field_structure_borrow_member_field_by_index(event_cc_f, 0);
      bt_field_string_set_value(event_cc_f_m_0, hostname);
    }
    {
      bt_field *event_cc_f_m_1 = bt_field_structure_borrow_member_field_by_index(event_cc_f, 1);
      bt_field_integer_signed_set_value(event_cc_f_m_1, vpid);
    }
    {
      bt_field *event_cc_f_m_2 = bt_field_structure_borrow_member_field_by_index(event_cc_f, 2);
      bt_field_integer_unsigned_set_value(event_cc_f_m_2, vtid);
    }
    {
      bt_field *event_cc_f_m_3 = bt_field_structure_borrow_member_field_by_index(event_cc_f, 3);
      bt_field_integer_signed_set_value(event_cc_f_m_3, ts);
    }
    {
      bt_field *event_cc_f_m_4 = bt_field_structure_borrow_member_field_by_index(event_cc_f, 4);
      bt_field_integer_signed_set_value(event_cc_f_m_4, backend);
    }
  }
  {
    bt_field *event_p_f = bt_event_borrow_payload_field(event);
    {
      bt_field *event_p_f_m_0 = bt_field_structure_borrow_member_field_by_index(event_p_f, 0);
      bt_field_string_set_value(event_p_f_m_0, name);
    }
    {
      bt_field *event_p_f_m_1 = bt_field_structure_borrow_member_field_by_index(event_p_f, 1);
      bt_field_integer_unsigned_set_value(event_p_f_m_1, dur);
    }
    {
      bt_field *event_p_f_m_2 = bt_field_structure_borrow_member_field_by_index(event_p_f, 2);
      bt_field_bool_set_value(event_p_f_m_2, err);
    }
  }

}

void btx_push_message_lttng_host(
    void *
        btx_handle, const char* hostname, int64_t vpid, uint64_t vtid, int64_t ts, int64_t backend, const char* name, uint64_t dur, bt_bool err) {

  common_data_t *common_data = (common_data_t *)btx_handle;

  bt_stream *stream = bt_trace_borrow_stream_by_index(
      common_data->downstream_trace, 0);
  bt_stream_class *stream_class = bt_stream_borrow_class(stream);
  bt_event_class *event_class = bt_stream_class_borrow_event_class_by_index(
      stream_class, 0);
  bt_message *message = bt_message_event_create(
      common_data->self_message_iterator, event_class, stream);

  bt_event *downstream_event = bt_message_event_borrow_event(message);

  btx_set_message_lttng_host(
      downstream_event, hostname, vpid, vtid, ts, backend, name, dur, err);

  btx_message_iterator_t *message_iterator_private_data =
      bt_self_message_iterator_get_data(common_data->self_message_iterator);
  btx_downstream_push_message(message_iterator_private_data, message);
}
