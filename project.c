/* Joao Gouveia - 102611
 * File containing project code */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"limits.h"
#include"structs.h"

/* Airport sorting funtions */
int less_ap(Airport aAirport1, Airport aAirport2);
void quicksort_ap(int iFirst, int iLast);
int partition_ap(int iFirst, int iLast);
void swap_ap(int i, int j);
/* Airport related funtions */
void print_ap(Airport aAirport);
int find_ap(char cId[IDAP]);
/* Flight sorting funtions */
int less_fl(Flight fFlight, char* sFlightTime1, int iArrivals);
void quicksort_fl(Flight* fArray, int iFirst, int iLast, int iArrivals);
int partition_fl(Flight* fArray, int iFirst, int iLast, int iArrivals);
void swap_fl(Flight* fArray, int i, int j);
/* Flight related funtions */
void format_time_fl(char* dest, Flight f, int iArrivals);
void print_fl(Flight fFlight, int iMode);
int invalid_idfl(char cChar, int iIndex);
int same_flight(Flight fFlight1, Flight fFlight2);
/* Date related funtions */
void convert_to_str(char* sDest, int iTime);
void read_date(Date* dDate, char* arg);
int is_30days(int iMonth);
void advance_day(int* iDate);
int same_date(Date dDate1, Date dDate2);
int invalid_date(Date dDate);
void arrival_date(char sDest[5][YEAR], Flight fFlight);
void fill_arrival_date(Flight* fFlight);
/* Time related funtions */
void read_time(Time* tTime, char* arg);
int read_duration(int* iDuration, char* arg);
int invalid_duration(int* iDuration);
/* Main project functions */
void add_ap(char* arg);
void list_all_ap();
void list_ap(char* arg);
void list_all_fl();
void add_fl(char* arg);
void list_arr_or_dep(char* cId, int iMode);
void arrivals(char* cId);
void advance_date(char* arg);

/* Global variables */
Airport aAirports[MAXAIRPORTS]; /* Airport array */
Flight fFlights[MAXFLIGHTS]; /* Flight array */
int iCurrentAirports, iCurrentFlights; /* Airport and flight counters */
Date today = {"01", "01", "2022"}; /* Program starting date */

int main () {
	char arg[ARGSIZE];
	do {
		fgets(arg, sizeof(char)*ARGSIZE, stdin);
		switch (arg[0]) {
			case 'q':
				break;
			case 'a':
				add_ap(arg+ARGSTART);
				break;
			case 'l':
				if (strlen(arg) == 2) {
					list_all_ap();
				}
				else {
					list_ap(arg+ARGSTART);
				}
				break;
			case 'v':
				if (strlen(arg) == 2) {
					list_all_fl();
				}
				else {
					add_fl(arg+ARGSTART);
				}
				break;
			case 'p':
				list_arr_or_dep(arg+ARGSTART, FALSE);
				break;
			case 'c':
				list_arr_or_dep(arg+ARGSTART, TRUE);
				break;
			case 't':
				advance_date(arg+ARGSTART);
				break;
		}
	} while (arg[0] != 'q');
	return 0;
}

/**
 * Function: less_ap
 * --------------------
 * Compares two airports
 * through their Id's.
 *
 *  Return: int
 **/
int less_ap(Airport aAirport1, Airport aAirport2) {
	return strcmp(aAirport1.id, aAirport2.id) <= 0;
}

/**
 * Function: quicksort_ap
 * --------------------
 *  Quicksort sorting algorithm
 *  applied to airport struct.
 *
 *  Return: void
 **/
void quicksort_ap(int iFirst, int iLast) {
	int iPartition;
	if (iFirst < iLast) {
		iPartition = partition_ap(iFirst, iLast);
		quicksort_ap(iFirst, iPartition-1);
		quicksort_ap(iPartition+1, iLast);
	}
}

/**
 * Function: partition_ap
 * --------------------
 * Calculates and returns a partition
 * for quicksort.
 *
 *  Return: int
 **/
int partition_ap(int iFirst, int iLast) {
	int iPivot = iFirst, i = iFirst, j = iLast;
	while (i < j) {
		while (less_ap(aAirports[i], aAirports[iPivot]) && i < iLast) {
			i++;
		}
		while (!less_ap(aAirports[j], aAirports[iPivot])) {
			j--;
		}
		if (i < j) {
			swap_ap(i, j);
		}
	}
	swap_ap(iPivot, j);
	return j;
}

/**
 * Function: swap_ap
 * --------------------
 * Swaps two airports in 
 * an array.
 *
 *  Return: void
 **/
void swap_ap(int i, int j) {
	Airport aAux;
	if (i == j) {
		return;
	}
	aAux = aAirports[i];
	aAirports[i] = aAirports[j];
	aAirports[j] = aAux;
}

/**
 * Function: print_ap
 * --------------------
 * Prints an airport.
 *
 *  Return: void
 **/
void print_ap(Airport aAirport) {
	printf("%s ", aAirport.id); 
	printf("%s ", aAirport.city); 
	printf("%s ", aAirport.country); 
	printf("%d\n", aAirport.departures); 
}

/**
 * Function: find_ap
 * --------------------
 * Finds an airport in aAirports
 * and returns its index. Returns
 * NOTFOUND if the airport doesn't
 * exist.
 *
 *  Return: int
 **/
int find_ap(char cId[IDAP]) {
	int i;
	for (i = 0; i < iCurrentAirports; i++) {
		if (strcmp(cId, aAirports[i].id) == 0) {
			return i;
		}
	}
	return NOTFOUND;
}

/**
 * Function: format_time_fl
 * --------------------
 * Formats flight time to be 
 * compared by the quicksort.
 *
 *  Return: void
 **/
void format_time_fl(char* dest, Flight f, int iArrivals) {
	if (iArrivals) {
		sprintf(dest, "%s%s%s%s%s", f.arrDate.year, f.arrDate.month, f.arrDate.day, f.arrTime.hours, f.arrTime.mins);
		return;
	}
	sprintf(dest, "%s%s%s%s%s", f.date.year, f.date.month, f.date.day, f.time.hours, f.time.mins);
}

/**
 * Function: copy_flights
 * --------------------
 * Copies flights from fFlights
 * with the requested id.
 *
 *  Return: int
 **/
int copy_flights(Flight* fArray, char* cId, int iArrivals) {
	int i, iSize = 0;
	if (iArrivals) {
		for (i = 0; i < iCurrentFlights; i++) {
			if (strcmp(fFlights[i].arrival, cId) == 0) {
				fArray[iSize] = fFlights[i];
				iSize++;
			}
		}
		return iSize;
	}
	for (i = 0; i < iCurrentFlights; i++) {
		if (strcmp(fFlights[i].departure, cId) == 0) {
			fArray[iSize] = fFlights[i];
			iSize++;
		}
	}
	return iSize;
}

/**
 * Function: less_fl
 * --------------------
 * Compares two flights
 * through their times.
 *
 *  Return: int
 **/
int less_fl(Flight fFlight, char* sFlightTime1, int iArrivals) {
	char sFlightTime2[CATDATETIME];
	format_time_fl(sFlightTime2, fFlight, iArrivals);
	return strcmp(sFlightTime2, sFlightTime1) <= 0;
}

/**
 * Function: quicksort_fl
 * --------------------
 *  Quicksort sorting algorithm
 *  applied to flight struct.
 *
 *  Return: void
 **/
void quicksort_fl(Flight* fArray, int iFirst, int iLast, int iArrivals) {
	int iPartition;
	if (iFirst < iLast) {
		iPartition = partition_fl(fArray, iFirst, iLast, iArrivals);
		quicksort_fl(fArray, iFirst, iPartition-1, iArrivals);
		quicksort_fl(fArray, iPartition+1, iLast, iArrivals);
	}
}

/**
 * Function: partition_fl
 * --------------------
 * Calculates and returns a partition
 * for quicksort.
 *
 *  Return: int
 **/
int partition_fl(Flight* fArray, int iFirst, int iLast, int iArrivals) {
	int i = iFirst, j = iLast;
	char sPivot[CATDATETIME];
	format_time_fl(sPivot, fArray[iFirst], iArrivals);
	while (i < j) {
		while (less_fl(fArray[i], sPivot, iArrivals) && i < iLast) {
			i++;
		}
		while (!less_fl(fArray[j], sPivot, iArrivals)) {
			j--;
		}
		if (i < j) {
			swap_fl(fArray, i, j);
		}
	}
	swap_fl(fArray, iFirst, j);
	return j;
}

/**
 * Function: swap_fl
 * --------------------
 * Swaps two flights in 
 * an array.
 *
 *  Return: void
 **/
void swap_fl(Flight* fArray, int i, int j) {
	Flight fAux;
	if (i == j) {
		return;
	}
	fAux = fArray[i];
	fArray[i] = fArray[j];
	fArray[j] = fAux;
}

/**
 * Function: print_fl
 * --------------------
 * Prints a flight.
 *
 *  Return: void
 **/
void print_fl(Flight fFlight, int iMode) {
	printf("%s ", fFlight.id);
	if (iMode != FALSE) {
		printf("%s ", fFlight.departure);
	}
	if (iMode != TRUE) {
		printf("%s ", fFlight.arrival);
		printf("%s-%s-%s ", fFlight.date.day, fFlight.date.month, fFlight.date.year);
		printf("%s:%s\n", fFlight.time.hours, fFlight.time.mins);
	}
	else {
		printf("%s-%s-%s ", fFlight.arrDate.day, fFlight.arrDate.month, fFlight.arrDate.year);
		printf("%s:%s\n", fFlight.arrTime.hours, fFlight.arrTime.mins);
	}
}

/**
 * Function: invalid_idfl
 * --------------------
 * Checks if a flight ID is invalid.
 *
 *  Return: int
 **/
int invalid_idfl(char cChar, int iIndex) {
	if (isupper(cChar) && iIndex < 2) {
		return FALSE;
	}
	return (!isdigit(cChar) || iIndex < 2);
}

/**
 * Function: same_flight
 * --------------------
 * Checks if two flights are the same.
 *
 *  Return: int
 **/
int same_flight(Flight fFlight1, Flight fFlight2) {
	if (strcmp(fFlight1.id, fFlight2.id) == 0) {
		return same_date(fFlight1.date, fFlight2.date);
	}
	return FALSE;
}

/**
 * Function: convert_to_str
 * --------------------
 * Converts a number to 
 * a string formating it
 * to be consistent with
 * HH:MM.
 *
 *  Return: void
 **/
void convert_to_str(char* sDest, int iTime) {
	sprintf(sDest, "%02d", iTime);
}

/**
 * Function: read_date
 * --------------------
 * Reads a date in DD-MM-YY
 * from a string.
 *
 *  Return: void
 **/
void read_date(Date* dDate, char* arg) {
	strncpy(dDate->day, arg, DAY-1);
	dDate->day[DAY-1] = '\0';
	strncpy(dDate->month, arg+DAY, DAY-1);
	dDate->month[DAY-1] = '\0';
	strncpy(dDate->year, arg+DAY+MONTH, YEAR-1);
	dDate->year[YEAR-1] = '\0';
}

/**
 * Function: is_30days
 * --------------------
 * Checks if a month has 30 days.
 *
 *  Return: int
 **/
int is_30days(int iMonth) {
	return (iMonth == 4 || iMonth == 6 || iMonth == 9 || iMonth == 11);
}

/**
 * Function: advance_day
 * --------------------
 * Advances a day in a date
 * and calculates the month
 * and year accordingly.
 *
 *  Return: void
 **/
void advance_day(int* iDate) {
	iDate[0]++;
	if (iDate[0] == 29 && iDate[1] == 2) {
		iDate[0] = 1;
		iDate[1] += 1;
		return;
	}
	if (iDate[0] == 31 && is_30days(iDate[1])) {
		iDate[0] = 1;
		iDate[1] += 1;
		return;
	}
	if (iDate[0] == 32 && !is_30days(iDate[1]) && iDate[1] != 12) {
		iDate[0] = 1;
		iDate[1] += 1;
		return;
	}
	if (iDate[0] == 32 && iDate[1] == 12) {
		iDate[0] = 1;
		iDate[1] = 1;
		iDate[2] += 1;
		return;
	}
}

/**
 * Function: same_date
 * --------------------
 * Checks if two dates are the same.
 *
 *  Return: int
 **/
int same_date(Date dDate1, Date dDate2) {
	if (strcmp(dDate1.day, dDate2.day) == 0 && strcmp(dDate1.month, dDate2.month) == 0) {
		return strcmp(dDate1.year, dDate2.year) == 0;
	}
	return FALSE;
}

/**
 * Function: invalid_date
 * --------------------
 * Checks if a date is invalid.
 *
 *  Return: int
 **/
int invalid_date(Date dDate) {
	int iYear;
	char cToday[CATDATE], cDate[CATDATE], cFuture[CATDATE];
	sprintf(cToday, "%s%s%s", today.year, today.month, today.day);
	sprintf(cDate, "%s%s%s", dDate.year, dDate.month, dDate.day);
	/* Converting year to int for easier math */
	iYear = atoi(today.year);
	iYear++;
	sprintf(cFuture, "%d%s%s", iYear, today.month, today.day);
	return strcmp(cDate, cFuture) > 0 || strcmp(cDate, cToday) < 0;
}

/**
 * Function: arrival_date
 * --------------------
 * Calculates the date in
 * which a flight arrives.
 *
 *  Return: void
 **/
void arrival_date(char sDest[5][YEAR], Flight fFlight) {
	int i, iDurMins, iDurHours;
	int iTime[5];
	/* Converting strings to int for easier math */
	iDurMins = fFlight.duration[1];
	iDurHours = fFlight.duration[0];
	iTime[0] = atoi(fFlight.time.mins);
	iTime[1] = atoi(fFlight.time.hours);
	iTime[2] = atoi(fFlight.date.day);
	iTime[3] = atoi(fFlight.date.month);
	iTime[4] = atoi(fFlight.date.year);
	/* Adding the duration and checking if the other variables need advancing */
	iTime[0] += iDurMins;
	iTime[1] += iDurHours;
	if (iTime[0] >= 60) {
		iTime[0] -= 60;
		iTime[1] += 1;
	}
	if (iTime[1] >= 24) {
		iTime[1] -= 24;
		advance_day(iTime+2);
	}
	/* Converting back to string and formatting all elements except year */
	for (i = 0; i < 4; i++) {
		convert_to_str(sDest[i], iTime[i]);
	}
	/* Year is converted seperately, since it doesn't need formatting */
	sprintf(sDest[4], "%d", iTime[4]);
}

void fill_arrival_date(Flight* fFlight) {
	char sTime[5][YEAR];
	arrival_date(sTime, *fFlight);
	strcpy(fFlight->arrTime.mins, sTime[0]);
	strcpy(fFlight->arrTime.hours, sTime[1]);
	strcpy(fFlight->arrDate.day, sTime[2]);
	strcpy(fFlight->arrDate.month, sTime[3]);
	strcpy(fFlight->arrDate.year, sTime[4]);
}

/**
 * Function: read_time
 * --------------------
 * Reads a time in HH:MM
 * or in H:MM from a string
 * and formats it to HH:MM.
 *
 *  Return: void
 **/
void read_time(Time* tTime, char* arg) {
	strncpy(tTime->hours, arg, HOURS-1);
	tTime->hours[HOURS-1] = '\0';
	strncpy(tTime->mins, arg+HOURS, MINS-1);
	tTime->mins[MINS-1] = '\0';
}

/**
 * Function: read_duration
 * --------------------
 * Reads a duration in HH:MM
 * or in H:MM from a string
 * and returns how much has
 * the pointer advanced.
 *
 *  Return: int
 **/
int read_duration(int* iDest, char* arg) {
	char cAux[MINS];
	int i, iOffset;
	for (i = 0; arg[i] != ':'; i++) {
		cAux[i] = arg[i];
	}
	cAux[i] = '\0';
	iDest[0] = atoi(cAux);
	iOffset = i+1;
	for (i = 0; arg[i+iOffset] != ' '; i++) {
		cAux[i] = arg[i+iOffset];
	}
	cAux[MINS-1] = '\0';
	iDest[1] = atoi(cAux);
	iOffset +=i+1;
	return iOffset;
}

/**
 * Function: invalid_duration
 * --------------------
 * Checks if duration is invalid.
 *
 *  Return: int
 **/
int invalid_duration(int* iDuration) {
	if(iDuration[0] > 12) {
		return TRUE;
	}
	if (iDuration[0] == 12 && iDuration[1] > 0) {
		return TRUE;
	}
	return FALSE;
}

/**
 * Function: add_ap
 * --------------------
 * Adds an airport to
 * aAirports.
 *
 *  Return: void
 **/
void add_ap(char* arg) {
	int i;
	Airport aNewAirport;
	for (i = 0; i < IDAP-1; i++) {
		if (!isupper(arg[i])) {
			printf("invalid airport ID\n");
			return;
		}
		aNewAirport.id[i] = arg[i];
	}
	aNewAirport.id[i] = '\0';
	/* Checking if there are too many airports */
	if (iCurrentAirports == MAXAIRPORTS) {
		printf("too many airports\n");
		return;
	}
	if (find_ap(aNewAirport.id) != NOTFOUND) {
		printf("duplicate airport\n");	
		return;
	}
	arg += IDAP;
	for (i = 0; arg[i] != ' ' && arg[i] != '\t'; i++) {
		aNewAirport.country[i] = arg[i];
	}
	aNewAirport.country[i] = '\0';
	arg += i+1;
	for (i = 0; arg[i] != '\n'; i++) {
		aNewAirport.city[i] = arg[i];
	}
	aNewAirport.city[i] = '\0';
	aNewAirport.departures = 0;
	aAirports[iCurrentAirports] = aNewAirport;
	iCurrentAirports++;
	printf("airport %s\n", aNewAirport.id);
}

/**
 * Function: list_all_ap
 * --------------------
 * Lists all airports, sorting
 * them alphabetically.
 *
 *  Return: void
 **/
void list_all_ap() {
	int i;
	if (iCurrentAirports != 0) {
		quicksort_ap(0, iCurrentAirports - 1);
		for (i = 0; i < iCurrentAirports; i++) {
			print_ap(aAirports[i]);
		}
	}
}

/**
 * Function: list_ap
 * --------------------
 * Lists airports requested
 * by the user.
 *
 *  Return: void
 **/
void list_ap(char* arg) {
	int i, iIndexAp, iSize = strlen(arg);
	char sAux[IDAP];
	for (i = 0; i < iSize; i += IDAP) {
		strncpy(sAux, arg+i, IDAP-1);
		sAux[IDAP-1] = '\0';
		iIndexAp = find_ap(sAux);
		if (iIndexAp == NOTFOUND) {
			printf("%s: no such airport ID\n", sAux);
		}
		else {
			print_ap(aAirports[iIndexAp]);
		}
	}
}

/**
 * Function: list_all_fl
 * --------------------
 * Lists all flights.
 *
 *  Return: void
 **/
void list_all_fl() {
	int i;
	for (i = 0; i < iCurrentFlights; i++) {
		print_fl(fFlights[i], 3);
	}
}

/**
 * Function: add_fl
 * --------------------
 * Adds a flight to fFlights
 *
 *  Return: void
 **/
void add_fl(char* arg) {
	int i, iDepartureIndex, iOffset;
	char cCapacity[CAPACITY];
	Flight fNewFlight;
	for (i = 0; arg[i] != ' '; i++) {
		if (invalid_idfl(arg[i], i)) {
			printf("invalid flight code\n");
			return;
		}
		fNewFlight.id[i] = arg[i];
	}	
	fNewFlight.id[i] = '\0';
	arg += i+1;
	read_date(&fNewFlight.date, arg+DATESTART);
	for (i = 0; i < iCurrentFlights; i++) {
		if (same_flight(fFlights[i], fNewFlight)) {
			printf("flight already exists\n");
			return;
		}
	}
	/* Checking if both ids exist */
	strncpy(fNewFlight.departure, arg, IDAP-1);
	fNewFlight.departure[IDAP-1] = '\0';
	iDepartureIndex = find_ap(fNewFlight.departure);
	if (iDepartureIndex == NOTFOUND) {
		printf("%s: no such airport ID\n", fNewFlight.departure);
		return;
	}
	arg += IDAP;
	strncpy(fNewFlight.arrival, arg, IDAP-1);
	fNewFlight.arrival[IDAP-1] = '\0';
	if (find_ap(fNewFlight.arrival) == NOTFOUND) {
		printf("%s: no such airport ID\n", fNewFlight.arrival);
		return;
	}
	/* Checking if there are too many flights */
	if (iCurrentFlights == MAXFLIGHTS) {
		printf("too many flights\n");
		return;
	}
	/* Checking if date is valid */
	if (invalid_date(fNewFlight.date)) {
		printf("invalid date\n");
		return;
	}
	arg += IDAP+DATE;
	/* Checking if duration is valid */
	read_time(&fNewFlight.time, arg);
	arg += TIME;
	iOffset = read_duration(fNewFlight.duration, arg);
	if(invalid_duration(fNewFlight.duration)) {
		printf("invalid duration\n");
		return;
	}
	arg += iOffset;
	/* Checking if capacity is valid */
	strncpy(cCapacity, arg, CAPACITY-1);
	cCapacity[CAPACITY-1] = '\0';
	fNewFlight.capacity = atoi(cCapacity);
	if(fNewFlight.capacity > 100 || fNewFlight.capacity < 10) {
		printf("invalid capacity\n");
		return;
	}
	/* Calculating and storing arrival date */
	fill_arrival_date(&fNewFlight);
	/* Adding one more flight to the departure counter */
	aAirports[iDepartureIndex].departures++;
	/* Adding flight to flight list */
	fFlights[iCurrentFlights] = fNewFlight;
	iCurrentFlights++;
}

/**
 * Function: list_arr_or_dep
 * --------------------
 * Lists all departures/arrivals 
 * from an airport, sorted by 
 * departure/arrival time.
 *
 *  Return: void
 **/
void list_arr_or_dep(char* cId, int iMode) {
	int i, iSize;
	Flight fSortedFlights[MAXFLIGHTS];
	cId[IDAP-1] = '\0';
	if (find_ap(cId) == NOTFOUND) {
		printf("%s: no such airport ID\n", cId);
		return;
	}
	iSize = copy_flights(fSortedFlights, cId, iMode);
	if (iSize != 0) {
		quicksort_fl(fSortedFlights, 0, iSize-1, iMode);
		for (i = 0; i < iSize; i++) {
			print_fl(fSortedFlights[i], iMode);
		}
	}
}

/**
 * Function: advance_date
 * --------------------
 * Advances system date.
 *
 *  Return: void
 **/
void advance_date(char* arg) {
	Date dNewToday;
	read_date(&dNewToday, arg);
	if (invalid_date(dNewToday)) {
		printf("invalid date\n");
		return;
	}
	today = dNewToday;
	printf("%s-%s-%s\n", today.day, today.month, today.year);
}
