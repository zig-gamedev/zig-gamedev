## live editing

This sample application watches for source code changes and loads it directly into a running game. This allows for extremely short edit to executing cycle.

This sample requires **Windows**.

To build and run on Windows:

```
zig build live_editing-run
```

Then change one of the following files:

1. `samples/live_editing/src/entry.zig`
2. `samples/live_editing/src/live_editing.hlsl`
