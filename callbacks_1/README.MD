# Matching callbacks (helpers approach)

* This preserves the previous approach, but it will deliver the responsability of determining the stream class according with the event name to the user.
* This imply the user must be able to know to which stream class an event message belongs to according to the event name. This is not the case in some hip API calls such as the ones shown below, unless we plan to keep the "...hip:" prefix.

```yaml
  - :name: lttng_ust_hip:_Z24hipHccModuleLaunchKernelP18ihipModuleSymbol_tjjjjjjmP12ihipStream_tPPvS4_P11ihipEvent_tS6__entry
  - :name: lttng_ust_hip:_Z24hipHccModuleLaunchKernelP18ihipModuleSymbol_tjjjjjjmP12ihipStream_tPPvS4_P11ihipEvent_tS6__exit
  - :name: lttng_ust_hip:_Z24hipExtModuleLaunchKernelP18ihipModuleSymbol_tjjjjjjmP12ihipStream_tPPvS4_P11ihipEvent_tS6_j_entry
  - :name: lttng_ust_hip:_Z24hipExtModuleLaunchKernelP18ihipModuleSymbol_tjjjjjjmP12ihipStream_tPPvS4_P11ihipEvent_tS6_j_exit
```
