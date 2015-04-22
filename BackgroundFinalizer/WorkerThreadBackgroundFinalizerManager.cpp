#include "WorkerThreadBackgroundFinalizerManager.h"

using namespace System::Threading;

namespace BackgroundFinalizer
{
	WorkerThreadBackgroundFinalizerManager::Node::Node(
		BackgroundFinalizer::Finalizer^ finalizer) :
		next(nullptr), finalizer(finalizer)
	{
	}

	WorkerThreadBackgroundFinalizerManager::Node::Node() :
		next(nullptr), finalizer(nullptr)
	{
	}

	Object^ WorkerThreadBackgroundFinalizerManager::AllocateSpaceImpl(
		BackgroundFinalizer::Finalizer^ finalizer)
	{
		return gcnew Node(finalizer);
	}

	void WorkerThreadBackgroundFinalizerManager::EnqueueFinalizerImpl(
		Object^ space)
	{
		Node^ newNode = dynamic_cast<Node^>(space);
		Node^ oldTail = tail;
		// Atomically point tail->next to newNode iff it's currently
		// null.
		while (Interlocked::CompareExchange<Node^>(tail->next, newNode,
			nullptr) != nullptr)
		{
			// tail->next is not null, so tail is lagging.

			// Atomically point tail to tail->next if it hasn't
			// changed.
			Interlocked::CompareExchange<Node^>(tail, oldTail->next,
				oldTail);
			oldTail = tail;
		}
		// We've updated tail->next, now point tail to newNode.
		Interlocked::CompareExchange<Node^>(tail, newNode,
			oldTail);
		// Tell the background thread we've enqueued something.
		enqueuedEvent.Set();
	}

	void WorkerThreadBackgroundFinalizerManager::BackgroundThread()
	{
		while (true)
		{
			// Only this thread touches head, so simpler than a fully
			// thread-safe queue.
			Node^ oldTail = tail;
			if (head == oldTail)
			{
				if (head->next == nullptr)
				{
					// queue is empty, wait.
					enqueuedEvent.WaitOne();
					enqueuedEvent.Reset();
				}
				else
				{
					// head->next is not null, and head == tail, so
					// tail is lagging.

					// Atomically point tail to tail->next if it hasn't
					// changed.
					Interlocked::CompareExchange<Node^>(tail,
						oldTail->next,
						oldTail);
				}
			}
			else
			{
				// We always have a dummy node at the head, pop the
				// current one off.
				head = head->next;
				head->finalizer();
				// Allow the finalizer to be garbage collected even if
				// this dummy node stays alive.
				head->finalizer = nullptr;
			}
		}
	}

	WorkerThreadBackgroundFinalizerManager::
		WorkerThreadBackgroundFinalizerManager() : tail(gcnew Node()),
		head(tail), enqueuedEvent(false)
	{
		Thread t(gcnew ThreadStart(this,
			&WorkerThreadBackgroundFinalizerManager::
				BackgroundThread));
		t.IsBackground = true;
		t.Start();
	}
}
