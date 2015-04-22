#include "BackgroundFinalizer.h"
#include "IBackgroundFinalizerManager.h"

namespace BackgroundFinalizer
{
	BackgroundFinalizer::BackgroundFinalizer(Finalizer^ finalizer,
		IBackgroundFinalizerManager^ manager) : manager(manager)
	{
		space = manager->AllocateSpace(finalizer);
	}

	BackgroundFinalizer::!BackgroundFinalizer()
	{
		manager->EnqueueFinalizer(space);
	}

	BackgroundFinalizer::~BackgroundFinalizer()
	{
		this->!BackgroundFinalizer();
	}
}
