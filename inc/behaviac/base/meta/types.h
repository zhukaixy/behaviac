/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tencent is pleased to support the open source community by making behaviac available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _BEHAVIAC_BASE_META_TYPES_H__INCLUDED
#define _BEHAVIAC_BASE_META_TYPES_H__INCLUDED

#include "behaviac/base/base.h"
#include "behaviac/base/core/assert_t.h"
#include "behaviac/base/core/staticassert.h"

#include "behaviac/base/meta/meta.h"

#include "behaviac/base/meta/removeconst.h"
#include "behaviac/base/meta/removeref.h"
#include "behaviac/base/meta/removeptr.h"
#include "behaviac/base/meta/isconst.h"
#include "behaviac/base/meta/isptr.h"
#include "behaviac/base/meta/isref.h"

#include "behaviac/base/meta/pointertype.h"
#include "behaviac/base/meta/ifthenelse.h"
#include "behaviac/base/meta/reftype.h"

#include "behaviac/base/meta/isenum.h"

#include "behaviac/base/meta/isderived.h"
#include "behaviac/base/meta/issame.h"

#define _BASETYPE_(T) typename behaviac::Meta::RemovePtr<typename behaviac::Meta::RemoveRef<T>::Result>::Result

#define REAL_BASETYPE(T) typename behaviac::Meta::RemoveConst<_BASETYPE_(T)>::Result

#define VALUE_TYPE(T) typename behaviac::Meta::RemoveConst<typename behaviac::Meta::RemoveRef<T>::Result>::Result
#define POINTER_TYPE(T) typename behaviac::Meta::PointerType<T>::Result

namespace behaviac
{
	class Agent;
	
	namespace Meta
	{

		template <typename T, bool bPtr>
		class ParamTypeConverter
		{
		public:
			typedef REAL_BASETYPE(T)		BaseType;
			typedef POINTER_TYPE(T)			PointerType;
		};

		template <>
		class ParamTypeConverter<const char*, true>
		{
		public:
			typedef behaviac::string		BaseType;
			typedef const char**			PointerType;
		};

		template<typename T>
		struct ParamCalledType
		{
		private:
			//can't remove const
			typedef typename behaviac::Meta::RemovePtr<typename behaviac::Meta::RemoveRef<T>::Result>::Result		RealBaseType_t;
			typedef typename behaviac::Meta::RefType<T>::Result												RefType_t;
		public:
			typedef typename behaviac::Meta::IfThenElse<behaviac::Meta::IsPtr<T>::Result, RefType_t, T>::Result Result;
		};

		template <typename T>
		struct IsAgent
		{
			typedef REAL_BASETYPE(T)		TBaseType;

			enum 
			{
				Result = behaviac::Meta::IsSame<behaviac::Agent, TBaseType>::Result || 
					behaviac::Meta::IsDerived<behaviac::Agent, TBaseType>::Result
			};
		};

	}//namespace Meta
}//namespace behaviac

#define PARAM_BASETYPE(T)		typename behaviac::Meta::ParamTypeConverter<T, behaviac::Meta::IsPtr<T>::Result>::BaseType
#define PARAM_POINTERTYPE(T)	typename behaviac::Meta::ParamTypeConverter<T, behaviac::Meta::IsPtr<T>::Result>::PointerType

#define PARAM_CALLEDTYPE(T)	typename behaviac::Meta::ParamCalledType<T>::Result
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define AGENT_PTR_TYPE(T) typename behaviac::Meta::IfThenElse<behaviac::Meta::IsAgent<T>::Result, PARAM_POINTERTYPE(T), T>::Result
#define STORED_TYPE(T) typename behaviac::Meta::IfThenElse<behaviac::Meta::IsPtr<T>::Result, T, AGENT_PTR_TYPE(T)>::Result

template<typename T, bool bAgent, bool bPtr>
struct GetValueSelector
{
	typedef STORED_TYPE(T) StoredType_t;

	static const T& GetValue(const StoredType_t& v)
	{
		return v;
	}
};

template<typename T>
struct GetValueSelector < T, true, false >
{
	typedef STORED_TYPE(T) StoredType_t;

	static const T& GetValue(const StoredType_t& v)
	{
		return *v;
	}
};

template<typename T>
struct GetValueSelector < T, true, true >
{
	typedef STORED_TYPE(T) StoredType_t;

	static const T& GetValue(const StoredType_t& v)
	{
		return v;
	}
};

#endif//_BEHAVIAC_BASE_META_TYPES_H__INCLUDED
