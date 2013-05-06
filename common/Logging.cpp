#include "Logging.h"
#include "boost/filesystem.hpp"

namespace fs = boost::filesystem;

Logger logger;

Logger::Logger() : thread_counter(0) {
  program_start = boost::posix_time::microsec_clock::local_time();
  addSink(new OStreamSink(true, Severity::LVL_INFO, &cout));
  thread_name.reset(new string("MainThread"));
}

void Logger::addSink(LogSink *s) {
  sinks.push_back(s);
}

void Logger::write_out(string logLine, Severity s, string file, int line) {
  boost::lock_guard<boost::mutex> lock(mutex);
  std::stringstream fileLine;
  fileLine.str("");

  string thread = thread_name.get() == NULL ? "???thread" : *(thread_name.get());
  boost::filesystem::path path(file);
  fileLine << " (" << path.filename().string() << ":";
  fileLine << line << ", " << thread << ")";

  for(auto *sink : sinks) {
    // skip messages lower than the sinks severity level
    if(s < sink->severity())
      continue;

    sink->write(get_logline_header(sink->chatty(), s, sink) +
      logLine + (sink->chatty() ? fileLine.str() : ""));
  }
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

string Logger::get_logline_header(bool chatty, Severity severity, LogSink *s) {
  std::stringstream header;
  header.str("");
  header << time_str();

  if(chatty) {
    header << " - ";
    auto now = boost::posix_time::microsec_clock::local_time();
    auto elapsed = (now - program_start).total_milliseconds();
    header.fill(' ');
    header.width(7);
    header << elapsed << "ms ";
  }

  switch(severity) {
    case Severity::LVL_INFO:
      header << s->color("[INFO]  ", LogSink::GREEN);
      break;
    case Severity::LVL_WARN:
      header << s->color("[WARN]  ", LogSink::YELLOW);
      break;
    case Severity::LVL_ERROR:
      header << s->color("[ERROR] ", LogSink::RED);
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

string OStreamSink::color(string s, Color c) {
  string color_start = "";
  switch(c) {
    case RED:
      color_start = "\033[0;31m";
      break;
    case GREEN:
      color_start = "\033[0;32m";
      break;
    case BLUE:
      color_start = "\033[0;34m";
      break;
    case YELLOW:
      color_start = "\033[0;33m";
      break;
    default:
      ;
  }
  return color_start + s + "\033[0m";
}
