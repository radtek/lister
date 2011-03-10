#include "UrpTime.h"

/*
  TODO: Add Jewish and Indian holidays from http://glossopian.co.uk/pmwiki.php?n=Main.WorkingDays
    since large number of bank staff are one or other
  - Get UK and Japan to work.
  - Consider linkback to call script that will pull next years from db
  - Support Fixed Income vs. Equity differences (2 days are different)
  - Differentiate between bank holidays and trading holidays
  - Is Inauguration Day federal holiday?
  - Get Good Friday calc working
  - Get latam holidays, mexico, etc.  Hong Kong dates differ?
*/
Vector<Index<Date> > holidays;

//==============================================================================================
void LoadHolidays() {
	Date curDate = GetSysDate();
	int y = curDate.year;
	Vector<Date> us;
	us.Add(GetDateByMonthWeekWeekday(y, JANUARY, THIRDWEEK, MONDAY));         // Martin Luther King's Birthday
	us.Add(GetDateByMonthWeekWeekday(y, FEBRUARY, THIRDWEEK, MONDAY));        // President's Day
	us.Add(GetDateByMonthWeekWeekday(y, MAY, LASTWEEK, MONDAY));              // Memorial Day
	us.Add(Date(y, JULY, 4));                                                 // Independence Day
	us.Add(GetDateByMonthWeekWeekday(y, SEPTEMBER, FIRSTWEEK, MONDAY));       // Labor Day
	us.Add(GetDateByMonthWeekWeekday(y, OCTOBER, SECONDWEEK, MONDAY));        // Columbus Day
	us.Add(Date(y, NOVEMBER, 11));                                            // Veteran's Day
	us.Add(GetDateByMonthWeekWeekday(y, NOVEMBER, FOURTHWEEK, THURSDAY));     // Thanksgiving Day
	us.Add(Date(y, DECEMBER, 25));                                            // Christmas Day
	// Inauguration Day, every 4 years starting 1940, January 20th
	// Good Friday
	holidays.Add(us);
	
}

// Calculate business days forward or back depending on a 5-day week.
// Send a negative bdaysaway to calculate days back.
// http://www.smart.net/~mmontes/ushols.html
//==============================================================================================
Date GetDateFromDateBDays(Date startDate, int bdaysaway, UrpCountry country) {
	Date fromdate = startDate; // Make working colpy
	int dayofweek = DayOfWeek(fromdate);
	if (dayofweek == 6 /* Sat */) fromdate+= 1; // Shift to sunday so week calc works
	int bdaysleftinweek = 5 - DayOfWeek(fromdate); // Calculate days left in week (backwards)
	int weeksinbetween = (bdaysaway - bdaysleftinweek + (bdaysaway > 0? 4 : 0)) / 5; // Substract current week if going forward
	bdaysaway = bdaysaway + (weeksinbetween * 2); // 2 days skipped per week

	// Holiday general calculator:
	// 1 + (Q-1)*7 + (N - DoW(Year,Month,1))%7
	// where N is the day of week, Q is what occurence (th) that we want DoW = Day of week for a date
	// http://www.tondering.dk/main/index.php/calendar-information
	Date edate;
	edate = fromdate + bdaysaway;
	
	if (holidays.GetCount() == 0) {
		LoadHolidays();
	}
	
	
	// If this is a holiday, skip ahead or back a working day (business day)
	if (holidays.At((int)country).Find(edate) != -1) {
		return GetDateFromDateBDays(edate, Sgn(bdaysaway), country);
	} else {
		// If the previous day is sunday and it was a holiday, since we only have official
		// non-working holidays in the database, then we calculate the observed holiday
		// as being Monday.  
		// Federal law (5 U.S.C. 6103) establishes the following public holidays for Federal employees. Please note that most Federal employees work on a Monday through Friday schedule. For these employees, when a holiday falls on a nonworkday -- Saturday or Sunday -- the holiday usually is observed on Monday (if the holiday falls on Sunday) or Friday (if the holiday falls on Saturday).
		// January 1, 2011 (the legal public holiday for New Year’s Day), falls on a Saturday. For most Federal employees, Friday, December 31, 2010, will be treated as a holiday for pay and leave purposes. (See 5 U.S.C. 6103(b).)
		// TODO: Does this work regardless of country?
		Date prevfrom_edate = edate - 1;
		
		if (DayOfWeek(prevfrom_edate) == SUNDAY && holidays.At((int)country).Find(prevfrom_edate) != -1) {
			return GetDateFromDateBDays(edate, Sgn(bdaysaway), country);
		}
		
		// Check if tomorrow is a saturday and a holiday. http://www.opm.gov/Operating_Status_Schedules/fedhol/2011.asp
		Date nextafter_edate = edate + 1;
		if (DayOfWeek(nextafter_edate) == SATURDAY && holidays.At((int)country).Find(nextafter_edate) != -1) {
			return GetDateFromDateBDays(edate, Sgn(bdaysaway), country);
		}

	}
	
	return edate;
}

// http://www.codeproject.com/KB/dotnet/HolidayCalculator.aspx
//==============================================================================================
Date GetDateByMonthWeekWeekday(int year, int month, int week, int weekday) {
	Date workDate(year, month, 1); // Calculate first

	while (DayOfWeek(workDate) != weekday) {
		workDate+= 1;
	}
	
	Date result;
	if (week == 1) {
		result = workDate;
	}
	else {
		int addDays = (week * 7) - 7;
		int day = workDate.day + addDays;
		if (day > LastDayOfMonth(workDate).day) {
			day = day - 7;
		}
		result = Date(workDate.year, workDate.month, day);
	}
	
	return result;
}

/*
US	Martin Luther King Day	Monday, January 17, 2011 
US	President's Day	Monday, February 21, 2011 
US	Good Friday	Friday, April 22, 2011
US	Memorial Day	Monday, May 30, 2011
US	U.S. Independence Day	Monday, July 4, 2011
US	Labor Day	Monday, September 5, 2011
US	Columbus Day	Monday, October 10, 2011 
US	Veterans Day	Friday, November 11, 2011
US	Thanksgiving Day	Thursday, November 24, 2011
US	Christmas Day	Monday, December 26, 2011 (Observed)
US	New Year's Day 2011/2012	Monday, January 2, 2012 (Observed)
UK	New Year's Day 2010/2011	Monday, January 3, 2011 (observed) 
UK	U.S. Martin Luther King Day	Monday, January 17, 2011 
UK	U.S. President's Day	Monday, February 21, 2011 
UK	Good Friday	Friday, April 22, 2011
UK	Easter Monday	Monday, April 25, 2011
UK	Bank Holiday (Prince William Royal Wedding)	Friday, April 29, 2011
UK	Early May Bank Holiday	Monday, May 2, 2011
UK	U.S. Memorial Day/ U.K. Spring Bank Holiday	Monday, May 30, 2011
UK	U.S. Independence Day	Monday, July 4, 2011
UK	Summer Bank Holiday	Monday, August 29, 2011
UK	U.S. Labor Day	Monday, September 5, 2011
UK	U.S. Columbus Day	Monday, October 10, 2011
UK	U.S. Veterans Day	Friday, November 11, 2011
UK	U.S. Thanksgiving Day	Thursday, November 24, 2011
UK	Christmas Day	Monday, December 26, 2011
UK	Boxing Day	Tuesday, December 27, 2011 (Observed)
UK	New Year's Day 2011/2012	Monday, January 2, 2012
Japan	Bank Holiday	Monday, January 3, 2011 
Japan	Adults Day	Monday, January 10, 2011 
Japan	U.S. Martin Luther King Day	Monday, January 17, 2011 
Japan	National Foundation Day	Friday, February 11, 2011 
Japan	U.S. President's Day	Monday, February 21, 2011 
Japan	Spring Equinox	Monday, March 21, 2011
Japan	Good Friday	Friday, April 22, 2011
Japan	Showa Day	Friday, April 29, 2011
Japan	Constitutional Day	Tuesday, May 3, 2011
Japan	Greenery Day	Wednesday, May 4, 2011
Japan	Children's Day	Thursday, May 5, 2011
Japan	U.S. Memorial Day/ U.K. Spring Bank Holiday	Monday, May 30, 2011
Japan	U.S. Independence Day	Monday, July 4, 2011
Japan	Marine Day	Monday, July 18, 2011
Japan	U.S. Labor Day	Monday, September 5, 2011
Japan	Respect for the Aged Day	Monday, September 19, 2011
Japan	Autumn Equinox	Friday, September 23, 2011
Japan	Sports Day/ U.S. Columbus Day	Monday, October 10, 2011
Japan	Culture Day	Thursday, November 3, 2011
Japan	U.S. Veterans Day	Friday, November 11, 2011
Japan	Labor Thanksgiving Day	Wednesday, November 23, 2011
Japan	U.S. Thanksgiving Day	Thursday, November 24, 2011
Japan	Emperor's Birthday	Friday, December 23, 2011
Japan	Christmas Day	Monday, December 26, 2011 (Observed)
Japan	New Year's Day 2011/2012	Monday, January 2, 2012 (Observed)
US	New Year's Day 2011/2012	Monday, January 2, 2012 (observed)
US	Martin Luther King Day	Monday, January 16, 2012 
US	Presidents Day	Monday, February 20, 2012 
US	Memorial Day	Monday, May 28, 2012 
US	U.S. Independence Day	Wednesday, July 4, 2012
US	Labor Day	Monday, September 3, 2012 
US	Columbus Day	Monday, October 8, 2012 
US	Veterans Day	Monday, November 12, 2012 
US	Thanksgiving Day	Thursday, November 22, 2012 
US	Christmas Day	Tuesday, December 25, 2012 
US	New Year's Day 2012/2013	 Tuesday, January 1, 2013 
UK	New Year's Day 2011/2012	 Monday, January 2, 2012 (observed) 
UK	U.S. Martin Luther King Day	Monday, January 16, 2012
UK	U.S. Presidents Day	Monday, February 20, 2012
UK	Good Friday	Friday, April 6, 2012
UK	Easter Monday	Monday, April 9, 2012
UK	Early May Bank Holiday	Monday, May 7, 2012
UK	U.S. Memorial Day	Monday, May 28, 2012
UK	Spring Bank Holiday	Monday, June 4, 2012
UK	Queen's Diamond Jubilee	Tuesday, June 5, 2012
UK	U.S. Independence Day	Wednesday, July 4, 2012
UK	Summer Bank Holiday	Monday, August 27, 2012
UK	U.S. Labor Day	 Monday, September 3, 2012
UK	U.S. Columbus Day	 Monday, October 8, 2012
UK	U.S. Veterans Day	 Monday, November 12, 2012
UK	U.S. Thanksgiving Day	Thursday, November 22. 2012
UK	Christmas Day	 Tuesday, December 25, 2012
UK	U.K. Boxing Day	 Wednesday, December 26, 2012
UK	New Year's Day 2012/2013	 Tuesday, January 1, 2013
Japan	New Year's Day 2011/2012	 Monday, January 2, 2012 (observed)
Japan	Bank Holiday	Tuesday, January 3, 2012
Japan	Adults Day	 Monday, January 9, 2012 
Japan	U.S. Martin Luther King Day	Monday, January 16, 2012
Japan	U.S. Presidents' Day	Monday, February 20, 2012
Japan	Spring Equinox	 Tuesday, March 20, 2012
Japan	Showa Day	 Monday, April 30, 2012 (observed)
Japan	Constitutional Day	 Thursday, May 3, 2012 
Japan	Greenery Day	 Friday, May 4, 2012  
Japan	U.S. Memorial Day	 Monday, May 28, 2012 
Japan	U.S. Independence Day	Wednesday, July 4, 2012
Japan	Marine Day	 Monday, July 16, 2012
Japan	U.S. Labor Day	Monday, September 3, 2012
Japan	Respect for the Aged Day	Monday, September 17, 2012
Japan	Sports Day/ U.S. Columbus Day	Monday, October 8, 2012
Japan	U.S. Veterans Day	Monday, November 12, 2012
Japan	Labor Thanksgiving Day	Friday, November 23, 2012
Japan	U.S. Thanksgiving Day	Thursday, November 22, 2012
Japan	Emperor's Birthday	Monday, December 24, 2012 (observed)
Japan	Christmas Day	Tuesday, December 25, 2012
Japan	Bank Holiday	Monday, December 31, 2012
Japan	New Year's Day	Tuesday, January 1, 2013
Japan	Bank Holidays (2013)	Wednesday, January 2, 2013
Japan	Bank Holidays (2013)	Thursday, January 3, 2013

Dates that are always the same:
US: New Year's Day: 
*/

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
int is_leap(int style, int year)
{
    return style==JULIAN
             ? year%4==0
             : (year%4==0 && year%100!=0) || year%400==0;
}


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
int days_in_month(int style, int year, int month)
{
    static int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    return month==2 && is_leap(style,year)
             ? 29
             : days[month-1];
}


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
int solar_number(int year)
{
    return (year + 8) % 28 + 1;
}


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
int day_of_week(int style, int year, int month, int day)
{
    int a = (14 - month) / 12;
    int y = year - a;
    int m = month + 12*a - 2;
    return style== JULIAN
             ? (5 + day + y + y/4 + (31*m)/12) % 7
             : (day + y + y/4 - y/100 + y/400 + (31*m)/12) % 7;
}


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
int golden_number(int year)
{
    return year%19 + 1;
}


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
int epact(int style, int year)
{
    if (style==JULIAN) {
        int je = (11 * (golden_number(year)-1)) % 30;
        return je==0 ? 30 : je;
    }
    else {
        int century = year/100 + 1;
        int solar_eq = (3*century)/4;
        int lunar_eq = (8*century + 5)/25;
        int greg_epact = epact(JULIAN,year) - solar_eq + lunar_eq + 8;

        while (greg_epact>30)
            greg_epact -= 30;
        while (greg_epact<1)
            greg_epact += 30;

        return greg_epact;
    }
}


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
void paschal_full_moon(int style, int year, int *month, int *day)
{
    if (style==JULIAN) {
        static struct {
            int month;
            int day;
        } jul_pfm[] = {
            { 4,  5 }, { 3, 25 }, { 4, 13 }, { 4,  2 }, { 3, 22 },
            { 4, 10 }, { 3, 30 }, { 4, 18 }, { 4,  7 }, { 3, 27 },
            { 4, 15 }, { 4,  4 }, { 3, 24 }, { 4, 12 }, { 4,  1 },
            { 3, 21 }, { 4,  9 }, { 3, 29 }, { 4, 17 }
        };

        int gn = golden_number(year);
        *month = jul_pfm[gn-1].month;
        *day = jul_pfm[gn-1].day;
    }
    else {
        int gepact = epact(GREGORIAN,year);

        if (gepact<=12) {
            *month = 4;
            *day = 13-gepact;
        }
        else if (gepact<=23) {
            *month = 3;
            *day = 44-gepact;
        }
        else if (gepact==24) {
            *month = 4;
            *day = 18;
        }
        else if (gepact==25) {
            *month = 4;
            *day = golden_number(year)>11 ? 17 : 18;
        }
        else {
            *month = 4;
            *day = 43-gepact;
        }
    }
}


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
void easter(int style, int year, int *month, int *day)
{
    int G, I, J, C, H, L;

    G = year % 19;

    if (style==JULIAN) {
        I = (19*G + 15) % 30;
        J = (year + year/4 + I) % 7;
    }
    else {
        C = year/100;
        H = (C - C/4 - (8*C+13)/25 + 19*G + 15) % 30;
        I = H - (H/28)*(1 - (29/(H + 1))*((21 - G)/11));
        J = (year + year/4 + I + 2 - C + C/4) % 7;
    }

    L = I - J;
    *month = 3 + (L + 40)/44;
    *day = L + 28 - 31*(*month/4);
}


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
void simple_gregorian_easter(int year, int *month, int *day)
{
    int H, I, J, L;

    if (year<1900 || year>2099) {
        *month = 0;
        return;
    }

    H = (24 + 19*(year % 19)) % 30;
    I = H - H/28;
    J = (year + year/4 + I - 13) % 7;
    L = I - J;
    *month = 3 + (L + 40)/44;
    *day = L + 28 - 31*(*month/4);
}


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
int indiction(int year)
{
    return (year + 2) % 15 + 1;
}


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
int julian_period(int year)
{
    return year+4713;
}


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
int date_to_jdn(int style, int year, int month, int day)
{
    int a = (14-month)/12;
    int y = year+4800-a;
    int m = month + 12*a - 3;
    return style==JULIAN
             ? day + (153*m+2)/5 + y*365 + y/4 - 32083
             : day + (153*m+2)/5 + y*365 + y/4 - y/100 + y/400 - 32045;
}
    

/*
 * jdn_to_date:
 * Calculates the date for a given Julian Day Number.
 * Input parameter:
 *     Calendar style (JULIAN or GREGORIAN)
 *     Julian Day Number
 * Output parameters:
 *     Address of year. The year 1 BC will be stored as 0, the year
 *         2 BC will be stored as -1, etc.
 *     Address of month (1..12)
 *     Address of day (1..31)
 *
 * Reference: Section 2.16.1 of version 2.9 of the FAQ.
 */
void jdn_to_date(int style, int JD, int *year, int *month, int *day)
{
    int b, c, d, e, m;

    if (style==JULIAN) {
        b = 0;
        c = JD + 32082;
    }
    else {
        int a = JD + 32044;
        b = (4*a+3)/146097;
        c = a - (b*146097)/4;
    }

    d = (4*c+3)/1461;
    e = c - (1461*d)/4;
    m = (5*e+2)/153;

    *day   = e - (153*m+2)/5 + 1;
    *month = m + 3 - 12*(m/10);
    *year  = b*100 + d - 4800 + m/10;
}


/*
 * week_number:
 * Calculates the ISO 8601 week number (and corresponding year) for a given
 * Gregorian date.
 * Input parameters:
 *     Year (must be >0)
 *     Month (1..12)
 *     Day
 * Output parameters:
 *     Address of week number (1..53)
 *     Address of corresponding year
 *
 * Reference: Section 7.8 of version 2.9 of the FAQ.
 */
void week_number(int year, int month, int day, int *week_number, int *week_year)
{
    int a, b, c, s, e, f, g, d, n;

    if (month<=2) {
        a = year-1;
        b = a/4 - a/100 + a/400;
        c = (a-1)/4 - (a-1)/100 + (a-1)/400;
        s = b-c;
        e = 0;
        f = day - 1 + 31*(month-1);
    }
    else {
        a = year;
        b = a/4 - a/100 + a/400;
        c = (a-1)/4 - (a-1)/100 + (a-1)/400;
        s = b-c;
        e = s+1;
        f = day + (153*(month-3)+2)/5 + 58 + s;
    }

    g = (a + b) % 7;
    d = (f + g - e) % 7;
    n = f + 3 - d;

    if (n<0) {
        *week_number = 53-(g-s)/5;
        *week_year = year-1;
    }
    else if (n>364+s) {
        *week_number = 1;
        *week_year = year+1;
    }
    else {
        *week_number = n/7 + 1;
        *week_year = year;
    }
}

/*
	http://en.wikipedia.org/wiki/Computus
	input y year for m and d of easter
	c = y / 100
	n = y - 19 * ( y / 19 )
	k = ( c - 17 ) / 25
	i = c - c / 4 - ( c - k ) / 3 + 19 * n + 15
	i = i - 30 * ( i / 30 )
	i = i - ( i / 28 ) * ( 1 - ( i / 28 ) * ( 29 / ( i + 1 ) )
	* ( ( 21 - n ) / 11 ) )
	j = y + y / 4 + i + 2 - c + c / 4
	j = j - 7 * ( j / 7 )
	l = i - j
	m = 3 + ( l + 40 ) / 44
	d = l + 28 - 31 * ( m / 4 )
	
    private DateTime easter(int y)
    {
      int a=y%19;
      int b=y/100;
      int c=y%100;
      int d=b/4;
      int e=b%4;
      int f=(b+8)/25;
      int g=(b-f+1)/3;
      int h=(19*a+b-d-g+15)%30;
      int i=c/4;
      int k=c%4;
      int l=(32+2*e+2*i-h-k)%7;
      int m=(a+11*h+22*l)/451;
      int easterMonth =(h+l-7*m+114)/31;
      int  p=(h+l-7*m+114)%31;
      int easterDay=p+1;
      DateTime est = new DateTime(y,easterMonth,easterDay);
      if (est < this.startingDate)
        return this.easter(y+1);
      else
        return new DateTime(y,easterMonth,easterDay);
    }
	
*/

