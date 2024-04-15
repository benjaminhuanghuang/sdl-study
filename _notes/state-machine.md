There are several ways to implement a state machine.
Minimally, the code must **update** the behavior of the AI based on the current state, and it must support
enter and exit actions. An AIComponent class can encapsulate this state behavior.

You also associate AIState with a specific
AIComponent through the mOwner member variable.