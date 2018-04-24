#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

int tab[25][25];

void pisanie(char plik[])
{
    clear();
    int kolumny = 0, rzedy = 0;
    int i, j, x, y, przycisk;
    echo();

    for(i = 0; i < 27; i++) //tworzenie "ramki"
    {
        for(j = 0; j < 27; j++)
        {
            if(i == 0 || i == 26)
                 mvprintw(i + 2, j + 2, "*");
            else
            {
                if(j == 0 || j == 26)
                    mvprintw(i + 2, j + 2, "*");
                else
                    mvprintw(i + 2, j + 2, " ");
            }
        }
    }

    for(i = 0; i < 25; i++) // wype³nianie pola wczytanym tekstem/tworzenie czystego pola
    {
        for(j = 0; j < 25; j++)
        {
            mvprintw(i+3, j+3, "%c", tab[i][j]);
        }
    }

    for(i = 0; i < 17; i++) // tworzenie kolejnej "ramki"
    {
        for(j = 0; j < 17; j++)
        {
            if(i == 0 || i == 16)
                mvprintw(i + 2, j + 32, "*");
            else
            {
                if(j == 0 || j == 16)
                    mvprintw(i + 2, j + 32, "*");
                else
                    mvprintw(i + 2, j + 32, " ");
            }
        }
    }

    //szcz¹tkowa instrukcja
    mvprintw(3, 33, "Aby zapisac");
    mvprintw(4, 33, "wcisnij klawisz");
    attron(A_BOLD);
    mvprintw(5, 33, "'HOME'");
    attroff(A_BOLD);
    mvprintw(7, 33, "Aby wyjsc bez");
    mvprintw(8, 33, "zapisu wcisnij");
    mvprintw(9, 33, "klawisz");
    attron(A_BOLD);
    mvprintw(10, 33, "'END'");
    attroff(A_BOLD);
    mvprintw(11, 33, "Pozycja");
    mvprintw(12, 33, "kursora");
    refresh();

    move(3, 3); //przesuniecie kursora na pozycje wyjsciow¹
    x = 3, y = 3;

    while(przycisk != KEY_HOME && przycisk != KEY_END)
    {
        przycisk = getch();
        if(przycisk > 31 && przycisk < 127)
        {
            tab[x - 3][y - 3] = przycisk;
            if(y - 3 < 24)
                y++;
            else
            {
                x++;
                y = 3;
            }
        }

        switch(przycisk)
        {
            case KEY_UP:
                if(x - 3 > 0)
                    x--;
                break;

            case KEY_DOWN:
                if(x - 3 < 24)
                    x++;
                break;

            case KEY_RIGHT:
                if(y - 3 < 24)
                    y++;
                break;

            case KEY_LEFT:
                if(y - 3 > 0)
                    y--;
                break;
        }

        refresh();
        mvprintw(13, 33, "(%d, %d)", x - 3, y - 3); //pozycja kursora
        move(x, y);


    }

    if(przycisk == KEY_HOME)
        if(zapis(plik) == -1)
            printw("Blad zapisu");

}
int zapis(char plik[])
{
    FILE *fp;
    int i, j;

    if((fp = fopen(plik, "w")) == NULL)
    {
        printw("Nie moge utworzyc pliku \n");
        return -1;
    }

    for(i = 0; i < 25; i++)
    {
        for(j = 0; j < 25; j++)
        {
            fprintf(fp, "%c", tab[i][j]);
        }
        fprintf(fp, "%c", '\n');
    }

    fclose(fp);
    return 0;

}
int wczytaj(char plik[])
{
    FILE *fp;
    int i, j;
    int temp;

    if((fp = fopen(plik, "r")) == NULL)
    {
        mvprintw(3, 2, "Nie moge utworzyc pliku \n");
        return -1;
    }

    for(i = 0; !feof(fp); i++)
    {
        for(j = 0; j < 25; j++)
        {
            temp = fgetc(fp);
            if(temp != '\n')
                tab[i][j] = temp;
        }
        fgetc(fp);
    }


    for(i = 0; i < 25; i++)
    {
        for(j = 0; j < 25; j++)
        {
            mvprintw(i+5, j+5, "%c", tab[i][j]);
            refresh();
        }
    }

    fclose(fp);
    getch();
    return 0;

}
void utworz_plik()
{
    char plik[20];

    clear();
    echo();
    mvprintw(2, 2, "Podaj nazwe pliku do utworzenia:");
    getstr(plik);
    pisanie(plik);

}
void wczytaj_plik()
{
    char plik[20];

    clear();
    echo();
    mvprintw(2, 2, "Podaj nazwe pliku do wczytania:");
    getstr(plik);

    if(wczytaj(plik) == 0)
    {
        pisanie(plik);
    }
    else
    {
        mvprintw(3, 2, "Blad wczytywania pliku \n");
        getch();
    }

}
int main()
{
    initscr();

    int wybor = 1, przycisk, i, j;
    int kolumny = 0, rzedy = 0;

    keypad(stdscr, TRUE);
    getmaxyx(stdscr, rzedy, kolumny);
    nonl();
    raw();
    noecho();

    while(1)
    {
        for(i = 0; i < 25; i++)     // czyszczenie tab
            for(j = 0; j < 25; j++)
                tab[i][j] = ' ';

        clear();
        noecho();
        switch(wybor)
        {
            case 1:
                mvprintw(rzedy/2 - 2, kolumny/2 - 8, "Menu \n");
                attron( A_REVERSE );
                mvprintw(rzedy/2 - 1, kolumny/2 - 8, "1. Nowy plik \n");
                attroff( A_REVERSE );
                mvprintw(rzedy/2, kolumny/2 - 8, "2. Wczytaj plik \n");
                mvprintw(rzedy/2 + 1, kolumny/2 - 8, "0. Wyjscie \n");
                mvprintw(rzedy/2 + 3, kolumny/2 - 8, "Aby wybrac opcje nacisnij t \n");
                refresh();

                break;

            case 2:
                mvprintw(rzedy/2 - 2, kolumny/2 - 8, "Menu \n");
                mvprintw(rzedy/2 - 1, kolumny/2 - 8, "1. Nowy plik \n");
                attron( A_REVERSE );
                mvprintw(rzedy/2, kolumny/2 - 8, "2. Wczytaj plik \n");
                attroff( A_REVERSE );
                mvprintw(rzedy/2 + 1, kolumny/2 - 8, "0. Wyjscie \n");
                mvprintw(rzedy/2 + 3, kolumny/2 - 8, "Aby wybrac opcje nacisnij t \n");
                refresh();

                break;

            case 3:
                mvprintw(rzedy/2 - 2, kolumny/2 - 8, "Menu \n");
                mvprintw(rzedy/2 - 1, kolumny/2 - 8, "1. Nowy plik \n");
                mvprintw(rzedy/2, kolumny/2 - 8, "2. Wczytaj plik \n");
                attron( A_REVERSE );
                mvprintw(rzedy/2 + 1, kolumny/2 - 8, "0. Wyjscie \n");
                attroff( A_REVERSE );
                mvprintw(rzedy/2 + 3, kolumny/2 - 8, "Aby wybrac opcje nacisnij t \n");
                refresh();

                break;
        }

        przycisk = getch();
        switch(wybor)
        {
            case 1:
                if(przycisk == KEY_DOWN)
                    wybor++;
                else if(przycisk == 't')
                    utworz_plik();

                break;

            case 2:
                if(przycisk == KEY_DOWN)
                    wybor++;
                else if(przycisk == KEY_UP)
                    wybor--;
                else if(przycisk == 't')
                    wczytaj_plik();

                break;

            case 3:
                if(przycisk == KEY_UP)
                    wybor--;
                else if(przycisk == 't')
                    exit(0);
                break;
        }

    }

    endwin();
    return 0;
}

