#pragma once
#include"stdafx.h"
#include"Subsystem\ISubsystem.h"

class Context final
{
public:
	Context() = default;

	~Context()
	{
		for (auto iter = subsystems.rbegin(); iter != subsystems.rend(); iter++) //뒤부터 지움
			SAFE_DELETE(*iter);
	}

	auto Initialize_subsystems()-> const bool
	{
		auto result = true;
		for (const auto& subsystem : subsystems)
		{
			if (!subsystem->Initialize())
				result = false;
		}
		return result;
	}

	template<typename T>
	auto Register_subsystem()-> T* const
	{
		static_assert(std::is_base_of<ISubsystem, T>::value, "Provided type does not implement ISubsystem");
		
		return static_cast<T*>(subsystems.emplace_back(new T(this)));
	}

	template<typename T>
	auto GetSubsystem() ->  T* const
	{
		static_assert(std::is_base_of<ISubsystem, T>::value, "Provided type does not implement ISubsystem");
		
		for (const auto& subsystem : subsystems) //RTTI map형식으로 개선필요
		{
			if (typeid(T) == typeid(*subsystem))
				return static_cast<T*>(subsystem);
		}
		return nullptr;
	}

	void Update_subsystems()
	{
		for(const auto& subsystem: subsystems)
			subsystem->Update();
	}


private:
	std::vector<ISubsystem*> subsystems;

};