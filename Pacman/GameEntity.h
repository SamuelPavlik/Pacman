#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "Vector2f.h"
#include "Constants.h"
#include "Component.h"

#include <memory>
#include <vector>

class Drawer;
class C_Sprite;

class GameEntity
{
public:
	GameEntity(const Vector2f& aPosition);
	~GameEntity(void);

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

	virtual void Start();
	virtual void Update(float time);
	bool Intersect(GameEntity* aGameEntity);

	Vector2f GetPosition() const { return myPosition; }
    Vector2f AddPosition(const Vector2f& toAdd);
	void SetPosition(const Vector2f& aPosition){ myPosition = aPosition; }

	void Draw(Drawer* drawer);
	
    std::shared_ptr<C_Sprite> GetSprite() const;

	void MarkForDelete() { myIdMarkedForDeleteFlag = true; }
	bool IsMarkedForDelete() const { return myIdMarkedForDeleteFlag; }

protected:

	bool myIdMarkedForDeleteFlag;
	Vector2f myPosition;
	std::shared_ptr<C_Sprite> mySprite;
    std::vector<std::shared_ptr<Component>> components;
};

#endif // GAMEENTITY_H