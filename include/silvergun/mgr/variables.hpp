/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(SLV_MGR_VARIABLES_HPP)
#define SLV_MGR_VARIABLES_HPP

#include <string>
#include <cstring>
#include <any>
#include <map>
#include <fstream>
#include <typeinfo>
#include <type_traits>

#include "silvergun/mgr/manager.hpp"

#include "silvergun/_debug/exceptions.hpp"
#include "silvergun/_globals/engine_time.hpp"

namespace slv::mgr {

/*!
 * \class variables
 * \brief Store game variables
 */
class variables final : private manager<variables> {
  private:
    variables() = default;
    ~variables() = default;

    //  Verify valid data type.
    template <typename T>
    inline static void verify(void) {
      static_assert(
        std::is_same<bool, T>::value ||
        std::is_same<int64_t, T>::value ||
        std::is_same<double, T>::value ||
        std::is_same<std::string, T>::value,
        "Variable must be a valid data type!"
      );
    };

    inline static std::string data_file_name = "game.cfg";  //  File to save variables to.
    inline static std::map<const std::string, std::any> _map;  //  Map of variables.

  public:
    /*!
     * \brief Set the file the game config variables will be written to.
     * \param fname Filename to set saving to.
     */
    static void set_data_file(const std::string& fname) { data_file_name = fname; };

    /*!
     * \brief Clear the current game config save.
     * \return False on fail, true on success.
     */
    static bool clear_save(void) {
      std::ofstream dfile(data_file_name, std::ofstream::trunc);
      if (!dfile.good()) return false;
      dfile.close();
      return true;
    };

    /*!
     * \brief Save a game config variable to file.
     * \tparam T Variable type.
     * \param var Variable name.
     * \return False on fail, true on success.
     */
    template <typename T>
    inline static bool save(
      const std::string& var
    ) {
      verify<T>();
      if (!isreg(var)) return false;
      std::ofstream dfile(data_file_name, std::ios::binary | std::ofstream::app);
      if (!dfile.good()) return false;

      try {
        T tempv = get<T>(var);

        //  Write variable name.
        {int32_t tempi = std::strlen(var.c_str()) + 1;
        dfile.write(reinterpret_cast<const char*>(&tempi), sizeof(int32_t));
        dfile.write(var.c_str(), tempi);}

        //  Write variable value.
        {int32_t tempi;
        if constexpr (std::is_same_v<std::string, T>)
          tempi = std::strlen(std::any_cast<const std::string>(tempv).c_str()) + 1;
        else tempi = sizeof(T);
        dfile.write(reinterpret_cast<const char*>(&tempi), sizeof(int32_t));
        if constexpr (std::is_same_v<std::string, T>)  //  Variable is a string.
          dfile.write(std::any_cast<const std::string>(tempv).c_str(), tempi);
        else  //  Handle simple variables.
          dfile.write(reinterpret_cast<const char*>(&tempv), tempi);}
      } catch(...) {
        dfile.close();
        return false;
      }

      dfile.close();
      return true;
    };

    /*!
     * \brief Load game config variables from file.
     * \tparam T Variable type.
     * \param var Variable name.
     * \return False on fail, true on success.
     */
    template <typename T>
    inline static bool load(
        const std::string& var
    ) {
        verify<T>();
        if (!isreg(var)) return false;
        std::ifstream dfile(data_file_name, std::ios::binary);
        if (!dfile.good()) return false;
        dfile.seekg(0, dfile.beg);

        while (true) {  //  Check entire file.
          try {
            if (dfile.peek() == EOF) return false;  //  Hit end of file and not found.
            std::string in_var;

            //  Read in variable name.
            {int32_t size;
            dfile.read(reinterpret_cast<char*>(&size), sizeof(int32_t));
            char* buffer = new char[size];
            dfile.read(buffer, size);
            in_var = std::string(buffer);
            delete[] buffer;}

            //  Read in variable size.
            int32_t size;
            dfile.read(reinterpret_cast<char*>(&size), sizeof(int32_t));
            if (var == in_var) {  //  Found variable.
              if constexpr (std::is_same_v<std::string, T>) {
                //  Variable is a string, read accordingly.
                char* buffer = new char[size];
                dfile.read(buffer, size);
                std::string in_val = std::string(buffer);
                delete[] buffer;
                set<std::string>(var, in_val);
              } else {
                //  Handle other data types.
                T in_val;
                dfile.read(reinterpret_cast<char*>(&in_val), size);
                set<T>(var, in_val);
              }
              break;  //  Found value, break loop.
            }
            dfile.seekg(size, dfile.cur);  //  Skip if not correct variable.
          } catch(...) {  //  Error reading, fail.
            dfile.close();
            return false;
          }
        }

        dfile.close();
        return true;
    };

    /*!
     * \brief Create a new entry in the map.
     * \tparam T Variable type.
     * \param var Variable name.
     * \param val Variable starting value.
     * \return False on fail, true on success.
     */
    template <typename T>
    inline static bool reg(
      const std::string& var,
      const T& val
    ) {
      verify<T>();
      auto ret = _map.insert(std::make_pair(var, std::make_any<T>(val)));
      return ret.second;
    };

    /*!
     * \brief Check if a variable is registered.
     * \param var Variable name.
     * \return True if it is, false if not.
     */
    static bool isreg(const std::string& var) {
      auto it = _map.find(var);
      if (it != _map.end()) return true;
      return false;
    };

    /*!
     * \brief Delete a variable.
     * \param var Variable name.
     * \return True if deleted, false if not.
     */
    static bool del(const std::string& var) {
      auto it = _map.find(var);
      if (it != _map.end()) {
        _map.erase(it);
        return true;
      }
      return false;
    };

    /*!
     * \brief Set a variable's value.
     * \tparam T Variable type.
     * \param var Variable name.
     * \param val New value.
     */
    template <typename T>
    inline static void set(
      const std::string& var,
      const T& val
    ) {
      verify<T>();
      try {
        _map.at(var) = std::make_any<T>(val);
      } catch(std::out_of_range& e) {
        std::string err_msg = "Could not set variable: " + var;
        throw engine_exception(err_msg.c_str(), "variables", engine_time::check());
      }
    };

    /*!
     * \brief Get a variable's value.
     * \tparam T Variable type.
     * \param var Variable name.
     * \return The variable's current value.
     */
    template <typename T>
    inline static const T get(
      const std::string& var
    ) {
      verify<T>();
      try {
        return std::any_cast<const T>(_map.at(var));
      } catch(std::out_of_range& e) {
        std::string err_msg = "Could not get variable: " + var;
        throw engine_exception(err_msg.c_str(), "variables", engine_time::check());
      } catch(std::bad_any_cast& e) {
        std::string err_msg = "Error reading variable: " + var;
        throw engine_exception(err_msg.c_str(), "variables", engine_time::check());
      }
    };
};

template <> bool manager<variables>::initialized = false;

}

#endif
