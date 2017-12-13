// projekt2.c -- Projekt2 - Tezeus a Minotaurus
// Adam Cierny, 19.4.2017 13:33:54

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char mapa[100][100], pom_mapa[100][100], k_mapa[100][100], smer_mapa[100][100], miest_mapa[100][100];  //Premenná do ktorej ukladám mapu, premenná na ukladanie miest kde som bol, premenná na ukladanie úrovne chodby, premenná na ukladanie smeru odkia¾ som prišiel, premenná na odlíšenie jednotlivých chodieb
int sirka, vyska, poc_dveri;  //Premenné pre reálnu výšku, šírku danej mapy a poèet všetkých dverí v mape

struct dvere {  //Štruktúra na zapamätanie údajov o jednotlivých dverách
  char meno;
  int pocDveri, prejdene;
  int x, y;
  struct dvere *nasl[25];
};

struct dvere D[26];  //Globálne pole pre všetky dvere
struct dvere *prvy;
FILE *f;

// ------------------------------- VYNULUJ ---------------------------------------
/*Pomocná funkcia na premazanie prememnných*/
void vynuluj()
{
  int r, s;
  poc_dveri = 0;
  for (r=0; r<vyska; r++)
    for (s = 0; s < sirka; s++)
    {
      pom_mapa[r][s] = 0;
      k_mapa[r][s] = 0;
      smer_mapa[r][s] = 0;
      miest_mapa[r][s] = -1;
    }

  for(r=0; r<vyska; r++)
    for (s = 0; s < sirka; s++)
    {
      if (r == 0 || s == sirka-1 || r == vyska-1 || s == 0)
        pom_mapa[r][s] = 1;
      if (mapa[r][s] == '.')
        pom_mapa[r][s] = 0;
    }

  for (r=0; r<26; r++)
  {
    D[r].meno = '\0';
    for (s = 0; s < 26; s++)
    {
      D[r].nasl[s] = NULL;
    }
    D[r].x = -1;
    D[r].y = -1;
    D[r].pocDveri = 0;
    D[r].prejdene = 0;
  }
}

// ------------------------------- NAJDI -----------------------------------------
struct dvere *najdi(char nazov)
{
  int i;
  for (i = 0; i<26; i++)
    if (D[i].meno == nazov)
      return &D[i];
}

// ------------------------------- PREMAZ ----------------------------------------
void premaz()
{
  int r, s, j;
  for (r = 0; r<vyska; r++)
    for (s = 0; s < sirka; s++)
    {
      smer_mapa[r][s] = 0;
      pom_mapa[r][s] = 0;
      if (r == 0 || s == sirka - 1 || r == vyska - 1 || s == 0)
      {
        pom_mapa[r][s] = 1;
        if (s == sirka - 1)
          pom_mapa[r][s + 1] = 1;
        if (r == vyska - 1)
          pom_mapa[r + 1][s] = 1;
      }
      if (mapa[r][s] == '.')
        pom_mapa[r][s] = 0;
    }
  for (j = 0; j < poc_dveri; j++)
    pom_mapa[D[j].y][D[j].x] = -1;
}

// ------------------------------- PREJDI (pre úrovne)----------------------------
void prejdi(int r, int s, int k)
{
  int i=0, Y=r, X=s, x, y;

  while (1)
  {
    if ((mapa[r][s-1]=='.' && pom_mapa[r][s-1]==0)  ||  (pom_mapa[r][s-1]==-1))
    {
      if (pom_mapa[r][s-1] == -1)
      {
        pom_mapa[r][s - 1] = i+1;
        continue;
      }
      pom_mapa[r][s-1] = i+1;
      if(k_mapa[r][s-1] == 0)
        k_mapa[r][s-1] = k;
      smer_mapa[r][s-1] = 1;
      s -= 1;
      continue;
    }
    if ((mapa[r+1][s]=='.' && pom_mapa[r+1][s]==0)  ||  (pom_mapa[r+1][s]==-1))
    {
      if (pom_mapa[r+1][s] == -1)
      {
        pom_mapa[r+1][s] = i+1;
        continue;
      }
      pom_mapa[r + 1][s] = i+1;
      if (k_mapa[r+1][s] == 0)
        k_mapa[r+1][s] = k;
      smer_mapa[r+1][s] = 2;
      r += 1;
      continue;
    }
    if ((mapa[r][s+1]=='.' && pom_mapa[r][s+1]==0)  ||  (pom_mapa[r][s+1]==-1))
    {
      if (pom_mapa[r][s+1] == -1)
      {
        pom_mapa[r][s+1] = i+1;
        continue;
      }
      pom_mapa[r][s+1] = i+1;
      if (k_mapa[r][s+1] == 0)
        k_mapa[r][s+1] = k;
      smer_mapa[r][s+1] = 3;
      s += 1;
      continue;
    }
    if ((mapa[r-1][s]=='.' && pom_mapa[r-1][s]==0)  ||  (pom_mapa[r-1][s]==-1))
    {
      if (pom_mapa[r-1][s] == -1)
      {
        pom_mapa[r-1][s] = i+1;
        continue;
      }
      pom_mapa[r-1][s] = i+1;
      if (k_mapa[r-1][s] == 0)
        k_mapa[r-1][s] = k;
      smer_mapa[r-1][s] = 4;
      r -= 1;
      continue;
    }
    if (r==Y && s==X)
    {
      if ((mapa[r][s-1]=='.' && pom_mapa[r][s-1]==0)  ||  (pom_mapa[r][s-1]==-1))
      {
        if (pom_mapa[r][s-1] == -1)
        {
          pom_mapa[r][s-1] = i+1;
          continue;
        }
        pom_mapa[r][s-1] = i+1;
        if (k_mapa[r][s-1] == 0)
          k_mapa[r][s-1] = k;
        smer_mapa[r][s-1] = 1;
        s -= 1;
        continue;
      }
      if ((mapa[r+1][s]=='.' && pom_mapa[r+1][s] == 0)  ||  (pom_mapa[r+1][s]==-1))
      {
        if (pom_mapa[r+1][s] == -1)
        {
          pom_mapa[r+1][s] = i+1;
          continue;
        }
        pom_mapa[r+1][s] = i+1;
        if (k_mapa[r+1][s] == 0)
          k_mapa[r+1][s] = k;
        smer_mapa[r+1][s] = 2;
        r += 1;
        continue;
      }
      if ((mapa[r][s+1]=='.' && pom_mapa[r][s+1]==0)  ||  (pom_mapa[r][s+1]==-1))
      {
        if (pom_mapa[r][s+1] == -1)
        {
          pom_mapa[r][s+1] = i+1;
          continue;
        }
        pom_mapa[r][s+1] = i+1;
        if (k_mapa[r][s+1] == 0)
          k_mapa[r][s+1] = k;
        smer_mapa[r][s+1] = 3;
        s += 1;
        continue;
      }
      if ((mapa[r-1][s]=='.' && pom_mapa[r-1][s]==0)  ||  (pom_mapa[r-1][s]==-1))
      {
        if (pom_mapa[r-1][s] == -1)
        {
          pom_mapa[r-1][s] = i+1;
          continue;
        }
        pom_mapa[r-1][s] = i+1;
        if (k_mapa[r-1][s] == 0)
          k_mapa[r-1][s] = k;
        smer_mapa[r-1][s] = 4;
        r -= 1;
        continue;
      }
      else
        return;
    }
    else
    {
      if (smer_mapa[r][s] == 1)
      {
        s += 1;
        continue;
      }
      if (smer_mapa[r][s] == 2)
      {
        r -= 1;
        continue;
      }
      if (smer_mapa[r][s] == 3)
      {
        s -= 1;
        continue;
      }
      if (smer_mapa[r][s] == 4)
      {
        r += 1;
        continue;
      }
    }
  }
}

// ------------------------------- VRSTVY MAPY ----------------------------------- 
void vrstvy_mapy()
{
  int r, s, i, j, x, k = 1, pocet = 0, pom_pocet, suhlasi=0, prejdene = 0;
  struct dvere *dalsie[26], *pom_pole[26];
  struct dvere *prvok = prvy;
  struct dvere *pom_prvok;

  r = prvy->y;
  s = prvy->x;
  prvy->prejdene = 1;
  premaz();
  prejdi(r, s, k);
  for (i=0; i<prvy->pocDveri; i++)
    dalsie[pocet++] = prvy->nasl[i];
  for (i=0; i<poc_dveri; i++)
    pom_pole[i] = NULL;
  k++;
  while (1)
  {
    for (i = 0; i < pocet; i++)
    {
      prvok = dalsie[i];
      r = prvok->y;
      s = prvok->x;
      prvok->prejdene = 1;
      prejdi(r, s, k);
    }
    pom_pocet = pocet;
    pocet = 0;
    for (i=0; i<pom_pocet; i++)
    {
      prvok = dalsie[i];
      for (j=0; j<prvok->pocDveri; j++)
      {
        pom_prvok = prvok->nasl[j];
        if (pom_prvok->prejdene == 0)
        {
          for (x = 0; x<26; x++)
            if (pom_prvok != pom_pole[x])
            {
              pom_pole[pocet++] = pom_prvok;
              break;
            }
        }
      }
    }
    for (i=0; i<pocet; i++)
      dalsie[i] = pom_pole[i];
    for (i = 0; i<poc_dveri; i++)
      pom_pole[i] = NULL;
    k++;
    prejdene = 0;
    for (i = 0; i < poc_dveri; i++)
      prejdene += D[i].prejdene;
    if (pocet == 0)
      break;
  }
}

// ------------------------------- ZORAD -----------------------------------------
void zorad(struct dvere *d)
{
  int i, pom_i, j;
  struct dvere pom_d;

  for (i = 0; i < poc_dveri; i++)
    for (j=i+1; j<poc_dveri; j++)
      if (d[i].meno > d[j].meno)
      {
        pom_d = d[i];
        d[i] = d[j];
        d[j] = pom_d;
      }
}

// ------------------------------- PREJDI MAPU -----------------------------------
/*Funkcia ktorá v mape vyh¾adá všetky dvere a významné body a nájde prepojenia medzi nimi*/
void prejdi_mapu()
{
  vynuluj();
  int r, s, i, j, miest=0;
  for (r=0; r<vyska; r++)  //Cyklus na vyh¾adanie všetkých dverí v bludisku
    for (s=0; s<sirka; s++)
      if (mapa[r][s] >= 'A' && mapa[r][s] <= 'Z')
      {
        pom_mapa[r][s] = -1;
        poc_dveri++;
        for (i = 0; i<26; i++)
          if (D[i].meno == 0)
          {
            D[i].meno = mapa[r][s];
            D[i].y = r;
            D[i].x = s;
            break;
          }
        if (mapa[r][s] == 'T')
          prvy = najdi('T');
      }

  for (i=0; i<poc_dveri; i++)
  {
    r = D[i].y;
    s = D[i].x;
    j = 0;
    while (1)
    {
      if (r == D[i].y && s == D[i].x)
        miest++;
      if ((mapa[r][s-1]=='.' && pom_mapa[r][s-1]==0)  ||  (pom_mapa[r][s-1]==-1))
      {
        if (pom_mapa[r][s-1] == -1)
        {
          pom_mapa[r][s-1] = i+1;
          if (D[i].meno != mapa[r][s-1])
          {
            D[i].nasl[j++] = najdi(mapa[r][s-1]);
            D[i].pocDveri++;
          }
          continue;
        }
        else if (miest_mapa[r][s-1] == -1)
          miest_mapa[r][s-1] = miest;
        pom_mapa[r][s-1] = i+1;
        smer_mapa[r][s-1] = 1;
        s-=1;
        continue;
      }
      if ((mapa[r+1][s]=='.' && pom_mapa[r+1][s]==0)  ||  (pom_mapa[r+1][s]==-1))
      {
        if (pom_mapa[r+1][s] == -1) 
        {
          pom_mapa[r+1][s] = i+1;
          if (D[i].meno != mapa[r+1][s])
          {
            D[i].nasl[j++] = najdi(mapa[r+1][s]);
            D[i].pocDveri++;
          }
          continue;
        }
        else if (miest_mapa[r+1][s] == -1)
          miest_mapa[r+1][s] = miest;
        pom_mapa[r+1][s] = i+1;
        smer_mapa[r+1][s] = 2;
        r+=1;
        continue;
      }
      if ((mapa[r][s+1]=='.' && pom_mapa[r][s+1]==0)  ||  (pom_mapa[r][s+1] == -1))
      {
        if (pom_mapa[r][s+1] == -1)
        {
          pom_mapa[r][s+1] = i+1;
          if (D[i].meno != mapa[r][s+1])
          {
            D[i].nasl[j++] = najdi(mapa[r][s+1]);
            D[i].pocDveri++;
          }
          continue;
        }
        else if (miest_mapa[r][s+1] == -1)
          miest_mapa[r][s+1] = miest;
        pom_mapa[r][s+1] = i+1;
        smer_mapa[r][s+1] = 3;
        s+=1;
        continue;
      }
      if ((mapa[r-1][s]=='.' && pom_mapa[r-1][s]==0)  ||  (pom_mapa[r-1][s] == -1))
      {
        if (pom_mapa[r-1][s] == -1)
        {
          pom_mapa[r-1][s] = i+1;
          if (D[i].meno != mapa[r-1][s])
          {
            D[i].nasl[j++] = najdi(mapa[r-1][s]);
            D[i].pocDveri++;
          }
          continue;
        }
        else if (miest_mapa[r-1][s] == -1)
          miest_mapa[r-1][s] = miest;
        pom_mapa[r-1][s] = i+1;
        smer_mapa[r-1][s] = 4;
        r-=1;
        continue;
      }
      if (r==D[i].y && s==D[i].x)
      {
        if ((mapa[r][s-1] == '.' && pom_mapa[r][s-1] == 0) || (pom_mapa[r][s-1] == -1))
        {
          if (pom_mapa[r][s-1] == -1)
          {
            pom_mapa[r][s-1] = i+1;
            if (D[i].meno != mapa[r][s-1])
            {
              D[i].nasl[j++] = najdi(mapa[r][s-1]);
              D[i].pocDveri++;
            }
            continue;
          }
          pom_mapa[r][s-1] = i+1;
          smer_mapa[r][s-1] = 1;
          s-=1;
          continue;
        }
        if ((mapa[r+1][s]=='.' && pom_mapa[r+1][s]==0)  ||  (pom_mapa[r+1][s]==-1))
        {
          if (pom_mapa[r+1][s] == -1)
          {
            pom_mapa[r+1][s] = i+1;
            if (D[i].meno != mapa[r+1][s])
            {
              D[i].nasl[j++] = najdi(mapa[r+1][s]);
              D[i].pocDveri++;
            }
            continue;
          }
          pom_mapa[r+1][s] = i+1;
          smer_mapa[r+1][s] = 2;
          r+=1;
          continue;
        }
        if ((mapa[r][s+1]=='.' && pom_mapa[r][s+1]==0)  ||  (pom_mapa[r][s+1]==-1))
        {
          if (pom_mapa[r][s+1] == -1)
          {
            pom_mapa[r][s+1] = i+1;
            if (D[i].meno != mapa[r][s+1])
            {
              D[i].nasl[j++] = najdi(mapa[r][s+1]);
              D[i].pocDveri++;
            }
            continue;
          }
          pom_mapa[r][s+1] = i+1;
          smer_mapa[r][s+1] = 3;
          s+=1;
          continue;
        }
        if ((mapa[r-1][s]=='.' && pom_mapa[r-1][s] == 0)  ||  (pom_mapa[r-1][s]==-1))
        {
          if (pom_mapa[r-1][s] == -1)
          {
            pom_mapa[r-1][s] = i+1;
            if (D[i].meno != mapa[r-1][s])
            {
              D[i].nasl[j++] = najdi(mapa[r-1][s]);
              D[i].pocDveri++;
            }
            continue;
          }
          pom_mapa[r-1][s] = i+1;
          smer_mapa[r-1][s] = 4;
          r-=1;
          continue;
        }
        else
          break;
      }
      else
      {
        if (smer_mapa[r][s] == 1)
        {
          s+=1;
          continue;
        }
        if (smer_mapa[r][s] == 2)
        {
          r -= 1;
          continue;
        }
        if (smer_mapa[r][s] == 3)
        {
          s -= 1;
          continue;
        }
        if (smer_mapa[r][s] == 4)
        {
          r += 1;
          continue;
        }
      }
    }
    premaz();
  }
  //Vypisovanie, vymaže sa 
  for (r = 0; r < vyska; r++)
    printf("%s", mapa[r]);
  printf("\n");
  //Vypisovanie, vymaže sa 
}

// ------------------------------- NACITAJ MAPU ----------------------------------
/*Funkcia ktorá slúži na naèítanie jednotlivých máp. Mapy naèítava po jednom. Keï naèíta nejakú mapu,
zavolá sa funkcia prejdi_mapu() na jej preh¾adanie. */
int nacitaj_mapu()
{
  int i=0, nacitane=0;
  char vstup[100], znak;
  while(fgets(vstup, 100, f) != NULL)
  {
    if (strcmp(vstup, "\n") == 0)
      break;
    strcpy(mapa[i], vstup);
    i++;
    if (nacitane == 0)
      nacitane = 1;
  }
  if (nacitane == 0)
    return 0;
  else
  {
    sirka = strlen(mapa[0]) - 1;
    vyska = i;
    prejdi_mapu();
    vrstvy_mapy();
    return 1;
  }
}

// ------------------------------- KRESLI MIESTNOSTI -----------------------------
void kresli_miestnosti(char *subor)
{
  int R[100], G[100], B[100], x, y, z, b, r=vyska-1, s=0, w=sirka*40, h=vyska*40;

  FILE *img = fopen(subor, "wb");
  
  srand(time(NULL));
  for (b = 0; b < 100; b++)
  {
    R[b] = rand()%254;
    G[b] = rand()%254;
    B[b] = rand()%254;
  }
  write_head(img, w, h);

  for (y = h; y>0; y--)
  {
    for (b = 0; b<40; b++)
    {
      for (x = 0; x<w; x += 40)
      {
        if (mapa[r][s] == '#')
        {
          for (z = x; z<x + 40; z++)
            write_pixel(img, 10, 10, 10);
        }
        else if (miest_mapa[r][s] != -1)
        {
          int c = miest_mapa[r][s];
          for (z = x; z<x + 40; z++)
            write_pixel(img, R[c], G[c], B[c]);
        }
        else if (mapa[r][s] >= 'A' && mapa[r][s] <= 'Z')
        {
          if (mapa[r][s] == 'T')
            for (z = x; z<x + 40; z++)
              write_pixel(img, 0, 0, 255);
          else if (mapa[r][s] == 'X')
            for (z = x; z<x + 40; z++)
              write_pixel(img, 255, 0, 0);
          else
            for (z = x; z<x + 40; z++)
              write_pixel(img, 110, 110, 110);
        }
        else 
          for (z = x; z<x + 40; z++)
            write_pixel(img, 255, 255, 255);
        s++;
      }
      s = 0;
    }
    r--;
  }
  fclose(img);
}

// ------------------------------- VYPÍŠ SUSEDNOSTI ------------------------------
void vypis_susednosti()
{
  int i, j;
  struct dvere *prvok;
  zorad(D);
  for (i = 0; i < poc_dveri; i++)
  {
    printf("%c: ", D[i].meno);
    for (j = 0; j < D[i].pocDveri; j++)
    {
      prvok = D[i].nasl[j];
      printf("%c, ", prvok->meno);
    }
    printf("\n");
  }
}

// ------------------------------- KRESLI MAPU -----------------------------------
void kresli_mapu(char *subor, int k)
{
  FILE *img = fopen(subor, "wb");
  int w=sirka*40, h=vyska*40, r=vyska-1, s=0, x, y, z, b;
  write_head(img, w, h);

  for (y=h; y>0; y--)
  {
    for (b=0; b<40; b++)
    {
      for (x=0; x<w; x+=41)
      {
        if (mapa[r][s] == '#')
        {
          for (z=x; z<x+40; z++)
            write_pixel(img, 10, 10, 10);
        }
        else if (mapa[r][s] == '.')
        {
          if(k_mapa[r][s]>0 && k_mapa[r][s]<=k)
            for (z=x; z<x+40; z++)
              write_pixel(img, 102, 255, 102);
          else
            for (z=x; z<x+40; z++)
              write_pixel(img, 255, 255, 255);
        }
        else if (mapa[r][s] >= 'A' && mapa[r][s] <= 'Z')
        {
          if (mapa[r][s] == 'T')
            for (z=x; z<x+40; z++)
              write_pixel(img, 0, 0, 255);
          else if (mapa[r][s] == 'X')
            for (z=x; z<x+40; z++)
              write_pixel(img, 255, 0, 0);
          else
            for (z=x; z<x+40; z++)
              write_pixel(img, 110, 110, 110);
        }
        s++;
      }
      s=0;
    }
    r--;
  }
  fclose(img);
}

int main(void)
{
  f = fopen("input.txt", "r");
  printf("%d\n",nacitaj_mapu());
  vypis_susednosti();
  /*kresli_mapu("mapa0.bmp", 0);
  kresli_mapu("mapa1.bmp", 1);
  kresli_mapu("mapa2.bmp", 2);
  kresli_mapu("mapa3.bmp", 3);
  kresli_mapu("mapa4.bmp", 4);
  kresli_mapu("mapa5.bmp", 5);
  kresli_miestnosti("miestnosti1.bmp");

  printf("%d\n", nacitaj_mapu());
  vypis_susednosti();
  kresli_mapu("mapa00.bmp", 0);
  kresli_mapu("mapa10.bmp", 1);
  kresli_mapu("mapa20.bmp", 2);
  kresli_mapu("mapa30.bmp", 3);
  kresli_mapu("mapa40.bmp", 4);
  kresli_mapu("mapa50.bmp", 5);
  kresli_miestnosti("miestnosti10.bmp");

  printf("%d\n", nacitaj_mapu());
  vypis_susednosti();
  kresli_mapu("mapa000.bmp", 0);
  kresli_mapu("mapa100.bmp", 1);
  kresli_mapu("mapa200.bmp", 2);
  kresli_mapu("mapa300.bmp", 3);
  kresli_mapu("mapa400.bmp", 4);
  kresli_mapu("mapa500.bmp", 5);
  kresli_mapu("mapa600.bmp", 6);
  kresli_miestnosti("miestnosti100.bmp");*/

  /*printf("%d\n", nacitaj_mapu());
  vypis_susednosti();
  kresli_mapu("mapa0000.bmp", 0);
  kresli_mapu("mapa1000.bmp", 1);
  kresli_mapu("mapa2000.bmp", 2);
  kresli_mapu("mapa3000.bmp", 3);
  kresli_mapu("mapa4000.bmp", 4);
  kresli_mapu("mapa5000.bmp", 5);
  kresli_miestnosti("miestnosti1000.bmp");

  printf("%d\n", nacitaj_mapu());
  vypis_susednosti();
  kresli_mapu("mapa00000.bmp", 0);
  kresli_mapu("mapa10000.bmp", 1);
  kresli_mapu("mapa20000.bmp", 2);
  kresli_mapu("mapa30000.bmp", 3);
  kresli_mapu("mapa40000.bmp", 4);
  kresli_mapu("mapa50000.bmp", 5);
  kresli_miestnosti("miestnosti10000.bmp");*/
  system("pause");
  fclose(f);
  return 0;
}
