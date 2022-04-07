/* Joao Gouveia - 102611
 * File defining structs */

typedef struct {
	char day[DAY];
	char month[MONTH];
	char year[YEAR];
} Date;

typedef struct {
	char hours[HOURS];
	char mins[MINS];
} Time;

typedef struct {
	char id[IDAP];
	char country[COUNTRY];
	char city[CITY];
	int departures;
} Airport;

typedef struct {
	char id[IDFL];
	char departure[IDAP];
	char arrival[IDAP];
	Date date;
	Date arrDate;
	Time time;
	Time arrTime;
	int duration[TUPLE];
	int capacity;
} Flight;
