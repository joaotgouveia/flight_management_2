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
	char* id;
	int size;
} Reservation;

typedef struct Node {
	Reservation* res;
	struct Node* next;
} *Link;
typedef struct Node Node;

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
	Link headRes;
	int totRes;
	int listLen;
} Flight;
