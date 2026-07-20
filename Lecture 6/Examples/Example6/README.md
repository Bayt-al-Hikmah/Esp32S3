# Working with RAM: Understanding the Stack

This example illustrates how stack memory changes as functions call one another. The program begins execution in `app_main()`, which calls `firstFunction()`. At this point, the processor creates a new stack frame for `firstFunction()`, allocating memory for its local variable `a` along with the function's return address and any required execution information.

While `firstFunction()` is still executing, it calls `secondFunction()`. This causes the processor to create another stack frame on top of the existing one. The new frame contains the local variable `b` and the execution context required for `secondFunction()`. Since each function owns its own stack frame, the local variable `b` is accessible only within `secondFunction()`, while the local variable `a` remains accessible only within `firstFunction()`. Neither function can directly access the other's local variables because each variable exists only within its own stack frame.

When `secondFunction()` finishes executing, its stack frame is immediately removed from memory, and execution returns to `firstFunction()`. After `firstFunction()` completes, its stack frame is also released, returning execution to `app_main()`. At this point, the memory previously occupied by both functions becomes available for future function calls.


