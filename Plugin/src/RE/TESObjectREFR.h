#pragma once

#include "RE/T/TESObjectREFR.h"

namespace RE
{
	namespace Override
	{
		// Modified in 1.10.31.0.
		// 08
		template <class Event>
		class BSTEventSourceLazyInit
		{
		public:
			// members
			BSTEventSource<Event>* eventSource;  // 00
		};
		static_assert(sizeof(BSTEventSourceLazyInit<void*>) == 0x8);

		// Modified in 1.10.31.0.
		// D0
		class TESObjectREFR :
			public TESHandleForm,                                                    // 00
			public BSTEventSink<BSTransformDeltaEvent>,                              // 30
			public IMovementProcessMessageInterface,                                 // 38
			public IPostAnimationChannelUpdateFunctor,                               // 40
			public BSTEventSink<BSAnimationGraphEvent>,                              // 48
			public BSTEventSink<BGSInventoryListEvent::Event>,                       // 50
			public IAnimationGraphManagerHolder,                                     // 58
			public IKeywordFormBase,                                                 // 60
			public ActorValueOwner,                                                  // 68
			public BSTEventSourceLazyInit<ActorValueEvents::ActorValueChangedEvent>  // 70
		{
		public:
			[[nodiscard]] TESBoundObject*       GetBaseObject() { return baseForm; }
			[[nodiscard]] const TESBoundObject* GetBaseObject() const { return baseForm; }

			// members
			NiPoint3          rotation;       // 78
			NiPoint3          position;       // 84
			TESBoundObject*   baseForm;       // 90
			BGSInventoryList* inventoryList;  // 98
			std::uint64_t     unkA0;          // A0
			TESObjectCELL*    parentCell;     // A8
			LOADED_REF_DATA*  loadedData;     // B0
			std::uint64_t     unkB8;          // B8
			ExtraDataList*    extraDataList;  // C0
			std::uint64_t     unkC8;          // C8
		};
		static_assert(sizeof(TESObjectREFR) == 0xD0);
	}
}