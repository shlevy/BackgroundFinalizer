#include "BackgroundFinalizer.h"
#include "IBackgroundFinalizerManager.h"

namespace BackgroundFinalizer
{
	BackgroundFinalizer::BackgroundFinalizer(Finalizer^ finalizer,
		IBackgroundFinalizerManager^ manager) : finalizer(finalizer),
			manager(manager)
	{
		this->manager->AllocateSpace();
	}

	BackgroundFinalizer::!BackgroundFinalizer()
	{
		manager->EnqueueFinalizer(finalizer);
	}

	BackgroundFinalizer::~BackgroundFinalizer()
	{
		this->!BackgroundFinalizer();
	}
}
