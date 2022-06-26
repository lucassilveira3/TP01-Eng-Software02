#ifndef DATE_TIME_H
#define DATE_TIME_H
#include <ctime>
#include <string>

using std::string;

class DateTime {
    public:
        static DateTime now();
        // Constructs a DateTime object from a UNIX timestamp
        DateTime(time_t time_stamp);
        // Constructs a DateTime object from a date string on the given format.
        // The default format is "%Y/%m/%d - %H:%M:%S"
        DateTime(const string& date_string, const char* format = "%Y/%m/%d - %H:%M:%S");
        int day() const;
        int month() const;
        int year() const;
        time_t timestamp() const;
        // Returns the date and time on a string with the "YYYY/MM/DD - HH:MM:SS" format 
        string to_string() const;
    private:
        time_t time_stamp_;
        tm time_data_;
};

#endif