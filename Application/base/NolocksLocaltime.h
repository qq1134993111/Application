#pragma once

#include <time.h>


inline int is_leap_year(int y) 
{
    if ((y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0)))
        // Leap year if perfectly divisible by 400
        // Leap year if perfectly divisible by 4
        // but if not divisible by 100
        return 1;
    else // all other years are not leap year
        return 0;
}



inline void nolocks_localtime(struct tm *tmp, time_t t, time_t tz = 0 /*区时*/, int dst=0 /*是否开启夏令时*/)
{
    const time_t secs_min = 60;
    const time_t secs_hour = 3600;
    const time_t secs_day = 3600 * 24;

    t -= tz;                       /* Adjust for timezone. */
    t += 3600 * dst;               /* Adjust for daylight time. */
    time_t days = t / secs_day;    /* Days passed since epoch. */
    time_t seconds = t % secs_day; /* Remaining seconds. */

    tmp->tm_isdst = dst;
    tmp->tm_hour =static_cast<int>(seconds / secs_hour);
    tmp->tm_min = (seconds % secs_hour) / secs_min;
    tmp->tm_sec = (seconds % secs_hour) % secs_min;

    /* 1/1/1970 was a Thursday, that is, day 4 from the POV of the tm structure * where sunday = 0, so to calculate the
     * day of the week we have to add 4 * and take the modulo by 7. */
    tmp->tm_wday = (days + 4) % 7;
    /* Calculate the current year. */
    tmp->tm_year = 1970;
    while (1)
    {
        /* Leap years have one day more. */
        time_t days_this_year = 365 + is_leap_year(tmp->tm_year);
        if (days_this_year > days)
            break;
        days -= days_this_year;
        tmp->tm_year++;
    }
    tmp->tm_yday = static_cast<int>(days); /* Number of day of the current year. */

    /* We need to calculate in which month and day of the month we are. To do * so we need to skip days according to how
     * many days there are in each * month, and adjust for the leap year that has one more day in February. */
    int mdays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    mdays[1] += is_leap_year(tmp->tm_year);

    tmp->tm_mon = 0;
    while (days >= mdays[tmp->tm_mon])
    {
        days -= mdays[tmp->tm_mon];
        tmp->tm_mon++;
    }

    tmp->tm_mday = static_cast<int>(days + 1); /* Add 1 since our 'days' is zero-based. */
    tmp->tm_year -= 1900;    /* Surprisingly tm_year is year-1900. */
}
