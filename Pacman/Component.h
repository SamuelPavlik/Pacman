#pragma once

class GameEntity;

class Component
{
public:
	Component(GameEntity& owner) : owner(owner) {}

	virtual void Start() {};

	virtual void Update(float deltaTime) {};

	GameEntity& owner;
};

