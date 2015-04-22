#include "BackgroundFinalizer.h"
#include "IBackgroundFinalizerManager.h"

namespace BackgroundFinalizer
{
	BackgroundFinalizer::BackgroundFinalizer(Finalizer^ finalizer,
		IBackgroundFinalizerManager^ manager) : finalizer(finalizer),
			manager(manager)
	{
		space = manager->AllocateSpace();
	}

	BackgroundFinalizer::!BackgroundFinalizer()
	{
		manager->EnqueueFinalizer(space, finalizer);
	}

	BackgroundFinalizer::~BackgroundFinalizer()
	{
		this->!BackgroundFinalizer();
	}
}
