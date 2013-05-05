#include "Logging.h"
#include "boost/filesystem.hpp"

namespace fs = boost::filesystem;

Logger::Logger() : _logLevel(Logger::Severity::LVL_INFO),
 chatty(true) {
  program_start = boost::posix_time::microsec_clock::local_time();
}

void Logger::write_out(string s, string file, int line) {
  cout << s;
  if(chatty) {
    boost::filesystem::path path(file);
    cout << " (" << path.filename().string() << ":" << line << ")";
  }
  cout << endl;
}

string Logger::time_str() {
  char buffer[80];

  std::string time_str;
  time_t raw_time;
  time( & raw_time );
  auto timeinfo = localtime (&raw_time);
  strftime(buffer, 80, "%Y-%m-%d %H:%M.%S", timeinfo);
  //without the newline character
  return string(buffer);
}

string Logger::get_logline_header(Logger::Severity severity) {

  std::stringstream header;
  header.str("");
  header << time_str() << " - ";

  auto now = boost::posix_time::microsec_clock::local_time();
  auto elapsed = (now - program_start).total_milliseconds();
  header.fill(' ');
  header.width(7);
  header << elapsed << "ms ";

  switch(severity) {
      case Severity::LVL_INFO:
           header<<"[INFO]  ";
           break;
      case Severity::LVL_WARN:
           header<<"[WARN]  ";
           break;
      case Severity::LVL_ERROR:
           header<<"[ERROR] ";
           break;
  };

  return header.str();
}


LogPrinter::LogPrinter(function<void(string)> printer) :
  printer(printer) {
}

LogPrinter::LogPrinter(const LogPrinter &other) :
  printer(other.printer) {
}


void LogPrinter::print_impl() {
    printer(log_stream.str());
}
