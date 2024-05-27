#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Maximum number of reservations
const int maxReservations = 100;

// Struct definition
struct PetHotel {
    char owner[50];
    char ownerID[14];
    char phone[15];
    char petName[20];
    char petType;
    char checkIn[11];
    char checkOut[11];
    int duration;
    char package;
};

// Helper function to clear the input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function prototypes
void mainMenu(struct PetHotel reservations[], int *count);
void addReservation(struct PetHotel reservations[], int *count);
void removeReservationByOwnerID(struct PetHotel reservations[], int *count);
int findReservationIndex(struct PetHotel reservations[], int count, const char ownerID[]);
void searchReservationByOwnerID(struct PetHotel reservations[], int count);
void updateReservationByOwnerID(struct PetHotel reservations[], int count);
void viewAllReservations(struct PetHotel reservations[], int count);
void displayReservationDetails(struct PetHotel reservation);
float calculateTotalPrice(const struct PetHotel *reservation);
float calculateTotalSales(struct PetHotel reservations[], int count);
int countTotalReservations(struct PetHotel reservations[], int count);
float calculateAverageSales(struct PetHotel reservations[], int count);
float calculateAverageSalesByMonth(struct PetHotel reservations[], int count);
int readReservationsFromFile(struct PetHotel reservations[], int *count, const char *filename);
int compareDates(const char *date1, const char *date2);
void sortReservationsByCheckIn(struct PetHotel reservations[], int count);

int main() {
    // Array to store reservations
    struct PetHotel reservations[maxReservations];

    // Current number of reservations
    int reservationCount = 0;

    // Read reservations from a file
    int success = readReservationsFromFile(reservations, &reservationCount, "reservations.txt");
    if (!success) {
        printf("Error reading reservations from file.\n");
        return 1; // Exit with an error code
    }

    mainMenu(reservations, &reservationCount);
    
    return 0;
}

// Function to read reservations from a file
int readReservationsFromFile(struct PetHotel reservations[], int *count, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return 0; // Failed to open the file
    }

    // Read reservation data from the file
    while (fscanf(file, "%s %s %s %s %c %s %s %d %c",
                  reservations[*count].owner,
                  reservations[*count].ownerID,
                  reservations[*count].phone,
                  reservations[*count].petName,
                  &reservations[*count].petType,
                  reservations[*count].checkIn,
                  reservations[*count].checkOut,
                  &reservations[*count].duration,
                  &reservations[*count].package) == 9) {
               
        float totalPrice = calculateTotalPrice(&reservations[*count]);
        printf("\nTotal Price: RM%.2f\n", totalPrice);

        // Increment the count after reading a reservation
        (*count)++;
    }

    fclose(file);
    return 1; // Successful read
}

void mainMenu(struct PetHotel reservations[], int *count) {
	system("cls");
	int choice, i;
   	printf("\t+----------------------------------+\n");
	printf("\t|          PET HOTEL SYSTEM        |\n");
	printf("\t+----------------------------------+\n");
	printf("\t|     1.  Add Reservation          |\n");
	printf("\t+----------------------------------+\n");
	printf("\t|     2.  View Reservation         |\n");
	printf("\t+----------------------------------+\n");
	printf("\t|     3.  Search Reservation       |\n");
	printf("\t+----------------------------------+\n");
	printf("\t|     4.  Update Reservation       |\n");
	printf("\t+----------------------------------+\n");
	printf("\t|     5.  Remove Reservation       |\n");
	printf("\t+----------------------------------+\n");
	printf("\t|     6.  Total / Average sales    |\n");
	printf("\t+----------------------------------+\n");
	printf("\t|     7.     LOG OUT               |\n");
	printf("\t+----------------------------------+\n");
	printf("\t    Choose Menu: ");
	scanf("%d", &choice);
	if (choice == 1){
    	addReservation(reservations, count);
		mainMenu(reservations, count);
	}
	else if (choice == 2){
    	// Sort reservations by check-in date
    	sortReservationsByCheckIn(reservations, *count);
    	// View all reservations after sorting
    	printf("\nReservations are sorted by earliest check-in date:\n\n");
    	viewAllReservations(reservations, *count);
		
    	float totalSales = calculateTotalSales(reservations, *count);
    	printf("\nTotal Sales: RM%.2f\n", totalSales);
    	int totalReservations = countTotalReservations(reservations, *count);
    	printf("Total Reservations: %d\n", totalReservations);
    	printf("\n\t Back to ADMINISTRATION MENU [ENTER].\n");
    	getch();
    	mainMenu(reservations, count);
	}
	else if (choice == 3){
    	searchReservationByOwnerID(reservations, *count);
    	printf("\n\t Back to ADMINISTRATION MENU [ENTER].\n");
		getch();
		mainMenu(reservations, count);
	}
	else if (choice == 4){
    	updateReservationByOwnerID(reservations, *count);
    	printf("\n\t Back to ADMINISTRATION MENU [ENTER].\n");
		getch();
		mainMenu(reservations, count);
	}
	else if (choice == 5){
    	removeReservationByOwnerID(reservations, count);
    	printf("\n\t Back to ADMINISTRATION MENU [ENTER].\n");
		getch();
		mainMenu(reservations, count);
	}
	else if (choice == 6) {
    	float averageSales = calculateAverageSalesByMonth(reservations, *count);
    	printf("\n\t Back to ADMINISTRATION MENU [ENTER].\n");
    	getch();
    	mainMenu(reservations, count);
	}
	else if (choice == 7){
		char answer;
		printf("Are you sure you want to exit the program? (ALL INFO WILL BE RESET)\n");
		printf("\tPress [X] to continue exit the program: ");
		scanf(" %c", answer);
		if (toupper(answer) == 'X'){
			printf("\n\tPROGRAM TERMINATED SUCCESSFULLY!\n");
			exit(0);
		}
		else{
			system("cls");
			mainMenu(reservations, count);
		}
	}
	else {
    	printf("\t\n  Invalid Input! Please choose accordingly.\n");
    	for(i=0; i<40; i++)
    	{
        	printf("_");
        	Sleep(75);
    	}
    	system("cls");
    	mainMenu(reservations, count);
	}
}

// Function to add a reservation
void addReservation(struct PetHotel reservations[], int *count) {
    // Check if there is space for a new reservation
    char add = 'Y';
    while(toupper(add) == 'Y'){
    	if (*count < maxReservations) {
        	struct PetHotel newReservation;
        	// Get input from the user
        	printf("\nEnter Owner name: ");
        	scanf("%s", newReservation.owner);
        	
        	printf("Enter Owner ID: ");
        	scanf("%s", newReservation.ownerID);
        	
        	printf("Enter Phone no: ");
        	scanf("%s", newReservation.phone);
        	
        	printf("Enter Pet name: ");
        	scanf("%s", newReservation.petName);
        	
        	printf("Pet Type [C]at/[D]og: ");
        	scanf(" %c", &newReservation.petType);
        	
			printf("Check-In (DD-MM-YYYY): ");
			scanf("%s", newReservation.checkIn);
			
        	printf("Check-Out (DD-MM-YYYY): ");
			scanf("%s", newReservation.checkOut);
			
			printf("Duration [in days]: ");
        	scanf("%d", &newReservation.duration);
        	
        	printf("Grooming package [Y]es/[N]o: ");
        	scanf(" %c", &newReservation.package);
        	
        	float totalPrice = calculateTotalPrice(&newReservation);
        	printf("\nTotal Price: RM%.2f\n", totalPrice);
        	
        	// Add the new reservation to the array
        	reservations[*count] = newReservation;
        	
        	(*count)++;
        	
        	printf("\nReservation added successfully.\n");
        	printf("\nDo you want to add another reservation [Y]es/[N]o ? : ");
        	scanf(" %c", &add);
    	} else {
        	printf("Error: Maximum number of reservations reached.\n");
    	}
	}
}

float calculateTotalPrice(const struct PetHotel *reservation) {
    float basePrice = 50 * reservation->duration;
    // Additional charges for grooming
    if (toupper(reservation->package) == 'Y') {
        basePrice += 90;
    }
    return basePrice;
}

void viewAllReservations(struct PetHotel reservations[], int count) {
    printf("\nAll Reservations:\n");
    int i;
    for (i = 0; i < count; i++) {
        displayReservationDetails(reservations[i]);
    }
}

void displayReservationDetails(struct PetHotel reservation) {
    printf("\nOwner: %s\n", reservation.owner);
    printf("Owner ID: %s\n", reservation.ownerID);
    printf("Phone: %s\n", reservation.phone);
    printf("Pet Name: %s\n", reservation.petName);
    printf("Pet Type: %s\n", (reservation.petType == 'C' || reservation.petType == 'c') ? "Cat" : "Dog");
    printf("Check-In: %s\n", reservation.checkIn);
    printf("Check-Out: %s\n", reservation.checkOut);
    printf("Duration: %d days\n", reservation.duration);
    printf("Grooming Package: %s\n", (reservation.package == 'Y' || reservation.package == 'y') ? "Yes" : "No");
    // Calculate and display the total price without storing it
    float totalPrice = 50.0 * reservation.duration;
    if (toupper(reservation.package) == 'Y') {
        totalPrice += 90.0;
    }
    printf("\nTotal Price: RM%.2f\n", totalPrice);
}

int findReservationIndex(struct PetHotel reservations[], int count, const char ownerID[]) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(reservations[i].ownerID, ownerID) == 0) {
            return i;
        }
    }
    return -1;
}

void searchReservationByOwnerID(struct PetHotel reservations[], int count) {
    char ownerID[14];
    printf("\nEnter owner's ID to search reservation: ");
    scanf("%s", ownerID);
        
    int index = findReservationIndex(reservations, count, ownerID);

    if (index != -1) {
       printf("\nThe Reservation Details: \n");
        displayReservationDetails(reservations[index]);
    } else {
        printf("Error: Reservation with owner's ID %s not found.\n", ownerID);
    }
}

void updateReservationByOwnerID(struct PetHotel reservations[], int count) {
    char ownerID[14];
    printf("\nEnter owner's ID to update reservation: ");
    scanf("%s", ownerID);
    clearInputBuffer();  // Clear the input buffer

    // Search for the reservation with the specified owner's ID
    int index = findReservationIndex(reservations, count, ownerID);

    // If found, update the reservation
    if (index != -1) {
    	printf("\nCurrent Reservation Details: \n");
        displayReservationDetails(reservations[index]);
    	printf("\n***[ENTER] to keep the current info***\n");
        char buffer[256];  // Assuming a reasonable buffer size

        // Get updated input from the user for each field
        printf("Enter new Owner name: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[0] != '\n') {
            sscanf(buffer, "%s", reservations[index].owner);
        }

        printf("Enter new Phone no: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[0] != '\n') {
            sscanf(buffer, "%s", reservations[index].phone);
        }
        
        printf("Enter new Pet name: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[0] != '\n') {
            sscanf(buffer, "%s", reservations[index].petName);
        }
        
        printf("Enter new Pet type [C]at/[D]og : ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[0] != '\n') {
            sscanf(buffer, " %c", &reservations[index].petType);
        }
        
        printf("Enter new Check-in [DD-MM-YYYY]: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[0] != '\n') {
            sscanf(buffer, "%s", reservations[index].checkIn);
        }
        
        printf("Enter new Check-out [DD-MM-YYYY]: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[0] != '\n') {
            sscanf(buffer, "%s", reservations[index].checkOut);
        }
        
        printf("Enter new Duration [in days]: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[0] != '\n') {
            sscanf(buffer, "%d", &reservations[index].duration);
        }
        
        printf("Enter new Grooming package [Y]/[N]: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[0] != '\n') {
            sscanf(buffer, " %c", &reservations[index].package);
        }

        // Recalculate total price
        float totalPrice = calculateTotalPrice(&reservations[index]);
    	printf("\nUpdated Total Price: RM%.2f\n", totalPrice);
        printf("\nReservation updated successfully.\n");
    } else {
        printf("Error: Reservation with owner's ID %s not found.\n", ownerID);
    }
}

// Function to remove a reservation based on owner's ID
void removeReservationByOwnerID(struct PetHotel reservations[], int *count) {
    char ownerID[14];
    printf("\nEnter owner's ID to remove reservation: ");
    scanf("%s", ownerID);

    // Search for the reservation with the specified owner's ID
    int index = findReservationIndex(reservations, *count, ownerID), i;

    // If found, remove the reservation
    if (index != -1) {
    	displayReservationDetails(reservations[index]);
    	char answer;
    	printf("Are you sure you want to delete this reservation? [Y]es/[N]o:");
    	scanf(" %c", &answer);
    	if (toupper(answer) == 'Y'){
        	// Shift remaining reservations to fill the gap
        	for (i = index; i < *count - 1; i++) {
            	reservations[i] = reservations[i + 1];
        	}

        	// Decrement the count after removing a reservation
        	(*count)--;

        	printf("Reservation removed successfully.\n", ownerID);
    	}
    } else {
        printf("Error: Reservation with owner's ID %s not found.\n", ownerID);
    }
}

// Function to calculate the total sales
float calculateTotalSales(struct PetHotel reservations[], int count) {
    float totalSales = 0.0;
    
    // Sum the total price for each reservation
    int i;
    for (i = 0; i < count; i++) {
        totalSales += calculateTotalPrice(&reservations[i]);
    }
    
    return totalSales;
}

// Function to count total reservations
int countTotalReservations(struct PetHotel reservations[], int count) {
    return count;
}

// Function to calculate the average sales for each month
float calculateAverageSalesByMonth(struct PetHotel reservations[], int count) {
    float totalSalesByMonth[13] = {0.0};  // Index 0 is not used
    int reservationsCountByMonth[13] = {0};  // Index 0 is not used

    // Sum the total price for each reservation and count for each month
    int i;
    for (i = 0; i < count; i++) {
        int month;
        sscanf(reservations[i].checkIn, "%*d-%d-%*d", &month);
        totalSalesByMonth[month] += calculateTotalPrice(&reservations[i]);
        reservationsCountByMonth[month]++;
    }

    // Display average sales for each month
    for (i = 1; i <= 12; i++) {
        if (reservationsCountByMonth[i] > 0) {
            float averageSales = totalSalesByMonth[i] / reservationsCountByMonth[i];
            printf("\n+----------------------------------------+\n");
            printf("|   Total Sales for Month %02d: RM%.2f   |\n", i, totalSalesByMonth[i]);
            printf("|  Average Sales for Month %02d: RM%.2f  |\n", i, averageSales);
            printf("+----------------------------------------+");
		}      
		else {
            printf("\n+----------------------------------------+\n");
			printf("|      No reservations for month %02d      |\n", i);
			printf("+----------------------------------------+");
		}
	}
}

// Function to compare two reservation dates
int compareDates(const char *date1, const char *date2) {
    // Assuming date format: DD-MM-YYYY
    int year1, month1, day1;
    int year2, month2, day2;

    sscanf(date1, "%d-%d-%d", &day1, &month1, &year1);
    sscanf(date2, "%d-%d-%d", &day2, &month2, &year2);

    // Compare years
    if (year1 < year2) {
        return -1;
    } else if (year1 > year2) {
        return 1;
    }

    // Compare months
    if (month1 < month2) {
        return -1;
    } else if (month1 > month2) {
        return 1;
    }

    // Compare days
    if (day1 < day2) {
        return -1;
    } else if (day1 > day2) {
        return 1;
    }

    // Dates are equal
    return 0;
}

// Function to sort reservations by check-in date using bubble sort
void sortReservationsByCheckIn(struct PetHotel reservations[], int count) {
    int i, j;
    struct PetHotel temp;

    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            // Compare check-in dates and swap if needed
            if (compareDates(reservations[j].checkIn, reservations[j + 1].checkIn) > 0) {
                temp = reservations[j];
                reservations[j] = reservations[j + 1];
                reservations[j + 1] = temp;
            }
        }
    }
}
