#pragma once

#include "Vector2f.h"
#include "Constants.h"
#include "Component.h"

#include <memory>
#include <vector>
#include <string>

class Drawer;
class C_Sprite;

class GameEntity
{
public:
	GameEntity(const Vector2f& aPosition);

    template <typename T, typename... Args> 
    std::shared_ptr<T> AddComponent(Args... args)
    {
        for (auto& exisitingComponent : components)
        {
            if (std::dynamic_pointer_cast<T>(exisitingComponent))
            {
                return std::dynamic_pointer_cast<T>(exisitingComponent);
            }
        }

        std::shared_ptr<T> newComponent = std::make_shared<T>(*this, args...);
        components.push_back(newComponent);

        if (std::dynamic_pointer_cast<C_Sprite>(newComponent))
        {
            mySprite = std::dynamic_pointer_cast<C_Sprite>(newComponent);
        }

        return newComponent;
    };

    template <typename T> 
    std::shared_ptr<T> GetComponent()
    {
        for (auto& exisitingComponent : components)
        {
            if (std::dynamic_pointer_cast<T>(exisitingComponent))
            {
                return std::dynamic_pointer_cast<T>(exisitingComponent);
            }
        }

        return nullptr;
    };

    virtual void Awake();
	virtual void Start();
	virtual void Update(float time);

    Vector2f GetPosition() const;
    Vector2f AddPosition(const Vector2f& toAdd);
    void SetPosition(const Vector2f& aPosition);

	void Draw();	
    std::shared_ptr<C_Sprite> GetSprite() const;

    void SetDelete();
    bool IsMarkedForDelete() const;

    std::string tag;

protected:
	bool isDeletedFlag;
	Vector2f myPosition;
	std::shared_ptr<C_Sprite> mySprite;
    std::vector<std::shared_ptr<Component>> components;
};