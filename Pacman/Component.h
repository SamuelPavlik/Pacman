#pragma once

class GameEntity;

class Component
{
public:
	Component(GameEntity& owner) : 
		owner(owner),
		isComponentOn(true) {}

	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update(float time) {};

	GameEntity& owner;
	bool isComponentOn;
};

