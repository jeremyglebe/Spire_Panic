#include "PseudoForce.h"
#include "StepTimer.h"

const float PseudoForce::Instant = 0.0f;

PseudoForce::PseudoForce(Vector2 velocity, float decayTime) {
	this->initial = velocity;
	this->decayTime = decayTime;
	this->startTime = m_pStepTimer->GetTotalSeconds();
}

Vector2 PseudoForce::get() {
	// "Instant Forces" should just return the force provided
	if (decayTime == PseudoForce::Instant)
		return initial;
	else {
		// Other forces should be proportional to their rate of decay
		float elapsedTime = m_pStepTimer->GetTotalSeconds() - startTime;
		float decayPercent = elapsedTime / decayTime;
		Vector2 decay = initial * decayPercent;
		return initial - decay;
	}
}

bool PseudoForce::isDead() {
	// "Instant Forces" die the first time they are checked
	if (decayTime == PseudoForce::Instant) return true;
	else {
		float elapsedTime = m_pStepTimer->GetTotalSeconds() - startTime;
		return decayTime - elapsedTime <= 0.0f;
	}
}