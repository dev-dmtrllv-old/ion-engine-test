#pragma once

#include "pch.hpp"
#include "v8.h"

namespace ion::js
{
	struct Scope
	{
	public:
		Scope(v8::Isolate* isolate);
		Scope(v8::Isolate* isolate, v8::Local<v8::Context> ctx);

	private:
		v8::HandleScope handleScope_;
		v8::Context::Scope contextScope_;
	};
}