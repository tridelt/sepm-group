/**
 * @file
 * @author  Julian Schrittwieser <j.schrittwieser@gmail.com>
 *
 * @section DESCRIPTION
 *
 * A simple logging framework, dumping all output to cout (for now).
 *
 * For easy logging, use one of three macros:
 *
 * `INFO("some debugging info")`
 *
 * `WARN("maybe something went wrong")`
 *
 * `ERROR("a severe problem")`
 */

#ifndef LOGGING_H
#define LOGGING_H

// strongly inspired by http://www.drdobbs.com/cpp/a-lightweight-logger-for-c/240147505?pgno=1

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <ctime>

using namespace std;

#define INFO logger.log<Logger::Severity::LVL_INFO>(__FILE__, __LINE__).print
#define WARN logger.log<Logger::Severity::LVL_WARN>(__FILE__, __LINE__).print
#define ERROR logger.log<Logger::Severity::LVL_ERROR>(__FILE__, __LINE__).print


class LogPrinter {
public:
  LogPrinter(function<void(string)> printer);
  LogPrinter(const LogPrinter &other);
  /**
   * main entry point to the logger, usually called by a macro
   *
   * this variadic function accepts and logs any number of arguments, it has to
   * be provided with the severity of the logging message as its first template
   * argument. Normally, it's not called directly - there are macros for the
   * different severity levels:
   *
   * `INFO("some debugging info")`
   *
   * `WARN("maybe something went wrong")`
   *
   * `ERROR("a severe problem")`
   */
  template< typename...Args >
  void print( Args...args );
private:
  void print_impl();
  template<typename First, typename...Rest >
  void print_impl(First parm1, Rest...parm);

  function<void(string)> printer;
  std::stringstream log_stream;
};


class Logger {
public:
  enum Severity {
    LVL_INFO = 1,
    LVL_WARN = 2,
    LVL_ERROR = 3
  };

  Logger();

  /**
   * set the current logging level. all messages with a lower severity level are
   * not displayed
   * @param l the new logging severity level
   */
  void logLevel(Severity l) { _logLevel = l; }

  /**
   * get the current logging level
   * @return current log level
   */
  Severity logLevel() { return _logLevel; }

  /**
   * sets if logging should be chatty. if it is, every log message will display
   * the source file and line it is coming from
   * @param shouldBeChatty the new chatty setting
   */
  void chatty(bool shouldBeChatty) { chatty = shouldBeChatty; }

  /**
   * get the current chatty state
   * @return true if currently chatty, false otherwise
   */
  bool chatty() { return chatty; }

  /**
   * intermediate function to handle source file and line information (template
   * arguments can't be strings, so have to pass the file information as a normal
   * function argument). For an explanation on how to log, see LogPrinter::print
   */
  template<Severity severity>
  LogPrinter log(string file, int line);

private:
  void write_out(string s, string file, int line);
  string get_logline_header(Severity severity);
  string time_str();

  boost::posix_time::ptime program_start;
  Severity _logLevel;
  bool chatty;
};


template< Logger::Severity severity>
LogPrinter Logger::log(string file, int line) {
  if(severity < _logLevel)
    return LogPrinter([](string){ });

  return LogPrinter([this, file, line](string s) {
    write_out(get_logline_header(severity) + s, file, line);
  });
}




template< typename...Args >
void LogPrinter::print( Args...args ) {
    print_impl( args... );
}

template<typename First, typename...Rest >
void LogPrinter::print_impl(First parm1, Rest...parm) {
    log_stream << parm1;
    print_impl(parm...);
}

static Logger logger;

#endif
