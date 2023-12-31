#include "uthash.h"
#include "utlist.h"
#include <babeltrace2/babeltrace.h>
#include <metababel/metababel.h>
#include <stdio.h>
#include <stdlib.h>

/* Loosely inspired by
 * https://babeltrace.org/docs/v2.0/libbabeltrace2/example-simple-flt-cmp-cls.html
 */

static char *get_port_name(uint64_t current) {
  int num_len = snprintf(NULL, 0, "in%ld", current);
  char *result = (char *)malloc(num_len + 1);
  sprintf(result, "in%ld", current);
  return result;
}

/*
 *  Handle Upstream Messages
 */

static inline bt_message_iterator_class_next_method_status
filter_message_iterator_next_initializing(
    bt_self_message_iterator *self_message_iterator,
    bt_message_array_const messages, uint64_t capacity, uint64_t *count) {

  /* Retrieve our private data from the message iterator's user data */
  btx_message_iterator_t *message_iterator_private_data =
      bt_self_message_iterator_get_data(self_message_iterator);

  /* Begining of Stream */
  btx_push_messages_stream_beginning(self_message_iterator,
      message_iterator_private_data);

  /* Call Initialize user callback */
  btx_call_callbacks_initialize_usr_data(
      message_iterator_private_data->common_data,
      &message_iterator_private_data->common_data->usr_data);
  /* Call read callbacks */
  btx_call_callbacks_read_params(
      message_iterator_private_data->common_data,
      message_iterator_private_data->common_data->usr_data,
      message_iterator_private_data->common_data->btx_params);

  // We need to transition in the procesing state
  message_iterator_private_data->state = BTX_FILTER_STATE_PROCESSING;
  if (message_iterator_private_data->queue)
    message_iterator_private_data->processing_state =
        BTX_FILTER_PROCESSING_STATE_SENDING;

  return BT_MESSAGE_ITERATOR_CLASS_NEXT_METHOD_STATUS_AGAIN;
}

static inline bt_message_iterator_class_next_method_status
filter_message_iterator_next_processing_sending(
    bt_self_message_iterator *self_message_iterator,
    bt_message_array_const messages, uint64_t capacity, uint64_t *count) {
  /* Retrieve our private data from the message iterator's user data */
  btx_message_iterator_t *message_iterator_private_data =
      bt_self_message_iterator_get_data(self_message_iterator);

  btx_downstream_move_messages(message_iterator_private_data, messages,
                               capacity, count);

  if (!message_iterator_private_data->queue) {
    message_iterator_private_data->processing_state =
        BTX_FILTER_PROCESSING_STATE_READING;
  }
  return BT_MESSAGE_ITERATOR_CLASS_NEXT_METHOD_STATUS_OK;
}

static inline bt_message_iterator_class_next_method_status
filter_message_iterator_next_processing_reading(
    bt_self_message_iterator *self_message_iterator,
    bt_message_array_const messages, uint64_t capacity, uint64_t *count) {
  /* Retrieve our private data from the message iterator's user data */
  btx_message_iterator_t *message_iterator_private_data =
      bt_self_message_iterator_get_data(self_message_iterator);

  common_data_t *common_data = message_iterator_private_data->common_data;

  /* Consume a batch of messages from the upstream message iterator */
  uint64_t upstream_message_count;
  bt_message_array_const upstream_messages;

  bt_message_iterator_next_status next_status = bt_message_iterator_next(
      message_iterator_private_data->head_mi->message_iterator,
      &upstream_messages, &upstream_message_count);

  /* We cannot have any message left in the downstream queue
   * Hence it's safe to pass any error messages downstream.
   */
  switch (next_status) {
  case BT_MESSAGE_ITERATOR_NEXT_STATUS_END:
    /* We are setting the state, so this function will never being call twice */
    bt_message_iterator_put_ref(
        message_iterator_private_data->head_mi->message_iterator);
    struct el_mi *tmp = message_iterator_private_data->head_mi;
    CDL_DELETE(message_iterator_private_data->head_mi, tmp);
    free(tmp);

    if (!message_iterator_private_data->head_mi) {
      /* Call Finalize user callback */
      btx_call_callbacks_finalize_usr_data(
          message_iterator_private_data->common_data,
          message_iterator_private_data->common_data->usr_data);
      /* End of Stream */
      btx_push_messages_stream_end(self_message_iterator,
                                   message_iterator_private_data);
      message_iterator_private_data->state = BTX_FILTER_STATE_FINALIZING;
      message_iterator_private_data->processing_state =
          BTX_FILTER_PROCESSING_STATE_FINISHED;
    }
    return BT_MESSAGE_ITERATOR_CLASS_NEXT_METHOD_STATUS_AGAIN;
  case BT_MESSAGE_ITERATOR_NEXT_STATUS_AGAIN:
    return BT_MESSAGE_ITERATOR_CLASS_NEXT_METHOD_STATUS_AGAIN;
  case BT_MESSAGE_ITERATOR_NEXT_STATUS_MEMORY_ERROR:
    message_iterator_private_data->state = BTX_FILTER_STATE_ERROR;
    return BT_MESSAGE_ITERATOR_CLASS_NEXT_METHOD_STATUS_MEMORY_ERROR;
  case BT_MESSAGE_ITERATOR_NEXT_STATUS_ERROR:
    message_iterator_private_data->state = BTX_FILTER_STATE_ERROR;
    return BT_MESSAGE_ITERATOR_CLASS_NEXT_METHOD_STATUS_ERROR;
  case BT_MESSAGE_ITERATOR_NEXT_STATUS_OK:
    break;
  default:
    message_iterator_private_data->state = BTX_FILTER_STATE_ERROR;
    return BT_MESSAGE_ITERATOR_CLASS_NEXT_METHOD_STATUS_ERROR;
  }

  /* For each consumed message */
  for (uint64_t upstream_i = 0; upstream_i < upstream_message_count;
       upstream_i++) {
    /* Current message */
    const bt_message *upstream_message = upstream_messages[upstream_i];
    /* Forward all non event-messages downstream */
    if (bt_message_get_type(upstream_message) != BT_MESSAGE_TYPE_EVENT) {
      btx_downstream_push_message(message_iterator_private_data,
                                  upstream_message);
      continue;

    }
    /* Borrow the event message's event and its class */
    const bt_event *event =
        bt_message_event_borrow_event_const(upstream_message);
    const bt_event_class *event_class = bt_event_borrow_class_const(event);

    /* NEW: Preserve current event for matching callbacks */
    common_data->current_event = event;

    /* Call dispatcher or forward message downstream */
    bool is_callback_called = false;
    const char *class_name = bt_event_class_get_name(event_class);

    {
      name_to_dispatcher_t *s = NULL;
      HASH_FIND_STR(common_data->name_to_dispatcher, class_name, s);
      if (s) {
        (*((dispatcher_t(*))(s->dispatcher)))(s->callbacks, common_data, upstream_message,
                                              &is_callback_called);
      }
    }

    {
      // const bt_stream_class *stream_class =
      //     bt_event_class_borrow_stream_class_const(event_class);
      // UPDATED: We do not need access to the stream class name any more.
      // const char *stream_class_name = bt_stream_class_get_name(stream_class);

      name_to_dispatcher_t *s = NULL;
      HASH_FIND_STR(common_data->name_to_matching_dispatcher, "matching_callback",
                    s);
      if (s) {
        // is_callback_called will only be modified if at least one callback is
        // called.
        (*((dispatcher_t(*))(s->dispatcher)))(s->callbacks, common_data, upstream_message,
                                              &is_callback_called);
      }
    }

    if (is_callback_called) {
      /* The message have been consumed, we can discard it */
      bt_message_put_ref(upstream_message);
    } else {
      btx_downstream_push_message(message_iterator_private_data,
                                  upstream_message);
    }

    /* NEW: At this point current_event is not valid any more. */
    common_data->current_event = NULL;
  }

  /* Round Robin between Upstream MessageIterator */
  message_iterator_private_data->head_mi =
      message_iterator_private_data->head_mi->next;

  if (message_iterator_private_data->queue) {
    message_iterator_private_data->processing_state =
        BTX_FILTER_PROCESSING_STATE_SENDING;
    /* optimization */
    return filter_message_iterator_next_processing_sending(
        self_message_iterator, messages, capacity, count);
  } else
    return BT_MESSAGE_ITERATOR_CLASS_NEXT_METHOD_STATUS_AGAIN;
}

static inline bt_message_iterator_class_next_method_status
filter_message_iterator_next_processing(
    bt_self_message_iterator *self_message_iterator,
    bt_message_array_const messages, uint64_t capacity, uint64_t *count) {
  /* Retrieve our private data from the message iterator's user data */
  btx_message_iterator_t *message_iterator_private_data =
      bt_self_message_iterator_get_data(self_message_iterator);

  switch (message_iterator_private_data->processing_state) {
  case BTX_FILTER_PROCESSING_STATE_READING:
    return filter_message_iterator_next_processing_reading(
        self_message_iterator, messages, capacity, count);
  case BTX_FILTER_PROCESSING_STATE_SENDING:
    return filter_message_iterator_next_processing_sending(
        self_message_iterator, messages, capacity, count);
  case BTX_FILTER_PROCESSING_STATE_FINISHED:
    return BT_MESSAGE_ITERATOR_CLASS_NEXT_METHOD_STATUS_ERROR;
  default:
    return BT_MESSAGE_ITERATOR_CLASS_NEXT_METHOD_STATUS_ERROR;
  }
}

static inline bt_message_iterator_class_next_method_status
filter_message_iterator_next_finalizing(
    bt_self_message_iterator *self_message_iterator,
    bt_message_array_const messages, uint64_t capacity, uint64_t *count) {
  /* Retrieve our private data from the message iterator's user data */
  btx_message_iterator_t *message_iterator_private_data =
      bt_self_message_iterator_get_data(self_message_iterator);

  // assert(message_iterator_private_data->processing_state ==
  // BTX_FILTER_PROCESSING_STATE_FINISHED);
  if (!message_iterator_private_data->queue) {
    message_iterator_private_data->state = BTX_FILTER_STATE_FINISHED;
    return BT_MESSAGE_ITERATOR_CLASS_NEXT_METHOD_STATUS_END;
  }

  btx_downstream_move_messages(message_iterator_private_data, messages,
                               capacity, count);
  return BT_MESSAGE_ITERATOR_CLASS_NEXT_METHOD_STATUS_OK;
}

static bt_message_iterator_class_next_method_status
filter_message_iterator_next(bt_self_message_iterator *self_message_iterator,
                             bt_message_array_const messages, uint64_t capacity,
                             uint64_t *count) {

  /* Retrieve our private data from the message iterator's user data */
  btx_message_iterator_t *message_iterator_private_data =
      bt_self_message_iterator_get_data(self_message_iterator);

  switch (message_iterator_private_data->state) {
  case BTX_FILTER_STATE_INITIALIZING:
    return filter_message_iterator_next_initializing(self_message_iterator,
                                                     messages, capacity, count);
  case BTX_FILTER_STATE_PROCESSING:
    return filter_message_iterator_next_processing(self_message_iterator,
                                                   messages, capacity, count);
  case BTX_FILTER_STATE_FINALIZING:
    return filter_message_iterator_next_finalizing(self_message_iterator,
                                                   messages, capacity, count);
  case BTX_FILTER_STATE_FINISHED:
    return BT_MESSAGE_ITERATOR_CLASS_NEXT_METHOD_STATUS_END;
  default:
    return BT_MESSAGE_ITERATOR_CLASS_NEXT_METHOD_STATUS_ERROR;
  }
}

/*
 * Initializes the filter component.
 */
static bt_component_class_initialize_method_status
filter_initialize(bt_self_component_filter *self_component_filter,
                  bt_self_component_filter_configuration *configuration,
                  const bt_value *params, void *initialize_method_data) {

  /* Allocate a private data structure */
  common_data_t *common_data = calloc(1, sizeof(common_data_t));
  common_data->btx_params = calloc(1, sizeof(btx_params_t));
  common_data->params = params;
  // Read parameters
  btx_populate_params(common_data);
  bt_value_get_ref(common_data->params);

  /* Register User Callbacks */
  btx_register_usr_callbacks((void *)common_data);

  /* Set the component's user data to our private data structure */
  bt_self_component_set_data(
      bt_self_component_filter_as_self_component(self_component_filter),
      common_data);

  /*
   * Add an input port named `in` to the filter component.
   *
   * This is needed so that this filter component can be connected to
   * a filter or a source component. With a connected upstream
   * component, this filter component's message iterator can create a
   * message iterator to consume messages.
   *
   * Add an output port named `out` to the filter component.
   *
   * This is needed so that this filter component can be connected to
   * a filter or a sink component. Once a downstream component is
   * connected, it can create our message iterator.
   */

  common_data->component =
      bt_self_component_filter_as_component_filter(self_component_filter);

  const uint64_t current =
      bt_component_filter_get_input_port_count(common_data->component);

  {
    char *name = get_port_name(current);
    bt_self_component_filter_add_input_port(self_component_filter, name, NULL,
                                            NULL);
    free(name);
  }
  bt_self_component_filter_add_output_port(self_component_filter, "out", NULL,
                                           NULL);

  /* Create message that will be used by the filter */
  bt_self_component *self_component =
      bt_self_component_filter_as_self_component(self_component_filter);
  /* Create a `trace_class` and all the children classes (stream and events) */
  bt_trace_class *trace_class =
      btx_downstream_trace_class_create_rec(self_component);
  /* Instantiate a `downstream_trace` of `trace_class` and all the children
   * stream */
  common_data->downstream_trace = btx_downstream_trace_create_rec(trace_class);
  btx_streams_get_ref(common_data->downstream_trace); // ??
  bt_trace_class_put_ref(trace_class);

  return BT_COMPONENT_CLASS_INITIALIZE_METHOD_STATUS_OK;
}

bt_component_class_port_connected_method_status
filter_input_port_connected(bt_self_component_filter *self_component_filter,
                            bt_self_component_port_input *self_port,
                            const bt_port_output *other_port) {
  const uint64_t current = bt_component_filter_get_input_port_count(
      bt_self_component_filter_as_component_filter(self_component_filter));

  {
    char *name = get_port_name(current);
    bt_self_component_filter_add_input_port(self_component_filter, name, NULL,
                                            NULL);
    free(name);
  }

  return BT_COMPONENT_CLASS_PORT_CONNECTED_METHOD_STATUS_OK;
}
/*
 * Initializes the message iterator.
 */
static bt_message_iterator_class_initialize_method_status
filter_message_iterator_initialize(
    bt_self_message_iterator *self_message_iterator,
    bt_self_message_iterator_configuration *configuration,
    bt_self_component_port_output *self_port) {
  /* Allocate a private data structure */
  btx_message_iterator_t *message_iterator_private_data =
      calloc(1, sizeof(btx_message_iterator_t));

  /* Retrieve the component's private data from its user data */
  common_data_t *common_data = bt_self_component_get_data(
      bt_self_message_iterator_borrow_component(self_message_iterator));

  /* Save a link to the self_message_iterator */
  common_data->self_message_iterator = self_message_iterator;

  /* Keep a link to the component's private data */
  message_iterator_private_data->common_data = common_data;

  /* Create the upstream message iterators */
  for (uint64_t i = 0;
       i < bt_component_filter_get_input_port_count(common_data->component);
       i++) {
    bt_self_component_port_input *self_port =
        bt_self_component_filter_borrow_input_port_by_index(
            (bt_self_component_filter *)common_data->component, i);

    const bt_port *port = bt_self_component_port_as_port(
        bt_self_component_port_input_as_self_component_port(self_port));

    /* Skip non-connected port */
    if (!bt_port_is_connected(port))
      continue;

    el_mi *mi = (el_mi *)malloc(sizeof *mi);
    bt_message_iterator_create_from_message_iterator(
        self_message_iterator, self_port, &mi->message_iterator);

    CDL_APPEND(message_iterator_private_data->head_mi, mi);
  }

  /* Set the message iterator's user data to our private data structure */
  bt_self_message_iterator_set_data(self_message_iterator,
                                    message_iterator_private_data);

  message_iterator_private_data->state = BTX_FILTER_STATE_INITIALIZING;
  message_iterator_private_data->processing_state =
      BTX_FILTER_PROCESSING_STATE_READING;

  return BT_MESSAGE_ITERATOR_CLASS_INITIALIZE_METHOD_STATUS_OK;
}

static void filter_finalize(bt_self_component_filter *self_component_filter) {
  common_data_t *common_data = bt_self_component_get_data(
      bt_self_component_filter_as_self_component(self_component_filter));

  btx_streams_put_ref(common_data->downstream_trace); // ??
  // We allocate it, we need to put ref
  bt_trace_put_ref(common_data->downstream_trace);

  // Delete name_to_dispatchers
  btx_delete_dispatchers(common_data);
  btx_delete_matching_dispatchers(common_data);

  // We allocate it, we need to free it
  free(common_data->btx_params);
  bt_value_put_ref(common_data->params);
  free(common_data);
}

static void filter_message_iterator_finalize(
    bt_self_message_iterator *self_message_iterator) {
  /* Retrieve our private data from the message iterator's user data */
  btx_message_iterator_t *message_iterator_private_data =
      bt_self_message_iterator_get_data(self_message_iterator);

  {
    struct el *elt, *tmp;
    DL_FOREACH_SAFE(message_iterator_private_data->pool, elt, tmp) {
      DL_DELETE(message_iterator_private_data->pool, elt);
      free(elt);
    }
  }
  assert(!message_iterator_private_data->head_mi);
  /* Free the allocated structure */
  free(message_iterator_private_data);
}

/* Mandatory */
BT_PLUGIN_MODULE();

BT_PLUGIN(metababel_filter);
BT_PLUGIN_FILTER_COMPONENT_CLASS(btx,
                                 filter_message_iterator_next);

BT_PLUGIN_FILTER_COMPONENT_CLASS_INITIALIZE_METHOD(
    btx, filter_initialize);
BT_PLUGIN_FILTER_COMPONENT_CLASS_FINALIZE_METHOD(
    btx, filter_finalize);
BT_PLUGIN_FILTER_COMPONENT_CLASS_MESSAGE_ITERATOR_CLASS_INITIALIZE_METHOD(
    btx, filter_message_iterator_initialize);
BT_PLUGIN_FILTER_COMPONENT_CLASS_MESSAGE_ITERATOR_CLASS_FINALIZE_METHOD(
    btx, filter_message_iterator_finalize);
BT_PLUGIN_FILTER_COMPONENT_CLASS_INPUT_PORT_CONNECTED_METHOD(
    btx, filter_input_port_connected);
