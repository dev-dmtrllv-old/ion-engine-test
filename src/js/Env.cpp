#include "pch.hpp"
#include "js/Env.hpp"

namespace ion::js
{
	Env::Env()
	{
		createParams_.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

		isolate_ = v8::Isolate::New(createParams_);

		v8::Isolate::Scope isolateScope(isolate_);
		v8::HandleScope handleScope(isolate_);

		v8::Local<v8::Context> ctx = v8::Context::New(isolate_);
		v8::Context::Scope contextScope(ctx);

		context_ = v8::Global<v8::Context>(isolate_, ctx);
		
		// assert(isolate_->GetNumberOfDataSlots() != 0);
		// isolate_->SetData(0, this);
	}

	Env::~Env()
	{
		context_.Reset();
		isolate_->Dispose();
		delete createParams_.array_buffer_allocator;
	}

	[[nodiscard]] v8::Isolate* Env::isolate() { return isolate_; }
	[[nodiscard]] v8::Local<v8::Context> Env::context() { return context_.Get(isolate_); }
}