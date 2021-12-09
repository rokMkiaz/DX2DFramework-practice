#pragma once
#include"Core\Context.h"

class ISubsystem
{
public:
	ISubsystem(class Context* context) : context(context) {};

	virtual ~ISubsystem() = default;

	virtual bool Initialize() = 0;
	virtual void Update() = 0;


	auto GetContext() const-> const class Context* const { return context; }

protected:
	class Context* context = nullptr;
};