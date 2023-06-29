#include <metababel/metababel.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

const char* matching_callbacks_event_dispatcher(
  void *btx_handle, void *usr_data, const char *event_class_name) {
    // The user must indicate the name of the stream class
    // That should be used to process a given event.
    // This imply the name of the stream class to be mandatory in 
    // the yaml file
    return "test_stream_class";
}

void usr_matching_condition(
  void *btx_handle, void *usr_data, const char* event_class_name, bool *matched,
  const char* hostname, int64_t vpid, uint64_t vtid, int64_t ts, int64_t backend)
{
    printf("urs_matching_condition %s, %ld, %ld, %ld, %ld\n", hostname, vpid, vtid, ts, backend);
    *matched = true;
}

void usr_conditional_callback(
  void *btx_handle, void *usr_data, const char* event_class_name,
  const char* hostname, int64_t vpid, uint64_t vtid, int64_t ts, int64_t backend)
{
    printf("usr_conditional_callback %s, %ld, %ld, %ld, %ld\n", hostname, vpid, vtid, ts, backend);
}

void btx_register_usr_callbacks(void *btx_handle) {
  btx_register_matching_callbacks_event_dispatcher(btx_handle, &matching_callbacks_event_dispatcher);
  btx_register_matching_callback_test_stream_class(btx_handle, &usr_matching_condition, &usr_conditional_callback);
}