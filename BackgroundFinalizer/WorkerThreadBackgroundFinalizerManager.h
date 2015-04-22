#pragma once

#include "IBackgroundFinalizerManager.h"

using namespace System::Threading;

namespace BackgroundFinalizer
{
	/// <summary>
	/// An IBackgroundFinalizerManager that uses a worker thread to
	/// run the finalizers.
	/// </summary>
	public ref class WorkerThreadBackgroundFinalizerManager :
		public IBackgroundFinalizerManager
	{
		virtual Object^ AllocateSpaceImpl(
			BackgroundFinalizer::Finalizer^ finalizer) sealed =
			IBackgroundFinalizerManager::AllocateSpace;

		virtual void EnqueueFinalizerImpl(Object^ space) sealed =
				IBackgroundFinalizerManager::EnqueueFinalizer;

		ref struct Node
		{
			Node^ next;
			BackgroundFinalizer::Finalizer^ finalizer;
			Node(BackgroundFinalizer::Finalizer^ finalizer);
			Node();
		};

		Node^ tail;

		Node^ head;

		ManualResetEvent enqueuedEvent;

		void BackgroundThread();
	public:
		/// <summary>
		/// Create a <c>WorkerThreadBackgroundFinalizerManager</c>.
		/// </summary>
		WorkerThreadBackgroundFinalizerManager();
	};
}
