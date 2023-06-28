/* Code generaed by source_callbacks_generator.rb */

#include <metababel/metababel.h>

void btx_push_usr_messages(void *btx_handle, void *usr_data, btx_source_status_t *status) {

  btx_push_message_lttng_host(btx_handle,"iris01",1289,1098,1,0,"host_function_name_1",2,0);
  btx_push_message_lttng_host(btx_handle,"iris01",1289,1098,1,0,"host_function_name_1",2,0);
  btx_push_message_lttng_host(btx_handle,"iris01",1289,1098,1,0,"host_function_name_1",2,0);
  btx_push_message_lttng_host(btx_handle,"iris01",1289,1098,1,0,"host_function_name_1",2,0);
  btx_push_message_lttng_host(btx_handle,"iris01",1289,1098,1,0,"host_function_name_1",2,0);
  btx_push_message_lttng_device(btx_handle,"iris01",1289,1098,1,0,"device_function_name_1",2,0,0,1,"No metadata");
  btx_push_message_lttng_device(btx_handle,"iris01",1289,1098,1,0,"device_function_name_1",2,0,0,1,"No metadata");
  btx_push_message_lttng_device(btx_handle,"iris01",1289,1098,1,0,"device_function_name_1",2,0,0,1,"No metadata");
  btx_push_message_lttng_device(btx_handle,"iris01",1289,1098,1,0,"device_function_name_1",2,0,0,1,"No metadata");
  btx_push_message_lttng_device(btx_handle,"iris01",1289,1098,1,0,"device_function_name_1",2,0,0,1,"No metadata");
  // btx_push_message_lttng_traffic(btx_handle,"iris01",1289,1098,1,0,"traffic_function_name_2",1000);
  // btx_push_message_lttng_traffic(btx_handle,"iris01",1289,1098,1,0,"traffic_function_name_2",1000);
  // btx_push_message_lttng_traffic(btx_handle,"iris01",1289,1098,1,0,"traffic_function_name_2",1000);
  // btx_push_message_lttng_traffic(btx_handle,"iris01",1289,1098,1,0,"traffic_function_name_2",1000);
  // btx_push_message_lttng_traffic(btx_handle,"iris01",1289,1098,1,0,"traffic_function_name_2",1000);
  // btx_push_message_lttng_device_name(btx_handle,"iris01",1289,1098,1,0,"device_name_function_name_2",0);
  // btx_push_message_lttng_device_name(btx_handle,"iris01",1289,1098,1,0,"device_name_function_name_2",0);
  // btx_push_message_lttng_device_name(btx_handle,"iris01",1289,1098,1,0,"device_name_function_name_2",0);
  // btx_push_message_lttng_device_name(btx_handle,"iris01",1289,1098,1,0,"device_name_function_name_2",0);
  // btx_push_message_lttng_device_name(btx_handle,"iris01",1289,1098,1,0,"device_name_function_name_2",0);
  // btx_push_message_lttng_ust_thapi_metadata(btx_handle,"iris01",1289,1098,1,0,"No THAPI Metadata");
  // btx_push_message_lttng_ust_thapi_metadata(btx_handle,"iris01",1289,1098,1,0,"No THAPI Metadata");
  // btx_push_message_lttng_ust_thapi_metadata(btx_handle,"iris01",1289,1098,1,0,"No THAPI Metadata");
  // btx_push_message_lttng_ust_thapi_metadata(btx_handle,"iris01",1289,1098,1,0,"No THAPI Metadata");
  // btx_push_message_lttng_ust_thapi_metadata(btx_handle,"iris01",1289,1098,1,0,"No THAPI Metadata");

  *status = BTX_SOURCE_END;
}

void btx_register_usr_callbacks(void *btx_handle) {
  btx_register_callbacks_push_usr_messages(btx_handle, &btx_push_usr_messages);
}

