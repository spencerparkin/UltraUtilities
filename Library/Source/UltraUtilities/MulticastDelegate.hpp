#pragma once

#include "UltraUtilities/Defines.h"
#include "UltraUtilities/Containers/DArray.hpp"

namespace UU
{
	/**
	 * This is a simple yet very useful way of decoupling systems that want
	 * to notify and systems that need to be notified.
	 */
	template<typename LambdaType, typename... LambdaArgs>
	class UU_API MulticastDelegate
	{
	public:
		MulticastDelegate()
		{
			this->nextHandle = 0;
		}
		
		virtual ~MulticastDelegate()
		{
		}

		/**
		 * Add a function to be called when the delegate is fired.
		 * 
		 * @param[in] lambda This is the lambda to call.
		 * @return A handle is returned that is used to disconnect the lambda later.
		 */
		int Connect(LambdaType lambda)
		{
			Entry entry{ lambda, this->nextHandle++ };
			this->lambdaArray.Push(entry);
			return entry.handle;
		}

		/**
		 * Remove a function from this delegate.
		 * 
		 * @param[in] handle This is the handle returned by @ref Connect.
		 * @return True is returned if successful; false, otherwise.
		 */
		bool Disconnect(int handle)
		{
			for (unsigned int i = 0; i < this->lambdaArray.GetSize(); i++)
			{
				Entry& entry = this->lambdaArray[i];
				if (entry.handle == handle)
				{
					this->lambdaArray.ShiftRemove(i);
					return true;
				}
			}

			return true;
		}

		/**
		 * Call all lambda bound to this delegate.
		 */
		void operator()(LambdaArgs... lambdaArgs)
		{
			for (unsigned int i = 0; i < this->lambdaArray.GetSize(); i++)
			{
				Entry& entry = this->lambdaArray[i];
				entry.lambda(lambdaArgs...);
			}
		}

	private:

		struct Entry
		{
			LambdaType lambda;
			int handle;
		};

		DArray<Entry> lambdaArray;
		int nextHandle;
	};
}