/* Copyright 2010 Jukka Jyl�nki

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */
#pragma once

/** @file Thread.h
	@brief The Thread class. Implements threading either using Boost or native Win32 constructs. */

#ifdef KNET_USE_BOOST
#include <boost/thread.hpp>
#elif WIN32
#include <Windows.h>

namespace kNet
{
typedef void (*ThreadEntryFunc)(void *threadStartData);
}

#endif

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

#include "SharedPtr.h"

namespace kNet
{

class Thread : public RefCountable
{
public:
	Thread();
	~Thread();

	bool ShouldQuit() const;

	bool IsRunning() const;

	/// Tries first to gracefully close the thread (waits for a while), and forcefully terminates the thread if
	/// it didn't respond in that time. \todo Allow specifying the timeout period.
	void Stop();

	template<typename Class, typename MemberFuncPtr, typename FuncParam>
	void Run(Class *obj, MemberFuncPtr memberFuncPtr, const FuncParam &param);

	template<typename Class, typename MemberFuncPtr>
	void Run(Class *obj, MemberFuncPtr memberFuncPtr);

private:
	Thread(const Thread &);
	void operator =(const Thread &);

	class ObjInvokeBase : public RefCountable
	{
	public:
		virtual void Invoke() = 0;
		virtual ~ObjInvokeBase() {}

		void operator()()
		{
			Invoke();
		}
	};

	template<typename Class, typename MemberFuncPtr>
	class ObjInvokerVoid : public ObjInvokeBase
	{
	public:
		Class *obj;
		MemberFuncPtr memberFuncPtr;
		ObjInvokerVoid(Class *obj_, MemberFuncPtr memberFuncPtr_)
		:obj(obj_), memberFuncPtr(memberFuncPtr_){}

		virtual void Invoke() { CALL_MEMBER_FN(*obj, memberFuncPtr)(); }
	};

	template<typename Class, typename MemberFuncPtr, typename FuncParam>
	class ObjInvokerUnary : public ObjInvokeBase
	{
	public:
		Class *obj;
		MemberFuncPtr memberFuncPtr;
		FuncParam param;
		ObjInvokerUnary(Class *obj_, MemberFuncPtr memberFuncPtr_, const FuncParam &param_)
		:obj(obj_), memberFuncPtr(memberFuncPtr_),param(param_){}

		virtual void Invoke() { CALL_MEMBER_FN(*obj, memberFuncPtr)(param); }
	};

	ObjInvokeBase *invoker;

	void StartThread();

#ifdef KNET_USE_BOOST
	boost::thread thread;
#elif WIN32
	HANDLE threadHandle;

	/// The entry point that is called from the trampoline. Do not call this function.
	void _ThreadRun();

	friend DWORD WINAPI ThreadEntryPoint(LPVOID lpParameter);
private:
	bool threadEnabled;

#endif
};

template<typename Class, typename MemberFuncPtr, typename FuncParam>
void Thread::Run(Class *obj, MemberFuncPtr memberFuncPtr, const FuncParam &param)
{
	Stop();
	invoker = new ObjInvokerUnary<Class, MemberFuncPtr, FuncParam>(obj, memberFuncPtr, param);
	StartThread();
}

template<typename Class, typename MemberFuncPtr>
void Thread::Run(Class *obj, MemberFuncPtr memberFuncPtr)
{
	Stop();
	invoker = new ObjInvokerVoid<Class, MemberFuncPtr>(obj, memberFuncPtr);
	StartThread();
}

} // ~kNet
