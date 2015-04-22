#pragma once

#include "BackgroundFinalizer.h"

namespace BackgroundFinalizer
{
	/// <summary>
	/// A manager for background finalizers.
	/// </summary>
	/// <remarks>
	/// Runs enqueued finalizers out-of-line with the garbage
	/// collector.
	/// </remarks>
	public interface class IBackgroundFinalizerManager
	{
		/// <summary>
		/// Allocate space for a new finalizer.
		/// </summary>
		IntPtr AllocateSpace();

		/// <summary>
		/// Add a finalizer to the background queue.
		/// </summary>
		/// <param name="space">The space to use for the queue.</param>
		/// <param name="finalizer">The finalizer to enqueue.</param>
		/// <remarks>
		/// <para>
		/// <paramref name="space"/> must come from a call to
		/// <c>AllocateSpace()</c>.
		/// </para>
		///
		/// <para>
		/// The <paramref name="finalizer"/> must follow the general
		/// rules for finalizers, except that it is guaranteed not to
		/// be called on the same thread as an active collection.
		/// </para>
		///
		/// <para>
		/// EnqueueFinalizer is meant to be called from a finalizer.
		/// As such, implementations must be sure not to have any
		/// opportunity for garbage collection while internal
		/// invariants are broken and must not block before
		/// returning from EnqueueFinalizer. In particular, it must
		/// not allocate any memory (that's what <c>AllocateSpace</c>
		/// is for).
		/// </para>
		///
		/// <para>
		/// Implementations may not be able to guarantee that
		/// finalizers enqueued shortly before application exit will
		/// be called.
		/// </para>
		/// </remarks>
		void EnqueueFinalizer(IntPtr space,
			const BackgroundFinalizer::Finalizer^ finalizer);
	};
}
