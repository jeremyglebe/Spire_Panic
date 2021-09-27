#include "BadCharacter.h"
#include "BaseLevelScene.h"
#include "Coin.h"

BadCharacter::BadCharacter(Scene* scene, float x, float y, float z) :
	Character(scene, BLACK_SQUARE, x, y, z)
{
	// Track the scene as a level scene
	level = (BaseLevelScene*)scene;
	// Bounding box configuration
	m_sAabb = BoundingBox({ x,y,z }, { 16,12,16 });
	// Paths configuration
	paths = ((BaseLevelScene*)scene)->getPaths();

	// Descriptor tags
	tags.insert("baddie");
	//tags.insert("unpassable");
}
BadCharacter::~BadCharacter() {}

// Functions labeled "update_State" are mutually exclusive states of behavior
// Whereas other "update_" functions are simply sub-behaviors meant to occur on
// all update states that I thought would best be separated into contained functions.
void BadCharacter::update() {
	// If the enemy's health is zero, it is dying
	if (hp < 0 || level->getGameOver()) {
		update_StateDying();
	}
	// If the spire is within range, attack the spire
	// (The spire has a designated "attack zone" where enemies can and do attack)
	else if (m_sAabb.Intersects(level->getSpire()->attackZone)) {
		update_StateAttackSpire();
	}
	// If a player is within range, use combat behavior (we have to check for both players)
	// Check for either player if that player is alive as well as in range
	else if (!level->getPlayers().first->isDead && !level->getPlayers().first->isDying
		&& objInRange(level->getPlayers().first, 128.0f)) {
		// Enter the combat state against player 1
		update_StateCombat(level->getPlayers().first);
	}
	// With player 2, we also have to see if they exist (aren't nullptr)
	else if (level->getPlayers().second != nullptr
		&& !level->getPlayers().second->isDead && !level->getPlayers().second->isDying
		&& objInRange(level->getPlayers().second, 128.0f)) {
		// Enter the combat state against player 2
		update_StateCombat(level->getPlayers().second);
	}
	// If none of the other states are active, and the enemy isn't dying, they
	// should march towards the destination
	else if (!dying) {
		update_StateMarch();
	}
	// Set the bad guy's destination
	update_DestinationLogic();
	// If the bad guy isn't dying, move animate it (dying baddies freeze and fade out)
	if (!dying) {
		stepAnimation();
	}
	// Do physics stuff
	applyForces();
	updateForces();
	// Starts running the appropriate animation if it's not already
	update_PlayAnimations();
}

// Starts running the appropriate animation if it's not already
void BadCharacter::update_PlayAnimations() {
	string key = "";
	// Possible animation prefixes
	if (isAttacking) key += "ATTACK_";
	else key += "WALK_";
	// Set the direction of the animation
	calculateAnimationDirection();
	key += animationDirection;
	// Check if the animation is already running
	if (!isPlayingAnimation(animations[key])) {
		// Start the animation if it isn't already running
		startAnimation(animations[key], true);
	}
}

void BadCharacter::update_DestinationLogic() {
	// The the bad guy is within range of his destination
	if (destinationID == "end") {
		GameObject* spire = level->getSpire();
		destinationCoordinates = { spire->getPosition().x, spire->getPosition().y };
	}
	// If the bad guy has arrived at their current destination, and there is a next
	// destination, update to the next destination.
	else if (distanceToDestination < 3 && paths[destinationID].next != "null") {
		destinationID = paths[destinationID].next;
		destinationCoordinates = { paths[destinationID].x, paths[destinationID].y };
	}
	// Calculate distance remaining to the destination
	float dx = m_vPos.x - destinationCoordinates.x;
	float dy = m_vPos.y - destinationCoordinates.y;
	float dx2 = dx * dx;
	float dy2 = dy * dy;
	distanceToDestination = sqrt(dx2 + dy2);
}


// Moves the baddie towards a given set of coordinates
void BadCharacter::update_Move(Vector2 towards) {
	// Get the vector direction to travel along
	Vector2 travelVector = m_vPos - towards;
	// Calculate angle to destination in radians
	float angle_in_radians = atan2f(travelVector.y, travelVector.x) + MY_PI;
	// Normalize to [0,2pi)
	if (angle_in_radians < 0) { angle += 2 * MY_PI; }
	// Calculate the angle in degrees
	angle = angle_in_radians * (180 / MY_PI);
	// Calculate the velocity
	Vector2 velocity = { speed * cosf(angle_in_radians), speed * sinf(angle_in_radians) };
	// Finally, move the baddie. But only if they aren't stopped.
	if (!isStopped)addForce(velocity, PseudoForce::Instant);
}

void BadCharacter::update_StateMarch() {
	// Set baddie flags
	isStopped = false;
	// Move the baddie towards the destination
	update_Move(destinationCoordinates);
}

void BadCharacter::update_StateDying() {
	// Flag the enemy as dying
	dying = true;
	// Stop the dying enemy from moving or attacking
	isStopped = true;
	isAttacking = false;
	// Enemy should be passable when it's in the dying state
	if (is("unpassable")) tags.erase(tags.find("unpassable"));
	// When the enemy is dying, it should fade away
	if (m_fAlpha > 0) {
		m_fAlpha -= .01f;
	}
	else {
		// Unless the game is over, the enemy should drop money on death
		if (!level->getGameOver())
			scene->addObject(new Coin(scene, m_pRandom->randn(1, 10), m_vPos.x, m_vPos.y));
		// Destroy this creature once it's faded out
		kill();
	}
}

void BadCharacter::update_StateAttackSpire() {
	Spire* spire = level->getSpire();
	// Stop the enemy from moving
	isStopped = true;
	// Flag the enemy as attacking
	isAttacking = true;
	// If the enemy is ready to attack, have it damage the spire
	if (readyToAttack()) {
		// Deal some damage to the spire
		spire->hp -= 20;
		// Assign a new time for the next attack
		nextAttackTime = m_pStepTimer->GetTotalSeconds() + attackCooldown;
	}
}

// Combat behavior for when players are nearby
void BadCharacter::update_StateCombat(PlayerCharacter* player) {
	// Move the baddie towards the player, if they aren't already too close
	if (!objInRange(player, 24.0f)) {
		isStopped = false;
		update_Move({ player->m_vPos.x, player->m_vPos.y });
	}
	else {
		isStopped = true;
	}
	// If the enemy is ready to attack, it should attack the player
	if (readyToAttack() && objInRange(player, 48.0f)) {
		// Flag the enemy as attacking
		isAttacking = true;
		// Actual handling of attack effects occur on the middle frame of attack animation
		int middleFrameOffset = (activeAnimation.endFrame - activeAnimation.startFrame) / 2;
		int middleFrame = activeAnimation.startFrame + middleFrameOffset;
		if (m_nCurrentFrame == middleFrame) {
			// Lower the player's health
			player->hp -= 20;
			// Get the vector between the baddie and the player
			Vector2 force = player->getPosition() - getPosition();
			force.Normalize();
			// Scale the knockback force
			force *= 300.0f;
			// Some knockback to teach that player a lesson
			player->addForce(force, 0.5f);
			// Assign a new time for the next attack
			nextAttackTime = m_pStepTimer->GetTotalSeconds() + attackCooldown;
		}
	}
}

// Determines if the enemy can attack based or if it's still in cooldown
bool BadCharacter::readyToAttack() {
	return m_pStepTimer->GetTotalSeconds() > nextAttackTime;
}

void BadCharacter::setDestinationCoordinates(float x, float y) {
	destinationCoordinates = { x, y };
}

void BadCharacter::setDestinationByID(string id) {
	destinationID = id;
	destinationCoordinates = { paths[id].x, paths[id].y };
}


void BadCharacter::onAnimationComplete(AnimationDesc& anim) {
	if (isAttacking) isAttacking = false;
}