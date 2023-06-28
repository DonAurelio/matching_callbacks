# Matching callbacks (helpers approach)

* We do not need to unpack neither event common context nor payload field data.
* We do not need to create function types for matching functions and callbacks to fullfil the signature of the every stream class. 
* Since we do not need to unpack, all the events, no matter the event class or stream class, can be dispatched or processed by the same dispatching function.
* The use is responsible for unpacking the required data.
* The user must ensure that if a given condition is met for an specific event, the callbacks called for that event use the appropriate helpers to unpack the required data.
* Matching callbacks will be registered for all the events, not only per stream class as before.

```c
void btx_matching_dispatch(UT_array *callbacks,
                          common_data_t *common_data,
                          const bt_message *upstream_message,
                          bool *is_callback_called)
```