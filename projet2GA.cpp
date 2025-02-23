#include <stdio.h>
#include "bibli2GA.h"

main() 
{
    while (1) 
	{
        homeecran();
        setlocale(LC_CTYPE, "");
        ecrire(5, 8, "Veuillez-vous connecter pour accéder à votre compte.", 9);
        setlocale(LC_CTYPE, "C");
        gotoxy(5, 10);
        textcolor(3);
        printf("Mot de passe : ");
        textcolor(7);
        i = 0;
        do {
            p = getch();
            if (p == '\r' || p == '\n') 
			{ // Check for both carriage return and newline
                break; // Exit the loop if Enter is pressed
            }
            if (p == '\b') { // Handle backspace
                if (i > 0) {
                    printf("\b \b");
                    i--;
                }
            } else {
                printf("*");
                mdp[i] = p;
                i++;
            }
        } while (1); // Infinite loop until Enter is pressed

        mdp[i] = '\0'; // Null-terminate the password string

        gotoxy(5, 10);
        if (strcmp(mdp, "admin") == 0) 
		{
            adminMenu();
        } 
		else 
		{
            f = fopen("comptes.dat", "rb");
            if (f == NULL) 
			{
				setlocale(LC_CTYPE, "");
				ecrire(5, 12, "Erreur lors de l'ouverture du fichier.", 4);
				setlocale(LC_CTYPE, "C");
                exit(1);
            }
            int match = 0;
            while (fread(&a1, sizeof(Account), 1, f) == 1) 
			{
                if (strcmp(mdp, a1.motDePasse) == 0) 
				{
                    match = 1;
                    break;
                }
            }
            fclose(f);
            if (match) 
			{
                userMenu();
            } 
			else 
			{
				ecrire(5, 12, "Incorrect password. Access denied.", 4);
				getchar();
                system("cls");
            }
        }
    }
}


