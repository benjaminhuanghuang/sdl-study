Then in Actor, you declare two functions: a non-virtual
ProcessInput and a virtual ActorInput function.
The idea here is that actor subclasses that want custom
input can override ActorInput but not ProcessInput
(like how there are separate Update and UpdateActor
functions):

```
// ProcessInput function called from Game (not overridable)
void ProcessInput(const uint8_t* keyState);

// Any actor-specific input code (overridable)
virtual void ActorInput(const uint8_t* keyState);
```

The Actor::ProcessInput function first checks if the
actor’s state is active. If it is, you first call
ProcessInput on all components and then call
ActorInput for any actor-overridable behavior

Finally, in Game::ProcessInput, you can loop over all
actors and call ProcessInput on each one:

```
const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
```

Then add keyboard-controlled
movement to Actor(like Ship) by simply creating an
InputComponent instance. (We omit the code for the
Ship constructor here, but it essentially sets the various
InputComponent member variables for the keys and
maximum speed.)
