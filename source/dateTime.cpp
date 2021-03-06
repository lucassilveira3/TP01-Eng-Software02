#include "dateTime.hpp"
#include <iomanip>
#include <sstream>

#define TIME_BUFFER_SIZE 30

DateTime DateTime::now() {
    return DateTime(time(NULL));
}

DateTime::DateTime(time_t time_stamp) {
    time_stamp_ = time_stamp;
    time_data_ = *localtime(&time_stamp_);
}

DateTime::DateTime(const string& date_string, const char* format) {
    std::istringstream input_string(date_string);
    time_data_ = { 0 };
    input_string >> std::get_time(&time_data_, format);
    time_stamp_ = mktime(&time_data_);
}

int DateTime::day() const {
    return time_data_.tm_mday;
}

int DateTime::month() const {
    return time_data_.tm_mon + 1;
}

int DateTime::year() const {
    return time_data_.tm_year + 1900;
}

time_t DateTime::timestamp() const {
    return time_stamp_;
}

string DateTime::to_string(const char* format) const {
    char time_string[TIME_BUFFER_SIZE];
    strftime(time_string, TIME_BUFFER_SIZE, format, &time_data_);
    return string(time_string);
}

