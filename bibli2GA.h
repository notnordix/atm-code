#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>
#include <myconio.h>
#include <windows.h>
#include <locale.h>
#include <stdbool.h>

// Structure de compte
struct Account {
    int numero;
    char nom[50];
    char adresse[100];
    char motDePasse[20];
    float solde;
};
Account a1;

struct Transaction {
    int accountNumber;
    char date[11];
    char time[9]; // HH:MM:SS
    char type[10];
    float montant;
    float solde;
};
struct Transaction t;
struct LogEntry {
    char event[100];
    char date[11];
    char time[9];
};
struct LogEntry logEntry;


// Global variables
FILE *f;
FILE* ftrans;
int choix, numero;
char p, mdp[50], numStr[20], soldeStr[20];
int i = 0;
float montant;
char logMessage[200];
char dateStr[20];
char timestr[20];
char montantStr[20];
char accountStr[10];
// Draw a frame with corners and borders
void cadre(int ch, int lh, int cb, int lb, int couleur)
{
    int i;
    textcolor(couleur);
    // 1ère ligne
    gotoxy(ch, lh); printf("%c", 218);
    for (i = 1; i <= cb - ch - 2; i++) printf("%c", 196);
    printf("%c", 191);
    // Les autres lignes
    int nombredeligne = lb - lh - 1;
    for (i = 1; i <= nombredeligne; i++)
    {
        gotoxy(ch, lh + i); printf("%c", 179);
        gotoxy(cb - 1, lh + i); printf("%c", 179);
    }
    // Draw the bottom line
    gotoxy(ch, lb); printf("%c", 192);
    for (int i = 1; i <= cb - ch - 2; i++) printf("%c", 196);
    printf("%c", 217);
}

// Write text at a specific position with a specified color
void ecrire(int col, int lig, char texte[], int couleur)
{
    gotoxy(col, lig);
    textcolor(couleur);
    printf("%s", texte);
    textcolor(7);
}

// Display the game screen
void homeecran()
{
    cadre(1, 1, 120, 15, 6);
    ecrire(56, 2,"#2GA#", 6);
    setlocale(LC_CTYPE, "");
    ecrire(40, 4, "Bienvenue sur le guichet automatique", 6);
    setlocale(LC_CTYPE, "C");
    textcolor(7);
}

void clientecran()
{
    cadre(1, 1, 120, 25, 6);
    ecrire(56, 2, "#2GA#", 6);
    setlocale(LC_CTYPE, "");
    ecrire(51, 4, "Menu Principal", 6);
    setlocale(LC_CTYPE, "C");
    textcolor(7);
}
void userecran()
{
    cadre(1, 1, 120, 25, 6);
    ecrire(56, 2, "#2GA#", 6);
    textcolor(7);
}
void ecran()
{
    cadre(1, 1, 120, 25, 6);
    ecrire(56, 2, "#2GA#", 6);
    setlocale(LC_CTYPE, "");
    ecrire(43, 4, "Système de Guichet Automatique", 6);
    ecrire(56, 6, "Admin", 6);
    setlocale(LC_CTYPE, "C");
    textcolor(7);
}

// Fonction pour ajouter un compte
void ajouterCompte(FILE *f)
{
    f = fopen("comptes.dat", "a+");
    setlocale(LC_CTYPE, "");
    if (f == NULL) 
	{
    	ecrire(5, 12, "Erreur lors de l'ouverture du fichier.", 4);
        exit(1);
    }
    ecrire(5, 8, "Menu Principal > Gérer les comptes > Créer un nouveau compte", 9);

    ecrire(5, 10, "Voulez-vous continuer pour créer un nouveau compte ? (1 pour oui, 0 pour non) : ", 3);
    gotoxy(85, 10); scanf("%d", &choix);
	setlocale(LC_CTYPE, "C");
    if (choix == 1) 
	{
        ecrire(5, 12, "Numéro de compte : ", 3);
        ecrire(5, 14, "Nom : ", 3);
        ecrire(5, 16, "Adresse : ", 3);
        ecrire(5, 18, "Mot de passe : ", 3);
        ecrire(5, 20, "Solde de départ en DH : ", 3);
        gotoxy(25, 12); scanf("%d", &a1.numero);
	    gotoxy(12, 14); fflush(stdin); fgets(a1.nom, sizeof(a1.nom), stdin);
	    gotoxy(16, 16); fflush(stdin); fgets(a1.adresse, sizeof(a1.adresse), stdin);
	    gotoxy(21, 18); fflush(stdin); scanf("%s", a1.motDePasse);
	    gotoxy(30, 20); scanf("%f", &a1.solde);
	    
	    a1.nom[strcspn(a1.nom, "\n")] = 0;
	    a1.adresse[strcspn(a1.adresse, "\n")] = 0;

        fseek(f, 0, SEEK_END); // Se positionner à la fin du fichier
        fwrite(&a1, sizeof(a1), 1, f);
        ecrire(5, 22, "Compte ajouté avec succès.\n", 2);
        getch();
        fclose(f);
    }
    else 
	{
        ecrire(5, 12, "Annulation de la création de compte.", 4);
        getch();
        fclose(f);
    }
    setlocale(LC_CTYPE, "C");
}

void modifierCompteParNumero(FILE *f)
{
    setlocale(LC_CTYPE, "");
    ecrire(5, 8, "Menu Principal > Gérer les comptes > Modifier un compte", 9);
    ecrire(5, 10, "Entrez le numéro du compte à modifier : ", 3);
    scanf("%d", &numero);

    // Open the file in read-write mode
    f = fopen("comptes.dat", "r+b");
    if (f == NULL) 
	{
        ecrire(5, 12, "Erreur lors de l'ouverture du fichier.", 4);
        return;
    }

    int found = 0;
    while (fread(&a1, sizeof(a1), 1, f) == 1)
	{
        if (a1.numero == numero) 
		{
            found = 1;
            int continuer;
            do {
                system("cls");
                ecran();
                setlocale(LC_CTYPE, "");
                sprintf(numStr, "%d", a1.numero);
    			sprintf(soldeStr, "%.2f", a1.solde);
                ecrire(5, 8, "Menu Principal > Gérer les comptes > Modifier un compte", 9);
                ecrire(5, 10, "Quels champs voulez-vous modifier ?", 3);
                ecrire(5, 12, "1. Numéro de compte :", 3); ecrire(27, 12, numStr, 7);
                ecrire(5, 13, "2. Nom :", 3); ecrire(14, 13, a1.nom, 7);
                ecrire(5, 14, "3. Adresse :", 3); ecrire(18, 14, a1.adresse, 7);
                ecrire(5, 15, "4. Mot de passe :", 3); ecrire(22, 15, a1.motDePasse, 7);
                ecrire(5, 16, "5. Solde de départ en DH :", 3); ecrire(32, 16, soldeStr, 7);
                ecrire(5, 18, "Votre choix : ", 3);
                int choix;
                scanf("%d", &choix);

                switch (choix) 
				{
                case 1:
                    ecrire(5, 20, "Nouveau numéro de compte : ", 3);
                    scanf("%d", &a1.numero);
                    break;
                case 2:
                    ecrire(5, 20, "Nouveau nom : ", 3);
                    fflush(stdin); fgets(a1.nom, sizeof(a1.nom), stdin);
                    a1.nom[strcspn(a1.nom, "\n")] = 0; // Remove newline character
                    break;
                case 3:
                    ecrire(5, 20, "Nouvelle adresse : ", 3);
                    fflush(stdin); fgets(a1.adresse, sizeof(a1.adresse), stdin);
                    a1.adresse[strcspn(a1.adresse, "\n")] = 0; // Remove newline character
                    break;
                case 4:
                    ecrire(5, 20, "Nouveau mot de passe : ", 3);
                    scanf("%s", a1.motDePasse);
                    break;
                case 5:
                    ecrire(5, 20, "Nouveau solde de départ en DH : ", 3);
                    scanf("%f", &a1.solde);
                    break;
                default:
                    ecrire(5, 20, "Choix invalide.\n", 4);
                }

                ecrire(5, 22, "Souhaitez-vous modifier un autre champ ? (1 pour oui, 0 pour non) : ", 9);
                scanf("%d", &continuer);
                setlocale(LC_CTYPE, "C");
            } while (continuer == 1);

            // Move file pointer to the position of the modified account
            fseek(f, -sizeof(a1), SEEK_CUR);
            // Write the modified account back to the file
            fwrite(&a1, sizeof(a1), 1, f);
            setlocale(LC_CTYPE, "");
            ecrire(5, 24, "Modification effectuée avec succès.", 2);
			setlocale(LC_CTYPE, "C");
            break;
        }
        
    }

    if (!found) 
	{
		setlocale(LC_CTYPE, "");
        ecrire(5, 12, "Aucun compte trouvé avec ce numéro.", 4);
        setlocale(LC_CTYPE, "C");
    }

    // Close the file
    fclose(f);
}

// Fonction pour supprimer un compte
void supprimerCompte(FILE *f) 
{
    setlocale(LC_CTYPE, "");
    ecrire(5, 8, "Menu Principal > Gérer les comptes > Supprimer un compte", 9);
    ecrire(5, 10, "Entrez le numéro de compte à supprimer : ", 3);
    scanf("%d", &numero);

    bool trouve = false;
    f = fopen("comptes.dat", "r+b");
    if (f == NULL) 
	{
        ecrire(5, 12, "Erreur lors de l'ouverture du fichier.", 4);
        return;
    }

    while (fread(&a1, sizeof(a1), 1, f) == 1) 
	{
        if (a1.numero == numero) 
		{
            trouve = true;
            break;
        }
    }
    fclose(f);

    if (!trouve) 
	{
        ecrire(5, 13, "Aucun compte trouvé avec ce numéro.", 4);
    } 
	else 
	{
        // Display account details
        sprintf(numStr, "%d", a1.numero);
        sprintf(soldeStr, "%.2f", a1.solde);
        ecrire(5, 13, "Compte trouvé :", 2);
        ecrire(5, 15, "Numéro", 3); ecrire(5, 17, numStr, 7);
        ecrire(18, 15, "Nom", 3); ecrire(18, 17, a1.nom, 7);
        ecrire(43, 15, "Adresse", 3); ecrire(43, 17, a1.adresse, 7);
        ecrire(83, 15, "Mot de passe", 3); ecrire(83, 17, a1.motDePasse, 7);
        ecrire(98, 15, "Solde", 3); ecrire(98, 17, soldeStr, 7);
        ecrire(5, 20, "Êtes-vous sûr de vouloir supprimer ce compte ? (O/N) : ", 4);
        char rep = getch();
        printf("%c", rep);
        if (rep == 'O' || rep == 'o') 
		{
            f = fopen("comptes.dat", "r+b");
            FILE *temp = fopen("temp.dat", "wb");
            if (f == NULL || temp == NULL) 
			{
                ecrire(5, 21, "Erreur lors de l'ouverture des fichiers.", 4);
                if (f != NULL) fclose(f);
                if (temp != NULL) fclose(temp);
                return;
            }

            rewind(f); // Reset file position indicator

            // Copy all records except the one to be deleted
            while (fread(&a1, sizeof(a1), 1, f) == 1) 
			{
                if (a1.numero != numero) {
                    fwrite(&a1, sizeof(a1), 1, temp);
                }
            }

            fclose(f);
            fclose(temp);

            remove("comptes.dat");
            rename("temp.dat", "comptes.dat");

            ecrire(5, 22, "Compte supprimé avec succès.", 2);
        } else {
            ecrire(5, 22, "Suppression annulée.", 4);
        }
    }
    getch();
    setlocale(LC_CTYPE, "C");
}

// Fonction pour chercher un compte par numéro
void chercherCompteParNumero(FILE *f)
{
    setlocale(LC_CTYPE, "");
    ecrire(5, 8, "Menu Principal > Gérer les comptes > Chercher par le numéro", 9);
    ecrire(5, 10, "Entrez le numéro de compte à chercher : ", 3);
    scanf("%d", &numero);

    f = fopen("comptes.dat", "rb");
    if (f == NULL) 
	{
        ecrire(5, 13, "Erreur lors de l'ouverture du fichier.", 4);
        return;
    }

    int found = 0;
    while (fread(&a1, sizeof(a1), 1, f) == 1) 
	{
        if (a1.numero == numero) 
		{
            found = 1;
            // Display the details of the found account
            sprintf(numStr, "%d", a1.numero);
            sprintf(soldeStr, "%.2f", a1.solde);
            ecrire(5, 13, "Compte trouvé :", 2);
            ecrire(5, 16, "Numéro", 3); ecrire(5, 18, numStr, 7);
            ecrire(18, 16, "Nom", 3); ecrire(18, 18, a1.nom, 7);
            ecrire(43, 16, "Adresse", 3); ecrire(43, 18, a1.adresse, 7);
            ecrire(83, 16, "Mot de passe", 3); ecrire(83, 18, a1.motDePasse, 7);
            ecrire(98, 16, "Solde", 3); ecrire(98, 18, soldeStr, 7);
            break;
        }
    }

    if (!found) {
        ecrire(5, 13, "Aucun compte trouvé avec ce numéro.", 4);
    }

    fclose(f);
    setlocale(LC_CTYPE, "C");
}

void afficherComptes(FILE *f)
{
    f = fopen("comptes.dat", "rb");
    setlocale(LC_CTYPE, "");
    if (f == NULL) 
	{
        ecrire(5, 8, "Erreur lors de l'ouverture du fichier.", 8);
        exit(1);
    }
    ecrire(5, 8, "Menu Principal > Gérer les comptes > Afficher les comptes", 9);
    rewind(f); // Se positionner au début du fichier

    // Table header
    ecrire(5, 11, "Numéro", 3);
    ecrire(18, 11, "Nom", 3);
    ecrire(43, 11, "Adresse", 3);
    ecrire(83, 11, "Mot de passe", 3);
    ecrire(98, 11, "Solde", 3);

    int ligne = 13; // Starting position of the first row

    while (fread(&a1, sizeof(struct Account), 1, f) == 1) 
	{
        // Table row
        sprintf(numStr, "%d", a1.numero);
        sprintf(soldeStr, "%.2f", a1.solde);
        ecrire(5, ligne, numStr, 7);
        ecrire(18, ligne, a1.nom, 7);
        ecrire(43, ligne, a1.adresse, 7);
        ecrire(83, ligne, a1.motDePasse, 7); // Display the password
        ecrire(98, ligne, soldeStr, 7);

        ligne++; // Move to the next row
    }
    setlocale(LC_CTYPE, "C");
    fclose(f);
}
void getCurrentDateTime(char* dateStr, char* timeStr) 
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(dateStr, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    sprintf(timeStr, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void logEvent(const char* event) 
{
    FILE* flog = fopen("journalisation.dat", "a");
    if (flog == NULL) 
	{
		ecrire(5, 8, "Erreur lors de l'ouverture du fichier de journalisation.", 4);
        return;
    }

    struct LogEntry logEntry;
    getCurrentDateTime(logEntry.date, logEntry.time);
    strncpy(logEntry.event, event, sizeof(logEntry.event) - 1);
    logEntry.event[sizeof(logEntry.event) - 1] = '\0'; // Ensure null-termination

    fwrite(&logEntry, sizeof(logEntry), 1, flog);
    fclose(flog);
}
void afficherJournalisation() 
{
    FILE* flog = fopen("journalisation.dat", "rb");
    setlocale(LC_CTYPE, "");
    if (flog == NULL) 
	{
		ecrire(5, 8, "Erreur lors de l'ouverture du fichier de journalisation.", 4);
        return;
    }
    ecrire(5, 8, "Menu Principal > Consulter les fichiers > Consulter un fichier spécifique > Journalisation", 9);
	ecrire(5, 11, "Événement", 3);   
	ecrire(90, 11, "Date", 3);
	ecrire(105, 11, "Heure", 3);
    struct LogEntry logEntry;
    int ligne = 13;
    while (fread(&logEntry, sizeof(logEntry), 1, flog) == 1) 
	{
	    sprintf(dateStr, "%s", logEntry.date);
	    sprintf(timestr, "%s", logEntry.time);
        ecrire(5, ligne, logEntry.event, 7);
        ecrire(90, ligne, dateStr, 7);
        ecrire(105, ligne, timestr, 7);
        ligne++; // Move to the next row
    }
	setlocale(LC_CTYPE, "C");
    fclose(flog);
}
void afficherTransactions() 
{
    FILE* ftrans = fopen("transactions.dat", "rb");
    setlocale(LC_CTYPE, "");
    if (ftrans == NULL) 
	{
		ecrire(5, 8, "Erreur lors de l'ouverture du fichier des transactions.", 4);
        return;
    }
    char accountStr[10];
    ecrire(5, 8, "Menu Principal > Consulter les fichiers > Consulter un fichier spécifique > Transactions", 9);
    ecrire(5, 11, "Numero de compte", 3);
    ecrire(35, 11, "Type de transaction", 3);
    ecrire(60, 11, "Montant", 3);
    ecrire(80, 11, "Date", 3);
    ecrire(100, 11, "Heure", 3);

    int ligne = 13; // Starting position for the first transaction

    while (fread(&t, sizeof(struct Transaction), 1, ftrans) == 1) 
	{
        // Display each transaction
        sprintf(accountStr, "%d", t.accountNumber);
        sprintf(dateStr, "%s", t.date);
        sprintf(montantStr, "%.2f", t.montant);
        ecrire(5, ligne, accountStr, 7);
        ecrire(35, ligne, t.type, 7);
        ecrire(60, ligne, montantStr, 7);
        ecrire(80, ligne, dateStr, 7);
        ecrire(100, ligne, t.time, 7); // Display the time
        ligne++; // Move to the next row
    }
    fclose(ftrans);
    setlocale(LC_CTYPE, "C");
}

void consulterFichierSpecifique() 
{
	setlocale(LC_CTYPE, "");
    ecrire(5, 8, "Menu Principal > Consulter les fichiers > Consulter un fichier spécifique", 9);
    ecrire(5, 11, "1. Transactions", 3);
    ecrire(5, 12, "2. Journalisation", 3);
    ecrire(5, 13, "0. Retour", 3);
    setlocale(LC_CTYPE, "C");
    gotoxy(5, 16);
    textcolor(3);
    printf("CHOIX ? ");
    textcolor(7);
    scanf("%d", &choix);

    switch (choix) 
	{
        case 1:
            system("cls");
            ecran();
            afficherTransactions();
            getch();
            break;
        case 2:
            system("cls");
            ecran();
            afficherJournalisation();
            getch();
            break;
        case 0:
            break;
        default:
        	ecrire(5, 18, "Choix invalide. Veuillez entrer un nombre entre 0 et 2.", 4);
    }
    
}
void consulterTousLesFichiers() 
{
	cadre(1, 1, 120, 30, 6);
    ecrire(56, 2, "#2GA#", 6);
    setlocale(LC_CTYPE, "");
    ecrire(43, 4, "Système de Guichet Automatique", 6);
    ecrire(56, 6, "Admin", 6);
    textcolor(7);
    FILE* ftrans = fopen("transactions.dat", "rb");
    FILE* flog = fopen("journalisation.dat", "rb");
    setlocale(LC_CTYPE, "");

    if (ftrans == NULL) 
    {
        ecrire(5, 8, "Erreur lors de l'ouverture du fichier des transactions.", 4);
        if (flog != NULL) fclose(flog);
        return;
    }

    if (flog == NULL) 
    {
        ecrire(5, 20, "Erreur lors de l'ouverture du fichier de journalisation.", 4);
        fclose(ftrans);
        return;
    }
    ecrire(5, 8, "Menu Principal > Consulter les fichiers > Consulter tous les fichiers", 9);
    
    // Transactions headers
    ecrire(5, 10, "Numero de compte", 3);
    ecrire(35, 10, "Type de transaction", 3);
    ecrire(60, 10, "Montant", 3);
    ecrire(80, 10, "Date", 3);
    ecrire(100, 10, "Heure", 3);

    int ligne = 11; // Starting position for the first transaction
    while (fread(&t, sizeof(t), 1, ftrans) == 1) 
    {
        // Display each transaction
        sprintf(accountStr, "%d", t.accountNumber);
        sprintf(dateStr, "%s", t.date);
        sprintf(montantStr, "%.2f", t.montant);
        ecrire(5, ligne, accountStr, 7);
        ecrire(35, ligne, t.type, 7);
        ecrire(60, ligne, montantStr, 7);
        ecrire(80, ligne, dateStr, 7);
        ecrire(100, ligne, t.time, 7); // Display the time
        ligne++; // Move to the next row
    }

    fclose(ftrans);

    // Leave a gap between transactions and journal entries
    ligne += 1;

    // Journalisation headers
    ecrire(5, ligne, "Événement", 3);
    ecrire(90, ligne, "Date", 3);
    ecrire(105, ligne, "Heure", 3);
    ligne += 2; // Move to the next row for journal entries
    while (fread(&logEntry, sizeof(logEntry), 1, flog) == 1) 
    {
        sprintf(dateStr, "%s", logEntry.date);
        ecrire(5, ligne, logEntry.event, 7);
        ecrire(90, ligne, dateStr, 7);
        ecrire(105, ligne, logEntry.time, 7);
        ligne++; // Move to the next row
    }

    fclose(flog);
    setlocale(LC_CTYPE, "C");
    getch();
}

void adminMenu() 
{
    while (1) {
        system("cls");
        ecran();
        setlocale(LC_CTYPE, "");
        ecrire(51, 8, "Menu Principal", 9);
        ecrire(5, 11, "1. Gérer les comptes", 3);
        ecrire(5, 12, "2. Consulter les fichiers", 3);
        ecrire(5, 13, "0. Quitter", 3);
        setlocale(LC_CTYPE, "C");
        gotoxy(5, 16);
        textcolor(3);
        printf("CHOIX ? ");
        textcolor(7);
        scanf("%d", &choix);

        switch (choix) {
            case 1: {
                while (1) {
                    system("cls");
                    ecran();
                    setlocale(LC_CTYPE, "");
                    ecrire(5, 8, "Menu Principal > Gérer les comptes", 9);
                    ecrire(5, 11, "1. Créer un nouveau compte", 3);
                    ecrire(5, 12, "2. Modifier un compte", 3);
                    ecrire(5, 13, "3. Supprimer un compte", 3);
                    ecrire(5, 14, "4. Chercher par le numéro", 3);
                    ecrire(5, 15, "5. Afficher les comptes", 3);
                    ecrire(5, 16, "0. Retour", 3);
                    setlocale(LC_CTYPE, "C");
                    gotoxy(5, 19);
                    textcolor(3);
			        printf("CHOIX ? ");
			        textcolor(7);
			        scanf("%d", &choix);

                    switch (choix) {
                        case 1:
                            system("cls");
                            ecran();
                            ajouterCompte(f);
                            break;
                        case 2:
                            system("cls");
                            ecran();
                            modifierCompteParNumero(f);
                            getch();
                            break;
                        case 3:
                            system("cls");
                            ecran();
                            supprimerCompte(f);
                            break;
                        case 4:
                            system("cls");
                            ecran();
                            chercherCompteParNumero(f);
                            getch();
                            break;
                        case 5:
                            system("cls");
                            ecran();
                            afficherComptes(f);
                            getch();
                            break;
                        case 0:
                            printf("Retour au menu précédent...\n");
                            break;
                        default:
                            printf("Choix invalide. Veuillez entrer un nombre entre 0 et 4.\n");
                    }
                    if (choix == 0) break;
                }
                break;
            }
            case 2: {
                while (1) {
                    system("cls");
                    ecran();
                    setlocale(LC_CTYPE, "");
                    ecrire(5, 8, "Menu Principal > Consulter les fichiers", 9);
                    ecrire(5, 11, "1. Consulter un fichier spécifique", 3);
                    ecrire(5, 12, "2. Consulter tous les fichiers", 3);
                    ecrire(5, 13, "0. Retour", 3);
                    setlocale(LC_CTYPE, "C");
                    gotoxy(5, 16);
                    textcolor(3);
			        printf("CHOIX ? ");
			        textcolor(7);
			        scanf("%d", &choix);

                    switch (choix) {
                        case 1:
                            system("cls");
                            ecran();
                            consulterFichierSpecifique();
                            break;
                        case 2:
                            system("cls");
                            consulterTousLesFichiers();
                            break;
                        case 0:
                            break;
                        default:
                            printf("Choix invalide. Veuillez entrer un nombre entre 0 et 2.\n");
                    }
                    if (choix == 0) break;
                }
                break;
        }
            case 0:
                system("cls");
                ecran();
                ecrire(5, 11, "Quitter le programme", 7);
                for (int i = 0; i < 3; i++) 
				{
			    	sleep(1);
			        printf(".");
			        fflush(stdout);
			    }
                system("cls");
                exit(0);
            default:
                printf("Choix invalide. Veuillez entrer un nombre entre 0 et 2.\n");
        }
    }
}

void consulterSolde(Account* a1) 
{
	setlocale(LC_CTYPE, "");
	ecrire(49, 4, "Consulter le solde", 9);
	textcolor(3);
	gotoxy(5, 9);printf("Solde disponible sur le compte numéro : %d", a1->numero);
	textcolor(2);
	gotoxy(5, 12);printf("[ %.2f ]", a1->solde);
	textcolor(7);
    getch();
    setlocale(LC_CTYPE, "C");
}

void afficherConfirmation(struct Account* a1, char* typeTransaction, float montant) 
{
    setlocale(LC_CTYPE, "");
    ecrire(45, 4, "Confirmation de transaction", 9);
    ecrire(5, 9, "Transaction réussie !", 2);
   
    char accountNum[100];
    textcolor(3);
    snprintf(accountNum, sizeof(accountNum), "Numéro de compte : %d", a1->numero);
    textcolor(7);
    ecrire(5, 12, accountNum, 7);

    char transactionType[100];
    textcolor(3);
    snprintf(transactionType, sizeof(transactionType), "Type de transaction : %s", typeTransaction);
    textcolor(7);
    ecrire(5, 14, transactionType, 7);

    char transactionAmount[100];
    textcolor(3);
    snprintf(transactionAmount, sizeof(transactionAmount), "Montant : %.2f", montant);
    textcolor(7);
    ecrire(5, 16, transactionAmount, 7);

    char newBalance[100];
    textcolor(3);
    snprintf(newBalance, sizeof(newBalance), "Nouveau solde : %.2f", a1->solde);
    textcolor(7);
    ecrire(5, 18, newBalance, 7);

    ecrire(5, 21, "[ Continuer ]", 2);

    getch();
    setlocale(LC_CTYPE, "C");
}

void retirerArgent(Account* a1) 
{
    setlocale(LC_CTYPE, "");
    ecrire(49, 4, "Retirer de l'argent", 9);

    f = fopen("comptes.dat", "r+b");
    if (f == NULL) {
        ecrire(5, 10, "Erreur lors de l'ouverture du fichier.", 4);
        logEvent("Erreur lors de l'ouverture du fichier pour retrait d'argent");
        return;
    }

    gotoxy(5, 9);
    textcolor(9);
    printf("Retirer de l'argent du compte numéro : %d", a1->numero);
    textcolor(7);
    ecrire(5, 12, "Enter amount to withdraw: ", 3);
    scanf("%f", &montant);
    setlocale(LC_CTYPE, "C");
    if (a1->solde >= montant) 
	{
        a1->solde -= montant;

        // Move file pointer to the beginning of the file
        rewind(f);

        // Find the account in the file
        Account temp;
        while (fread(&temp, sizeof(temp), 1, f) == 1) 
		{
            if (temp.numero == a1->numero) 
			{
                // Move file pointer to the position of the modified account
                fseek(f, -sizeof(temp), SEEK_CUR);
                // Write the modified account back to the file
                fwrite(a1, sizeof(*a1), 1, f);
                break;
            }
        }
        FILE* ftrans = fopen("transactions.dat", "ab");
        if (ftrans == NULL) 
		{
            ecrire(5, 16, "Erreur lors de l'ouverture du fichier des transactions.", 4);
            fclose(f);
            getch();
            return;
        }

        struct Transaction t;
        t.accountNumber = a1->numero;
        getCurrentDateTime(t.date, t.time); // Get the current date and time
        strcpy(t.type, "Retrait");
        t.montant = montant;
        t.solde = a1->solde;

        fwrite(&t, sizeof(t), 1, ftrans);
        fclose(ftrans);

        system("cls");
        userecran();
        afficherConfirmation(a1, "Retrait", montant);
        snprintf(logMessage, sizeof(logMessage), "Retrait réussi pour le compte numéro %d, montant %.2f", a1->numero, montant);
        logEvent(logMessage);
    } 
    else 
    {
        ecrire(5, 16, "Insufficient funds.", 4);
        snprintf(logMessage, sizeof(logMessage), "Tentative de retrait échouée pour le compte numéro %d, montant %.2f", a1->numero, montant);
        logEvent(logMessage);
    }
    fclose(f);
    getch();
}

void deposerArgent(Account* a1) 
{
    setlocale(LC_CTYPE, "");
    ecrire(49, 4, "Déposer de l'argent ", 9);

    f = fopen("comptes.dat", "r+b");
    if (f == NULL) {
        ecrire(5, 10, "Erreur lors de l'ouverture du fichier.", 4);
        logEvent("Erreur lors de l'ouverture du fichier pour dépôt d'argent");
        return;
    }

    gotoxy(5, 9);
    textcolor(9);
    printf("Déposer de l'argent sur le compte numéro : %d", a1->numero);
    textcolor(7);
    ecrire(5, 12, "Enter amount to deposit: ", 3);
    scanf("%f", &montant);
	setlocale(LC_CTYPE, "C");
    a1->solde += montant;

    // Move file pointer to the beginning of the file
    rewind(f);

    // Find the account in the file
    Account temp;
    while (fread(&temp, sizeof(temp), 1, f) == 1) 
	{
        if (temp.numero == a1->numero) {
            // Move file pointer to the position of the modified account
            fseek(f, -sizeof(temp), SEEK_CUR);
            // Write the modified account back to the file
            fwrite(a1, sizeof(*a1), 1, f);
            break;
        }
    }
    FILE* ftrans = fopen("transactions.dat", "ab");
    if (ftrans == NULL) {
        ecrire(5, 16, "Erreur lors de l'ouverture du fichier des transactions.", 4);
        fclose(f);
        return;
    }

    struct Transaction t;
    t.accountNumber = a1->numero;
    getCurrentDateTime(t.date, t.time); // Get both date and time
    strcpy(t.type, "Depot");
    t.montant = montant;
    t.solde = a1->solde;

    fwrite(&t, sizeof(t), 1, ftrans);
    fclose(ftrans);

    system("cls");
    userecran();
    afficherConfirmation(a1, "Dépôt", montant);
    snprintf(logMessage, sizeof(logMessage), "Dépôt réussi pour le compte numéro %d, montant %.2f", a1->numero, montant);
    logEvent(logMessage);

    fclose(f);
    getch();
    setlocale(LC_CTYPE, "C");
}

void effectuerVirement(Account* a1, FILE* f) 
{
    int numDest;
    Account dest;
    int found = 0;
    int sourceUpdated = 0;

    setlocale(LC_CTYPE, "");
    ecrire(48, 4, "Effectuer un virement", 9);
    gotoxy(5, 9);
    textcolor(9);
    printf("Effectuer un virement du compte numéro : %d", a1->numero);
    textcolor(7);
    f = fopen("comptes.dat", "rb+");
    ecrire(5, 12, "Numéro de compte bénéficiaire : ", 3);
    scanf("%d", &numDest);
	
    // Check if the destination account exists
    rewind(f);
    while (fread(&dest, sizeof(Account), 1, f) == 1) 
	{
        if (dest.numero == numDest) 
		{
            found = 1;
            break;
        }
    }

    if (!found) {
        ecrire(5, 14, "Destination account not found.", 4);
        snprintf(logMessage, sizeof(logMessage), "Tentative de virement échouée pour le compte numéro %d vers le compte numéro %d", a1->numero, numDest);
        logEvent(logMessage);
        fclose(f);
        getch();
        return;
    }

    ecrire(5, 15, "Montant à transférer : ", 3);
    scanf("%f", &montant);
	setlocale(LC_CTYPE, "C");
    FILE* ftemp = fopen("temp.dat", "wb");
    if (ftemp == NULL) 
	{
        ecrire(5, 16, "Erreur lors de l'ouverture du fichier temporaire.", 4);
        logEvent("Erreur lors de l'ouverture du fichier temporaire pour virement");
        fclose(f);
        return;
    }

    // Move file pointer to the beginning of the file
    rewind(f);

    // Process the transfer
    while (fread(&dest, sizeof(Account), 1, f) == 1) 
	{
        if (dest.numero == a1->numero) 
		{
            // Update the source account balance if transfer is successful
            if (a1->solde >= montant) 
			{
                a1->solde -= montant;
                sourceUpdated = 1;  // Mark the source account as updated
                fwrite(a1, sizeof(Account), 1, ftemp); // Write the updated source account
            } 
			else 
			{
                ecrire(5, 17, "Insufficient funds.", 4);
                fwrite(&dest, sizeof(Account), 1, ftemp); // Write the source account as is
                snprintf(logMessage, sizeof(logMessage), "Tentative de virement échouée pour le compte numéro %d, montant %.2f (fonds insuffisants)", a1->numero, montant);
                logEvent(logMessage);
            }
        } 
		else if (dest.numero == numDest) 
		{
            dest.solde += montant;
            fwrite(&dest, sizeof(Account), 1, ftemp); // Write the updated destination account
        } 
		else 
		{
            fwrite(&dest, sizeof(Account), 1, ftemp); // Write other accounts as is
        }
    }

    fclose(f);
    fclose(ftemp);

    // Replace the original file with the updated temp file
    remove("comptes.dat");
    rename("temp.dat", "comptes.dat");
    if (sourceUpdated) 
	{
        FILE* ftrans = fopen("transactions.dat", "ab");
        if (ftrans == NULL) 
		{
            ecrire(5, 18, "Erreur lors de l'ouverture du fichier des transactions.", 4);
            logEvent("Erreur lors de l'ouverture du fichier des transactions pour virement");
            getch();
            return;
        }

        struct Transaction t;
        t.accountNumber = a1->numero;
        getCurrentDateTime(t.date, t.time); // Get both date and time
        strcpy(t.type, "Virement");
        t.montant = montant;
        t.solde = a1->solde;

        fwrite(&t, sizeof(t), 1, ftrans);
        fclose(ftrans);

        system("cls");
        userecran();
        afficherConfirmation(a1, "Virement", montant);
        snprintf(logMessage, sizeof(logMessage), "Virement réussi pour le compte numéro %d, montant %.2f, vers le compte numéro %d", a1->numero, montant, numDest);
        logEvent(logMessage);

    }
    getch();
}

// Function to display the bank statement
void afficherReleveBancaire(Account* a1, FILE* f) 
{
    setlocale(LC_CTYPE, "");
    ecrire(50, 4, "Relevé bancaire", 9);
    char dateStr[11];
    getCurrentDateTime(dateStr, NULL);

    char header[100];
    snprintf(header, sizeof(header), "Date : %s", dateStr);
    ecrire(5, 8, header, 9);

    char accountInfo[100];
    snprintf(accountInfo, sizeof(accountInfo), "Compte : %d - %s", a1->numero, a1->nom);
    ecrire(5, 10, accountInfo, 9);

    ecrire(5, 12, "Date", 3);
    ecrire(25, 12, "Type", 3);
    ecrire(45, 12, "Montant", 3);
    ecrire(65, 12, "Solde", 3);

    // Open the transactions file (assuming transactions are stored in "transactions.dat")
    FILE* ftrans = fopen("transactions.dat", "rb");
    if (ftrans == NULL) 
	{
        ecrire(5, 14, "Erreur lors de l'ouverture du fichier des transactions.", 4);
        getch();
        return;
    }
    int ligne = 14; // Starting position for the first transaction

    while (fread(&t, sizeof(struct Transaction), 1, ftrans) == 1) 
	{
        // Display each transaction if it matches the current account number
        if (t.accountNumber == a1->numero) {
            sprintf(dateStr, "%s", t.date);
            sprintf(montantStr, "%.2f", t.montant);
            sprintf(soldeStr, "%.2f", t.solde);

            ecrire(5, ligne, dateStr, 7);
            ecrire(25, ligne, t.type, 7);
            ecrire(45, ligne, montantStr, 7);
            ecrire(65, ligne, soldeStr, 7);

            ligne++; // Move to the next row
        }
    }

    fclose(ftrans);
    getch();
    setlocale(LC_CTYPE, "C");
}



void changerMotDePasse(Account* a1) 
{
    setlocale(LC_CTYPE, "");
    ecrire(47, 4, "Changer le mot de passe", 9);

    f = fopen("comptes.dat", "r+b");
    if (f == NULL) 
	{
        ecrire(5, 10, "Erreur lors de l'ouverture du fichier.", 4);
        logEvent("Erreur lors de l'ouverture du fichier pour changer le mot de passe");
        return;
    }

    gotoxy(5, 9);
    printf("Changer le mot de passe pour le compte numéro : %d", a1->numero);
    
    char nouveauMotDePasse[20];
    char confirmerMotDePasse[20];
    
    ecrire(5, 13, "Entrez le nouveau mot de passe : ", 3);
    ecrire(5, 15, "Confirmez le nouveau mot de passe : ", 3);
    gotoxy(38, 13);scanf("%s", nouveauMotDePasse);
    gotoxy(41, 15);scanf("%s", confirmerMotDePasse);

    if (strcmp(nouveauMotDePasse, confirmerMotDePasse) == 0) 
	{
        strcpy(a1->motDePasse, nouveauMotDePasse);

        // Move file pointer to the beginning of the file
        rewind(f);

        // Find the account in the file
        Account temp;
        while (fread(&temp, sizeof(temp), 1, f) == 1) 
		{
            if (temp.numero == a1->numero) 
			{
                // Move file pointer to the position of the modified account
                fseek(f, -sizeof(temp), SEEK_CUR);
                // Write the modified account back to the file
                fwrite(a1, sizeof(*a1), 1, f);
                break;
            }
        }

        ecrire(5, 19, "Password changed successfully.", 2);
        snprintf(logMessage, sizeof(logMessage), "Mot de passe changé avec succès pour le compte numéro %d", a1->numero);
        logEvent(logMessage);
    } 
	else 
	{
        ecrire(5, 19, "Les mots de passe ne correspondent pas. Veuillez réessayer.", 4);
        snprintf(logMessage, sizeof(logMessage), "Tentative de changement de mot de passe échouée pour le compte numéro %d", a1->numero);
        logEvent(logMessage);
    }

    fclose(f);
    getch();
    setlocale(LC_CTYPE, "C");
}

void aPropos() 
{
    cadre(1, 1, 120, 30, 6); // Draw a frame
    setlocale(LC_CTYPE, "");
    ecrire(56, 2, "#2GA#", 6);
    ecrire(5, 4, "Système de gestion de guichet automatique ou 2GA est un logiciel développé par Noureddine Elmhassani, un", 7);
    ecrire(5, 5, "étudiant passionné de programmation. Ce projet a été créé dans le cadre d'un travail académique pour le", 7);
    ecrire(5, 6, "cours des techniques et logique de programmation de la 1ère année du cycle Bac+3 en Génie ", 7);
    ecrire(5, 7, "Informatique.", 7);

    ecrire(5, 9, "2GA est un programme écrit en langage C qui permet de réaliser les fonctionnalités suivantes :", 7);
    ecrire(5, 10, "- Création de compte utilisateur", 7);
    ecrire(5, 11, "- Garder une trace des opérations de transactions bancaires notamment le dépôt, le retrait et le virement", 7);
    ecrire(5, 12, "- L’impression du relevé bancaire", 7);
    ecrire(5, 13, "- Le changement du mot de passe", 7);

    ecrire(5, 15, "Ce projet a été une occasion pour moi d'apprendre et de mettre en pratique mes compétences en", 7);
    ecrire(5, 16, "programmation, ainsi que de relever des défis techniques.", 7);

    ecrire(5, 18, "Je suis conscient(e) que ce programme peut comporter des bogues ou des erreurs, mais j'ai fait de mon", 7);
    ecrire(5, 19, " mieux pour le tester et le déboguer. Si vous rencontrez l'adresse noureddineelmhassani@gmailcom.", 7);
    ecrire(5, 20, "Vos commentaires des problèmes ou si vous avez des commentaires, n'hésitez pas à me contacter à", 7);
    ecrire(5, 21, " sont précieux pour moi et m'aideront à améliorer ce programme à l'avenir.", 7);

    ecrire(5, 23, "Je suis reconnaissant(e) de cette opportunité qui m'a permis d'acquérir de nouvelles compétences en", 7);
    ecrire(5, 24, " programmation et j'espère que 2GA vous sera utile et vous aidera à atteindre vos objectifs.", 7);

    getch(); // Wait for a key press
    setlocale(LC_CTYPE, "C");
}

void userMenu() 
{
    while (1) 
	{
        system("cls");
        clientecran();
        setlocale(LC_CTYPE, "");
        ecrire(5, 7, "Que voulez-vous faire ?", 9);
        ecrire(5, 10, "1. Consulter le solde ", 3);
        ecrire(5, 11, "2. Retirer de l'argent", 3);
        ecrire(5, 12, "3. Déposer de l'argent", 3);
        ecrire(5, 13, "4. Effectuer un Virement ", 3);
        ecrire(5, 14, "5. Relevé bancaire", 3);
        ecrire(5, 15, "6. Changer le mot de passe", 3);
        ecrire(5, 16, "7. A Propos", 3);
        ecrire(5, 17, "0. Quitter ", 3);
        setlocale(LC_CTYPE, "C");
        gotoxy(5, 19);
        textcolor(3);
        printf("CHOIX ? ");
        textcolor(7);
        scanf("%d", &choix);

        switch (choix) 
		{
            case 1:
                system("cls");
                userecran();
                consulterSolde(&a1);
                break;
            case 2:
                system("cls");
                userecran();
                retirerArgent(&a1);
                break;
            case 3:
                system("cls");
                userecran();
                deposerArgent(&a1);
                break;
            case 4:
                system("cls");
                userecran();
                effectuerVirement(&a1, f);
                break;
            case 5:
                system("cls");
                userecran();
                afficherReleveBancaire(&a1, f);
                break;
            case 6:
                system("cls");
                userecran();
                changerMotDePasse(&a1);
                break;
            case 7:
                system("cls");
                aPropos();
                getch();
                break;
            case 0:
            	system("cls");
            	clientecran();
            	gotoxy(5, 9);
                printf("Quitting");
                for (int i = 0; i < 3; i++) 
				{
			    	sleep(1);
			        printf(".");
			        fflush(stdout);
			    }
                exit(0);
            default:
                printf("Choix invalide. Veuillez entrer un nombre entre 0 et 7.");
        }
        if (choix == 0) break;
    }
}
