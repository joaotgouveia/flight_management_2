/* Joao Gouveia - 102611
 * File containing project code */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
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
Airport aAirports[MAXAIRPORTS]; /* Airport array */
Flight fFlights[MAXFLIGHTS]; /* Flight array */
int iCurrentAirports, iCurrentFlights; /* Airport and flight counters */
Date today = {"01", "01", "2022"}; /* Program starting date */

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
	if (strcmp(cDate, cFuture) > 0 || strcmp(cDate, cToday) < 0) {
		printf("invalid date\n");
		return TRUE;
	}
	return FALSE;
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
	fNewFlight.listLen = 0;
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
	for (i = 0; i < iCurrentFlights; i++)
		free_all(fFlights[i].headRes);
}

/**
 * Function: check_mem
 * --------------------
 * Checks for available memory
 * before allocating.
 *
 *  Return: void
 **/
void* check_mem(int iSize) {
	void* ptr = malloc(iSize);
	if (ptr == NULL) {
		printf("no memory\n");
		free(ptr);
		free_flights();
		exit(0);
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
 *  Return: void
 **/
Link new_link(char* cIdRes, int iIdLen, int iResSize) {
	Link link = (Link) check_mem(sizeof(Node));
	link->res = (Reservation*) check_mem(sizeof(Reservation));
	link->res->id = (char*) check_mem(sizeof(char)*(iIdLen+1));
	strncpy(link->res->id, cIdRes, iIdLen);
	link->res->id[iIdLen] = '\0';
	link->res->size = iResSize;
	link->next = NULL;
	return link;
}

/**
 * Function: copy_linked_list
 * --------------------
 * Copies reservations from a
 * linked list.
 *
 *  Return: void
 **/
void copy_linked_list(Reservation* rArray, Link head) {
	int i = 0;
	Link link;
	for (link = head, i = 0; link != NULL; link = link->next, i++)
		rArray[i] = *link->res;
}

/**
 * Function: get_tail
 * --------------------
 * Finds the tail
 * of a linked list.
 *
 *  Return: Link
 **/
Link get_tail(Link head) {
	Link tail;
	for (tail = head; tail->next != NULL; tail = tail->next);
	return tail;
}

/**
 * Function: print_rs
 * --------------------
 * Prints a reservation.
 *
 *  Return: void
 **/
void print_rs(Reservation rRes) {
	printf("%s %d\n", rRes.id, rRes.size); 
}

/**
 * Function: less_rs
 * --------------------
 * Compares two reservations
 * through their Id's.
 *
 *  Return: int
 **/
int less_rs(Reservation rReservation1, Reservation rReservation2) {
	return strcmp(rReservation1.id, rReservation2.id) <= 0;
}

/**
 * Function: swap_rs
 * --------------------
 * Swaps two reservations in 
 * an array.
 *
 *  Return: void
 **/
void swap_rs(Reservation* rArray, int i, int j) {
	Reservation rAux;
	if (i == j)
		return;
	rAux = rArray[i];
	rArray[i] = rArray[j];
	rArray[j] = rAux;
}

/**
 * Function: partition_rs
 * --------------------
 * Calculates and returns a partition
 * for quicksort.
 *
 *  Return: int
 **/
int partition_rs(Reservation* rArray, int iFirst, int iLast) {
	int iPivot = iFirst, i = iFirst, j = iLast;
	while (i < j) {
		while (less_rs(rArray[i], rArray[iPivot]) && i < iLast)
			i++;
		while (!less_rs(rArray[j], rArray[iPivot]))
			j--;
		if (i < j)
			swap_rs(rArray, i, j);
	}
	swap_rs(rArray, iPivot, j);
	return j;
}

/**
 * Function: quicksort_rs
 * --------------------
 *  Quicksort sorting algorithm
 *  applied to reservation struct.
 *
 *  Return: void
 **/
void quicksort_rs(Reservation* rArray, int iFirst, int iLast) {
	int iPartition;
	if (iFirst < iLast) {
		iPartition = partition_rs(rArray, iFirst, iLast);
		quicksort_rs(rArray, iFirst, iPartition-1);
		quicksort_rs(rArray, iPartition+1, iLast);
	}
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
 * Function: find_rs
 * --------------------
 * Finds the reservation
 * corresponding to an id.
 *
 *  Return: int
 **/
Link find_rs(Link head, char* cId) {
	Link link;
	for (link = head; link != NULL; link = link->next)
		if (strcmp(link->res->id, cId) == 0)
			return link;
	return NULL;
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
	if (!isupper(cIdChar) && !isdigit(cIdChar)) {
		printf("invalid reservation code\n");
		return TRUE;
	}
	return FALSE;
}

/**
 * Function: used_idfl
 * --------------------
 * Checks if a reservation
 * ID was already used in
 * a flight.
 *
 *  Return: int
 **/
int used_idfl(Flight fFlight, char* cId) {
	if (find_rs(fFlight.headRes, cId) != NULL) {
		printf("%s: flight reservation already used\n", cId);
		return TRUE;
	}
	return FALSE;
}

/**
 * Function: used_id
 * --------------------
 * Checks if a reservation
 * ID was already used.
 *
 *  Return: int
 **/
int used_id(char* arg, int iIdLen) {
	int i, iStatus = FALSE;
	char* cId = (char*) check_mem(sizeof(char)*(iIdLen+1));
	strncpy(cId, arg, iIdLen);
	cId[iIdLen] = '\0';
	for (i = 0; i < iCurrentFlights; i++) {
		if (used_idfl(fFlights[i], cId)) {
			iStatus = TRUE;
			break;
		}
	}
	free(cId);
	return iStatus;
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
	if (*iInd == NOTFOUND) {
		return FALSE;
	}
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
	if (*iSize < 10) {
		printf("invalid reservation code\n");
		return FALSE;
	}
	return TRUE;
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
	if (iSize < 0) {
		printf("invalid passenger number\n");
		return FALSE;
	}
	return TRUE;
}

/**
 * Function: list_rs
 * --------------------
 * Lists all reservations
 * from a flight.
 *
 *  Return: void
 **/
void list_rs(Link lHead, int iLen) {
	int i;
	Reservation* rArray;
	if (lHead == NULL)
		return;
	rArray = (Reservation*) check_mem(sizeof(Reservation)*iLen);
	copy_linked_list(rArray, lHead);
	quicksort_rs(rArray, 0, iLen-1);
	for (i = 0; i < iLen; i++)
		print_rs(rArray[i]);
	free(rArray);
}

/**
 * Function: new_rs
 * --------------------
 * Creates a new reservation
 * and adds it to a flight.
 *
 *  Return: void
 **/
void new_rs(int iIndex, char* cId, int iIdLen, int iPassengers) {
	Link lTail, lNewLink;
	lNewLink = new_link(cId, iIdLen, iPassengers);
	fFlights[iIndex].totRes += lNewLink->res->size;
	fFlights[iIndex].listLen++;
	if (fFlights[iIndex].headRes == NULL) {
		fFlights[iIndex].headRes = lNewLink;
		return;
	}
	lTail = get_tail(fFlights[iIndex].headRes);
	lTail->next = lNewLink;
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
	list_rs(fFlights[iIndex].headRes, fFlights[iIndex].listLen);
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
void add_rs(int iValidFl, int iIndex, char* cIdFl, Date dDate, char* arg) {
	int iIdLen, iPassengers;
	if (!check_idrs(&iIdLen, arg))
		return;
	if (!valid_fl(cIdFl, iValidFl))
		return;
	if (used_id(arg, iIdLen))
		return;
	if (!check_passengers(&iPassengers, fFlights[iIndex], arg+iIdLen+1))
		return;
	if (invalid_date(dDate))
		return;
	if (!check_size(iPassengers))
		return;
	new_rs(iIndex, arg, iIdLen, iPassengers);
}

/**
 * Function: manage_reservations
 * --------------------
 * Lists all the reservations
 * of a flight or adds a new one.
 *
 *  Return: void
 **/
void manage_reservations(char* arg) {
	int iInd, iOffset, iValidFl, iArgLen = strlen(arg);
	char cIdFl[IDFL];
	Date dDate;
	iValidFl = check_flight(cIdFl, &iInd, &dDate, &iOffset, arg);
	if (iArgLen == iOffset)
		show_rs(iValidFl, iInd, cIdFl, dDate);
	else
		add_rs(iValidFl, iInd, cIdFl, dDate, arg+iOffset);
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
int remove_rs(Flight* fFlight, char* cId) {
	Link link, prev;
	if (fFlight->headRes == NULL)
		return FALSE;
	for (link = fFlight->headRes, prev = NULL; link != NULL; prev = link, link = link->next)
		if (strcmp(link->res->id, cId) == 0) {
			if (link == fFlight->headRes)
				fFlight->headRes = fFlight->headRes->next;
			else
				prev->next = link->next;
			fFlight->totRes -= link->res->size;
			fFlight->listLen--;
			free_link(link);
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
void delete_rs(char* cId) {
	int i;
	for (i = 0; i < iCurrentFlights; i++)
		if (remove_rs(fFlights+i, cId))
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
void remove_fl(int iIndex) {
	int i;
	free_all(fFlights[iIndex].headRes);
	for (i = iIndex+1; i < iCurrentFlights; i++)
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
void delete_fl(char* cId) {
	int i;
	if (!find_flid(&i, cId)) {
		printf("not found\n");
		return;
	}
	do
		remove_fl(i);
	while (find_flid(&i, cId));
}

/**
 * Function: delete_fl_or_rs
 * --------------------
 * Deletes a flight or a reservation.
 *
 *  Return: void
 **/
void delete_fl_or_rs(char* cId) {
	int iLen = strlen(cId) - 1;
	cId[iLen] = '\0';
	if (iLen < 10)
		delete_fl(cId);
	else
		delete_rs(cId);
}

int main () {
	char arg[ARGSIZE];
	do {
		fgets(arg, sizeof(char)*ARGSIZE, stdin);
		switch (arg[0]) {
			case 'q': break;
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
			case 'r': manage_reservations(arg+ARGSTART);
				break;
			case 'e': delete_fl_or_rs(arg+ARGSTART);
				break;
		}
	} while (arg[0] != 'q');
	free_flights();
	return 0;
}

/* Atencao ao malloc do array de reservas substituido por check mem.
 * ta na funcao pa listar as reservas. se der wrong answer pode ser por esse malloc desnecessario
 * exceder o limite de memoria que posso usar. bom teste e tirar o memcheck e ver se o erro passa
 * a segmentation error */

