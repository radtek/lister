#ifndef _lister_Urp_UrpTime_h_
#define _lister_Urp_UrpTime_h_

#include "UrpShared.h"

// See \upp\uppsrc\CtrlLib\DateTimeCtrl.h enum {SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY};
enum {JANUARY=1, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER};
enum {FIRSTWEEK=1, SECONDWEEK, THIRDWEEK, FOURTHWEEK, LASTWEEK};
enum UrpCountry { URP_US, URP_UK, URP_JAPAN };

// Calculate business days forward or back depending on a 5-day week.
Date GetDateFromDateBDays(Date startDate, int bdaysaway, UrpCountry country = URP_US);
Date GetDateByMonthWeekWeekday(int year, int month, int week, int weekday);


/*
 * Calendar styles
 */
#define JULIAN    0
#define GREGORIAN 1


/*
 * is_leap:
 * Determines if a year is a leap year.
 * Input parameters:
 *     Calendar style (JULIAN or GREGORIAN)
 *     Year (must be >0)
 * Returns:
 *     1 if the year is a leap year, 0 otherwise.
 *
 * Note: The algorithm assumes that AD 4 is a leap year. This may be
 * historically inaccurate. See the FAQ.
 *
 * Reference: Sections 2.1.1 and 2.2.1 of version 2.9 of the FAQ.
 */
int is_leap(int style, int year);


/*
 * days_in_month:
 * Calculates the number of days in a month.
 * Input parameters:
 *     Calendar style (JULIAN or GREGORIAN)
 *     Year (must be >0)
 *     Month (1..12)
 * Returns:
 *     The number of days in the month (28..31)
 */
int days_in_month(int style, int year, int month);


/*
 * solar_number:
 * Calculates the Solar Number of a given year.
 * Input parameter:
 *     Year (must be >0)
 * Returns:
 *     Solar Number (1..28)
 *
 * Reference: Section 2.4 of version 2.9 of the FAQ.
 */
int solar_number(int year);


/*
 * day_of_week:
 * Calculates the weekday for a given date.
 * Input parameters:
 *     Calendar style (JULIAN or GREGORIAN)
 *     Year (must be >0)
 *     Month (1..12)
 *     Day (1..31)
 * Returns:
 *     0 for Sunday, 1 for Monday, 2 for Tuesday, etc.
 *
 * Reference: Section 2.6 of version 2.9 of the FAQ.
 */
int day_of_week(int style, int year, int month, int day);


/*
 * golden_number:
 * Calculates the Golden Number of a given year.
 * Input parameter:
 *     Year (must be >0)
 * Returns:
 *     Golden Number (1..19)
 *
 * Reference: Section 2.13.3 of version 2.9 of the FAQ.
 */
int golden_number(int year);


/*
 * epact:
 * Calculates the Epact of a given year.
 * Input parameters:
 *     Calendar style (JULIAN or GREGORIAN)
 *     Year (must be >0)
 * Returns:
 *     Epact (1..30)
 *
 * Reference: Section 2.13.5 of version 2.9 of the FAQ.
 */
int epact(int style, int year);


/*
 * paschal_full_moon:
 * Calculates the date of the Paschal full moon.
 * Input parameters:
 *     Calendar style (JULIAN or GREGORIAN)
 *     Year (must be >0)
 * Output parameters:
 *     Address of month of Paschal full moon (3..4)
 *     Address of day of Pascal full moon (1..31)
 *
 * Reference: Section 2.13.4 and 2.13.6 of version 2.9 of the FAQ.
 */
void paschal_full_moon(int style, int year, int *month, int *day);


/*
 * easter:
 * Calculates the date of Easter Sunday.
 * Input parameters:
 *     Calendar style (JULIAN or GREGORIAN)
 *     Year (must be >0)
 * Output parameters:
 *     Address of month of Easter Sunday (3..4)
 *     Address of day of Easter Sunday (1..31)
 *
 * Reference: Section 2.13.7 of version 2.9 of the FAQ.
 */
void easter(int style, int year, int *month, int *day);


/*
 * simple_gregorian_easter:
 * Calculates the date of Easter Sunday in the Gregorian calendar.
 * Input parameter:
 *     Year (must be in the range 1900..2099)
 * Output parameters:
 *     Address of month of Easter Sunday (3..4)
 *     Address of day of Easter Sunday (1..31)
 *
 * If the year is outside the legal range, *month is set to zero.
 *
 * Reference: Section 2.13.8 of version 2.9 of the FAQ.
 */
void simple_gregorian_easter(int year, int *month, int *day);


/*
 * indiction:
 * Calculates the Indiction of a given year.
 * Input parameter:
 *     Year (must be >0)
 * Returns:
 *     Indiction (1..15)
 *
 * Reference: Section 2.15 of version 2.9 of the FAQ.
 */
int indiction(int year);


/*
 * julian_period:
 * Calculates the year in the Julian Period corresponding to a given
 * year.
 * Input parameter:
 *    Year (must be in the range -4712..3267). The year 1 BC must be
 *        given as 0, the year 2 BC must be given as -1, etc.
 * Returns:
 *    The corresponding year in the Julian period
 *
 * Reference: Section 2.16 of version 2.9 of the FAQ.
 */
int julian_period(int year);


/*
 * date_to_jdn:
 * Calculates the Julian Day Number for a given date.
 * Input parameters:
 *     Calendar style (JULIAN or GREGORIAN)
 *     Year (must be > -4800). The year 1 BC must be given as 0, the
 *         year 2 BC must be given as -1, etc.
 *     Month (1..12)
 *     Day (1..31)
 * Returns:
 *     Julian Day Number
 *
 * Reference: Section 2.16.1 of version 2.9 of the FAQ.
 */
int date_to_jdn(int style, int year, int month, int day);
#endif
