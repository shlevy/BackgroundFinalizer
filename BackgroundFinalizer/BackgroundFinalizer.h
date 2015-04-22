#pragma once

using namespace System;

namespace BackgroundFinalizer {
	interface class IBackgroundFinalizerManager;

	/// <summary>
	/// A finalizer that runs in the background of garbage collection.
	/// </summary>
	/// <remarks>
	/// <para>
	/// When garbage collected, a <c>BackgroundFinalizer</c> passes
	/// its finalizer delegate to an
	/// <c>IBackgroundFinalizerManager</c>.
	/// <see cref="IBackgroundFinalizerManager"/> for details.
	/// </para>
	///
	/// <para>
	/// It is expected that a <c>BackgroundFinalizer</c> will be
	/// declared as a <c>private readonly</c> member of a class
	/// needing finalization.
	/// </para>
	/// </remarks>
	public ref class BackgroundFinalizer
	{
	public:
		/// <summary>
		/// A function to run as a finalizer
		/// </summary>
		delegate void Finalizer();

		/// <summary>
		/// Create a <c>BackgroundFinalizer</c>.
		/// </summary>
		/// <param name="finalizer">The finalizer to run.</param>
		/// <param name="manager">
		/// The manager to handle the finalizer.
		/// </param>
		BackgroundFinalizer(Finalizer^ finalizer,
			IBackgroundFinalizerManager^ manager);

		/// <summary>
		/// Finalize a <c>BackgroundFinalizer</c>
		/// </summary>
		/// <remarks>
		/// Passes off the <c>finalizer</c> to the <c>manager</c>.
		/// </remarks>
		!BackgroundFinalizer();

		/// <summary>
		/// Destruct a <c>BackgroundFinalizer</c>
		/// </summary>
		/// <remarks>
		/// Calls the finalizer.
		/// </remarks>
		~BackgroundFinalizer();

	private:
		const Finalizer^ finalizer;

		initonly IBackgroundFinalizerManager^ manager;
	};
}
