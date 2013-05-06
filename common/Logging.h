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
#include <boost/thread/tss.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <ctime>
#include <stdlib.h>

using namespace std;

#define INFO logger.log<Severity::LVL_INFO>(__FILE__, __LINE__).print
#define WARN logger.log<Severity::LVL_WARN>(__FILE__, __LINE__).print
#define ERROR logger.log<Severity::LVL_ERROR>(__FILE__, __LINE__).print

enum class Severity {
  LVL_INFO = 1,
  LVL_WARN = 2,
  LVL_ERROR = 3
};

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

class LogSink {
public:
  enum Color {
    RED,
    GREEN,
    BLUE,
    YELLOW
  };

  LogSink(bool chatty, Severity s) : _chatty(chatty), _logLevel(s) {
  }
  virtual ~LogSink() { }

  /**
   * Get the sinks chatty state, used in Logger::write_out to determine how much
   * to log to this sink
   * @return the chatty state - true means log more details
   */
  bool chatty() { return _chatty; }

  /**
   * accessor for the severity level of the sink - messages with lower severity
   * won't be logged to this sink
   * @return current severity
   */
  Severity severity() { return _logLevel; }

  /**
   * helper function to colorize logging messages. Sinks that support color can
   * override this to provide color markup.
   * @param  s     base string
   * @param  Color desired color
   * @return       base string, possibly with added color markup
   */
  virtual string color(string s, Color) { return s; }

  /**
   * function to actually write out log message - children must override this
   * @param s log message to write
   */
  virtual void write(string s) = 0;
protected:
  bool _chatty;
  Severity _logLevel;
};

class OStreamSink : public virtual LogSink {
public:
  /**
   * create a new sink to log to ostreams - this is used by default by the
   * Logger to log to cout. cerr and other streams are of course possible too.
   */
  OStreamSink(bool chatty, Severity l, ostream *s) : LogSink(chatty, l), stream(s) {
  }
  void write(string s) { *stream << s << endl; }
  /**
   * ostreams support color, so this adds the linux escape codes.
   * @param  s     base string
   * @param  Color desired color
   * @return       base string, possibly with added color markup
   */
  string color(string s, Color c);
protected:
  ostream *stream;
};

class FileSink : public virtual LogSink {
public:
  /**
   * creates a new FileSink for logging to a given file. New content is appended
   * to the end of the file, with 6 new lines added at the beginning.
   */
  FileSink(bool chatty, Severity l, string filename) : LogSink(chatty, l) {
    file.open(filename, ofstream::out | ofstream::app);
    file << "\n\n\n\n\n" << endl;
  }
  ~FileSink() { file.close(); }
  void write(string s) { file << s << endl; }
protected:
  ofstream file;
};


class Logger {
public:
  Logger();

  /**
   * intermediate function to handle source file and line information (template
   * arguments can't be strings, so have to pass the file information as a normal
   * function argument). For an explanation on how to log, see LogPrinter::print
   */
  template<Severity severity>
  LogPrinter log(string file, int line);

  /**
   * add a sink to the internal list. By default, only an OStreamSink for cout
   * is present.
   * @param s new sink
   */
  void addSink(LogSink *s);

  void registerThread() {
    boost::lock_guard<boost::mutex> lock(mutex);
    thread_name.reset(new string(string("Thread") + to_string(thread_counter++)));
  }

  void removeThread() {
    boost::lock_guard<boost::mutex> lock(mutex);
    if(thread_name.get() != NULL)
      delete thread_name.release();
  }

private:
  void write_out(string logLine, Severity s, string file, int line);
  string get_logline_header(bool chatty, Severity severity, LogSink *s);
  string time_str();

  boost::mutex mutex;
  boost::posix_time::ptime program_start;
  vector<LogSink*> sinks;
  boost::thread_specific_ptr<string> thread_name;
  int thread_counter;
};

template< Severity severity>
LogPrinter Logger::log(string file, int line) {
  return LogPrinter([this, file, line](string s) {
    write_out(s, severity, file, line);
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

extern Logger logger;

#endif
