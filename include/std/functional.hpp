#pragma once

#include <std/type_traits.hpp>
 
// template< class F, class... Args>
// std::invoke_result_t<F, Args...> invoke(F&& f, Args&&... args) 
//   noexcept(std::is_nothrow_invocable_v<F, Args...>)
// {
//     return detail::INVOKE(std::forward<F>(f), std::forward<Args>(args)...);
// }