#pragma once
#include <list>
#include "Component.h"

#define USE_MEMORY_

#ifdef USE_MEMORY
#include <memory>
#endif

class GameObject
{
public:
	GameObject();
	~GameObject();

	template<class T>
	void AddComponent();

	template<class T>
	T* GetComponent();

	template<class T>
	void DelComponent();

	void ListShow() const;

public:
#ifdef USE_MEMORY
	std::list<std::shared_ptr<Component>> m_componentList;
#else
	std::list<Component*> m_componentList;
#endif
};

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
#ifndef USE_MEMORY
	for (auto& item : m_componentList)
	{
		delete item;
		item = nullptr;
	}
#endif
}

template <class T>
inline void GameObject::AddComponent()
{
#ifdef USE_MEMORY
	std::shared_ptr<T> buffer = std::make_shared<T>();
	m_componentList.emplace_back(buffer);
#else
	#if true
	#endif
		T* buffer = new T;
		m_componentList.emplace_back(buffer);
#endif
}

template <class T>
inline T* GameObject::GetComponent()
{
	for (const auto& item : m_componentList)
	{
#ifdef USE_MEMORY
		T* buffer = dynamic_cast<T*>(item.get());
#else
		T* buffer = dynamic_cast<T*>(item);
#endif
		if (buffer)
		{
			return buffer;
		}
	}
}

template <class T>
inline void GameObject::DelComponent()
{
#ifdef USE_MEMORY
	m_componentList.remove_if(
		[](const auto& item)
		{
			T* buffer = dynamic_cast<T*>(item.get());
			return (buffer != nullptr);
		});
#else
	//m_componentList.remove_if(
	//	[](const auto& item)
	//	{
	//		T* buffer = dynamic_cast<T*>(item);
	//		bool isExist = buffer;
	//		if (isExist)
	//		{
	//			delete buffer;
	//			item = nullptr;
	//		}
	//		return isExist;
	//	});

	// ÉâÉÄÉ_éÆÇégÇÌÇ»Ç¢èÍçá
	for (auto& item : m_componentList)
	{
		T* buffer = dynamic_cast<T*>(item);
		if (buffer)
		{
			delete item;
			item = nullptr;
			// MEMO: deleteÇÃï˚ÇÕbuffer, item Ç«ÇøÇÁÇ≈Ç‡Ç¢Ç¢Ç™= nullptrÇÕitemÇ…ÇµÇ»Ç¢Ç∆Ç¢ÇØÇ»Ç¢
			//       Å®bufferÇnullptrÇ…ÇµÇƒÇ‡listì‡ÇÃÇ‡ÇÃé©ëÃ(item)ÇÕnullptrÇ…Ç»ÇÁÇ»Ç¢
		}
	}
	m_componentList.remove(nullptr);
#endif
}

void GameObject::ListShow() const
{
	for (const auto& item : m_componentList)
	{
		item->ShowName();
	}
}