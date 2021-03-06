#pragma once

#include "Vector2f.h"
#include "Constants.h"
#include "Component.h"

#include <memory>
#include <vector>
#include <string>
#include <algorithm>

class Drawer;
class C_Sprite;

class GameEntity
{
public:
	GameEntity(const Vector2f& position);

	template <typename T, typename... Args> 
	std::shared_ptr<T> AddComponent(Args&&... args)
	{
		auto it = std::find_if(components.begin(), components.end(), [](const auto& comp) {
			return std::dynamic_pointer_cast<T>(comp);
			});
		if (it != components.end())
		{
			return std::dynamic_pointer_cast<T>(*it);
		}

		auto newComponent = std::make_shared<T>(*this, std::forward<Args>(args)...);
		components.push_back(newComponent);

		if (std::dynamic_pointer_cast<C_Sprite>(newComponent))
		{
			sprite = std::dynamic_pointer_cast<C_Sprite>(newComponent);
		}

		return newComponent;
	};

	template <typename T> 
	std::shared_ptr<T> GetComponent() const
	{
		auto it = std::find_if(components.begin(), components.end(), [](const auto& comp) {
			return std::dynamic_pointer_cast<T>(comp);
			});
		if (it != components.end())
		{
			return std::dynamic_pointer_cast<T>(*it);
		}

		return nullptr;
	};

	virtual void Awake();
	virtual void Start();
	virtual void Update(float time);

	Vector2f GetPosition() const noexcept;
	Vector2f AddPosition(const Vector2f& toAdd) noexcept;
	void SetPosition(const Vector2f& position) noexcept;

	void Draw();	
	std::shared_ptr<C_Sprite> GetSprite() const noexcept;

	void SetDelete() noexcept;
	bool IsMarkedForDelete() const noexcept;

	std::string tag;

protected:
	bool isDeletedFlag;
	Vector2f position;
	std::shared_ptr<C_Sprite> sprite;
	std::vector<std::shared_ptr<Component>> components;
};