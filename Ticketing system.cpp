#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEATS 50

struct Ticket {
    int ticketID;
    char name[50];
    char source[30];
    char destination[30];
    int seatNo;
};

// Function declarations
void addTicket();
void viewTickets();
void searchTicket();
void updateTicket();
void deleteTicket();
int isSeatBooked(int seatNo);
void showAvailableSeats();

// ?? Check seat availability
int isSeatBooked(int seatNo) {
    FILE *fp = fopen("tickets.dat", "rb");
    struct Ticket t;

    if (seatNo < 1 || seatNo > MAX_SEATS)
        return 1;

    if (fp == NULL)
        return 0; // no file means no bookings

    while (fread(&t, sizeof(struct Ticket), 1, fp)) {
        if (t.seatNo == seatNo) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// ?? Show available seats
void showAvailableSeats() {
    printf("\nAvailable Seats:\n");
    for (int i = 1; i <= MAX_SEATS; i++) {
        if (!isSeatBooked(i))
            printf("%d ", i);
    }
    printf("\n");
}

// ? CREATE
void addTicket() {
    FILE *fp = fopen("tickets.dat", "ab");
    struct Ticket t;

    printf("\nEnter Ticket ID: ");
    scanf("%d", &t.ticketID);

    printf("Enter Name: ");
    scanf(" %[^\n]", t.name);

    printf("Enter Source: ");
    scanf("%s", t.source);

    printf("Enter Destination: ");
    scanf("%s", t.destination);

    do {
        printf("Enter Seat No (1-50): ");
        scanf("%d", &t.seatNo);

        if (isSeatBooked(t.seatNo))
            printf("? Seat not available! Try again.\n");

    } while (isSeatBooked(t.seatNo));

    fwrite(&t, sizeof(struct Ticket), 1, fp);
    fclose(fp);

    printf("? Ticket Booked Successfully!\n");
}

// ?? READ
void viewTickets() {
    FILE *fp = fopen("tickets.dat", "rb");
    struct Ticket t;

    if (fp == NULL) {
        printf("\nNo tickets found.\n");
        return;
    }

    printf("\n--- All Tickets ---\n");

    while (fread(&t, sizeof(struct Ticket), 1, fp)) {
        printf("\nID: %d\nName: %s\nFrom: %s\nTo: %s\nSeat: %d\n",
               t.ticketID, t.name, t.source, t.destination, t.seatNo);
    }

    fclose(fp);
}

// ?? SEARCH
void searchTicket() {
    FILE *fp = fopen("tickets.dat", "rb");
    struct Ticket t;
    int id, found = 0;

    if (fp == NULL) {
        printf("\nNo data available.\n");
        return;
    }

    printf("Enter Ticket ID: ");
    scanf("%d", &id);

    while (fread(&t, sizeof(struct Ticket), 1, fp)) {
        if (t.ticketID == id) {
            printf("\nTicket Found!\n");
            printf("Name: %s\nFrom: %s\nTo: %s\nSeat: %d\n",
                   t.name, t.source, t.destination, t.seatNo);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("? Ticket Not Found!\n");

    fclose(fp);
}

// ?? UPDATE
void updateTicket() {
    FILE *fp = fopen("tickets.dat", "rb+");
    struct Ticket t;
    int id, found = 0;

    if (fp == NULL) {
        printf("\nNo data available.\n");
        return;
    }

    printf("Enter Ticket ID to update: ");
    scanf("%d", &id);

    while (fread(&t, sizeof(struct Ticket), 1, fp)) {
        if (t.ticketID == id) {

            printf("Enter New Name: ");
            scanf(" %[^\n]", t.name);

            printf("Enter New Source: ");
            scanf("%s", t.source);

            printf("Enter New Destination: ");
            scanf("%s", t.destination);

            int newSeat;
            do {
                printf("Enter New Seat No (1-50): ");
                scanf("%d", &newSeat);

                if (newSeat != t.seatNo && isSeatBooked(newSeat))
                    printf("? Seat already booked!\n");

            } while (newSeat != t.seatNo && isSeatBooked(newSeat));

            t.seatNo = newSeat;

            fseek(fp, -(long)sizeof(struct Ticket), SEEK_CUR);
            fwrite(&t, sizeof(struct Ticket), 1, fp);

            printf("? Ticket Updated!\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("? Ticket Not Found!\n");

    fclose(fp);
}

// ? DELETE
void deleteTicket() {
    FILE *fp = fopen("tickets.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    struct Ticket t;
    int id, found = 0;

    if (fp == NULL) {
        printf("\nNo data available.\n");
        return;
    }

    printf("Enter Ticket ID to delete: ");
    scanf("%d", &id);

    while (fread(&t, sizeof(struct Ticket), 1, fp)) {
        if (t.ticketID != id) {
            fwrite(&t, sizeof(struct Ticket), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("tickets.dat");
    rename("temp.dat", "tickets.dat");

    if (found)
        printf("? Ticket Deleted!\n");
    else
        printf("? Ticket Not Found!\n");
}

// ?? MAIN
int main() {
    int choice;

    while (1) {
        printf("\n===== Train Ticketing System =====\n");
        printf("1. Add Ticket\n");
        printf("2. View Tickets\n");
        printf("3. Search Ticket\n");
        printf("4. Update Ticket\n");
        printf("5. Delete Ticket\n");
        printf("6. Show Available Seats\n");
        printf("7. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTicket(); break;
            case 2: viewTickets(); break;
            case 3: searchTicket(); break;
            case 4: updateTicket(); break;
            case 5: deleteTicket(); break;
            case 6: showAvailableSeats(); break;
            case 7: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}
