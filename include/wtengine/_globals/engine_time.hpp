/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_ENGINE_TIME_HPP)
#define WTE_ENGINE_TIME_HPP

#include <cstdint>

namespace wte {

/*!
 * \class engine_time
 * \brief Tracks current time of the engine.
 */
class engine_time final {
  friend class engine;

  private:
    //  Sets the internal timer. Called internally by engine.
    static void set(const int64_t& t) {  current_time = t; };
    inline static int64_t current_time = 0;  //  Track game timer

  public:
    engine_time() = delete;                       //  Delete constructor.
    ~engine_time() = delete;                      //  Delete destructor.
    engine_time(const engine_time&) = delete;     //  Delete copy constructor.
    void operator=(engine_time const&) = delete;  //  Delete assignment operator.

    /*!
     * \brief Check the internal engine timer.
     * \return Timer value.
     */
    static int64_t check(void) { return current_time; };
};

}  //  end namespace wte

#endif
