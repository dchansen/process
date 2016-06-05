// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008 Ilya Sokolov, Boris Schaeling
// Copyright (c) 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
// Copyright (c) 2011, 2012 Jeff Flinn, Boris Schaeling
// Copyright (c) 2016 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/**
 * \file boost/process/child.hpp
 *
 * Defines a child process class.
 */

#ifndef BOOST_PROCESS_CHILD_HPP
#define BOOST_PROCESS_CHILD_HPP

#include <boost/process/detail/config.hpp>
#include <boost/process/detail/child_decl.hpp>
#include <boost/process/detail/execute_impl.hpp>

namespace boost {

namespace process {

template<typename ...Args>
child::child(Args&&...args)
	: child(detail::execute_impl(std::forward<Args>(args)...)) {}

/** Launch a process and detach it. Returns no handle.
 *
 */
template<typename ...Args>
inline void spawn(Args && ...args)
{
	child c(std::forward<Args>(args)...);
	c.detach();
}

/** Launches a process and waits for its exit. Similar to std::system. */
template<typename ...Args>
inline int system(Args && ...args)
{
	child c(std::forward<Args>(args)...);
	c.wait();
	return c.exit_code();
}

#if defined(BOOST_PROCESS_DOXYGEN)
/* The main class to hold a child process. It is simliar to std::thread,
 * in that it has a join and detach function.
 *
 * @attention The destructor will call terminate on the process if not joined or detached without any warning.
 *
 */

class child
{
	/** Type definition for the native process handle. */
    typedef platform_specific native_handle_t;

    /** Construct the child from a pid.
     *
     * @attention There is no guarantee that this will work. The process need the right access rights, which are very platform specific.
     */
    explicit child(pid_t & pid) : _child_handle(pid) {};

    /** Move-Constructor. */
    child(child && lhs);

    /** Construct a child from a property list and launch it */
    template<typename ...Args>
    explicit child(Args&&...args);

    /** Construct an empty child. */
    child() = default;

    /** Move assign. */
    child& operator=(child && lhs);

    /** Detach the child, i.e. let it run after this handle dies. */
    void detach();
    /** Join the child. This just calls wait, but that way the naming is similar to std::thread */
    void join();
    /** Check if the child is joinable. */
    bool joinable();

    /** Destructor.
     * @attention Will call terminate (without warnign) when the child was neither joined nor detached.
     */
    ~child():

	/** Get the native handle for the child process. */
    native_handle_t native_handle() const;

    /** Get the exit_code. The return value is without any meaning if the child wasn't waited for. */
    int exit_code() const;
    /** Get the Process Identifier. */
    pid_t id()      const;

    /** Check if the child process is running. */
    bool running();
    /** Terminate the child process. */
    void terminate();

    /** Wait for the child process to exit. */
    void wait();

    /** Wait for the child process to exit for a period of time. */
    template< class Rep, class Period >
    bool wait_for  (const std::chrono::duration<Rep, Period>& rel_time);

    /** Wait for the child process to exit until a point in time. */
    template< class Clock, class Duration >
    bool wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time );

    /** Check if this handle holds a child process.
     * @note That does not mean, that the process is still running. It only means, that the handle does or did exist.
     */
    bool valid() const;
    /** Same as valid, for convenience. */
    explicit operator bool() const;

    /** Check if the the chlid process is in any process group. */
    bool in_group() const;
};

#endif

}}
#endif

