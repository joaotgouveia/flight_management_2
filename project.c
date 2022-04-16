/* Joao Gouveia - 102611
 * File containing project code */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"const.h"
#include"structs.h"

/* Project 1 functions */
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
const int mDays[] =	/* days per month (jan=1) */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
Airport aAirports[MAXAIRPORTS]; /* Airport array */
Flight fFlights[MAXFLIGHTS]; /* Flight array */
int iCurrentAirports, iCurrentFlights; /* Airport and flight counters */
Date today = {"01", "01", "2022"}; /* Program starting date */

int is_upper_char(char c) {
	return (c >= 'A' && c <= 'Z');
}

int is_digit_char(char d) {
	return (d >= '0' && d <= '9');
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
	if (is_upper_char(cChar) && iIndex < 2) {
		return FALSE;
	}
	return (!is_digit_char(cChar) || iIndex < 2);
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
 * Function: convert_date
 * --------------------
 * COnverts a date to int.
 *
 *  Return: int
 **/
int convert_date(int y, int m, int d) {
	return (y-FIRSTYEAR)*YEARDAYS+mDays[m-1]+d-1;
}

/**
 * Function: invalid_date
 * --------------------
 * Checks if a date is invalid.
 *
 *  Return: int
 **/
int invalid_date(Date d) {
	int iDate = convert_date(atoi(d.year), atoi(d.month), atoi(d.day));
	if (!(iDate < convert_date(atoi(today.year), atoi(today.month), atoi(today.day)) ||
		iDate >  convert_date(atoi(today.year)+1, atoi(today.month), atoi(today.day))))
		return FALSE;
	printf("invalid date\n");
	return TRUE;
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
		if (!is_upper_char(arg[i])) {
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
	fNewFlight.capacity = atoi(arg);
	if(fNewFlight.capacity < 10) {
		printf("invalid capacity\n");
		return;
	}
	/* Calculating and storing arrival date */
	fill_arrival_date(&fNewFlight);
	/* Initializing linked list */
	fNewFlight.headRes = NULL;
	fNewFlight.totRes = 0;
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
		return;
	}
	today = dNewToday;
	printf("%s-%s-%s\n", today.day, today.month, today.year);
}

/* --------------------- Start of second project ---------------------- */

/**
 * Function: free_link
 * --------------------
 * Frees a link.
 *
 *  Return: void
 **/
void free_link(Link lLink) {
	free(lLink->res->id);
	free(lLink->res);
	free(lLink);
}

/**
 * Function: free_all
 * --------------------
 * Frees an entire linked list.
 *
 *  Return: void
 **/
void free_all(Link head) {
	Link link, prev;
	if (head == NULL)
		return;
	for (link = head->next, prev = head; link != NULL; prev = link, link = link->next)
		free_link(prev);
	free_link(prev);
}

/**
 * Function: free_flights
 * --------------------
 * Frees fFlights.
 *
 *  Return: void
 **/
void free_flights() {
	int i;
	if (iCurrentFlights == 0)
		return;
	for (i = 0; i < iCurrentFlights; i++)
		free_all(fFlights[i].headRes);
}

/**
 * Function: free_chains
 * --------------------
 * Frees an entire 
 * chain linked list.
 *
 *  Return: void
 **/
void free_chains(Chain head) {
	Chain chain, prev;
	if (head == NULL)
		return;
	for (chain = head->next, prev = head; chain != NULL; prev = chain, chain = chain->next)
		free(prev);
	free(prev);
}

/**
 * Function: free_hash
 * --------------------
 * Frees an hash table.
 *
 *  Return: void
 **/
void free_hash(Chain* table) {
	int i;
	if (iCurrentFlights != 0)
		for (i = 0; i < TABLESIZE; i++)
			free_chains(table[i]);
	free(table);
}

/**
 * Function: check_mem
 * --------------------
 * Checks for available memory
 * before allocating.
 *
 *  Return: void
 **/
void* check_mem(Chain* t, unsigned int iSize) {
	void* ptr = malloc(iSize);
	if (ptr == NULL) {
		printf("No memory\n");
		free_hash(t);
		free_flights();
		exit(TOO_MUCH_MEMORY);
	}
	return ptr;
}

/**
 * Function: new_link
 * --------------------
 * Creates a new link
 * for the reservations
 * linked list.
 *
 *  Return: Link
 **/
Link new_link(Chain* t, char* cIdRes, int iIdLen, int iResSize) {
	Link link = (Link) check_mem(t, sizeof(Node));
	link->res = (Reservation*) check_mem(t, sizeof(Reservation));
	link->res->id = (char*) check_mem(t, sizeof(char)*(iIdLen+1));
	strncpy(link->res->id, cIdRes, iIdLen);
	link->res->id[iIdLen] = '\0';
	link->res->size = iResSize;
	link->next = NULL;
	return link;
}

/**
 * Function: new_chain
 * --------------------
 * Creates a new chain
 * for the hash table.
 *
 *  Return: Chain
 **/
Chain new_chain(Chain* t, char* id, int index) {
	Chain chain = (Chain) check_mem(t, sizeof(HashNode));
	chain->id = id;
	chain->iFl = index;
	chain->next = NULL;
	return chain;
}

/**
 * Function: hash
 * --------------------
 *  String hash funtion.
 *
 *  Return: int
 **/
int hash(char* id) {
	int h, a = 31415, b = 27183;
	for (h = 0; *id != '\0'; id++, a = a*b % (TABLESIZE-1))
		h = (a*h + *id) % TABLESIZE;
	return h;
}

/**
 * Function: insert_chain
 * --------------------
 * Inserts a new chain in a 
 * linked list;
 *
 *  Return: Chain
 **/
Chain insert_chain(Chain head, Chain newChain) {
	if (head == NULL)
		return newChain;
	newChain->next = head;
	return newChain;
}

/**
 * Function: insert_table
 * --------------------
 * Inserts an id in a hash
 * table.
 *
 *  Return: void
 **/
void insert_table(Chain* table, char* id, int index) {
	int i = hash(id);
	Chain newChain = new_chain(table, id, index);
	table[i] = insert_chain(table[i], newChain);
}

/**
 * Function: chain_search
 * --------------------
 * Looks for an id in a chain
 * linked list.
 *
 *  Return: int
 **/
int chain_search(Chain head, char* id) {
	Chain chain;
	if (head == NULL)
		return FALSE;
	for (chain = head; chain != NULL; chain = chain->next)
		if (strcmp(chain->id, id) == 0)
			return TRUE;
	return FALSE;
}

/**
 * Function: table_search
 * --------------------
 * Looks for an id in an
 * hash table.
 *
 *  Return: int
 **/
int table_search(Chain* table, char* id) {
	int iStatus = chain_search(table[hash(id)], id);
	if (iStatus)
		printf("%s: flight reservation already used\n", id);
	return iStatus;
}

/**
 * Function: print_rs
 * --------------------
 * Prints a reservation.
 *
 *  Return: void
 **/
void print_rs(Reservation* rRes) {
	printf("%s %d\n", rRes->id, rRes->size); 
}

/**
 * Function: read_fl
 * --------------------
 * Reads a flight id
 * from a string. Returns
 * the id size.
 *
 *  Return: int
 **/
int read_fl(char* cId, char* arg) {
	int i;
	for (i = 0; arg[i] != ' '; i++)
		cId[i] = arg[i];
	cId[i] = '\0';
	return i+1;
}

/**
 * Function: find_fl
 * --------------------
 * Finds a Flight in fFlights
 * and returns its index. Returns
 * NOTFOUND if the flight doesn't
 * exist.
 *
 *  Return: int
 **/
int find_fl(char cId[IDFL], Date dDate) {
	int i;
	for (i = 0; i < iCurrentFlights; i++)
		if (strcmp(cId, fFlights[i].id) == 0 && same_date(dDate, fFlights[i].date))
			return i;
	return NOTFOUND;
}

/**
 * Function: invalid_idrs
 * --------------------
 * Checks if a reservation
 * ID is invalid.
 *
 *  Return: int
 **/
int invalid_idrs(char cIdChar) {
	if (is_upper_char(cIdChar) || is_digit_char(cIdChar))
		return FALSE;
	printf("invalid reservation code\n");
	return TRUE;
}

/**
 * Function: used_id
 * --------------------
 * Checks if a reservation
 * ID was already used.
 *
 *  Return: int
 **/
int used_id(char* arg, Chain* table, int iIdLen) {
	arg[iIdLen] = '\0';
	return table_search(table, arg);
}

/**
 * Function: invalid_passengers
 * --------------------
 * Checks if a reservation
 * exceeds the flight capacity.
 *
 *  Return: int
 **/
int invalid_passengers(Flight fFlight, int iResSize) {
	if (iResSize + fFlight.totRes > fFlight.capacity) {
		printf("too many reservations\n");
		return TRUE;
	}
	return FALSE;
}

/**
 * Function: valid_fl
 * --------------------
 * Checks if a flight is invalid
 * and handles printing the error message.
 *
 *  Return: int
 **/
int valid_fl(char* cId, int iValidFl) {
	if (!iValidFl) {
		printf("%s: flight does not exist\n", cId);
		return FALSE;
	}
	return TRUE;
}

/**
 * Function: check_flight
 * --------------------
 * Checks if a flight ID and it's
 * date are valid. Stores it's index
 * in fFlights in iInd and it's
 * date in dDate.
 *
 *  Return: int
 **/
int check_flight(char* cId, int* iInd, Date* dDate, int* iOffset, char* arg) {
	*iOffset = read_fl(cId, arg);
	arg += *iOffset;
	read_date(dDate, arg);
	*iOffset += DATE;
	*iInd = find_fl(cId, *dDate);
	if (*iInd == NOTFOUND)
		return FALSE;
	return TRUE;
}

/**
 * Function: check_idrs
 * --------------------
 * Checks if a reservation ID
 * is valid. Stores it's size
 * in iSize.
 *
 *  Return: int
 **/
int check_idrs(int* iSize, char* arg) {
	for (*iSize = 0; arg[*iSize] != ' ' && arg[*iSize] != '\t'; (*iSize)++)
		if (invalid_idrs(arg[*iSize]))
			return FALSE;
	if (*iSize >= 10)
		return TRUE;
	printf("invalid reservation code\n");
	return FALSE;
}

/**
 * Function: check_passengers
 * --------------------
 * Checks if a flight allows
 * the desired number of passengers.
 * Stores the number of passengers
 * in iPassengers.
 *
 *  Return: int
 **/
int check_passengers(int* iPassengers, Flight fFlight, char* arg) {
	*iPassengers = atoi(arg);
	if (invalid_passengers(fFlight, *iPassengers))
		return FALSE;
	return TRUE;
}

/**
 * Function: check_size
 * --------------------
 * Checks checks if the provided
 * size is a positive integer.
 *
 *  Return: int
 **/
int check_size(int iSize) {
	if (iSize > 0)
		return TRUE;
	printf("invalid passenger number\n");
	return FALSE;
}

/**
 * Function: list_rs
 * --------------------
 * Lists all reservations
 * from a flight.
 *
 *  Return: void
 **/
void list_rs(Link head) {
	Link l;
	if (head == NULL)
		return;
	for (l = head; l != NULL; l = l->next)
		print_rs(l->res);
}

/**
 * Function: less_ln
 * --------------------
 *  Compares two links
 *  through their ids.
 *
 *  Return: Link
 **/
int less_ln(Link l1, Link l2) {
	return strcmp(l1->res->id, l2->res->id) < 0;
}

/**
 * Function: insert_link
 * --------------------
 * Inserts a new link in a 
 * ordered linked list 
 * according to its id.
 *
 *  Return: Link
 **/
Link insert_link(Link head, Link new) {
	Link l, prev;
	if (less_ln(new, head)) {
		new->next = head;
		return new;
	}
	else if (head->next == NULL) {
		head->next = new;
		return head;
	}
	for (l = head->next, prev = head; l != NULL; prev = l, l = l->next)
		if (less_ln(prev, new) && less_ln(new, l)) {
			prev->next = new;
			new->next = l;
			return head;
		}
	prev->next = new;
	return head;
}

/**
 * Function: new_rs
 * --------------------
 * Creates a new reservation
 * and adds it to a flight.
 *
 *  Return: void
 **/
void new_rs(Chain* table, int i, char* cId, int iIdLen, int iPassengers) {
	Link lNewLink;
	lNewLink = new_link(table, cId, iIdLen, iPassengers);
	fFlights[i].totRes += lNewLink->res->size;
	insert_table(table, lNewLink->res->id, i);
	if (fFlights[i].headRes == NULL) {
		fFlights[i].headRes = lNewLink;
		return;
	}
	fFlights[i].headRes = insert_link(fFlights[i].headRes, lNewLink);
}

/**
 * Function: show_rs
 * --------------------
 * Checks user input,
 * creates a new reservation
 * and adds it to a flight.
 *
 *  Return: void
 **/
void show_rs(int iValidFl, int iIndex, char* cIdFl, Date dDate) {
	if (!valid_fl(cIdFl, iValidFl))
		return;
	if (invalid_date(dDate))
		return;
	list_rs(fFlights[iIndex].headRes);
}

/**
 * Function: add_rs
 * --------------------
 * Checks user input,
 * creates a new reservation
 * and adds it to a flight.
 *
 *  Return: void
 **/
void add_rs(Chain* table, int iValFl, int i, char* cIdFl, Date d, char* arg) {
	int iIdLen, iPassengers;
	if (!check_idrs(&iIdLen, arg))
		return;
	if (!valid_fl(cIdFl, iValFl))
		return;
	if (used_id(arg, table, iIdLen))
		return;
	if (!check_passengers(&iPassengers, fFlights[i], arg+iIdLen+1))
		return;
	if (invalid_date(d))
		return;
	if (!check_size(iPassengers))
		return;
	new_rs(table, i, arg, iIdLen, iPassengers);
}

/**
 * Function: manage_reservations
 * --------------------
 * Lists all the reservations
 * of a flight or adds a new one.
 *
 *  Return: void
 **/
void manage_reservations(char* arg, Chain* table) {
	int iInd, iOffset, iValidFl, iArgLen = strlen(arg);
	char cIdFl[IDFL];
	Date dDate;
	iValidFl = check_flight(cIdFl, &iInd, &dDate, &iOffset, arg);
	if (iArgLen == iOffset)
		show_rs(iValidFl, iInd, cIdFl, dDate);
	else
		add_rs(table, iValidFl, iInd, cIdFl, dDate, arg+iOffset);
}

/**
 * Function: remove_table
 * --------------------
 * Removes a reservation from
 * an hash table.
 *
 *  Return: Chain*
 **/
Chain* remove_table(Chain* table, char* id) {
	int i = hash(id);
	Chain chain, prev;
	if (table[i] == NULL)
		return table;
	for (chain = table[i], prev = NULL; chain != NULL; prev = chain, chain = chain->next)
		if (strcmp(chain->id, id) == 0) {
			if (chain == table[i])
				table[i] = table[i]->next;
			else
				prev->next = chain->next;
			free(chain);
			return table;
		}
	return table;
}

/**
 * Function: find_flIndex
 * --------------------
 * Finds the index of a flight using
 * an hash table.
 *
 *  Return: int
 **/
int find_flIndex(Chain* table, char* id) {
	int h = hash(id), i = NOTFOUND;
	Chain chain;
	if (table[h] == NULL)
		return NOTFOUND;
	for (chain = table[h]; chain != NULL; chain = chain->next)
		if (strcmp(chain->id, id) == 0) {
			i = chain->iFl;
			break;
		}
	return i;
}

/**
 * Function: remove_rs
 * --------------------
 * Removes a reservation from
 * a given linked list.
 * Returns true if it was
 * successeful removing it.
 *
 *  Return: int
 **/
int remove_rs(Chain* table, char* cId) {
	int i = find_flIndex(table, cId);
	Link link, prev;
	if (i == NOTFOUND)
		return FALSE;
	for (link = fFlights[i].headRes, prev = NULL; link != NULL; prev = link, link = link->next)
		if (strcmp(link->res->id, cId) == 0) {
			if (link == fFlights[i].headRes)
				fFlights[i].headRes = fFlights[i].headRes->next;
			else
				prev->next = link->next;
			fFlights[i].totRes -= link->res->size;
			free_link(link);
			remove_table(table, cId);
			return TRUE;
		}  
	return FALSE;
}

/**
 * Function: delete_rs
 * --------------------
 * Finds and deletes a reservation.
 *
 *  Return: void
 **/
void delete_rs(Chain* table, char* cId) {
	if (remove_rs(table, cId))
		return;
	printf("not found\n");
}

/**
 * Function: find_flid
 * --------------------
 * Finds an instance of 
 * a flight id. Returns true
 * if successeful and stores
 * it's index in i.
 *
 *  Return: void
 **/
int find_flid(int* i, char* cId) {
	for (*i = 0; *i < iCurrentFlights; (*i)++)
		if (strcmp(fFlights[*i].id, cId) == 0)
			return TRUE;
	return FALSE;
}

/**
 * Function: remove_fl
 * --------------------
 * Removes an index from
 * fFlights and frees it's
 * linked list.
 *
 *  Return: void
 **/
void remove_fl(Chain* table, int iInd) {
	int i;
	Link l;
	if (fFlights[iInd].headRes != NULL)
		for (l = fFlights[iInd].headRes; l != NULL; l = l->next)
			table = remove_table(table, l->res->id);
	free_all(fFlights[iInd].headRes);
	for (i = iInd+1; i < iCurrentFlights; i++)
		fFlights[i-1] = fFlights[i];
	iCurrentFlights--;
}

/**
 * Function: delete_fl
 * --------------------
 * Finds and deletes a flight.
 *
 *  Return: void
 **/
void delete_fl(Chain* table, char* cId) {
	int i;
	if (!find_flid(&i, cId)) {
		printf("not found\n");
		return;
	}
	do
		remove_fl(table, i);
	while (find_flid(&i, cId));
}

/**
 * Function: delete_fl_or_rs
 * --------------------
 * Deletes a flight or a reservation.
 *
 *  Return: void
 **/
void delete_fl_or_rs(char* cId, Chain* table) {
	int iLen = strlen(cId) - 1;
	cId[iLen] = '\0';
	if (iLen < 10)
		delete_fl(table, cId);
	else
		delete_rs(table, cId);
}

int main () {
	char arg[ARGSIZE];
	Chain* hashTable = (Chain*) calloc(TABLESIZE, sizeof(Chain));
	while (fgets(arg, sizeof(char)*ARGSIZE, stdin)) {
		switch (arg[0]) {
			case 'q': 
				free_hash(hashTable);
				free_flights();
				return 0;
			case 'a': add_ap(arg+ARGSTART);
				break;
			case 'l':
				if (strlen(arg) == ARGSTART)
					list_all_ap();
				else
					list_ap(arg+ARGSTART);
				break;
			case 'v':
				if (strlen(arg) == ARGSTART)
					list_all_fl();
				else
					add_fl(arg+ARGSTART);
				break;
			case 'p': list_arr_or_dep(arg+ARGSTART, FALSE);
				break;
			case 'c': list_arr_or_dep(arg+ARGSTART, TRUE);
				break;
			case 't': advance_date(arg+ARGSTART);
				break;
			case 'r': manage_reservations(arg+ARGSTART, hashTable);
				break;
			case 'e': delete_fl_or_rs(arg+ARGSTART, hashTable);
				break;
		}
	}
	return 0;
}
