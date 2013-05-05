#ifndef LOGGING_H
#define LOGGING_H

// strongly inspired by http://www.drdobbs.com/cpp/a-lightweight-logger-for-c/240147505?pgno=1

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <ctime>

using namespace std;

#define INFO logger.print<Logger::Severity::LVL_INFO>
#define WARN logger.print<Logger::Severity::LVL_WARN>
#define ERROR logger.print<Logger::Severity::LVL_ERROR>

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
  template<Severity severity, typename...Args >
  void print( Args...args );
private:
  void print_impl();
  template<typename First, typename...Rest >
  void print_impl(First parm1, Rest...parm);
  void write_out(string s);
  string get_logline_header();
  string time_str();

  boost::mutex mutex;
  Severity _logLevel;
  std::stringstream log_stream;
  boost::posix_time::ptime program_start;
};

template< Logger::Severity severity , typename...Args >
void Logger::print( Args...args ) {
    boost::lock_guard<boost::mutex> lock(mutex);

    if(severity < _logLevel)
      return;

    switch(severity) {
        case Severity::LVL_INFO:
             log_stream<<"[INFO]  ";
             break;
        case Severity::LVL_WARN:
             log_stream<<"[WARN]  ";
             break;
        case Severity::LVL_ERROR:
             log_stream<<"[ERROR] ";
             break;
    };
    print_impl( args... );
}

template<typename First, typename...Rest >
void Logger::print_impl(First parm1, Rest...parm) {
    log_stream << parm1;
    print_impl(parm...);
}

static Logger logger;

#endif
