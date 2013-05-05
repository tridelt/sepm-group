#include "Logging.h"


Logger::Logger() : _logLevel(Logger::Severity::LVL_INFO) {
  program_start = boost::posix_time::microsec_clock::local_time();
}



void Logger::print_impl() {
    write_out( get_logline_header() + log_stream.str() );
    log_stream.str("");
}


void Logger::write_out(string s) {
  cout << s << endl;
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


string Logger::get_logline_header() {
  std::stringstream header;
  header.str("");
  header <<  time_str() << " - ";

  auto now = boost::posix_time::microsec_clock::local_time();
  auto elapsed = (now - program_start).total_milliseconds();
  header.fill(' ');
  header.width(7);
  header << elapsed << "ms ";
  return header.str();
}
