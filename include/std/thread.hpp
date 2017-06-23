#pragma once

namespace std {
	class thread {
	public:

		template< class Function, class... Args > 
		explicit thread( Function&& f, Args&&... args );
			
	};

	// template< class Function, class... Args > 
	// explicit thread::thread( Function&& f, Args&&... args ) {
	// 	std::invoke(decay_copy(std::forward<Function>(f)), decay_copy(std::forward<Args>(args))...);
	// }
			
};
