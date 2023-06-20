#include "js/Scope.hpp"

namespace ion::js
{
	Scope::Scope(v8::Isolate* isolate) :
		handleScope_(isolate),
		contextScope_(isolate->GetCurrentContext())
	{}

	Scope::Scope(v8::Isolate* isolate, v8::Local<v8::Context> ctx) :
		handleScope_(isolate),
		contextScope_(ctx)
	{}
}