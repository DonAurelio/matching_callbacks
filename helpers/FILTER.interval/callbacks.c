#include <metababel/metababel.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

void usr_matching_condition(
  void *btx_handle, void *usr_data, const char* event_class_name, bool *matched) {
    *matched = true;
}

void usr_conditional_callback(
  void *btx_handle, void *usr_data, const char* event_class_name) {

    // Common Context Fields
    int64_t _vpid;
    uint64_t _vtid;
    int64_t _ts;
    int64_t _backend;

    // Payload Fields
    uint64_t _dur;

    {
      bool succeed = false;
      btx_event_common_context_field_integer_signed_get_value(
        btx_handle, "vpid", &_vpid, &succeed);
      assert((succeed ==  true) &&  "Member not found 'vpid'.");
    }

    {
      bool succeed = false;
      btx_event_common_context_field_integer_unsigned_get_value(
        btx_handle, "vtid", &_vtid, &succeed);
      assert((succeed ==  true) &&  "Member not found 'vtid'.");
    }

    {
      bool succeed = false;
      btx_event_common_context_field_integer_signed_get_value(
        btx_handle, "ts", &_ts, &succeed);
      assert((succeed ==  true) &&  "Member not found 'ts'.");
    }

    {
      bool succeed = false;
      btx_event_common_context_field_integer_signed_get_value(
        btx_handle, "backend", &_backend, &succeed);
      assert((succeed ==  true) &&  "Member not found 'backend'.");
    }

    {
      bool succeed = false;
      btx_event_payload_field_integer_unsigned_get_value(
        btx_handle, "dur", &_dur, &succeed);
      assert((succeed ==  true) &&  "Member not found 'dur'.");
    }

    printf("usr_conditional_callback: %ld, %ld, %ld, %ld, %ld\n", _vpid, _vtid, _ts, _backend, _dur);
}

void btx_register_usr_callbacks(void *btx_handle) {
  btx_register_matching_callback(
    btx_handle, &usr_matching_condition, &usr_conditional_callback);
}