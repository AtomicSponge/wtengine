/*
  WTEngine
  By:  Matthew Evans
  File:  make_thread.hpp

  See LICENSE.txt for copyright information

  Wrapper class for creating a threaded object
*/

#ifndef WTE_MAKE_THREAD_HPP
#define WTE_MAKE_THREAD_HPP

#include <thread>
#include <future>
#include <chrono>

namespace wte
{

class make_thread {
    public:
        inline virtual ~make_thread() { stop(); };

        make_thread(const make_thread&) = delete;
        void operator=(make_thread const&) = delete;

        inline void start(void) {
            if(started == true) return;
            future_obj = exit_signal.get_future();
            std::thread th([&]() { run(); });
            th.detach();
            started = true;
        };

        inline void stop(void) {
            exit_signal.set_value();
            started = false;
            exit_signal = std::promise<void>();
        }

    private:
        std::promise<void> exit_signal;
        std::future<void> future_obj;

        bool started;

    protected:
        inline make_thread() : started(false) {};

        inline bool is_running(void) {
            if(future_obj.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
                return true;
            return false;
        }

        virtual void run() = 0;
};

} //  end namespace wte

#endif
