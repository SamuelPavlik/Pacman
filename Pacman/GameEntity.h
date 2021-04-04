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
    std::shared_ptr<T> AddComponent(Args... args)
    {
        auto it = std::find_if(components.begin(), components.end(), [](auto comp) {
            return std::dynamic_pointer_cast<T>(comp);
            });
        if (it != components.end())
            return std::dynamic_pointer_cast<T>(*it);

        std::shared_ptr<T> newComponent = std::make_shared<T>(*this, args...);
        components.push_back(newComponent);

        if (std::dynamic_pointer_cast<C_Sprite>(newComponent))
        {
            sprite = std::dynamic_pointer_cast<C_Sprite>(newComponent);
        }

        return newComponent;
    };

    template <typename T> 
    std::shared_ptr<T> GetComponent()
    {
        auto it = std::find_if(components.begin(), components.end(), [](auto comp) {
            return std::dynamic_pointer_cast<T>(comp);
            });
        if (it != components.end())
            return std::dynamic_pointer_cast<T>(*it);

        return nullptr;
    };

    virtual void Awake();
	virtual void Start();
	virtual void Update(float time);

    Vector2f GetPosition() const;
    Vector2f AddPosition(const Vector2f& toAdd);
    void SetPosition(Vector2f position);

	void Draw();	
    std::shared_ptr<C_Sprite> GetSprite() const;

    void SetDelete();
    bool IsMarkedForDelete() const;

    std::string tag;

protected:
	bool isDeletedFlag;
	Vector2f position;
	std::shared_ptr<C_Sprite> sprite;
    std::vector<std::shared_ptr<Component>> components;
};