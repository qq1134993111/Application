/**
 * @file   anyfunction.hpp
 * @author Sébastien Rouault <sebmsg@free.fr>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version. Please see https://gnu.org/licenses/gpl.html
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * @section DESCRIPTION
 *
 * A replacement for std::function, with hybrid memory allocation (internal storage/heap) for the closure.
**/

#pragma once
// ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
// ▁ Declarations ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔

// External headers
#include <cstddef>
#include <cstdint>
#include <exception>
#include <type_traits>
#include <utility>

// ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔ Declarations ▔
// ▁ Exceptions ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔

namespace AnyFunction {
namespace Exception {

// ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――

/** Defines an exception.
 * @param name   Exception name
 * @param parent Parent exception (use ::std::exception as the root)
 * @param text   Explanatory string
**/
#define EXCEPTION(name, parent, text) \
    class name: public parent { \
    public: \
        /** Return the explanatory string. \
         * @return Explanatory string \
        **/ \
        virtual char const* what() const noexcept { \
            return "anyfunction: " text; \
        } \
    }

/** Exceptions tree.
**/
EXCEPTION(Any, ::std::exception, "exception");
    EXCEPTION(Empty, Any, "no function to call");

#undef EXCEPTION

// ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――

}
}

// ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔ Exceptions ▔
// ▁ Function object holder class ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔

namespace AnyFunction {

// ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――

/** Function object holder template class declaration.
**/
template<class Any, size_t local_storage_size = 32> class Function;

/** Check if a given class instance is an instance of 'Function' class template.
 * @param Type Type to identify
**/
template<class Type> class is_function_holder: public ::std::false_type {};
template<class Return, class... Args, size_t local_storage_size> class is_function_holder<Function<Return(Args...), local_storage_size>>: public ::std::true_type {};

/** Enable template overload only if the given type is not a 'Function' class template instance.
 * @param Type Type to decay then identify
**/
template<class Type> using enable_if_not_function_holder = typename ::std::enable_if<!is_function_holder<typename ::std::decay<Type>::type>::value>::type;

// ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――

/** Functor specialized invoker.
 * @param Functor  Actual functor class
 * @param Return   Return type
 * @param Args...  Argument types
 * @param instance Functor instance
 * @param ...      Arguments to forward
 * @return Functor return value
**/
template<class Functor, class Return, class... Args> static Return specialized_invoker(void* instance, Args... args) {
    return (*reinterpret_cast<Functor*>(instance))(::std::forward<Args>(args)...);
}

/** Functor manager commands.
**/
enum class Command {
    copy_allocate,  // Allocate and copy constructor
    copy_construct, // Copy constructor
    move_allocate,  // Allocate and move constructor
    move_construct, // Move constructor
    destroy, // Destroy instance
    free,    // Delete instance
    type_size, // Get instance size
    type_align // Get instance alignment
};

/** Function manager return value.
**/
union ManagerReturn {
public:
    void*  ptr;
    size_t value;
public:
    /** Pointer constructor.
     * @param ptr Any pointer, or nullptr
    **/
    ManagerReturn(::std::nullptr_t): ptr(nullptr) {}
    ManagerReturn(void* ptr): ptr(ptr) {}
    /** Value constructor.
     * @param ptr Any pointer
    **/
    ManagerReturn(size_t value): value(value) {}
};

/** Functor specialized manager.
 * @param Functor  Actual functor class
 * @param instance Functor instance
 * @param command  Command to execute
 * @param other    Other optional instance
 * @return Optional pointer or value
**/
template<class Functor> static ManagerReturn specialized_manager(void* instance, Command command, void* other) {
    switch (command) {
        case Command::copy_allocate:
            return new Functor(*reinterpret_cast<Functor const*>(other));
        case Command::copy_construct:
            new(instance) Functor(*reinterpret_cast<Functor const*>(other));
            break;
        case Command::move_allocate:
            return new Functor(::std::move(*reinterpret_cast<Functor*>(other)));
        case Command::move_construct:
            new(instance) Functor(::std::move(*reinterpret_cast<Functor*>(other)));
            break;
        case Command::destroy:
            reinterpret_cast<Functor*>(instance)->~Functor();
            break;
        case Command::free:
            delete reinterpret_cast<Functor*>(instance);
            break;
        case Command::type_size:
            return sizeof(Functor);
        case Command::type_align:
            return alignof(Functor);
    }
    return nullptr;
}

// ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――

/** Possible holder status.
**/
enum class Status {
    invalid,    // No functor
    standalone, // Standalone function (no storage)
    local,      // Locally-stored functor
    remote      // Heap-stored functor
};

/** Function object holder template class.
 * @param Return(Args...)    Expected function type.
 * @param local_storage_size Size reserved for the local storage (in bytes, optional)
**/
template<class Return, class... Args, size_t local_storage_size> class Function<Return(Args...), local_storage_size> {
    template<class, size_t> friend class Function;
protected:
    /** Types of function/helpers used.
    **/
    using Standalone = Return (*)(Args...);
    using Invoker = Return (*)(void*, Args...);
    using Manager = ManagerReturn (*)(void*, Command, void*);
protected:
    Status status; // Holder status
    union {
        struct {
            Invoker invoker; // Functor invoker function
            Manager manager; // Functor manager function
            void* instance; // Functor instance
            uint8_t storage[local_storage_size]; // Local storage
        };
        Standalone function; // Standalone function
    };
private:
    /** For functors, try "allocating" structure on local storage.
     * @param Functor/manager Actual functor class (so not a forwarding reference)/specialized manager to use
     * @return Pointer to the local storage if fits, nullptr otherwise
    **/
    void* do_local_alloc(size_t size, size_t align) const noexcept {
        uintptr_t local = reinterpret_cast<uintptr_t>(storage); // Local storage address
        uintptr_t delta = (-(local % align)) % align; // Delta for alignment (works since alignof(Functor) divides 2^(8*sizeof(uintptr_t)))
        if (size + delta > local_storage_size) // Do not fit into local storage
            return nullptr;
        return reinterpret_cast<void*>(local + delta);
    }
    template<class Functor> void* local_alloc() const noexcept {
        return do_local_alloc(sizeof(Functor), alignof(Functor));
    }
    void* local_alloc(Manager manager) const noexcept {
        return do_local_alloc(manager(nullptr, Command::type_size, nullptr).value, manager(nullptr, Command::type_align, nullptr).value);
    }
protected:
    /** Copy functor via manager.
     * @param func Functor holder to copy
    **/
    template<size_t other_storage_size> void via_manager(Function<Return(Args...), other_storage_size> const& func) {
        switch (func.status) {
            case Status::invalid:
                break;
            case Status::standalone:
                function = func.function;
                status = Status::standalone;
                break;
            case Status::local:
            case Status::remote: {
                auto ptr = local_alloc(func.manager);
                if (ptr) { // Local allocation done
                    func.manager(ptr, Command::copy_construct, func.instance); // Can throw
                    instance = ptr;
                    status = Status::local;
                } else { // Heap allocation to do
                    instance = func.manager(nullptr, Command::copy_allocate, func.instance).ptr; // Can throw
                    status = Status::remote;
                }
                invoker = func.invoker;
                manager = func.manager;
            } break;
        }
    }
    /** Move functor via manager.
     * @param func Functor holder to move
    **/
    template<size_t other_storage_size> void via_manager(Function<Return(Args...), other_storage_size>&& func) {
        switch (func.status) {
            case Status::invalid:
                break;
            case Status::standalone:
                function = func.function;
                status = Status::standalone;
                func.status = Status::invalid; // Other function holder is then invalid (for consistency with other status)
                break;
            case Status::local: {
                auto ptr = local_alloc(func.manager);
                if (ptr) { // Local allocation done
                    func.manager(ptr, Command::move_construct, func.instance); // Can throw
                    instance = ptr;
                    status = Status::local;
                } else { // Heap allocation to do
                    instance = func.manager(nullptr, Command::move_allocate, func.instance).ptr; // Can throw
                    status = Status::remote;
                }
                invoker = func.invoker;
                manager = func.manager;
                func.clear(); // Other function holder is then invalid
            } break;
            case Status::remote: { // Just take over instance
                instance = func.instance;
                status = Status::remote;
                invoker = func.invoker;
                manager = func.manager;
                func.status = Status::invalid; // Other function holder is then invalid
            } break;
        }
    }
    /** Copy/move functor via class.
     * @param functor Functor to copy/move
    **/
    template<class Type> void via_class(Type&& functor) {
        using Functor = typename ::std::decay<Type>::type;
        { // Check functor callability
            using ResultOf = typename ::std::result_of<Functor(Args...)>::type; // Since C++14, not defined if function can not be called with the arguments
            static_assert(::std::is_same<Return, ResultOf>::value || ::std::is_base_of<Return, ResultOf>::value, "'Functor' result type is incompatible"); // '::std::is_same' needed for fundamental types
        }
        auto ptr = local_alloc<Functor>();
        if (ptr) { // Local allocation done
            new(ptr) Functor(::std::forward<Type>(functor)); // Can throw
            instance = ptr;
            status = Status::local;
        } else { // Heap allocation to do
            instance = new Functor(::std::forward<Type>(functor)); // Can throw
            status = Status::remote;
        }
        invoker = specialized_invoker<Functor, Return, Args...>;
        manager = specialized_manager<Functor>;
    }
public:
    /** No functor constructor/assignment.
     * @return Current instance
    **/
    Function() noexcept: status(Status::invalid) {}
    Function(::std::nullptr_t) noexcept(noexcept(Function())): Function() {}
    Function& operator=(::std::nullptr_t) {
        clear();
        return *this;
    }
    /** Function holder copy constructor/assignment.
     * @param func Function holder to copy
     * @return Current instance
    **/
    Function(Function<Return(Args...), local_storage_size> const& func): status(Status::invalid) {
        via_manager(func);
    }
    template<size_t other_storage_size> Function(Function<Return(Args...), other_storage_size> const& func): status(Status::invalid) {
        via_manager(func);
    }
    Function& operator=(Function<Return(Args...), local_storage_size> const& func) {
        clear();
        via_manager(func);
        return *this;
    }
    template<size_t other_storage_size> Function& operator=(Function<Return(Args...), other_storage_size> const& func) {
        clear();
        via_manager(func);
        return *this;
    }
    /** Function holder move constructor/assignment.
     * @param func Function holder to move; if no exception occurs, gets invalidated, otherwise left untouched by the holder (so actual exception safety only depends on the functor itself)
     * @return Current instance
    **/
    Function(Function<Return(Args...), local_storage_size>&& func): status(Status::invalid) {
        via_manager(::std::move(func));
    }
    template<size_t other_storage_size> Function(Function<Return(Args...), other_storage_size>&& func): status(Status::invalid) {
        via_manager(::std::move(func));
    }
    Function& operator=(Function<Return(Args...), local_storage_size>&& func) {
        clear();
        via_manager(::std::move(func));
        return *this;
    }
    template<size_t other_storage_size> Function& operator=(Function<Return(Args...), other_storage_size>&& func) {
        clear();
        via_manager(::std::move(func));
        return *this;
    }
    /** Standalone function constructor/assignment.
     * @param func Standalone function
     * @return Current instance
    **/
    Function(Standalone func) noexcept: status(Status::standalone), function(func) {}
    Function& operator=(Standalone func) {
        clear();
        function = func;
        status = Status::standalone;
        return *this;
    }
    /** Functor copy/move constructor/assignment.
     * @param functor Function instance to copy/move
     * @return Current instance
    **/
    template<class Functor, class = enable_if_not_function_holder<Functor>> Function(Functor&& functor): status(Status::invalid) {
        via_class(::std::forward<Functor>(functor));
    }
    template<class Functor, class = enable_if_not_function_holder<Functor>> Function& operator=(Functor&& functor) {
        clear();
        via_class(::std::forward<Functor>(functor));
        return *this;
    }
    /** Clear destructor.
    **/
    ~Function() {
        clear();
    }
public:
    /** Tell whether a functor is held, and so is callable.
     * @return True if held a functor, false otherwise
    **/
    operator bool() const noexcept {
        return status != Status::invalid;
    }
    /** Call the held function with the given parameters.
     * @param ... Arguments to pass to the function
     * @return Return value of the function
    **/
    Return operator()(Args... args) {
        switch (status) {
            case Status::invalid:
                throw Exception::Empty();
            case Status::standalone:
                return function(::std::forward<Args>(args)...);
            case Status::local:
            case Status::remote:
                return invoker(instance, ::std::forward<Args>(args)...);
        }
    }
    /** Clear the functor holder to the no-functor status.
    **/
    void clear() {
        switch (status) {
            case Status::invalid:
                break;
            case Status::standalone:
                status = Status::invalid;
                break;
            case Status::local:
                manager(instance, Command::destroy, nullptr); // Destroy instance (can throw exception)
                status = Status::invalid; // Must happen after destruction, so actual exception safety depends on the functor itself
                break;
            case Status::remote:
                manager(instance, Command::free, nullptr); // Delete instance (can throw exception)
                status = Status::invalid; // Must happen after destruction, so actual exception safety depends on the functor itself
                break;
        }
    }
};

// ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――

}

// ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔ ... ▔
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔
