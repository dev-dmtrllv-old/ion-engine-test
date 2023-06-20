#pragma once

#include "pch.hpp"
#include "v8.h"
#include "js/Scope.hpp"

namespace ion::js
{
	class Env;

	template<typename T>
	concept IsRunCallback = std::same_as<T, void(*)(Env& env)>;

	class Env
	{
	private:
		static v8::Isolate::CreateParams initIsolateParams();

	public:
		Env();
		Env(const Env&) = delete;
		Env(Env&&) = delete;
		~Env();

		[[nodiscard]] v8::Isolate* isolate();
		[[nodiscard]] v8::Local<v8::Context> context();

		template<IsRunCallback Callback>
		void run(Callback runCallback)
		{
			Scope scope_(isolate_);
			runCallback(isolate_);
		}

		template<IsRunCallback Callback>
		void runGlobal(Callback runCallback)
		{
			runCallback(isolate_);
		}

	private:
		v8::Isolate::CreateParams createParams_;
		v8::Isolate* isolate_;
		v8::Global<v8::Context> context_;
	};
}