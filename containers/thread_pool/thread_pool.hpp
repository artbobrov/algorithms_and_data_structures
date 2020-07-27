#pragma once

#include <utility>
#include <vector>
#include <type_traits>
#include <deque>
#include <functional>
#include <future>
#include <memory>
#include <atomic>
#include <optional>
#include <unordered_map>


namespace utils::detail::wrapper {
    template<typename T>
    struct rvalue_wrapper {
        template<typename=std::enable_if_t<std::is_rvalue_reference_v<T &&>>>

        explicit rvalue_wrapper(T &&t) noexcept :object(std::move(t)) {}

        rvalue_wrapper(const rvalue_wrapper &rhs) = default;

        rvalue_wrapper(rvalue_wrapper &&rhs) noexcept = default;

        rvalue_wrapper &operator=(const rvalue_wrapper &rhs) = default;

        rvalue_wrapper &operator=(rvalue_wrapper &&rhs) noexcept = default;

        template<class ...Args>
        T &&operator()(Args ...) {
            return std::move(object);
        }

    private:
        T object;
    };
}


namespace std {
    template<typename T>
    struct is_bind_expression<utils::detail::wrapper::rvalue_wrapper<T>> : std::true_type {
    };
}

namespace utils {
    struct cancelation_exception : std::runtime_error {
        explicit cancelation_exception(const std::string &string) : runtime_error(string) {}
    };

    struct shutdown_exception : std::runtime_error {
        explicit shutdown_exception(const std::string &string) : runtime_error(string) {}
    };

    struct recursion_found_exception : std::runtime_error {
        explicit recursion_found_exception(const std::string &string) : runtime_error(string) {}
    };

    struct thread_pool;

    bool is_executing_in_pool(thread_pool &pool);

    namespace detail {
        template<typename T,
                typename = std::enable_if_t<std::is_rvalue_reference_v<T &&>>>
        wrapper::rvalue_wrapper<T> wrap(T &&t) {
            return wrapper::rvalue_wrapper<T>(std::forward<T>(t));
        }

        template<typename T,
                typename = std::enable_if_t<!std::is_rvalue_reference_v<T &&>>>
        auto wrap(T &&t) {
            return std::move(t);
        }

        template<class F, class ...Args>
        auto build_function(F &&function, Args &&... args) {
            return std::bind(std::forward<F>(function), detail::wrap(std::forward<Args>(args))...);
        }

        template<class R>
        struct base_manager {
            base_manager() = default;

            [[nodiscard]] bool is_running() const { return _is_running.load(); }

            [[nodiscard]] bool is_done() const { return _is_done.load(); }

            [[nodiscard]] bool is_canceled() const { return _is_canceled.load(); }

            void set_running(bool flag) const { _is_running = flag; }

            void set_done(bool flag) const { _is_done = flag; }

            void set_canceled(bool flag) const { _is_canceled = flag; }

            void set_future(std::future<R> &&future) {
                _future = std::move(future);
            }

        protected:
            mutable std::atomic_bool _is_canceled = false;
            mutable std::atomic_bool _is_running = false;
            mutable std::atomic_bool _is_done = false;

            mutable std::future<R> _future;
            mutable std::mutex _mutex;
        };

        template<class R>
        struct manager : base_manager<R> {
            manager() = default;

            explicit manager(
                    thread_pool *pool,
                    std::shared_ptr<std::packaged_task<R()>> packaged_task
            ) : _pool(pool), _packaged_task(packaged_task) {}

            explicit manager(R value) : _return_value(std::move(value)) {}

            R get_value() const {
                if (this->is_canceled())
                    throw cancelation_exception("");
                std::lock_guard<std::mutex> _lock(this->_mutex);

                if (_packaged_task && !this->is_done() && !this->is_running() && is_executing_in_pool(*_pool)) {
                    this->set_running(true);
                    (*_packaged_task)();
                    this->set_done(true);
                    this->set_running(false);
                }

                if (_return_value)
                    return *_return_value;

                _return_value = this->_future.get();
                return *_return_value;
            }

            void completed() const {
                if (_completion)
                    _completion(get_value());
            }

            void set_completion(const std::function<void(R)> &completion) {
                std::lock_guard<std::mutex> _lock(this->_mutex);
                _completion = completion;
            }

        protected:
            thread_pool *_pool = nullptr;
            mutable std::optional<R> _return_value;
            std::function<void(R)> _completion;
            std::shared_ptr<std::packaged_task<R()>> _packaged_task;
        };

        template<>
        struct manager<void> : base_manager<void> {
            void completed() {
                if (_completion)
                    _completion();
            }

            void set_completion(const std::function<void()> &completion) {
                _completion = completion;
            }

        protected:
            std::function<void()> _completion;
        };

        template<typename R>
        struct base_task {
            explicit base_task(
                    std::shared_ptr<detail::manager<R>> manager
            ) : _manager(std::move(manager)) {}

            base_task(const base_task &) = default;

            base_task(base_task &&) noexcept = default;

            base_task &operator=(base_task other) {
                swap(other);
                return *this;
            }

            void swap(base_task &other) { _manager.swap(other._manager); }

            void cancel() noexcept {
                _manager->set_canceled(true);
            }

            [[nodiscard]] bool is_running() const { return _manager->is_running(); }

            [[nodiscard]] bool is_done() const { return _manager->is_done(); }

            [[nodiscard]] bool is_canceled() const { return _manager->is_canceled(); }

            R get() const {
                return _manager->get_value();
            }

        protected:
            std::shared_ptr<detail::manager<R>> _manager;
        };

    }

    template<typename R>
    struct task final : detail::base_task<R> {
        task(
                std::shared_ptr<detail::manager<R>> manager
        ) : detail::base_task<R>(std::move(manager)) {}

        void invoke_on_completion(const std::function<void(R)> &completion) {
            this->_manager->set_completion(completion);
            if (this->_manager->is_done())
                try {
                    this->_manager->completed();
                } catch (...) {}
        }
    };

    template<>
    struct task<void> final : detail::base_task<void> {
        task(std::shared_ptr<detail::manager<void>> manager) : detail::base_task<void>(std::move(manager)) {}

        void invoke_on_completion(const std::function<void()> &completion) {
            this->_manager->set_completion(completion);
            if (this->_manager->is_done())
                try {
                    this->_manager->completed();
                } catch (...) {}
        }
    };

    struct thread_pool final {
        friend bool is_executing_in_pool(thread_pool &pool) {
            return pool._workers.find(std::this_thread::get_id()) != pool._workers.end();
        }

        template<typename T>
        using task_t = task<T>;


        explicit thread_pool(size_t thread_count = std::thread::hardware_concurrency()) {
            _workers.reserve(thread_count);

            for (size_t i = 0; i < thread_count; ++i) {
                executable closure = [&]() {
                    while (true) {
                        executable exec;
                        {
                            std::unique_lock _lock(_mutex);
                            _cv.wait(_lock, [&]() {
                                return !_tasks.empty() || _is_stopped.load();
                            });
                            if (_tasks.empty() && _is_stopped.load())
                                return;
                            exec = _tasks.back();
                            _tasks.pop_back();
                        }
                        exec();
                    }
                };
                std::thread thread(closure);
                _workers.insert({thread.get_id(), std::move(thread)});
            }
        }

        thread_pool(const thread_pool &) = delete;

        thread_pool &operator=(const thread_pool &) = delete;

        thread_pool(thread_pool &&) = delete;

        thread_pool &operator=(const thread_pool &&) = delete;

        ~thread_pool() {
            _is_stopped.store(true);
            _cv.notify_all();
            for (auto &thread:_workers)
                thread.second.join();
        }

        template<typename F, typename ...Args>
        void enqueue(F &&function, Args &&... args) {
            submit(std::forward<F>(function), std::forward<Args>(args)...);
        }

        template<typename F, typename ... Args>
        task<std::result_of_t<F(Args...)>> submit(F &&function, Args &&... args) {
            using return_t = std::result_of_t<F(Args...)>;
            if (this->_is_stopped)
                throw shutdown_exception("");
            if (is_executing_in_pool(*this)) {
                return sync_task<return_t>(std::forward<F>(function), std::forward<Args>(args)...);
            }

            auto closure = detail::build_function(std::forward<F>(function), std::forward<Args>(args)...);
            auto rw = make_reader_writer(std::move(closure));
            auto &[exec, manager] = rw;
            {
                std::lock_guard _lock(_mutex);
                _tasks.push_back(exec);
            }
            _cv.notify_one();
            return task<return_t>{manager};
        }

        [[nodiscard]] size_t threads_count() const { return _workers.size(); }

        [[nodiscard]] size_t remaining_tasks() const { return _tasks.size(); }

    private:
        using executable = std::function<void()>;

        template<typename F, typename R = std::result_of_t<F()>>
        std::enable_if_t<!std::is_void_v<R>, std::tuple<executable, std::shared_ptr<detail::manager<R>>>>
        make_reader_writer(F &&function) {
            using manager_t = detail::manager<R>;
            using packaged_task_t = std::packaged_task<R()>;
            std::shared_ptr<packaged_task_t> packaged_task = std::make_shared<packaged_task_t>(
                    std::forward<F>(function)
            );
            std::shared_ptr<manager_t> manager = std::make_shared<manager_t>(this, packaged_task);
            manager->set_future(packaged_task->get_future());
            executable exec = [packaged_task, manager]() {
                if (manager->is_canceled() || manager->is_done())
                    return;
                manager->set_running(true);
                (*packaged_task)();
                try {
                    manager->completed();
                } catch (...) {}
                manager->set_done(true);
                manager->set_running(false);
            };

            return std::make_tuple(exec, manager);
        }

        template<typename F, typename R = std::result_of_t<F()>>
        std::enable_if_t<std::is_void_v<R>, std::tuple<executable, std::shared_ptr<detail::manager<void>>>>
        make_reader_writer(F &&function) {
            using manager_t = detail::manager<void>;
            using packaged_task_t = std::packaged_task<void()>;
            std::shared_ptr<packaged_task_t> packaged_task = std::make_shared<packaged_task_t>(
                    std::forward<F>(function)
            );
            std::shared_ptr<manager_t> manager = std::make_shared<manager_t>();
            manager->set_future(packaged_task->get_future());
            executable exec = [packaged_task, manager]() {
                if (manager->is_canceled() || manager->is_done())
                    return;
                manager->set_running(true);
                (*packaged_task)();
                try {
                    manager->completed();
                } catch (...) {}
                manager->set_done(true);
                manager->set_running(false);
            };

            return std::make_tuple(exec, manager);
        }

        template<class R, class F, class ...Args>
        std::enable_if_t<!std::is_void_v<R>, task<R>> sync_task(F &&function, Args... args) {
            using manager_t = detail::manager<R>;
            R result = std::invoke(std::forward<F>(function), std::forward<Args>(args)...);
            std::shared_ptr<manager_t> manager = std::make_shared<manager_t>(result);
            return task<R>{manager};
        }


        template<class R, class F, class ...Args>
        std::enable_if_t<std::is_void_v<R>, task<R>> sync_task(F &&function, Args... args) {
            using manager_t = detail::manager<R>;
            std::invoke(std::forward<F>(function), std::forward<Args>(args)...);
            std::shared_ptr<manager_t> manager = std::make_shared<manager_t>();
            return task<R>{manager};
        }

        std::unordered_map<std::thread::id, std::thread> _workers;
        std::deque<executable> _tasks;
        std::condition_variable _cv;
        std::mutex _mutex;
        std::atomic_bool _is_stopped = false;
    };


}

