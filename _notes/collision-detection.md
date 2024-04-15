To support collision detection of actors, you can
create a CircleComponent and a method to test for
intersection between two circle components. You can
then add a CircleComponent to any actor that
needs collision.


```
Asteroid::Asteroid(Game* game) :Actor(game),mCircle(nullptr)
{
	...

  // Create a circle component (for collision)
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);

	// Add to mAsteroids in game
	game->AddAsteroid(this);
}
```


Because each laser fired by the ship needs to check for
collision against all the asteroids, you can add a
std::vector of Asteroid pointers to Game. 
Then, in Laser::UpdateActor, you can easily test for
intersection against each of these asteroids: