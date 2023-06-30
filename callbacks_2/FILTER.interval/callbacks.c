#include <metababel/metababel.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

const char* matching_callbacks_event_dispatcher(
  void *btx_handle, void *usr_data, const char *event_class_name) {
    // The user must be sure that the event can be unpacked 
    // by the stream associated to the returned identifier.
    return "0";
}

void usr_conditional_callback(
  void *btx_handle, void *usr_data, const char* event_class_name,
  const char* hostname, int64_t vpid, uint64_t vtid, int64_t ts, int64_t backend)
{
    printf("usr_conditional_callback %s, %ld, %ld, %ld, %ld\n", hostname, vpid, vtid, ts, backend);
}

void btx_register_usr_callbacks(void *btx_handle) {
  btx_register_matching_callbacks_event_dispatcher(btx_handle, &matching_callbacks_event_dispatcher);
  btx_register_matching_callback_test_stream_class(btx_handle, "0", &usr_conditional_callback);
}